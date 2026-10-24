-- Real production-ROM/mGBA smoke for Kanto Route 5's optional two-parent daycare.
-- The original one-mon FRLG caretaker remains at (4,4); two new NPCs
-- forward to Johto's shared breeding interactions at (2,4) and (6,4).
-- Tests actual loaded ObjectEvents, approach and dialogue in a disposable
-- GBA flash-save/Continue, not eggs, compatibility, hatching or postgame maps.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols required"))
local F=require("lib").new(S,"WorldKantoDaycare")

local GROUP, MAP=51,0 -- gMapGroup_IndoorRoute5_Frlg, Route5_PokemonDayCare_Frlg

local function stageInRoute5Daycare()
  -- Use a real flash save and core reset so the production map loader
  -- instantiates the map's objects, rather than injecting ObjectEvents.
  F.press("Start",2);F.idle(100)
  if not F.selectStartIcon(7) then return false,"Save action unavailable" end
  local base=F.sb1()
  local loc=base+S.SaveBlock1.mapGroup
  F.w8(loc,GROUP)
  F.w8(base+S.SaveBlock1.mapNum,MAP)
  F.w8(loc+2,0)
  F.w16(base+S.SaveBlock1.x,4)
  F.w16(base+S.SaveBlock1.y,6)
  F.w16(loc+4,0xFFFF)
  F.w16(loc+6,0xFFFF)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(700)
  client.reboot_core()
  F.idle(240)
  return F.boot(GROUP,true),"battery Save/core reboot/Continue"
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
  local loaded,reason=stageInRoute5Daycare()
  F.check("Kanto Route 5 interior loads from real Save/Continue",
    loaded and F.ow() and F.grp()==GROUP and F.mapn()==MAP,
    reason..(" grp=%d map=%d"):format(F.grp(),F.mapn()))
  if not loaded or F.grp()~=GROUP or F.mapn()~=MAP then F.finish();return end
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
