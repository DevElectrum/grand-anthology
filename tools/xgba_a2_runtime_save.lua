-- Disposable XGBA gameplay persistence exercise.
-- Never send A to an unverified graphical menu icon. The save audit, not
-- successful input delivery, determines whether a transaction committed.
local function addr(name)
    local value = os.getenv(name)
    if not value or not value:match("^0[xX][%da-fA-F]+$") then
        return nil
    end
    return tonumber(value)
end

local main = assert(addr("A2_GMAIN"))
local overworld = assert(addr("A2_CB_OVERWORLD")) & 0xFFFFFFFE
local sb1ptr = assert(addr("A2_SB1_PTR"))
local usmStateAddr = addr("A2_USM_STATE_PTR")
local usmMemoryAddr = addr("A2_USM_MEMORY_PTR")
local saveCounterAddr = addr("A2_SAVE_COUNTER")
local saveStatusAddr = addr("A2_SAVE_ATTEMPT_STATUS")
local flashPresentAddr = addr("A2_FLASH_PRESENT")
local mode = os.getenv("A2_ACTION") or "save"
assert(mode == "save" or mode == "resume")

local A = 1 << C.GBA_KEY.A
local START = 1 << C.GBA_KEY.START
local RIGHT = 1 << C.GBA_KEY.RIGHT
local phase = "boot"
local frame = 0
local queue = {}
local current = 1
local remaining = 0
local attempts = 0

local function report(s)
    print(s .. " frame=" .. tostring(frame))
    io.stdout:flush()
end

local function finish(message, code)
    emu:setKeys(0)
    if saveCounterAddr then
        report("A2_XGBA_DIAG_SAVE_COUNTER=" .. tostring(emu:read32(saveCounterAddr)))
    end
    if saveStatusAddr then
        report("A2_XGBA_DIAG_SAVE_STATUS=" .. tostring(emu:read8(saveStatusAddr)))
    end
    if flashPresentAddr then
        report("A2_XGBA_DIAG_FLASH_PRESENT=" .. tostring(emu:read8(flashPresentAddr)))
    end
    report(message)
    os.exit(code)
end

local function hubReady()
    local cb = emu:read32(main + 4) & 0xFFFFFFFE
    local sb1 = emu:read32(sb1ptr)
    return cb == overworld and sb1 >= 0x02000000
        and sb1 < 0x02040000 and emu:read8(sb1 + 4) == 100
end

local function setActions(actions)
    queue = actions
    current = 1
    remaining = actions[1][2]
end

local function driveActions()
    if current > #queue then
        emu:setKeys(0)
        return true
    end
    emu:setKeys(queue[current][1])
    remaining = remaining - 1
    if remaining == 0 then
        current = current + 1
        if current <= #queue then
            remaining = queue[current][2]
        end
    end
    return false
end

local function readSelectedIcon()
    local location = usmStateAddr or usmMemoryAddr
    if not location then
        return nil, "No linked graphical menu state symbol"
    end
    local p = emu:read32(location)
    if p < 0x02000000 or p >= 0x02040000 then
        return nil, "Menu state pointer not active"
    end
    -- Exact struct Usm_State offsets, corroborated by existing lib.lua:
    -- selectedVisibleIdx at +5, itemOffset +10, items[] +11, count +23.
    local count = emu:read8(p + 23)
    local absolute = emu:read8(p + 5) + emu:read8(p + 10)
    if count < 1 or count > 12 or absolute >= count then
        return nil, "Invalid menu state index/count"
    end
    return emu:read8(p + 11 + absolute), tostring(absolute) .. "/" .. tostring(count)
end

callbacks:add("frame", function()
    frame = frame + 1
    if frame > 150000 then
        finish("A2_XGBA_SEQUENCE_TIMEOUT", 3)
    end

    if phase == "boot" then
        if frame % 30 == 0 and hubReady() then
            if mode == "resume" then
                finish("A2_XGBA_COLD_RESUME_HUB_PASS", 0)
            end
            phase = "open"
            report("A2_XGBA_HUB_READY")
            setActions({{0, 180}, {START, 2}, {0, 60}})
        else
            local tick = (frame - 1) % 30
            if tick < 3 then
                emu:setKeys(A)
            elseif tick >= 15 and tick < 17 then
                emu:setKeys(START)
            else
                emu:setKeys(0)
            end
            return
        end
    end

    if phase == "open" then
        if driveActions() then
            phase = "seek"
        end
        return
    end

    if phase == "advance" then
        if driveActions() then
            phase = "seek"
        end
        return
    end

    if phase == "seek" then
        local icon, info = readSelectedIcon()
        if not icon then
            finish("A2_XGBA_MENU_UNVERIFIED " .. tostring(info), 3)
        end
        if icon == 7 then -- USM_ICO_SAVE defined in constants/unbound_start_menu.h
            report("A2_XGBA_MENU_SELECTED_SAVE position=" .. info)
            phase = "confirm"
            setActions({
                {A, 2}, {0, 90}, {A, 2}, {0, 60},
                {A, 2}, {0, 200}, {0, 300}
            })
        else
            attempts = attempts + 1
            if attempts > 12 then
                finish("A2_XGBA_MENU_CANNOT_FIND_SAVE icon=" .. tostring(icon), 3)
            end
            phase = "advance"
            setActions({{RIGHT, 2}, {0, 12}})
        end
        return
    end

    if phase == "confirm" then
        if driveActions() then
            finish("A2_XGBA_SAVE_INPUT_SEQUENCE_FINISHED_AUDIT_REQUIRED", 0)
        end
        return
    end
end)
