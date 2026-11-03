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
local A, B = 1 << C.GBA_KEY.A, 1 << C.GBA_KEY.B
local START = 1 << C.GBA_KEY.START
local UP, DOWN = 1 << C.GBA_KEY.UP, 1 << C.GBA_KEY.DOWN
local LEFT, RIGHT = 1 << C.GBA_KEY.LEFT, 1 << C.GBA_KEY.RIGHT
local frame, stage = 0, "boot"
local queue, index, remaining = {}, 1, 0
local moveX, moveY, retries, clicks = 0, 0, 0, 0
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
    report("R8_PC_CONTROLLER_FAIL " .. reason .. " ui_trace=" .. emu:read32(uiTrace))
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
local function move(key, dx, dy)
    local x, y = coords()
    moveX, moveY = x + dx, y + dy
    stage = "moving"
    schedule({{key, 2}, {0, 40}})
end
callbacks:add("frame", function()
    frame = frame + 1
    if frame > 120000 then fail("global_timeout") end
    if stage ~= "boot" and emu:read32(uiTrace) == expectedStage then
        emu:setKeys(0)
        local x, y = coords()
        report("R8_PC_CONTROLLER_SMOKE_PASS genuine_pc_ui_active=1 x=" .. x .. " y=" .. y)
        os.exit(0)
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
        if drive() then
            clicks = clicks + 1
            local witness = emu:read32(uiTrace)
            report("R8_PC_CONTROLLER_MENU_PROGRESS click=" .. clicks .. " trace=" .. witness)
            if witness == 0 and clicks >= 5 then fail("pc_field_callback_not_entered") end
            if clicks >= 9 then fail("pc_storage_screen_never_active") end
            schedule({{A, 2}, {0, 115}})
        end
        return
    end
end)
