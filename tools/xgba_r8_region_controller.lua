-- R8 C5 genuine controller transit from the existing audited Hub Save2.
-- Each of Kanto/Johto/Hoenn runs on a distinct disposable Save2 clone.
-- No direct game function calls, save writes, or artificial region mutation.
local function address(name)
    local str = assert(os.getenv(name), "Missing linked symbol " .. name)
    assert(str:match("^0[xX][%da-fA-F]+$"), "Invalid address " .. name)
    return tonumber(str)
end

local main = address("A2_GMAIN")
local overworld = address("A2_CB_OVERWORLD") & 0xFFFFFFFE
local sb1ptr = address("A2_SB1_PTR")
local sb2ptr = address("A2_R8_SB2_PTR")
local party = address("A2_R8_PARTIES")
local counterAddr = address("A2_SAVE_COUNTER")

local wanted = assert(os.getenv("R8_C5_REGION"), "Missing target region")
local regionInfo = {
    kanto = {id=1, x=11},
    johto = {id=2, x=16},
    hoenn = {id=3, x=21},
}
local target = assert(regionInfo[wanted], "Unsupported region " .. wanted)
local mode = os.getenv("R8_C5_ACTION") or "transit"
assert(mode == "transit" or mode == "save" or mode == "cold")
local usmTrace = mode == "save" and address("A2_USM_TRACE") or nil
local usmState = mode == "save" and address("A2_USM_STATE_PTR") or nil
local saveDialog = mode == "save" and address("A2_SAVE_DIALOG_TRACE") or nil
local gameSave = mode == "save" and address("A2_GAME_SAVE_TRACE") or nil
local A = 1 << C.GBA_KEY.A
local B = 1 << C.GBA_KEY.B
local UP = 1 << C.GBA_KEY.UP
local DOWN = 1 << C.GBA_KEY.DOWN
local LEFT = 1 << C.GBA_KEY.LEFT
local RIGHT = 1 << C.GBA_KEY.RIGHT
local frame, state, retries = 0, "boot", 0
local queue, queueIndex, remaining = {}, 1, 0
local expectedX, expectedY = 0, 0
local wp = 1
local route = {{16,6}, {target.x,6}, {target.x,3}}
local beforeParty, beforeCollection, initialCounter, transitStart
local menuAttempts, iconAttempts, saveStart = 0, 0, 0

local function report(msg)
    print(msg .. " target=" .. wanted .. " frame=" .. frame)
    io.stdout:flush()
end

local function fail(reason)
    emu:setKeys(0)
    report("R8_C5_REGION_FAIL " .. reason)
    os.exit(3)
end

local function run(actions)
    queue, queueIndex = actions, 1
    remaining = actions[1][2]
end

local function drive()
    if queueIndex > #queue then
        emu:setKeys(0)
        return true
    end
    emu:setKeys(queue[queueIndex][1])
    remaining = remaining - 1
    if remaining <= 0 then
        queueIndex = queueIndex + 1
        if queueIndex <= #queue then
            remaining = queue[queueIndex][2]
        end
    end
    return false
end

local function sb1()
    local p = emu:read32(sb1ptr)
    if p < 0x02000000 or p >= 0x02040000 then fail("invalid_saveblock1") end
    return p
end
local function sb2()
    local p = emu:read32(sb2ptr)
    if p < 0x02000000 or p >= 0x02040000 then fail("invalid_saveblock2") end
    return p
end
local function region()
    -- Source-pinned SaveBlock2.currentRegion = 0x90.
    return emu:read8(sb2() + 0x90)
end
local function mapgroup()
    return emu:read8(sb1() + 4)
end
local function coords()
    local p = sb1()
    local x, y = emu:read16(p), emu:read16(p + 2)
    if x >= 25 or y >= 18 then fail("unexpected_hub_coordinates") end
    return x, y
end
local function isHub()
    return (emu:read32(main + 4) & 0xFFFFFFFE) == overworld
        and mapgroup() == 100
end
local function checksum(start, count, seed)
    local hash = seed or 2166136261
    for i = 0, count - 1 do
        hash = ((hash ~ emu:read8(start + i)) * 16777619) & 0xFFFFFFFF
    end
    return hash
