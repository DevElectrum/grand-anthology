-- True GBA mGBA test of three WORLD TRANSIT gate scripts with SIX party slots.
-- Works against make release, not a test-only gameplay binary. The suite seeds
-- six valid-format BoxPokemon records in throwaway emulator RAM to isolate
-- continuity (party/held item) from the story's starter-selection pacing.
-- Source ABI: include/pokemon.h; 100-byte Pokemon, BoxPokemon 80 bytes,
-- PID=OTID=0 => unencrypted secure data; checksum is 24 LE halfword sum.
-- This is NOT a claim about normally obtaining these six monsters.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local S = dofile(assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols required"))
local F = require("lib").new(S, "WorldRegionPartyTravel")

local HUB, HUB_MAP = 100, 0
local REGION_KANTO, REGION_JOHTO, REGION_HOENN = 1, 2, 3
local PARTY_BYTES, MON_BYTES = 600, 100
local destinations = {
  { name="Kanto", region=REGION_KANTO, x=11 },
  { name="Johto", region=REGION_JOHTO, x=16 },
  { name="Hoenn", region=REGION_HOENN, x=21 },
}

local function location()
  return ("group=%d map=%d region=%d pos=(%d,%d)"):format(
    F.grp(), F.mapn(), F.r8(F.sb2() + S.SaveBlock2.currentRegion), F.pos())
end

local function snapshot()
  local out = {}
  for i=0,PARTY_BYTES-1 do out[i+1] = F.r8(S.gParties + i) end
  return out
end

local function sameParty(expected)
  if F.r8(S.gPartiesCount) ~= 6 then return false, "party count changed" end
  for i=1,#expected do
    local b = F.r8(S.gParties + i - 1)
    if b ~= expected[i] then
      return false, ("slot %d byte %d expected 0x%02X got 0x%02X"):format(
        (i-1)//MON_BYTES, (i-1)%MON_BYTES, expected[i], b)
    end
  end
  return true, "all six 100-byte Pokemon structs match including held-item bits"
end

-- The exact six-byte-on-eight-byte struct offsets are already part of the
-- tested GenLuaSymbols.py's Pokemon ABI record. Keep fixture data in emulator
-- RAM only: no save files, game binaries or ROM source assets are modified.
local function seedSixMons()
  local species = { 1, 4, 7, 25, 152, 258 } -- Kanto + Johto + Hoenn
  for slot=0,5 do
    local base = S.gParties + slot*MON_BYTES
    for i=0,MON_BYTES-1 do F.w8(base+i, 0) end
    -- nickname and OT name terminate immediately; GBA EOS is 0xFF.
    F.w8(base+8, 0xFF); F.w8(base+20, 0xFF)
    F.w8(base+19, 0x02) -- BoxPokemon.hasSpecies=1
    -- Secure substruct 0, PID=0, OT ID=0, no encryption/permutation.
    local spec = species[slot+1]
    local held = slot == 0 and 28 or 0 -- Potion; test held-item survival.
    F.w16(base+32, spec) -- species:11 / teraType:5
    F.w16(base+34, held) -- heldItem:10
    F.w16(base+36, 125)  -- level-five EXP; zero remaining high bits
    F.w8(base+41, 70)    -- friendship
    F.w16(base+42, 1)    -- Poké Ball id 1
    -- First 48 decrypted bytes, checksum summed as little-endian u16.
    local checksum=0
    for ofs=32,78,2 do checksum=(checksum+F.r16(base+ofs))%65536 end
    F.w16(base+28,checksum)
    F.w8(base+84,5)  -- level
    F.w8(base+85,0xFF) -- no mail
    F.w16(base+86,20); F.w16(base+88,20) -- HP
    for ofs=90,98,2 do F.w16(base+ofs,10) end
  end
  F.w8(S.gPartiesCount,6)
end

local function findNewKeyItem(before)
  local after = select(2,F.keyItemSlot(0xFFFF))
  local old = {}
  for _,id in ipairs(before) do old[id] = true end
  for _,id in ipairs(after) do if not old[id] then return id end end
  return nil
end

local function inHub()
  return F.grp()==HUB and F.mapn()==HUB_MAP
end

local function waitUntil(predicate,budget)
  for _=1,budget do
    if predicate() then F.idle(150); return true end
    F.press("A",2); F.idle(22)
  end
  return predicate()
end

local function crossGate(x)
  -- Attendants stand at (11,2), (16,2), (21,2); talk from row 3.
  if not F.route({{16,4},{x,4},{x,3}},"gate_"..x) then return false end
  F.face("Up")
  F.press("A",3); F.idle(90)
  F.press("Up",2); F.idle(18) -- YES even when the default is NO
  F.press("A",2); F.idle(60)
  return waitUntil(function() return F.grp()~=HUB end,130)
end

local function returnViaHubPass(item)
  -- The player's real key-item callback; never a debug teleport.
  F.w16(F.sb1()+S.SaveBlock1.registeredItem,item)
  F.dismiss(65)
  F.press("Select",2); F.idle(100)
  -- Hub Pass first displays a paginated item message, THEN opens a
  -- confirmation whose default cursor is NO. Blind A-mashing accepts NO
  -- and leaves the room. Hold Up before EVERY A, including after pages:
  -- this makes YES explicit on the frame the actual prompt appears.
  for attempt=1,115 do
    if inHub() then F.idle(150); return true end
    F.press("Up",2); F.idle(18)
    F.press("A",2); F.idle(40)
  end
  return inHub()
end

F.run(function()
  if not F.boot(HUB) then F.check("fresh first boot to hub",false); F.finish(); return end
  F.check("fresh release party starts empty",F.r8(S.gPartiesCount)==0)
  local _, keysBefore = F.keyItemSlot(0xFFFF)
  seedSixMons()
  local initial = snapshot()
  F.check("seeded six checksum-protected Pokemon in emulator RAM",
    F.r8(S.gPartiesCount)==6 and F.r8(S.gParties+19)==2
       and F.r16(S.gParties+34)==28)
  local hubPass
  for index, dest in ipairs(destinations) do
    if not inHub() then F.check(dest.name.." starts at World Transit",false,location()); break end
    local crossed = crossGate(dest.x)
    F.check(dest.name.." real attendant warps to regional campaign",
      crossed and F.r8(F.sb2()+S.SaveBlock2.currentRegion)==dest.region, location())
    if not crossed then F.shot("cross_"..dest.name.."_failed"); break end
    local equal, note = sameParty(initial)
    F.check(dest.name.." gate retains all 6 Pokemon and held items",equal,note)
    F.check(dest.name.." gate retains six slot party count",F.r8(S.gPartiesCount)==6)
    if index==1 then
      hubPass=findNewKeyItem(keysBefore)
      F.check("first gate gives real Hub Pass",hubPass~=nil,"item="..tostring(hubPass))
      if not hubPass then break end
    end
    local back=returnViaHubPass(hubPass)
    F.check(dest.name.." Hub Pass returns to World Transit",back,location())
    if not back then F.shot("return_"..dest.name.."_failed");break end
    equal,note=sameParty(initial)
    F.check(dest.name.." roundtrip retains byte-identical 6-slot party",equal,note)
  end

  -- Save/Continue is an actual game operation on a disposable battery-save
  -- file created by mgba-run.sh, not a savestate or RAM snapshot. Every gate
  -- has already run, and we are back in the hub with Hoenn selected.
  local ready = inHub() and F.r8(F.sb2()+S.SaveBlock2.currentRegion)==REGION_HOENN
  F.check("all travel finished in hub with Hoenn as active campaign",ready,location())
  if ready and hubPass then
    local okBefore, detailBefore = sameParty(initial)
    F.check("before save six Pokemon still match the first snapshot",okBefore,detailBefore)
    F.press("Start",2); F.idle(100)
    local selected = F.selectStartIcon(7) -- USM_ICO_SAVE (from existing StoryProgress suite)
    F.check("normal production Start menu exposes Save",selected)
    if selected then
      -- Follow the existing StoryProgress save input cadence on the real
      -- graphical Start menu. Do NOT touch a user's battery save.
      F.press("A",2); F.idle(90)
      F.press("A",2); F.idle(90)
      F.press("A",2); F.idle(700)
      F.shot("saved_six_party_after_three_real_gates")
      client.reboot_core()
      F.idle(240)
      local continued = F.boot(HUB)
      F.check("Continue reloads saved World Transit hub from battery flash",continued,location())
      if continued then
        local equal, detail = sameParty(initial)
        F.check("save/restart restores all six unmodified 100-byte Pokemon",equal,detail)
        F.check("save/restart retains all six party slots",F.r8(S.gPartiesCount)==6)
        F.check("save/restart retains Hoenn as persisted active campaign",
          F.r8(F.sb2()+S.SaveBlock2.currentRegion)==REGION_HOENN,location())
        local slot=F.keyItemSlot(hubPass)
        F.check("save/restart retains usable Hub Pass key item",
          slot>=0,"item="..hubPass.." slot="..slot)
        F.check("save/restart stamps latest supported save format",
          F.r8(F.sb2()+S.SaveBlock2.saveVersion)==S.SAVE_FORMAT_VERSION,
          "saveVersion="..F.r8(F.sb2()+S.SaveBlock2.saveVersion))
        F.shot("continued_six_party")
      end
    end
  end
  F.finish()
end)
