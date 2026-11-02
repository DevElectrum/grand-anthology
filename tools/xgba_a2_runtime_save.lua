-- Disposable XGBA game/save exercise. Output is an input sequence verdict;
-- the CI runner MUST separately audit the E6 file before reporting save success.
local function addr(name)
    local value = os.getenv(name)
    assert(value and value:match("^0[xX][%da-fA-F]+$"), "Missing linked symbol " .. name)
    return assert(tonumber(value))
end

local main = addr("A2_GMAIN")
local overworld = addr("A2_CB_OVERWORLD") & 0xFFFFFFFE
local sb1ptr = addr("A2_SB1_PTR")
local mode = os.getenv("A2_ACTION") or "save"
assert(mode == "save" or mode == "resume", "Unknown disposable test mode")

local A = 1 << C.GBA_KEY.A
local START = 1 << C.GBA_KEY.START
local LEFT = 1 << C.GBA_KEY.LEFT
local RIGHT = 1 << C.GBA_KEY.RIGHT
local phase = "boot"
local frame = 0
local actions = {}
local current = 1
local remaining = 0

local function add(keys, frames)
    actions[#actions + 1] = { keys, frames }
end

local function hubReady()
    local cb = emu:read32(main + 4) & 0xFFFFFFFE
    local sb1 = emu:read32(sb1ptr)
    return cb == overworld and sb1 >= 0x02000000
        and sb1 < 0x02040000 and emu:read8(sb1 + 4) == 100
end

local function finish(message, rc)
    emu:setKeys(0)
    print(message .. " frames=" .. tostring(frame))
    io.stdout:flush()
    os.exit(rc)
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
            phase = "sequence"
            add(0, 180)
            add(START, 2)
            add(0, 60)
            -- The original SaveHarvest.lua pins the wheel left and moves
            -- right twice into Save. Each press has a release interval.
            for i = 1, 10 do
                add(LEFT, 2)
                add(0, 8)
            end
            for i = 1, 2 do
                add(RIGHT, 2)
                add(0, 12)
            end
            add(A, 2)
            add(0, 90)
            add(A, 2)
            add(0, 60)
            add(A, 2)
            add(0, 200)
            add(0, 300)
            current = 1
            remaining = actions[1][2]
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

    if current > #actions then
        finish("A2_XGBA_SAVE_INPUT_SEQUENCE_FINISHED_AUDIT_REQUIRED", 0)
    end
    emu:setKeys(actions[current][1])
    remaining = remaining - 1
    if remaining == 0 then
        current = current + 1
        if current <= #actions then
            remaining = actions[current][2]
        end
    end
end)
