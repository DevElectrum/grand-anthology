-- Disposable A2 game boot probe for XGBA's real headless Lua core.
-- Requires a game-matched ELF symbol mapping. Never use on a live save.
local function envAddress(name)
    local value = os.getenv(name)
    assert(value and value:match("^0[xX][%da-fA-F]+$"), "Missing hexadecimal symbol: " .. name)
    return assert(tonumber(value))
end

local gMain = envAddress("A2_GMAIN")
local cbOverworld = envAddress("A2_CB_OVERWORLD")
local sb1Ptr = envAddress("A2_SB1_PTR")
local maskA = 1 << C.GBA_KEY.A
local maskStart = 1 << C.GBA_KEY.START
local frame = 0
local done = false

callbacks:add("frame", function()
    if done then
        return
    end
    frame = frame + 1
    if frame % 30 == 0 then
        local callback = emu:read32(gMain + 4) & 0xFFFFFFFE
        local sb1 = emu:read32(sb1Ptr)
        if callback == (cbOverworld & 0xFFFFFFFE)
            and sb1 >= 0x02000000 and sb1 < 0x02040000 then
            local group = emu:read8(sb1 + 4)
            if group == 100 then
                emu:setKeys(0)
                done = true
                print("A2_XGBA_REAL_NEW_GAME_HUB_PASS frames=" .. tostring(frame))
                io.stdout:flush()
                os.exit(0)
            end
        end
    end
    if frame >= 120000 then
        print("A2_XGBA_REAL_NEW_GAME_HUB_FAIL frames=" .. tostring(frame))
        io.stdout:flush()
        os.exit(2)
    end

    -- Mirrors the already-established WorldFirstBoot.lua title/intro cadence.
    -- It never reads or mutates another save, and the CI runner supplies only
    -- a fresh, create-only E6 image.
    local phase = (frame - 1) % 30
    if phase < 3 then
        emu:setKeys(maskA)
    elseif phase >= 15 and phase < 17 then
        emu:setKeys(maskStart)
    else
        emu:setKeys(0)
    end
end)
