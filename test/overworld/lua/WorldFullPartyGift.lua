-- A genuine RELEASE GBA game test of six-slot overflow to global PC.
-- The same engine API (ScriptGiveMon via givemon) is used by Oak and Elm's
-- regional starter scripts, and Birch's extra gift. This validates the ENGINE
-- box route, not a claim that any original NPC scene was traversed here.
-- Six old monsters are a test-only RAM fixture; the new gift is made by the
-- player's actual graphical Start -> Cheats -> Quick Pokemon menu.
-- mGBA runs only disposable .gba/.sav paths, never a real user save.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local S = dofile(assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols required"))
local F = require("lib").new(S, "WorldFullPartyGift")

local function snapshot(base,bytes)
  local t={}
  for i=0,bytes-1 do t[i+1]=F.r8(base+i) end
  return t
end

local function unchanged(base,bytes,expected)
  for i=0,bytes-1 do
    local got=F.r8(base+i)
    if got~=expected[i+1] then
      return false,("byte %d: was %d, became %d"):format(i,expected[i+1],got)
    end
  end
  return true,"byte-identical"
end

local function seedSix()
  -- Original validated full-party fixture: BoxPokemon header 32B,
  -- secure area 48B, 20B party extension; PID=OTID=0, checksum known.
  local mons={1,4,7,25,152,258}
  for slot=0,5 do
    local a=S.gParties+slot*100
    for ofs=0,99 do F.w8(a+ofs,0) end
    F.w8(a+8,255);F.w8(a+20,255);F.w8(a+19,2)
    F.w16(a+32,mons[slot+1]);F.w16(a+34,slot==0 and 28 or 0)
    F.w16(a+36,125);F.w8(a+41,70);F.w16(a+42,1)
    local checksum=0
    for ofs=32,78,2 do checksum=(checksum+F.r16(a+ofs))%65536 end
    F.w16(a+28,checksum)
    F.w8(a+84,5);F.w8(a+85,255)
    F.w16(a+86,20);F.w16(a+88,20)
    for ofs=90,98,2 do F.w16(a+ofs,10) end
  end
  F.w8(S.gPartiesCount,6)
end

local function firstBox()
  local ptr=F.r32(S.gPokemonStoragePtr)
  if ptr<0x02000000 or ptr>=0x02040000 then return nil end
  -- PokemonStorage.currentBox is one byte, then THREE padding bytes
  -- to align BoxPokemon.personality (u32); box0/slot0 starts at offset 4.
  -- boxNames begins at 0x8344 = 4 + 14 * 30 * 80, independently
  -- confirming this ABI. The old header comment saying 0x0001 was stale.
  return ptr+4
end

local function giftWithNormalUI()
  F.press("Start",2);F.idle(100)
  if not F.selectStartIcon(11) then return false end
  F.press("A",2);F.idle(120) -- safe curated CHEATS
  F.press("A",2);F.idle(120) -- Quick Pokemon
  F.press("A",2);F.idle(120) -- species #1
  F.press("A",2);F.idle(200) -- level #1; ScriptGiveMon
  return F.dismiss(30)
end

F.run(function()
  local started=F.boot(100)
  F.check("fresh Hub game ready for full-party gift",started)
  if not started then F.finish();return end
  local box=firstBox()
  F.check("shared PC storage allocated inside GBA EWRAM",box~=nil)
  if not box then F.finish();return end
  F.check("fresh shared PC box begins empty",(F.r8(box+19)&2)==0)
  seedSix()
  local party=snapshot(S.gParties,600)
  F.check("fixture occupies six independent party slots",F.r8(S.gPartiesCount)==6)
  local given=giftWithNormalUI()
  F.check("normal player Cheats Quick gift runs with a full party",given)
  F.check("full party stays at six (no overflow or replacement)",F.r8(S.gPartiesCount)==6)
  local same,why=unchanged(S.gParties,600,party)
  F.check("all original six Pokemon remain byte-identical after gift",same,why)
  F.check("gift occupies first global PC slot",(F.r8(box+19)&2)~=0)
  local boxed=snapshot(box,80)
  local hasPayload=false
  for i=1,80 do if boxed[i]~=0 then hasPayload=true;break end end
  F.check("PC gift has nonzero 80-byte boxed Pokemon payload",hasPayload)
  -- Persistence is the actual field Save dialog, not a savestate or injected
  -- flash: only after confirming Save does emulator core reset.
  if given and same and (F.r8(box+19)&2)~=0 then
    F.press("Start",2);F.idle(100)
    local save=F.selectStartIcon(7)
    F.check("game Save icon remains usable with 6 party plus 1 PC",save)
    if save then
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(700)
      client.reboot_core();F.idle(240)
      local loaded=F.boot(100)
      F.check("Continue reloads game with a boxed seventh Pokemon",loaded)
      if loaded then
        local restored=firstBox()
        F.check("shared global PC pointer restored after Continue",restored~=nil)
        if restored then
          local equal,detail=unchanged(restored,80,boxed)
          F.check("boxed starter-route gift is byte-identical after restart",equal,detail)
        end
        local equal,detail=unchanged(S.gParties,600,party)
        F.check("all six original party mons persist across Save/Continue",equal,detail)
        F.check("six party slots remain occupied after reload",F.r8(S.gPartiesCount)==6)
      end
    end
  end
  F.finish()
end)
