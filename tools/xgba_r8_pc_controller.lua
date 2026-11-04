-- C3C pilot: actual simulated controller input at the real Region Hub PC.
-- No direct game-function calls or direct save writes. Every test uses
-- disposable E6 sidecars; jump-save starts with real populated Save2.
local function address(name)
    local value = os.getenv(name)
    assert(value and value:match("^0[xX][%da-fA-F]+$"), "Linked ELF symbol missing " .. name)
    return tonumber(value)
end
local main = address("A2_GMAIN")
local overworld = address("A2_CB_OVERWORLD") & 0xFFFFFFFE
local sb1ptr = address("A2_SB1_PTR")
local uiTrace = address("A2_R8_PC_UI_TRACE")
local engine = assert(os.getenv("R8_PC_ENGINE"), "Missing R8_PC_ENGINE")
assert(engine == "classic" or engine == "swsh", "Unknown PC engine")
local expectedStage = engine == "classic" and 4 or 5
local expectedOptionsStage = engine == "classic" and 7 or 8
local expectedChooserStage = engine == "classic" and 9 or 10
local action = os.getenv("R8_PC_ACTION") or "entry"
assert(action == "entry" or action == "preview-cancel"
       or action == "jump-commit" or action == "jump-save"
       or action == "withdraw" or action == "roundtrip"
       or action == "roundtrip-save")
local chooserTraceAddr = action ~= "entry" and address("A2_R8_PC_CHOOSER_TRACE") or nil
local saveMode = action == "jump-save" or action == "roundtrip-save"
local roundtripMode = action == "roundtrip" or action == "roundtrip-save"
local withdrawMode = action == "withdraw" or roundtripMode
local c3dTraceAddr = withdrawMode and address("A2_R8_PC_C3D_TRACE") or nil
local expectedMonMenuStage = engine == "classic" and 13 or 14
local expectedDepositChooserStage = engine == "classic" and 15 or 16
local saveCounterAddr = saveMode and address("A2_SAVE_COUNTER") or nil
local gameSaveTraceAddr = saveMode and address("A2_GAME_SAVE_TRACE") or nil
local saveDialogTraceAddr = saveMode and address("A2_SAVE_DIALOG_TRACE") or nil
local usmTraceAddr = saveMode and address("A2_USM_TRACE") or nil
local usmStateAddr = saveMode and address("A2_USM_STATE_PTR") or nil
local expectedPcMenuStage = engine == "classic" and 11 or 12
local A, B = 1 << C.GBA_KEY.A, 1 << C.GBA_KEY.B
local START = 1 << C.GBA_KEY.START
local UP, DOWN = 1 << C.GBA_KEY.UP, 1 << C.GBA_KEY.DOWN
local LEFT, RIGHT = 1 << C.GBA_KEY.LEFT, 1 << C.GBA_KEY.RIGHT
local L, R = 1 << C.GBA_KEY.L, 1 << C.GBA_KEY.R
local frame, stage = 0, "boot"
local queue, index, remaining = {}, 1, 0
local moveX, moveY, retries, clicks = 0, 0, 0, 0
local lastUiTrace = nil
local commitStarted = 0
local withdrawStarted, partyRouteSteps = 0, 0
local exitStarted, saveStarted, attempts, menuAttempts = 0, 0, 0, 0
-- Actual Region Hub collision map: y=8 is a wall east of x=8,
-- and y=9 is a counter east of x=6. Go around the west end,
-- then approach the PC from the southern public aisle.
-- Keep this in sync with test_c3c_controller_route_is_walkable.
local waypoints = {{16, 6}, {6, 6}, {6, 13}, {17, 13}, {17, 10}}
local waypoint = 1
local function report(s)
    print(s .. " engine=" .. engine .. " frame=" .. frame)
    io.stdout:flush()
