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
local saveDialogTraceAddr = assert(addr("A2_SAVE_DIALOG_TRACE"), "Linked A2 Save-dialog trace is required")
local usmTraceAddr = assert(addr("A2_USM_TRACE"), "Linked graphical Start-menu trace is required")
local gameSaveTraceAddr = assert(addr("A2_GAME_SAVE_TRACE"), "Linked A2 save journal trace is required")
local saveStatusAddr = addr("A2_SAVE_ATTEMPT_STATUS")
local flashPresentAddr = addr("A2_FLASH_PRESENT")
local mode = os.getenv("A2_ACTION") or "save"
if mode == "save" then
    assert(saveCounterAddr, "Game's native save counter is required for Save dialogue test")
end
assert(mode == "save" or mode == "resume")
local expectedCounter = tonumber(os.getenv("A2_EXPECTED_SAVE_COUNTER") or "")
if mode == "resume" then
    assert(saveCounterAddr and expectedCounter and expectedCounter > 0,
           "Cold resume requires linked native counter and audited generation")
end

local A = 1 << C.GBA_KEY.A
local B = 1 << C.GBA_KEY.B
local START = 1 << C.GBA_KEY.START
local RIGHT = 1 << C.GBA_KEY.RIGHT
local phase = "boot"
local frame = 0
local queue = {}
local current = 1
local remaining = 0
local attempts = 0
local menuOpenAttempts = 0
local tourAdvanceFrames = 0
local tourAdvanceUsed = false
local lastMenuTrace = nil
local lastDialogTrace = nil
local lastNativeCounter = nil
local lastGameSaveTrace = nil
local lastActiveGameSaveTrace = nil
local sawActualSaveDialog = false

local function report(s)
    print(s .. " frame=" .. tostring(frame))
    io.stdout:flush()
end

