-- C3C pilot: actual simulated controller input at the real Region Hub PC.
-- No direct game-function calls, no custom save writes, disposable Save1 only.
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
assert(action == "entry" or action == "preview-cancel" or action == "jump-commit")
local chooserTraceAddr = action ~= "entry" and address("A2_R8_PC_CHOOSER_TRACE") or nil
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
           .. " chooser_trace=" .. (chooserTraceAddr and emu:read32(chooserTraceAddr) or "none"))
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
local function verifyChooser(expectedPreview)
    local witness = emu:read32(uiTrace)
    local snapshot = emu:read32(chooserTraceAddr)
    local preview = snapshot & 0xFFFF
    local active = (snapshot >> 16) & 0xFF
    if witness ~= expectedChooserStage or (snapshot & 0x80000000) == 0
       or active ~= 0 or preview ~= expectedPreview then
        fail("chooser_mismatch expected_preview=" .. expectedPreview
             .. " actual_preview=" .. preview .. " active=" .. active
             .. " state=" .. witness)
    end
    report("R8_PC_CONTROLLER_CHOOSER_OK preview=" .. preview
           .. " active_page=" .. active)
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
            verifyChooser(0)
            if action == "jump-commit" then
                stage = "commit-right"
            else
                stage = "preview-right"
            end
            schedule({{R, 2}, {0, 75}})
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
            os.exit(0)
        end
        if frame - commitStarted >= 2500 then
            fail("jump_commit_never_reopened_storage")
        end
        return
    end
end)