end
local function fail(reason)
    emu:setKeys(0)
    report("R8_PC_CONTROLLER_FAIL " .. reason .. " ui_trace=" .. emu:read32(uiTrace)
           .. " chooser_trace=" .. (chooserTraceAddr and emu:read32(chooserTraceAddr) or "none")
           .. " native_counter=" .. (saveCounterAddr and emu:read32(saveCounterAddr) or "none")
           .. " game_save_trace=" .. (gameSaveTraceAddr and emu:read32(gameSaveTraceAddr) or "none"))
    os.exit(3)
end
local function schedule(a)
    queue, index = a, 1
    remaining = a[1][2]
end
local function drive()
    if index > #queue then
        emu:setKeys(0)
        return true
    end
    emu:setKeys(queue[index][1])
    remaining = remaining - 1
    if remaining <= 0 then
        index = index + 1
        if index <= #queue then remaining = queue[index][2] end
    end
    return false
end
local function isHub()
    local sb = emu:read32(sb1ptr)
    return (emu:read32(main + 4) & 0xFFFFFFFE) == overworld
        and sb >= 0x02000000 and sb < 0x02040000
        and emu:read8(sb + 4) == 100
end
local function coords()
    local sb = emu:read32(sb1ptr)
    if sb < 0x02000000 or sb >= 0x02040000 then fail("bad_saveblock") end
    local x, y = emu:read16(sb), emu:read16(sb + 2)
    if x >= 25 or y >= 18 then fail("bad_map_coords " .. x .. "," .. y) end
    return x, y
end
local function verifyChooser(expectedPreview, expectedActive)
    expectedActive = expectedActive or 0
    local witness = emu:read32(uiTrace)
    local snapshot = emu:read32(chooserTraceAddr)
    local preview = snapshot & 0xFFFF
    local active = (snapshot >> 16) & 0xFF
    if witness ~= expectedChooserStage or (snapshot & 0x80000000) == 0
       or active ~= expectedActive or preview ~= expectedPreview then
        fail("chooser_mismatch expected_preview=" .. expectedPreview
             .. " actual_preview=" .. preview .. " active=" .. active
             .. " state=" .. witness)
    end
    report("R8_PC_CONTROLLER_CHOOSER_OK preview=" .. preview
           .. " active_page=" .. active)
end
-- This is the same real Usm_State layout validated by the existing
-- xgba_a2_runtime_save.lua driver. An icon number alone is never trusted:
-- both genuine graphical Start-menu callbacks and a plausible live pointer
-- must be present before the driver sends A.
local function readSelectedSaveIcon()
    if not saveMode or (emu:read32(usmTraceAddr) & 3) ~= 3 then
        return nil
    end
    local p = emu:read32(usmStateAddr)
    if p < 0x02000000 or p >= 0x02040000 then return nil end
    local count = emu:read8(p + 23)
    local absolute = emu:read8(p + 5) + emu:read8(p + 10)
    if count < 1 or count > 12 or absolute >= count then return nil end
    return emu:read8(p + 11 + absolute)
end
local function cursorState()
    local trace = emu:read32(c3dTraceAddr)
    return (trace >> 16) & 0xFF, (trace >> 8) & 0xFF, trace & 0xFF
end
local function assertDepositChooser(expectedGlobal)
    if emu:read32(uiTrace) ~= expectedDepositChooserStage then
        fail("deposit_chooser_not_live")
    end
    local marker = emu:read32(chooserTraceAddr)
    local preview = marker & 0xFFFF
    local active = (marker >> 16) & 0xFF
    if (marker & 0x80000000) == 0 or active ~= 1 or preview ~= expectedGlobal then
        fail("deposit_chooser_mismatch expected=" .. expectedGlobal
             .. " actual=" .. preview .. " page=" .. active)
    end
end
local function checkSavedBox15()
    local marker = emu:read32(chooserTraceAddr)
    if (marker & 0x40000000) == 0 or ((marker >> 16) & 0xFF) ~= 1
       or (marker & 0xFFFF) ~= 0 then
        fail("post_jump_resident_page_or_box_changed")
    end
end
local function move(key, dx, dy)
    local x, y = coords()
    moveX, moveY = x + dx, y + dy
    stage = "moving"
    schedule({{key, 2}, {0, 40}})