end
local function snapshotCollection()
    local hash = 2166136261
    -- Exactly 8 x 14 x 30 x 80 encrypted record bytes (metadata omitted).
    for page = 0, 7 do
        hash = checksum(0x20000000 + page * 34144 + 4, 33600, hash)
    end
    return hash
end
-- Only trust Save icon 7 when the actual graphical Start-menu task is active.
local function selectedSaveIcon()
    if mode ~= "save" or (emu:read32(usmTrace) & 3) ~= 3 then return nil end
    local p = emu:read32(usmState)
    if p < 0x02000000 or p >= 0x02040000 then return nil end
    local count = emu:read8(p + 23)
    local absolute = emu:read8(p + 5) + emu:read8(p + 10)
    if count < 1 or count > 12 or absolute >= count then return nil end
    return emu:read8(p + 11 + absolute)
end
local function move(key, dx, dy)
    local x,y=coords()
    expectedX, expectedY=x+dx,y+dy
    state="moving"
    run({{key,2},{0,40}})
end

callbacks:add("frame", function()
    frame=frame+1
    if frame > (mode == "transit" and 45000 or 100000) then
        fail("bounded_timeout_state_" .. state .. "_counter_" .. emu:read32(counterAddr))
    end
    if mode == "cold" then
        -- New XGBA process: real Continue must restore counter3 and target.
        if frame % 6000 == 0 then
            report("R8_C5_REGION_COLD_DIAG current_region=" .. region()
                   .. " group=" .. mapgroup()
                   .. " counter=" .. emu:read32(counterAddr))
        end
        if frame % 30 == 0
           and (emu:read32(main + 4) & 0xFFFFFFFE) == overworld
           and emu:read32(counterAddr) == 3
           and mapgroup() ~= 100 and region() == target.id then
            report("R8_C5_REGION_COLD_SAVE3_PASS counter=3 party_hash="
                   .. checksum(party, 600) .. " boxes_hash=" .. snapshotCollection())
            emu:setKeys(0)
            os.exit(0)
        end
        local tick = (frame - 1) % 30
        if tick < 3 then emu:setKeys(A)
        elseif tick >= 15 and tick < 17 then emu:setKeys(1 << C.GBA_KEY.START)
        else emu:setKeys(0) end
        return
    end
    if state=="boot" then
        if frame%30==0 and isHub() and emu:read32(counterAddr)==2 then
            report("R8_C5_REGION_HUB_SAVE2_READY")
            initialCounter=emu:read32(counterAddr)
            transitStart=frame
            beforeParty=checksum(party,600)
            beforeCollection=snapshotCollection()
            local actions={{0,240}}
            for i=1,18 do
                actions[#actions+1]={B,2}
                actions[#actions+1]={0,52}
            end
            run(actions)
            state="intro"
        else
            local tick=(frame-1)%30
            if tick<3 then emu:setKeys(A)
            elseif tick>=15 and tick<17 then emu:setKeys(1 << C.GBA_KEY.START)
            else emu:setKeys(0) end
        end
        return
    end
    if state=="intro" then
        if drive() then
            if not isHub() then fail("left_hub_during_intro") end
            local x,y=coords()
            if x~=16 or (y~=4 and y~=7) then fail("unexpected_save2_position_"..x.."_"..y) end
            state="navigate"
        end
        return
    end
    if state=="navigate" then
        if not isHub() then fail("left_hub_before_attendant") end
        local x,y=coords()
        local destination=route[wp]
        if destination and x==destination[1] and y==destination[2] then
            wp=wp+1
            destination=route[wp]
        end
        if not destination then
            state="face"
            run({{UP,2},{0,50}})
        elseif x<destination[1] then move(RIGHT,1,0)
        elseif x>destination[1] then move(LEFT,-1,0)
        elseif y<destination[2] then move(DOWN,0,1)
        else move(UP,0,-1) end
        return
    end
    if state=="moving" then
        if drive() then
            local x,y=coords()
            if x~=expectedX or y~=expectedY then
                retries=retries+1
                if retries>2 then fail("blocked_attendant_route_"..x.."_"..y) end
            else
                retries=0
            end
            state="navigate"
        end
        return
    end
    if state=="face" then
        if drive() then
            local x,y=coords()
            if x~=target.x or y~=3 then fail("bad_attendant_position") end
            state="talk"
            local actions={{A,2},{0,115}}
            -- Yes is the default for the real hub's MSGBOX_YESNO.
            for i=1,24 do
                actions[#actions+1]={A,2}
                actions[#actions+1]={0,120}
            end
            run(actions)
        end
        return
    end
    if state=="talk" then
        if mapgroup()~=100 and region()==target.id then
            state="postwarp"
            run({{0,150}})
            emu:setKeys(0)
        elseif drive() then
            fail("attendant_did_not_warp_region_"..region())
        end
        return
    end
    if state=="postwarp" then
        if drive() then
            if region()~=target.id or mapgroup()==100 then
                fail("wrong_warp_or_region_id")
            end
            local afterParty=checksum(party,600)
            local afterCollection=snapshotCollection()
            if afterParty~=beforeParty then
                fail("travel_changed_party_"..beforeParty.."_"..afterParty)
            end
            if afterCollection~=beforeCollection then
                fail("travel_changed_3360_box_records_"..beforeCollection.."_"..afterCollection)
            end
            if emu:read32(counterAddr)~=initialCounter then
                fail("travel_committed_unexpected_save")
            end
            local travelFrames = frame - transitStart
            if travelFrames > 18000 then fail("travel_frame_budget_exceeded_" .. travelFrames) end
            report("R8_C5_REGION_TRANSIT_FRAMES count=" .. travelFrames)
            report("R8_C5_REGION_TRANSIT_PASS party_and_3360_box_records_bit_identical=1"
                   .. " party_hash=" .. beforeParty .. " boxes_hash=" .. beforeCollection)
            if mode == "transit" then emu:setKeys(0); os.exit(0) end
            state = "save-open"
            run({{0, 240}, {1 << C.GBA_KEY.START, 2}, {0, 115}})
        end
        return
    end
    if state == "save-open" then
        if drive() then state = "save-seek" end
        return
    end
    if state == "save-seek" then
        if region() ~= target.id or mapgroup() == 100 then fail("save_menu_left_target_region") end
        local icon = selectedSaveIcon()
        if icon == nil then
            menuAttempts = menuAttempts + 1
            if menuAttempts > 24 then fail("destination_start_menu_never_verified") end
            local seq = {{B, 2}, {0, 90}}
            if menuAttempts > 6 then
                seq[#seq + 1] = {A, 2}
                seq[#seq + 1] = {0, 135}
            end
            seq[#seq + 1] = {1 << C.GBA_KEY.START, 2}
            seq[#seq + 1] = {0, 100}
            state = "save-open"
            run(seq)
            return
        end
        if icon ~= 7 then
            iconAttempts = iconAttempts + 1
            if iconAttempts > 12 then fail("destination_save_icon_not_found") end
            state = "save-open"
            run({{RIGHT, 2}, {0, 30}})
            return
        end
        report("R8_C5_REGION_REAL_SAVE_ICON_SELECTED")
        local seq = {}
        for i = 1, 9 do
            seq[#seq + 1] = {A, 2}
            seq[#seq + 1] = {0, 120}
        end
        state, saveStart = "save-confirm", frame
        run(seq)
        return
    end
    if state == "save-confirm" or state == "save-await" then
        local counter = emu:read32(counterAddr)
        local saveStage = emu:read32(gameSave) >> 24
        if saveStage >= 0xE0 then fail("journal_error_stage_" .. saveStage) end
        if counter == 3 and saveStage == 8 then
            report("R8_C5_REGION_REAL_SAVE3_COMMITTED counter=3 stage=8")
            state = "save-settle"
            run({{0, 240}})
            return
        end
        if frame - saveStart > 12000 then fail("destination_save3_timeout") end
        if state == "save-await" then emu:setKeys(0); return end
        if (emu:read32(saveDialog) & 256) ~= 0 then
            state = "save-await"
            emu:setKeys(0)
            return
        end
        if drive() then fail("destination_save_dialog_not_entered") end
        return
    end
    if state == "save-settle" then
        if drive() then
            if region() ~= target.id or mapgroup() == 100
                or emu:read32(counterAddr) ~= 3
                or checksum(party, 600) ~= beforeParty
                or snapshotCollection() ~= beforeCollection then
                fail("post_save3_destination_or_collection_changed")
            end
            report("R8_C5_REGION_SAVE3_PASS counter=3 party_hash="
                   .. beforeParty .. " boxes_hash=" .. beforeCollection)
            emu:setKeys(0)
            os.exit(0)
        end
        return
    end
end)
