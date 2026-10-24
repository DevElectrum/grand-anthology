-- Optimized GBA/mGBA scripted-legendary battle START smoke.
-- Uses the actual Granite Cave Steven room and overworld object interaction.
-- Seeds post-letter + two-badge story flags and a legal party in disposable RAM.
-- Catch/faint/revisit/save acceptance is NOT asserted by this suite.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols required"))
local F=require("lib").new(S,"WorldDiancieStatic")

local GROUP, MAP=24,10 -- gMapGroup_Dungeons/GraniteCave_StevensRoom
local CAVE_1F=7 -- same group, entry map; enter Steven's room by its actual warp
local SPECIES_DIANCIE=719
local FLAG_DELIVERED_STEVEN_LETTER=0xBD
-- Hoenn's Stone/Knuckle badges: SYSTEM_FLAGS 0x948 plus 7/8, respectively.
local FLAG_STONE_BADGE=0x94F
local FLAG_KNUCKLE_BADGE=0x950
local FLAG_HIDE_GRANITE_CAVE_STEVEN=0x341
local FLAG_HIDE_DIANCIE=0xDD4

local function setFlag(id, enabled)
  local addr=F.sb1()+S.SaveBlock1.flags+(id//8)
  local mask=1<<(id%8)
  local old=F.r8(addr)
  F.w8(addr,enabled and (old|mask) or (old&(~mask&0xFF)))
end
local function getFlag(id)
  return (F.r8(F.sb1()+S.SaveBlock1.flags+(id//8))&(1<<(id%8)))~=0
end
-- Release builds intentionally disable R+Start developer menus. Stage only
-- a legal one-Pokemon party, and relocate one disposable battery save so the
-- next authentic Continue loads the real map/event logic.
local function seedParty()
  local base=S.gParties
  for i=0,99 do F.w8(base+i,0) end
  F.w8(base+8,0xFF);F.w8(base+20,0xFF)
  F.w8(base+19,2)
  F.w16(base+32,1)  -- Bulbasaur
  F.w16(base+36,125)
  F.w8(base+41,70)
  F.w16(base+42,1)
  local sum=0
  for i=32,78,2 do sum=(sum+F.r16(base+i))&0xFFFF end
  F.w16(base+28,sum)
  F.w8(base+84,5)
  F.w8(base+85,0xFF)
  F.w16(base+86,20);F.w16(base+88,20)
  for i=90,98,2 do F.w16(base+i,10) end
  F.w8(S.gPartiesCount,1)
end

local function stageSaveAtGraniteCaveEntrance()
  -- A full real game save is written before core reset, not a savestate.
  -- We seed the cave's *previous* map and enter Steven's room through its
  -- real overworld warp. Saving directly inside another map without loading
  -- that map's templates bypasses the transition script and saved object
  -- arrays, producing a false negative even if real map entry is correct.
  F.press("Start",2);F.idle(100)
  if not F.selectStartIcon(7) then return false,"no Save action" end
  local base=F.sb1()
  local group=base+S.SaveBlock1.mapGroup
  F.w8(group,GROUP)
  F.w8(base+S.SaveBlock1.mapNum,CAVE_1F)
  F.w8(group+2,0) -- ordinary Granite Cave 1F map
  F.w16(base+S.SaveBlock1.x,5) -- just above the real Steven room warp
  F.w16(base+S.SaveBlock1.y,9)
  F.w16(group+4,0xFFFF)
  F.w16(group+6,0xFFFF)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(700)
  client.reboot_core()
  F.idle(240)
  return F.boot(GROUP,true), "saved at Granite Cave 1F + GBA core reset + Continue"
end
-- The disposable fixture changes the *saved map* from World Transit to the
-- real cave entrance. Continue restores World Transit's player ObjectEvent,
-- including its old elevation nibble. That does not happen when the player
-- walks into Granite Cave in normal play. Correct this fixture-only mismatch
-- so the original elevation-3 cave doorway can execute its actual warp.
local function normalizeCavePlayerElevation()
  for i=0,15 do
    local addr=S.gObjectEvents+i*S.ObjectEvent.stride
    if (F.r8(addr)&1)~=0 and F.r8(addr+8)==255 then
      local previous=F.r8(addr+11)
      F.w8(addr+11,0x33)
      return true, ("player original elevation nibble byte=0x%02X, normalized to cave 0x33"):format(previous)
    end
  end
  return false,"active player ObjectEvent not found"
end

local function staticObject()
  for i=0,15 do
    local addr=S.gObjectEvents+i*S.ObjectEvent.stride
    if (F.r8(addr)&1)==1 then
      local gfx=F.r16(addr+S.ObjectEvent.graphicsId)
      if (gfx&0x4000)~=0 and (gfx&0x0FFF)==SPECIES_DIANCIE then
        return addr, F.rs16(addr+S.ObjectEvent.x)-7,F.rs16(addr+S.ObjectEvent.y)-7
      end
    end
  end
  return nil
end
local function where()
  local x,y=F.pos()
  return ("map=%d/%d pos=(%d,%d) ow=%s"):format(F.grp(),F.mapn(),x,y,tostring(F.ow()))
end
-- Distinguish a broken ROM warp from an invalid synthetic Save/Continue.
-- Source map data alone cannot prove the *live* MapHeader was restored.
local function liveWarpState()
  local events=F.r32(S.gMapHeader+4)
  if events==0 then return false,false,false,"gMapHeader.events is NULL" end
  local count=F.r8(events+1)
  local list=F.r32(events+8)
  local found=false
  if count<=32 and list~=0 then
    for i=0,count-1 do
      local ptr=list+8*i
      if F.rs16(ptr)==5 and F.rs16(ptr+2)==10 and F.r8(ptr+4)==3
          and F.r8(ptr+6)==MAP and F.r8(ptr+7)==GROUP then
        found=true
      end
    end
  end
  local avatar=nil
  for i=0,15 do
    local addr=S.gObjectEvents+i*S.ObjectEvent.stride
    if (F.r8(addr)&1)~=0 and F.r8(addr+8)==255 then avatar=addr;break end
  end
  if not avatar then
    return found,false,false,("events=0x%X count=%d warps=0x%X; no player"):format(events,count,list)
  end
  local elev=F.r8(avatar+11)>>4
  local behavior=F.r8(avatar+30)
  local facing=F.r16(avatar+24)&0xF
  local detail=("events=0x%X count=%d warps=0x%X elevation=%d behavior=%d facing=%d avatarflags=0x%02X")
    :format(events,count,list,elev,behavior,facing,F.r8(S.gPlayerAvatar))
  return found,behavior==101,elev==3 and facing==2,detail
end

local function waitBattle()
  for i=1,1100 do
    if i%10==0 then F.press("A",2) end
    F.idle(2)
    if not F.ow() and F.battlers()>=2 then return true end
    if i%100==0 and F.reportCrash("diancie_battle_"..i) then return false end
  end
  return not F.ow() and F.battlers()>=2
end

F.run(function()
  if not F.boot(100) then F.check("boot World Transit",false);F.finish();return end
  seedParty()
  F.check("legitimate battle precondition: nonempty party",F.r8(S.gPartiesCount)==1)
  setFlag(FLAG_DELIVERED_STEVEN_LETTER,true)
  setFlag(FLAG_STONE_BADGE,true)
  setFlag(FLAG_KNUCKLE_BADGE,true)
  setFlag(FLAG_HIDE_GRANITE_CAVE_STEVEN,true)
  setFlag(FLAG_HIDE_DIANCIE,true) -- map transition must reconcile it
  local continued,reason=stageSaveAtGraniteCaveEntrance()
  F.check("real Granite Cave entrance loaded from Save/Continue",
    continued and F.ow() and F.grp()==GROUP and F.mapn()==CAVE_1F,
    reason.." "..where())
  if not continued or F.mapn()~=CAVE_1F then F.finish();return end
  local normalized, elevationDetail=normalizeCavePlayerElevation()
  F.check("fixture player elevation matches real Granite Cave 1F",normalized,elevationDetail)
  if not normalized then F.finish();return end
  local reachedDoor=F.step("Down")
  F.idle(22)
  local xDoor,yDoor=F.pos()
  F.check("entered Granite Cave north-arrow doorway tile",
    reachedDoor and xDoor==5 and yDoor==10 and F.mapn()==CAVE_1F,where())
  if not reachedDoor or xDoor~=5 or yDoor~=10 then F.finish();return end
  -- This tile is MB_NORTH_ARROW_WARP. The southern neighbour (5,11)
  -- is solid cave wall; approach is from (5,9), then face north *while
  -- standing on the arrow*. A two-frame tap is not a reliable held-D-pad
  -- field input: hold Up through a complete field-control interval.
  F.press("Up",1)
  F.idle(16)
  local xTurn,yTurn=F.pos()
  F.check("turned north without leaving Granite Cave warp tile",
    (xTurn==5 and yTurn==10) or F.mapn()==MAP,where())
  if F.mapn()==CAVE_1F and (xTurn~=5 or yTurn~=10) then F.finish();return end
  if F.mapn()==CAVE_1F then
    local hasWarp,isArrow,playerReady,detail=liveWarpState()
    F.check("runtime map header has real Steven-room warp",hasWarp,detail)
    F.check("runtime player tile has north-arrow metatile behavior",isArrow,detail)
    F.check("runtime player faces north at cave elevation three",playerReady,detail)
    if not (hasWarp and isArrow and playerReady) then F.finish();return end
  end
  local entered = F.mapn()==MAP
  for _=1,90 do
    if entered then break end
    F.press("Up",1)
    if F.mapn()==MAP then entered=true end
  end
  F.idle(350)
  F.check("real overworld north-arrow warp entered Steven room",
    entered and F.ow() and F.grp()==GROUP and F.mapn()==MAP,where())
  if F.mapn()~=MAP or not F.ow() then F.finish();return end
  F.check("disposable Save/Continue retained Steven's delivered letter",
    getFlag(FLAG_DELIVERED_STEVEN_LETTER))
  F.check("disposable Save/Continue retained both required badges",
    getFlag(FLAG_STONE_BADGE) and getFlag(FLAG_KNUCKLE_BADGE))
  F.check("save/reload restored eligible player party",F.r8(S.gPartiesCount)==1)
  F.check("Diancie is visible after Steven letter and two badges before Champion",
    not getFlag(FLAG_HIDE_DIANCIE),where())
  local obj,x,y=staticObject()
  F.check("Diancie species object spawned at canonical tile",
    obj~=nil and x==10 and y==9,
    obj and ("object at "..x..","..y) or "not spawned")
  if not obj then F.finish();return end
  local routed=F.route({{7,4},{8,4},{8,7},{9,9}},"diancie_approach")
  F.check("reachable on foot adjacent to Diancie",routed,where())
  if not routed then F.finish();return end
  F.face("Right")
  F.press("A",2)
  local battle=waitBattle()
  F.check("the real scripted Diancie battle opened",battle,
    "battlers="..F.battlers().." "..where())
  if battle then
    F.idle(70)
    local enemy=F.r16(S.gBattleMons+S.BattlePokemon.size)
    F.check("opposing battle Pokemon is Diancie",enemy==SPECIES_DIANCIE,
      "opponent species="..enemy)
    F.shot("actual_diancie_battle")
  else
    F.check("opposing battle Pokemon is Diancie",false,"battle never started")
    F.shot("diancie_battle_failed")
  end
  F.finish()
end)