end
callbacks:add("frame", function()
    frame = frame + 1
    if frame > 120000 then fail("global_timeout") end
    if stage == "clicks" and emu:read32(uiTrace) == expectedStage then
        emu:setKeys(0)
        local x, y = coords()
        report("R8_PC_CONTROLLER_SMOKE_PASS genuine_pc_ui_active=1 x=" .. x .. " y=" .. y)
        if action == "entry" then
            os.exit(0)
        end
        report("R8_PC_CONTROLLER_PREVIEW_BEGIN")
        stage = "preview-to-title"
        schedule({{0, 90}, {UP, 2}, {0, 90}})
        return
    end
    if stage == "boot" then
        if frame % 30 == 0 and isHub() then
            report("R8_PC_CONTROLLER_HUB_READY")
            local actions = {{0, 240}}
            for i = 1, 18 do
                actions[#actions + 1] = {B, 2}
                actions[#actions + 1] = {0, 52}
            end
            schedule(actions)
            stage = "intro"
        else
            local t = (frame - 1) % 30
            if t < 3 then emu:setKeys(A)
            elseif t >= 15 and t < 17 then emu:setKeys(START)
            else emu:setKeys(0) end
        end
        return
    end
    if stage == "intro" then
        if drive() then
            local x, y = coords()
            report("R8_PC_CONTROLLER_TRAVEL_START x=" .. x .. " y=" .. y)
            stage = "navigate"
        end
        return
    end
    if stage == "navigate" then
        if not isHub() then fail("left_hub_while_navigating") end
        local x, y = coords()
        local point = waypoints[waypoint]
        if point and x == point[1] and y == point[2] then
            report("R8_PC_CONTROLLER_WAYPOINT " .. waypoint .. " at=" .. x .. "," .. y)
            waypoint = waypoint + 1
            point = waypoints[waypoint]
        end
        if not point then
            if x ~= 17 or y ~= 10 then fail("end_of_route_position_incorrect") end
            stage = "face"
            schedule({{UP, 2}, {0, 35}})
        elseif x < point[1] then move(RIGHT, 1, 0)
        elseif x > point[1] then move(LEFT, -1, 0)
        elseif y < point[2] then move(DOWN, 0, 1)
        else move(UP, 0, -1) end
        return
    end
    if stage == "moving" then
        if drive() then
            local x, y = coords()
            if x ~= moveX or y ~= moveY then
                retries = retries + 1
                if retries >= 3 then
                    fail("blocked expected=" .. moveX .. "," .. moveY
                         .. " actual=" .. x .. "," .. y)
                end
            else
                retries = 0
            end
            stage = "navigate"
        end
        return
    end
    if stage == "face" then
        if drive() then
            local x, y = coords()
            if x ~= 17 or y ~= 10 then fail("pc_facing_changed_position") end
            stage, clicks = "clicks", 0
            schedule({{A, 2}, {0, 115}})
        end
        return
    end
    if stage == "clicks" then
        local witness = emu:read32(uiTrace)
        if witness ~= lastUiTrace then
            report("R8_PC_CONTROLLER_UI_TRACE old=" .. tostring(lastUiTrace)
                   .. " new=" .. witness)
            lastUiTrace = witness
        end
        if drive() then
            clicks = clicks + 1
            report("R8_PC_CONTROLLER_MENU_PROGRESS click=" .. clicks .. " trace=" .. witness)
            -- pc.inc has a startup text, a multichoice, login text and a
            -- storage-open text before ShowPokemonStorageSystemPC is called.
            -- Never expect that callback after only a few A presses.
            if witness == 0 and clicks >= 3 then
                fail("pc_turn_on_special_not_observed")
            end
            if witness == 6 and clicks >= 14 then
                fail("pc_script_prompts_never_reached_storage")
            end
            if clicks >= 18 then fail("pc_storage_screen_never_active") end
            schedule({{A, 2}, {0, 115}})
        end
        return
    end
    if stage == "preview-to-title" then
        if drive() then
            stage = "preview-open-options"
            schedule({{A, 2}, {0, 100}})
        end
        return
    end
    if stage == "preview-open-options" then
        if drive() then
            if emu:read32(uiTrace) ~= expectedOptionsStage then
                fail("box_title_options_menu_not_entered")
            end
            stage = "preview-open-jump"
            schedule({{A, 2}, {0, 115}})
        end
        return
    end
    if stage == "preview-open-jump" then
        if drive() then
            if not saveMode and not withdrawMode then verifyChooser(0) end
            if saveMode or withdrawMode then
                -- Populated Save2 starts on global Box 30 (zero-index 29).
                -- L switches to global Box 16, LEFT selects Box 15.
                verifyChooser(29, 2)
                stage = "save-jump-page-left"
                schedule({{L, 2}, {0, 75}})
            else
                if action == "jump-commit" then
                    stage = "commit-right"
                else
                    stage = "preview-right"
                end
                schedule({{R, 2}, {0, 75}})
            end
        end
        return
    end
    if stage == "preview-right" then
        if drive() then
            verifyChooser(14)
            stage = "preview-left"
            schedule({{L, 2}, {0, 75}})
        end
        return
    end
    if stage == "preview-left" then
        if drive() then
            verifyChooser(0)
            stage = "preview-cancel"
            schedule({{B, 2}, {0, 120}})
        end
        return
    end
    if stage == "preview-cancel" then
        if drive() then
            local marker = emu:read32(chooserTraceAddr)
            local seen = emu:read32(uiTrace)
            if seen ~= expectedStage or (marker & 0x40000000) == 0
               or ((marker >> 16) & 0xFF) ~= 0 or (marker & 0xFFFF) ~= 0 then
                fail("cancel_changed_resident_box_or_failed_to_return")
            end
            report("R8_PC_CONTROLLER_PREVIEW_CANCEL_PASS active_page=0 box=0")
            emu:setKeys(0)
            os.exit(0)
        end
        return
    end
    if stage == "save-jump-page-left" then
        if drive() then
            verifyChooser(15, 2)
            stage = "save-jump-local-left"
            schedule({{LEFT, 2}, {0, 75}})
        end
        return
    end
    if stage == "save-jump-local-left" then
        if drive() then
            verifyChooser(14, 2)
            stage = "commit-select"
            schedule({{A, 2}, {0, 140}})
        end
        return
    end
    if stage == "commit-right" then
        if drive() then
            verifyChooser(14)
            stage = "commit-select"
            schedule({{A, 2}, {0, 140}})
        end
        return
    end
    if stage == "commit-select" then
        if drive() then
            commitStarted = frame
            stage = "commit-await"
            emu:setKeys(0)
        end
        return
    end

    if stage == "withdraw-down-from-title" then
        if drive() then
            local area, pos, partyCount = cursorState()
            if area ~= 0 or pos > 5 or partyCount >= 6 then
                fail("withdraw_cursor_not_in_first_row area=" .. area .. " pos=" .. pos
                     .. " party_count=" .. partyCount)
            end
            local actions = {}
            for i = pos + 1, 5 do
                actions[#actions + 1] = {RIGHT, 2}
                actions[#actions + 1] = {0, 55}
            end
            if #actions == 0 then actions = {{0, 15}} end
            stage = "withdraw-select-mon"
            schedule(actions)
        end
        return
    end
    if stage == "withdraw-select-mon" then
        if drive() then
            local area, pos = cursorState()
            if area ~= 0 or pos ~= 5 then fail("withdraw_wrong_source_cursor " .. area .. "," .. pos) end
            stage = "withdraw-open-menu"
            schedule({{A, 2}, {0, 145}})
        end
        return
    end
    if stage == "withdraw-open-menu" then
        if drive() then
            if emu:read32(uiTrace) ~= expectedMonMenuStage then
                fail("real_mon_menu_not_active")
            end
            stage = "withdraw-menu-choose"
            -- Move, Summary, Withdraw is the verified MOVE_MONS menu order.
            schedule({{DOWN, 2}, {0, 45}, {DOWN, 2}, {0, 45}, {A, 2}, {0, 120}})
        end
        return
    end
    if stage == "withdraw-menu-choose" then
        if drive() then
            withdrawStarted = frame
            stage = "withdraw-await"
        end
        return
    end
    if stage == "withdraw-await" then
        emu:setKeys(0)
        local trace = emu:read32(c3dTraceAddr)
        if (trace & 0x80000000) ~= 0 and emu:read32(uiTrace) == expectedStage then
            local area, pos, partyCount = cursorState()
            report("R8_PC_CONTROLLER_WITHDRAW_PASS global_box=15 source_slot=5 party_count=" .. partyCount)
            if roundtripMode then
                -- Do not guess a key sequence from the post-withdraw cursor.
                -- Classic must use the bottom Party button; SwSh crosses the
                -- left edge of the box. Route from the observed live state.
                partyRouteSteps = 0
                stage = "roundtrip-to-party"
                schedule({{0, 40}})
                return
            end
            os.exit(0)
        end
        if frame - withdrawStarted > 4000 then fail("real_graphical_withdraw_never_verified") end
        return
    end
    if stage == "roundtrip-to-party" then
        if drive() then
            if emu:read32(uiTrace) ~= expectedStage then
                fail("party_route_not_in_main_input")
            end
            local area, pos, partyCount = cursorState()
            if partyCount < 2 or partyCount > 5 then
                fail("party_route_bad_party_count=" .. partyCount)
            end
            if area == 1 then
                local last = partyCount - 1
                local actions = {}
                local key = pos < last and DOWN or UP
                for i = 1, math.abs(last - pos) do
                    actions[#actions + 1] = {key, 2}
                    actions[#actions + 1] = {0, 70}
                end
                if #actions == 0 then actions = {{0, 30}} end
                stage = "roundtrip-select-party"
                schedule(actions)
                return
            end

            partyRouteSteps = partyRouteSteps + 1
            if partyRouteSteps > 12 then
                fail("party_route_exceeded_budget area=" .. area .. " pos=" .. pos)
            end
            local key, keyName
            if engine == "classic" then
                if area == 0 and pos < 30 then
                    -- Box positions 24..29 go DOWN to bottom buttons.
                    key, keyName = DOWN, "down"
                elseif area == 2 then
                    key, keyName = DOWN, "down-from-title"
                elseif area == 3 and pos == 1 then
                    key, keyName = LEFT, "select-party-button"
                elseif area == 3 and pos == 0 then
                    key, keyName = A, "open-party"
                end
            else
                if area == 0 and pos < 30 then
                    -- Only column zero crosses into SwSh's party panel.
                    key, keyName = LEFT, "move-left-toward-party"
                elseif area == 2 then
                    key, keyName = DOWN, "down-from-title"
                end
            end
            if not key then
                fail("party_route_invalid_state area=" .. area .. " pos=" .. pos)
            end
            report("R8_PC_CONTROLLER_PARTY_ROUTE step=" .. partyRouteSteps
                   .. " area=" .. area .. " pos=" .. pos .. " action=" .. keyName)
            schedule({{key, 2}, {0, key == A and 200 or 95}})
        end
        return
    end
    if stage == "roundtrip-select-party" then
        if drive() then
            local area, pos, partyCount = cursorState()
            if area ~= 1 or pos ~= partyCount - 1 then
                fail("roundtrip_selected_wrong_party_member")
            end
            stage = "roundtrip-open-store-menu"
            schedule({{A, 2}, {0, 150}})
        end
        return
    end
    if stage == "roundtrip-open-store-menu" then
        if drive() then
            if emu:read32(uiTrace) ~= expectedMonMenuStage then
                fail("roundtrip_party_store_menu_not_open")
            end
            stage = "roundtrip-first-chooser"
            schedule({{DOWN, 2}, {0, 45}, {DOWN, 2}, {0, 45}, {A, 2}, {0, 155}})
        end
        return
    end
    if stage == "roundtrip-first-chooser" then
        if drive() then
            assertDepositChooser(14)
            stage = "roundtrip-cancel"
            schedule({{B, 2}, {0, 155}})
        end
        return
    end
    if stage == "roundtrip-cancel" then
        if drive() then
            local proof = emu:read32(c3dTraceAddr)
            if emu:read32(uiTrace) ~= expectedStage
               or (proof & 0x80000000) == 0 or (proof & 0x40000000) ~= 0 then
                fail("roundtrip_cancel_mutated_party_or_storage")
            end
            report("R8_PC_CONTROLLER_DEPOSIT_CANCEL_PASS")
            stage = "roundtrip-reopen-menu"
            schedule({{A, 2}, {0, 150}})
        end
        return
    end
    if stage == "roundtrip-reopen-menu" then
        if drive() then
            if emu:read32(uiTrace) ~= expectedMonMenuStage then
                fail("roundtrip_store_menu_not_reopened")
            end
            stage = "roundtrip-second-chooser"
            schedule({{DOWN, 2}, {0, 45}, {DOWN, 2}, {0, 45}, {A, 2}, {0, 155}})
        end
        return
    end
    if stage == "roundtrip-second-chooser" then
        if drive() then
            assertDepositChooser(14)
            stage = "roundtrip-remote-preview"
            schedule({{R, 2}, {0, 85}, {R, 2}, {0, 85}})
        end
        return
    end
    if stage == "roundtrip-remote-preview" then
        if drive() then
            assertDepositChooser(42)
            stage = "roundtrip-commit-deposit"
            schedule({{A, 2}, {0, 120}})
        end
        return
    end
    if stage == "roundtrip-commit-deposit" then
        if drive() then
            withdrawStarted = frame
            stage = "roundtrip-await-deposit"
        end
        return
    end
    if stage == "roundtrip-await-deposit" then
        emu:setKeys(0)
        local proof = emu:read32(c3dTraceAddr)
        if emu:read32(uiTrace) == expectedStage and (proof & 0x40000000) ~= 0 then
            report("R8_PC_CONTROLLER_ROUNDTRIP_PASS source_box=15 destination_box=43")
            if saveMode then
                stage = "exit-storage-prompt"
                schedule({{B, 2}, {0, 125}})
            else
                os.exit(0)
            end
        end
        if frame - withdrawStarted > 4000 then fail("remote_deposit_never_verified") end
        return
    end
    -- Storage B opens Continue? (Yes/No); a second B selects No, which
    -- closes the screen without picking up or modifying any Pokémon.
    if stage == "exit-storage-prompt" then
        if drive() then
            checkSavedBox15()
            stage = "exit-storage-confirm"
            schedule({{B, 2}, {0, 165}})
        end
        return
    end
    if stage == "exit-storage-confirm" then
        if drive() then
            exitStarted = frame
            stage = "await-pc-main"
        end
        return
    end
    if stage == "await-pc-main" then
        emu:setKeys(0)
        if emu:read32(uiTrace) == expectedPcMenuStage then
            report("R8_PC_CONTROLLER_RETURNED_TO_REAL_PC_MENU")
            checkSavedBox15()
            stage = "close-pc-main"
            schedule({{B, 2}, {0, 180}})
        elseif frame - exitStarted > 2700 then
            fail("storage_screen_did_not_return_to_pc_main_menu")
        end
        return
    end
    if stage == "close-pc-main" then
        if drive() then
            if not isHub() then fail("did_not_return_to_hub_field") end
            checkSavedBox15()
            stage = "open-start-menu"
            schedule({{START, 2}, {0, 110}})
        end
        return
    end
    if stage == "open-start-menu" then
        if drive() then
            stage = "seek-real-save"
        end
        return
    end
    if stage == "retry-start-menu" then
        if drive() then stage = "seek-real-save" end
        return
    end
    if stage == "advance-start-menu" then
        if drive() then stage = "seek-real-save" end
        return
    end
    if stage == "seek-real-save" then
        checkSavedBox15()
        local icon = readSelectedSaveIcon()
        if icon == nil then
            menuAttempts = menuAttempts + 1
            if menuAttempts > 8 then fail("graphical_start_menu_not_verified") end
            stage = "retry-start-menu"
            schedule({{B, 2}, {0, 90}, {START, 2}, {0, 110}})
            return
        end
        if icon == 7 then  -- USM_ICO_SAVE
            report("R8_PC_CONTROLLER_REAL_START_MENU_SAVE_SELECTED")
            local a = {}
            for i = 1, 9 do
                a[#a + 1] = {A, 2}
                a[#a + 1] = {0, 120}
            end
            stage = "confirm-real-save"
            saveStarted = frame
            schedule(a)
        else
            attempts = attempts + 1
            if attempts > 12 then fail("start_menu_save_icon_not_found") end
            stage = "advance-start-menu"
            schedule({{RIGHT, 2}, {0, 15}})
        end
        return
    end
    if stage == "confirm-real-save" or stage == "await-real-save" then
        local counter = emu:read32(saveCounterAddr)
        local saveTrace = emu:read32(gameSaveTraceAddr)
        local gameStage = saveTrace >> 24
        if gameStage >= 0xE0 then
            fail("real_save_journal_error stage=" .. gameStage
                 .. " detail=" .. (saveTrace & 0xFFFFFF))
        end
        if gameStage == 8 and counter ~= 2 then
            if counter ~= 3 then fail("third_save_counter_mismatch") end
            report("R8_PC_CONTROLLER_REAL_SAVE3_COMMITTED counter=3 stage=8")
            stage = "save3-settle"
            schedule({{0, 240}})
            return
        end
        if frame - saveStarted >= 12000 then
            fail("third_save_transaction_timeout")
        end
        if stage == "await-real-save" then
            emu:setKeys(0)
            return
        end
        if (emu:read32(saveDialogTraceAddr) & 256) ~= 0 then
            report("R8_PC_CONTROLLER_REAL_SAVE3_JOURNAL_ENTERED")
            stage = "await-real-save"
            emu:setKeys(0)
            return
        end
        if drive() then fail("real_save_dialog_not_entered") end
        return
    end
    if stage == "save3-settle" then
        if drive() then
            local counter = emu:read32(saveCounterAddr)
            if counter ~= 3 or (emu:read32(gameSaveTraceAddr) >> 24) ~= 8 then
                fail("third_save_not_settled")
            end
            checkSavedBox15()
            if roundtripMode then
                report("R8_PC_CONTROLLER_ROUNDTRIP_SAVE3_PASS source_box=15 destination_box=43 counter=3")
            else
                report("R8_PC_CONTROLLER_JUMP_SAVE3_PASS global_box=15 counter=3")
            end
            os.exit(0)
        end
        return
    end
    if stage == "commit-await" then
        emu:setKeys(0)
        local witness = emu:read32(uiTrace)
        local marker = emu:read32(chooserTraceAddr)
        if witness == expectedStage and (marker & 0x40000000) ~= 0 then
            local page = (marker >> 16) & 0xFF
            local localBox = marker & 0xFFFF
            if page ~= 1 or localBox ~= 0 then
                fail("jump_committed_wrong_resident_box page=" .. page
                     .. " local_box=" .. localBox)
            end
            report("R8_PC_CONTROLLER_JUMP_COMMIT_PASS global_box=15 resident_page=1 local_box=0")
            if withdrawMode then
                stage = "withdraw-down-from-title"
                schedule({{DOWN, 2}, {0, 115}})
                return
            end
            if not saveMode then
                os.exit(0)
            end
            if emu:read32(saveCounterAddr) ~= 2 then
                fail("jump_save_requires_previously_audited_save2")
            end
            stage = "exit-storage-prompt"
            schedule({{B, 2}, {0, 125}})
            return
        end
        if frame - commitStarted >= 2500 then
            fail("jump_commit_never_reopened_storage")
        end
        return
    end
end)