local function finish(message, code)
    emu:setKeys(0)
    report("A2_XGBA_DIAG_GAME_SAVE_TRACE=" .. tostring(emu:read32(gameSaveTraceAddr)))
    report("A2_XGBA_DIAG_LAST_NONZERO_SAVE_TRACE=" .. tostring(lastActiveGameSaveTrace))
    report("A2_XGBA_DIAG_USM_TRACE=" .. tostring(emu:read32(usmTraceAddr)))
    report("A2_XGBA_DIAG_SAVE_DIALOG_TRACE=" .. tostring(emu:read32(saveDialogTraceAddr)))
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
    -- A plausible EWRAM pointer and arbitrary bytes can imitate an icon.
    -- Require BOTH the actual Usm_InitStartMenu and Task_UsmMain probes
    -- before trusting any selected icon or pressing its A action.
    if (emu:read32(usmTraceAddr) & 3) ~= 3 then
        return nil, "Real Start-menu init/task has not run"
    end
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
    -- Lua host state survives any in-game soft reset; the EWRAM debug trace
    -- does not. Record transitions every frame BEFORE any scripts or input
    -- changes, so a Save-failure reset cannot erase its own evidence.
    local gameSaveTrace = emu:read32(gameSaveTraceAddr)
    local menuTrace = emu:read32(usmTraceAddr)
    local dialogTrace = emu:read32(saveDialogTraceAddr)
    local counter = saveCounterAddr and emu:read32(saveCounterAddr) or 0
    if gameSaveTrace ~= 0 then
        lastActiveGameSaveTrace = gameSaveTrace
    end
    if phase ~= "boot" then
        if gameSaveTrace ~= lastGameSaveTrace then
            report("A2_XGBA_GAME_SAVE_TRACE_CHANGE old=" .. tostring(lastGameSaveTrace)
                   .. " new=" .. tostring(gameSaveTrace)
                   .. " stage=" .. tostring(gameSaveTrace >> 24)
                   .. " detail=" .. tostring(gameSaveTrace & 0xFFFFFF))
        end
        if menuTrace ~= lastMenuTrace then
            report("A2_XGBA_MENU_TRACE_CHANGE old=" .. tostring(lastMenuTrace)
                   .. " new=" .. tostring(menuTrace))
        end
        if dialogTrace ~= lastDialogTrace then
            report("A2_XGBA_DIALOG_TRACE_CHANGE old=" .. tostring(lastDialogTrace)
                   .. " new=" .. tostring(dialogTrace))
        end
        if counter ~= lastNativeCounter then
            report("A2_XGBA_NATIVE_COUNTER_CHANGE old=" .. tostring(lastNativeCounter)
                   .. " new=" .. tostring(counter))
        end
        if dialogTrace ~= 0 then
            sawActualSaveDialog = true
        end
        if phase == "confirm" and sawActualSaveDialog and dialogTrace == 0
           and lastDialogTrace and lastDialogTrace ~= 0 then
            finish("A2_XGBA_SAVE_DIALOG_DISAPPEARED_POSSIBLE_SOFT_RESET", 3)
        end
    end
    lastGameSaveTrace = gameSaveTrace
    lastMenuTrace = menuTrace
    lastDialogTrace = dialogTrace
    lastNativeCounter = counter
    if frame > 150000 then
        finish("A2_XGBA_SEQUENCE_TIMEOUT", 3)
    end

    if phase == "boot" then
        if frame % 30 == 0 and hubReady() then
            if mode == "resume" then
                -- A fresh New Game also reaches this map. A cold reload is
                -- accepted only if the actual native counter equals the
                -- generation obtained by auditing the committed E6 image.
                local got = emu:read32(saveCounterAddr)
                if got ~= expectedCounter then
                    finish("A2_XGBA_COLD_RESUME_COUNTER_MISMATCH expected="
                           .. tostring(expectedCounter) .. " got=" .. tostring(got), 3)
                end
                finish("A2_XGBA_COLD_RESUME_HUB_PASS counter=" .. tostring(got), 0)
            end
            -- RegionHub first-visit ON_FRAME automatically locks controls
            -- while the curator offers a guided tour. A fixed 180-frame
            -- delay is not enough: the old test pressed Start into lockall.
            -- On disposable saves B declines the offer, dismisses dialogue,
            -- then the player regains field control.
            phase = "intro"
            report("A2_XGBA_HUB_READY_TOUR_MAY_BE_LOCKED")
            local actions = {{0, 240}}
            for i = 1, 18 do
                actions[#actions + 1] = {B, 2}
                actions[#actions + 1] = {0, 52}
            end
            setActions(actions)
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

    if phase == "intro" then
        if driveActions() then
            phase = "open"
            setActions({{START, 2}, {0, 75}})
        end
        return
    end

    if phase == "open" then
        if driveActions() then
            phase = "seek"
        end
        return
    end

    if phase == "tour-advance" then
        -- The Hub's mandatory first-visit curator tour may still be holding
        -- script controls. B alone is insufficient in some dialogue states.
        -- Advance dialogue with spaced A presses and retry Start until the
        -- real menu's init+task trace confirms full field control.
        local trace = emu:read32(usmTraceAddr)
        local p = usmStateAddr and emu:read32(usmStateAddr) or 0
        if (trace & 3) == 3 and p >= 0x02000000 and p < 0x02040000 then
            report("A2_XGBA_TOUR_CLEARED_AND_REAL_MENU_STARTED")
            phase = "tour-menu-settle"
            emu:setKeys(0)
            setActions({{0, 90}})
            return
        end
        tourAdvanceFrames = tourAdvanceFrames + 1
        if tourAdvanceFrames > 30000 then
            finish("A2_XGBA_HUB_INTRO_NEVER_RELEASED_FIELD_INPUT", 3)
        end
        local tick = (tourAdvanceFrames - 1) % 150
        if tick < 3 then
            emu:setKeys(A)
        elseif tick >= 75 and tick < 78 then
            emu:setKeys(START)
        else
            emu:setKeys(0)
        end
        return
    end

    if phase == "tour-menu-settle" then
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
            if not tourAdvanceUsed then
                tourAdvanceUsed = true
                phase = "tour-advance"
                report("A2_XGBA_TOUR_UNLOCK_REQUIRED " .. tostring(info))
                emu:setKeys(0)
                return
            end
            menuOpenAttempts = menuOpenAttempts + 1
            report("A2_XGBA_MENU_NOT_OPEN attempt=" .. tostring(menuOpenAttempts)
                   .. " reason=" .. tostring(info))
            if menuOpenAttempts > 18 then
                finish("A2_XGBA_MENU_UNVERIFIED_TOUR_LOCK_OR_START_REJECTED", 3)
            end
            -- Only B/Start retries while the USM pointer is NULL.
            -- Never press A unless the actual USM icon equals Save.
            phase = "open"
            setActions({{B, 2}, {0, 105}, {START, 2}, {0, 75}})
            return
        end
        if icon == 7 then -- USM_ICO_SAVE defined in constants/unbound_start_menu.h
            report("A2_XGBA_MENU_SELECTED_SAVE position=" .. info
                   .. " menu_trace=" .. tostring(emu:read32(usmTraceAddr)))
            phase = "confirm"
            -- save_dialog.c may present Confirm -> Already Saved File ->
            -- Confirm Overwrite (SKIP_SAVE_CONFIRMATION=FALSE) before the
            -- actual journal transaction. We do not assume a fixed number
            -- of prompts; stop pressing A immediately when native counter
            -- advances, then let the host audit exact E6 metadata.
            local confirmations = {}
            for i = 1, 9 do
                confirmations[#confirmations + 1] = {A, 2}
                confirmations[#confirmations + 1] = {0, 120}
            end
            setActions(confirmations)
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
        local counter = emu:read32(saveCounterAddr)
        if counter > 0 then
            report("A2_XGBA_GAME_NATIVE_COUNTER_ADVANCED=" .. tostring(counter))
            phase = "commit-settle"
            setActions({{0, 240}})
            return
        end
        if driveActions() then
            finish("A2_XGBA_SAVE_DIALOG_EXHAUSTED_WITHOUT_NATIVE_COMMIT", 3)
        end
        return
    end

    if phase == "commit-settle" then
        if driveActions() then
            if emu:read32(saveCounterAddr) == 0 then
                finish("A2_XGBA_SAVE_COUNTER_REVERTED", 3)
            end
            finish("A2_XGBA_SAVE_INPUT_SEQUENCE_FINISHED_AUDIT_REQUIRED", 0)
        end
        return
    end
end)
