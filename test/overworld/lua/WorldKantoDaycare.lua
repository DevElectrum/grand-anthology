-- Real production-ROM/mGBA smoke for Kanto Route 5's optional two-parent daycare.
-- The original one-mon FRLG caretaker remains at (4,4); two new NPCs
-- forward to Johto's shared breeding interactions at (2,4) and (6,4).
-- Tests actual loaded ObjectEvents, approach and dialogue after a real
-- doorstep warp, using disposable flash Save/Continue to stage the exterior.
-- Not eggs, compatibility, hatching or postgame map traversal.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols required"))
local F=require("lib").new(S,"WorldKantoDaycare")

local GROUP, MAP=51,0 -- gMapGroup_IndoorRoute5_Frlg, Route5_PokemonDayCare_Frlg
local OUTSIDE_GROUP, OUTSIDE_MAP=37,23 -- gMapGroup_OutdoorKanto, Route5_Frlg

local function where()
  local x,y=F.pos()
  return ("grp=%d map=%d (%d,%d)"):format(F.grp(),F.mapn(),x,y)
end

local function stageAtRoute5Door()
  -- Important: Save/Continue into the *interior* reuses stale World
  -- Transit's serialized object pool. The first version of this test saw
  -- hub NPCs at (13,10)/(14,10) instead of the new daycare NPCs.
  -- Save outside the house, walk through its ACTUAL door, and let the
  -- production warp/map transition instantiate the new map objects.
  F.press("Start",2);F.idle(100)
  if not F.selectStartIcon(7) then return false,"Save action unavailable" end
  local base=F.sb1()
  local loc=base+S.SaveBlock1.mapGroup
  F.w8(loc,OUTSIDE_GROUP)
  F.w8(base+S.SaveBlock1.mapNum,OUTSIDE_MAP)
  F.w8(loc+2,0)
  F.w16(base+S.SaveBlock1.x,23) -- south of Route 5 daycare door
  F.w16(base+S.SaveBlock1.y,26) -- exterior warp at (23,25)
  F.w16(loc+4,0xFFFF)
  F.w16(loc+6,0xFFFF)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(700)
  client.reboot_core()
  F.idle(240)
  return F.boot(OUTSIDE_GROUP,true),"battery Save/core reboot/Continue outside Route 5 daycare"
end

local function findObject(localId)
  for i=0,15 do
    local b=S.gObjectEvents+i*S.ObjectEvent.stride
    if (F.r8(b)&1)==1 and F.r8(b+S.ObjectEvent.localId)==localId then
      return {x=F.rs16(b+S.ObjectEvent.x)-7,
              y=F.rs16(b+S.ObjectEvent.y)-7}
    end
  end
  return nil
end

local function scriptRunning()
  local ptr=F.r32(S.sGlobalScriptContext+S.ScriptCtx.scriptPtr)
  return ptr>=0x08000000 and ptr<0x0A000000
end

local function talkFrom(x,y,dir,label)
  local ok=F.route({{x,y}},label.."_approach")
  local px,py=F.pos()
  F.check(label.." approach tile walkable",ok,
    ("actual=(%d,%d) target=(%d,%d)"):format(px,py,x,y))
  if not ok then return false end
  F.face(dir)
  F.press("A",2);F.idle(35)
  local opened=scriptRunning()
  F.check(label.." actual NPC interaction opens script",opened,
    ("grp=%d map=%d"):format(F.grp(),F.mapn()))
  F.shot(label.."_dialogue")
  F.dismiss(24)
  return opened
end

F.run(function()
  if not F.boot(100) then
    F.check("fresh optimized ROM reaches World Transit",false)
    F.finish();return
  end
  F.check("fresh optimized ROM reaches World Transit",F.grp()==100)
  local loaded,reason=stageAtRoute5Door()
  F.check("Kanto Route 5 exterior loads from real Save/Continue",
    loaded and F.ow() and F.grp()==OUTSIDE_GROUP and F.mapn()==OUTSIDE_MAP,
    reason.." "..where())
  if not loaded or F.grp()~=OUTSIDE_GROUP or F.mapn()~=OUTSIDE_MAP then F.finish();return end
  F.shot("route5_before_real_door")
  local x,y=F.pos()
  F.check("Route 5 saved doorstep is south of the real daycare warp",x==23 and y==26,where())
  if x~=23 or y~=26 then F.finish();return end
  -- Ordinary player movement through the actual exterior entrance, not
  -- a direct-save teleport or a debug menu warp.
  for t=1,12 do
    if F.grp()==GROUP and F.mapn()==MAP then break end
    F.press("Up",4);F.idle(45)
  end
  F.idle(100)
  local entered=F.ow() and F.grp()==GROUP and F.mapn()==MAP
  F.check("real Route 5 daycare door loads live map NPC templates",entered,where())
  if not entered then F.shot("route5_door_not_entered");F.finish();return end
  F.shot("route5_daycare_three_services")
  local expected={{4,4,"original FRLG one-mon caretaker"},
                  {2,4,"Kanto two-parent breeding lady"},
                  {6,4,"Kanto two-parent egg collector"}}
  for id,item in ipairs(expected) do
    local o=findObject(id)
    F.check(item[3].." loaded at intended coordinate",
      o~=nil and o.x==item[1] and o.y==item[2],
      o and ("actual=(%d,%d)"):format(o.x,o.y) or "ObjectEvent missing")
  end
  -- Only assert reachability and actual dialogue, not the result of an
  -- intentionally unprimed daycare (no deposited compatible parents yet).
  talkFrom(2,5,"Up","breeding_lady")
  talkFrom(6,5,"Up","egg_collector")
  F.check("Kanto two-parent interactions retain map identity",
    F.grp()==GROUP and F.mapn()==MAP)
  F.finish()
end)
