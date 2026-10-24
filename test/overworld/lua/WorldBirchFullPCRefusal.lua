-- Authentic release GBA Route 101/Birch starter-storage refusal regression.
-- Travel via the normal World Transit Hoenn attendant and walk the real house,
-- Littleroot and Route 101. Stage only completed clock/rival prerequisite vars
-- and disposable six-party/420-PC mon fixtures; run Birch's real introductory
-- rescue cutscene and interact with his actual bag via the GBA A button.
-- No debug warps, no script bypasses, no user-owned SRAM.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols"))
local F=require("lib").new(S,"WorldBirchFullPCRefusal")
local function at(g,m) return F.grp()==g and F.mapn()==m end
local function where() return ("group=%d map=%d (%d,%d)"):format(F.grp(),F.mapn(),F.pos()) end
local function varAddress(id) return F.sb1()+S.SaveBlock1.vars+(id-0x4000)*2 end
local function var(id) return F.r16(varAddress(id)) end
local function flag(id)
  local b=F.r8(F.sb1()+S.SaveBlock1.flags+(id//8))
  return (b & (1<<(id%8)))~=0
end
local function pcBase()
  local ptr=F.r32(S.gPokemonStoragePtr)
  if ptr<0x02000000 or ptr+4+420*80>0x02040000 then return nil end
  return ptr+4
end
local function snap(base,size)
  local out={}
  for i=0,size-1 do out[i+1]=F.r8(base+i) end
  return out
end
local function equal(base,old)
  for i=1,#old do
    local actual=F.r8(base+i-1)
    if actual~=old[i] then
      return false,("byte %d originally=%d now=%d"):format(i-1,old[i],actual)
    end
  end
  return true,"byte-identical"
end
local function seedSix()
  local species={1,4,7,25,152,258}
  for slot=0,5 do
    local base=S.gParties+slot*100
    for i=0,99 do F.w8(base+i,0) end
    F.w8(base+8,0xff);F.w8(base+20,0xff);F.w8(base+19,2)
    F.w16(base+32,species[slot+1])
    F.w16(base+34,slot==0 and 28 or 0)
    F.w16(base+36,125);F.w8(base+41,70);F.w16(base+42,1)
    local checksum=0
    for i=32,78,2 do checksum=(checksum+F.r16(base+i))&0xffff end
    F.w16(base+28,checksum)
    F.w8(base+84,5);F.w8(base+85,0xff)
    F.w16(base+86,20);F.w16(base+88,20)
    for i=90,98,2 do F.w16(base+i,10) end
  end
  F.w8(S.gPartiesCount,6)
end
local function fillAllPc()
  local start=pcBase()
  if not start then return false end
  -- This only changes an initially empty disposable storage area.
  -- Personality=OTID=0 means unshuffled, unencrypted PokemonSubstruct0;
  -- species=1 and checksum=1 produce legitimate test BoxPokemon records.
  for slot=0,419 do
    local base=start+slot*80
    F.w8(base+8,0xff);F.w8(base+20,0xff)
    F.w8(base+19,2)
    F.w16(base+32,1)
    F.w16(base+28,1)
  end
  return true
end
local function everyBoxOccupied()
  local start=pcBase()
  if not start then return false,0 end
  for slot=0,419 do
    local base=start+slot*80
    if (F.r8(base+19)&2)==0 then return false,slot end
    if (F.r16(base+32)&0x7ff)~=1 then return false,slot end
    if F.r16(base+28)~=1 then return false,slot end
  end
  return true,420
end
-- Collision-guided bounded DFS: only D-pad inputs, never write coordinates.
local function walkMap(srcG,srcM,dstG,dstM,w,h,goalX,goalY,label)
  local visited,attempts={},0
  local function done()
    if not at(dstG,dstM) then return false end
    if srcG==dstG and srcM==dstM then
      local x,y=F.pos()
      return x==goalX and y==goalY
    end
    return true
  end
  local dirs={{"Up",0,-1,"Down"},{"Right",1,0,"Left"},{"Down",0,1,"Up"},{"Left",-1,0,"Right"}}
  local function search(depth)
    if done() then return true end
    if not at(srcG,srcM) or depth>w*h or attempts>=w*h*5 then return false end
    local x,y=F.pos()
    visited[x..","..y]=true
    local options={}
    for _,d in ipairs(dirs) do
      local nx,ny=x+d[2],y+d[3]
      if nx>=0 and ny>=0 and nx<w and ny<h and not visited[nx..","..ny] then
        options[#options+1]=d
      end
    end
    table.sort(options,function(a,b)
      return math.abs(goalX-(x+a[2]))+math.abs(goalY-(y+a[3]))
        <math.abs(goalX-(x+b[2]))+math.abs(goalY-(y+b[3]))
    end)
    for _,d in ipairs(options) do
      local nx,ny=x+d[2],y+d[3]
      if not visited[nx..","..ny] then
        attempts=attempts+1
        local before=where()
        local moved=F.step(d[1])
        F.idle(26)
        if attempts<=20 then
          F.L(("  Birch %s try %d %s moved=%s %s -> %s"):format(
            label,attempts,d[1],tostring(moved),before,where()))
        end
        if done() then return true end
        if moved and at(srcG,srcM) then
          local ax,ay=F.pos()
          if ax==nx and ay==ny then
            if search(depth+1) then return true end
            if not F.step(d[4]) then return false end
            F.idle(26)
          end
        end
      end
    end
    return false
  end
  local found=search(0)
  F.L(("Birch path %s: %d attempts, reached=%s %s"):format(label,attempts,tostring(found),where()))
  if not found then F.shot("birch_"..label.."_blocked") end
  return found
end

F.run(function()
  local boot=F.boot(100)
  F.check("disposable release new game begins in World Transit",boot and at(100,0))
  if not boot then F.finish();return end
  F.check("shared PC is originally empty",(function()
    local p=pcBase()
    return p~=nil and (F.r8(p+19)&2)==0
  end)())
  local approached=F.route({{16,4},{21,4},{21,3}},"hoenn_attendant")
  F.check("walked to Hoenn attendant",approached,where())
  if not approached then F.finish();return end
  F.face("Up");F.press("A",3);F.idle(90)
  F.press("Up",2);F.idle(18);F.press("A",2)
  local arrived=false
  for _=1,130 do
    if at(1,1) then F.idle(150);arrived=true;break end
    F.press("A",2);F.idle(25)
  end
  F.check("genuine Hoenn gate reaches original Brendan bedroom",arrived,where())
  if not arrived then F.finish();return end

  -- Clock-setting and meeting the rival precede this focused scene.
  -- Preserve Birch's *actual* Route 101 rescue state and triggers.
  F.w16(varAddress(0x4092),7) -- intro clock + mother done
  F.w16(varAddress(0x408d),3) -- rival met
  F.w16(varAddress(0x4050),1) -- town allows rescue approach
  F.check("only Hoenn home/rival prerequisites staged; rescue is untouched",
    var(0x4092)==7 and var(0x4050)==1 and not flag(0x52))

  local downstairs=walkMap(1,1,1,0,12,11,7,1,"bedroom_stairs")
  F.check("walked original 2F stairs to 1F",downstairs,where())
  if not downstairs then F.finish();return end
  -- Hoenn's 1F landing can still be in a map-entry/Mom scene immediately
  -- after the stair warp. Match Oak/Elm's verified dialog-settle behavior;
  -- no scripted flags, coordinates or collision are bypassed.
  F.idle(160)
  F.dismiss(65)
  F.L("Brendan 1F settled "..where().." intro="..var(0x4092))
  local town=walkMap(1,0,0,9,14,11,9,8,"house_exit")
  F.check("left real Hoenn house into Littleroot Town",town,where())
  if not town then F.finish();return end
  F.dismiss(50) -- Hub arrival's one-time welcome narration
  -- Littleroot's genuine route entrance is guarded by the twin at (11,1).
  -- With VAR_LITTLEROOT_TOWN_STATE==1, entering this tile runs the real
  -- GoSaveBirchTrigger and only THEN changes the scene to state 2.
  -- The former test aimed for (10,0) and stalled inside the twin dialogue.
  local warned=walkMap(0,9,0,9,20,20,11,1,"town_twin_warning")
  F.check("walked to real Littleroot twin rescue warning",warned,where())
  if not warned then F.finish();return end
  local cleared=false
  for _=1,90 do
    if var(0x4050)==2 then cleared=true;break end
    F.press("A",2);F.idle(32)
  end
  F.check("original twin naturally advances town to Birch rescue state",
    cleared and var(0x4050)==2,"state="..var(0x4050))
  if not cleared then F.shot("littleroot_twin_dialog_stuck");F.finish();return end
  F.dismiss(45)
  -- World-map connections require walking THROUGH row 0 from the approach.
  -- Do not teleport, write player coordinates or suppress the twin script.
  local route=at(0,16)
  for _=1,4 do
    if route then break end
    F.step("Up");F.idle(100)
    route=at(0,16)
  end
  F.check("walked north from Littleroot to original Route 101",route,where())
  if not route then F.finish();return end

  -- Step onto the script trigger at (10,19) if the map transition left
  -- us just outside it, then play out the professor/Zigzagoon introduction.
  local intro=false
  for _=1,160 do
    if var(0x4060)==2 then F.idle(120);intro=true;break end
    F.press("A",2);F.idle(32)
    if at(0,16) and var(0x4060)==1 then
      local x,y=F.pos()
      if y>=19 and (x==10 or x==11) then F.step("Up") end
    end
  end
  F.check("actual Birch/Zigzagoon rescue introduction reaches bag offer",
    intro and at(0,16),where().." state="..var(0x4060))
  if not intro then F.shot("birch_intro_stalled");F.finish();return end

  local bag=walkMap(0,16,0,16,25,23,7,15,"birch_bag")
  F.check("walked to real Birch bag at (7,14)",bag,where())
  if not bag then F.finish();return end
  local pc=pcBase()
  F.check("live PC pointer valid after Hoenn travel and rescue scene",pc~=nil)
  if not pc then F.finish();return end
  F.check("first PC slot still empty before stress fixture",(F.r8(pc+19)&2)==0)
  seedSix()
  F.check("party fixture holds six distinct legitimate boxed structures",
    F.r8(S.gPartiesCount)==6 and (F.r8(S.gParties+19)&2)~=0)
  F.check("420 PC slots can be populated in disposable emulator EWRAM",fillAllPc())
  local occupied,slot=everyBoxOccupied()
  F.check("all 420 shared PC slots are occupied",occupied,"checked="..slot)
  if not occupied then F.finish();return end

  local originalParty=snap(S.gParties,600)
  local firstBox=snap(pc,80)
  F.check("rescue quest and starter bag flags unset before full-storage interaction",
    not flag(0x52) and not flag(0x2bc))
  F.face("Up");F.press("A",2);F.idle(120)
  local returned=false
  for _=1,85 do
    if at(0,9) then F.idle(130);returned=true;break end
    F.press("A",2);F.idle(35)
  end
  F.check("real bag refuses full storage and warps back to Littleroot",returned,where())
  F.check("Birch is NOT marked rescued after refused gift",not flag(0x52))
  F.check("Birch starter bag is NOT consumed after refused gift",not flag(0x2bc))
  F.check("no seventh Pokémon overwrites the six-member party",F.r8(S.gPartiesCount)==6)
  local same,why=equal(S.gParties,originalParty)
  F.check("all six original Pokémon remain byte-identical",same,why)
  local live=pcBase()
  F.check("PC pointer still valid after automatic refusal warp",live~=nil)
  if live then
    same,why=equal(live,firstBox)
    F.check("first existing boxed Pokémon is byte-identical",same,why)
    occupied,slot=everyBoxOccupied()
    F.check("no existing slot in all 420 PC locations was displaced",occupied,"checked="..slot)
  end
  F.shot("birch_rescue_full_storage_refused")
  F.finish()
end)
