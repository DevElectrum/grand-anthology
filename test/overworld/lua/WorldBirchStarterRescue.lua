-- Original Route 101 Birch successful starter gift, rescue battle and flash persistence.
-- Travel via the normal World Transit Hoenn attendant and walk the real house,
-- Littleroot and Route 101. Stage only completed clock/rival prerequisite vars
-- and disposable six-party/420-PC mon fixtures; run Birch's real introductory
-- rescue cutscene and interact with his actual bag via the GBA A button.
-- No debug warps, no script bypasses, no user-owned SRAM.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols"))
local F=require("lib").new(S,"WorldBirchStarterRescue")
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


-- Production BoxPokemon secure data: 48 bytes XORed with PID ^ OTID,
-- substruct0 selected from the 24 canonical permutations. Species:11.
local function boxSpecies(base)
  local pid,otid=F.r32(base),F.r32(base+4)
  local sub0={0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3}
  return (F.r32(base+32+12*sub0[(pid%24)+1])~pid~otid)&0x7ff
end
local function boxChecksum(base)
  local key=F.r32(base)~F.r32(base+4)
  local total=0
  for word=0,11 do
    local value=F.r32(base+32+4*word)~key
    total=(total+(value&0xffff)+((value>>16)&0xffff))&0xffff
  end
  return total==F.r16(base+28),total
end
local function prepareTackle()
  -- Fixture slot 0 is a legitimate party Pokémon and needs one legal
  -- damaging move; untouched all other party members. This is NOT a
  -- debug win: the original first battle still runs via controller A.
  local p=S.gParties
  F.w16(p+44,33) -- PokemonSubstruct1.move1 = MOVE_TACKLE
  F.w8(p+52,35) -- PP 35
  F.w16(p+90,105) -- Attack
  F.w16(p+88,120);F.w16(p+86,120)
  local sum=0
  for i=32,78,2 do sum=(sum+F.r16(p+i))&0xffff end
  F.w16(p+28,sum)
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
  F.check("shared PC is live after Route 101 rescue introduction",pc~=nil)
  if not pc then F.finish();return end
  F.check("first box available before accepting Birch Treecko",(F.r8(pc+19)&2)==0)
  seedSix()
  prepareTackle()
  F.check("six normal party slots occupied with one legal Tackle user",
    F.r8(S.gPartiesCount)==6 and F.r16(S.gParties+44)==33
      and F.r8(S.gParties+52)==35)
  F.check("original rescue and starter bag flags still unset before success",
    not flag(0x52) and not flag(0x2bc))

  -- Walk up to Birch's original physical bag, not the debug "give mon".
  F.face("Up");F.press("A",2);F.idle(220)
  F.check("Birch bag opens native graphical three-starter chooser",not F.ow(),where())
  -- Native chooser initializes at index 1 (Torchic). Select index 0:
  -- Treecko, confirm YES, and wait for CB2_GiveStarter's actual script gift.
  F.press("Left",2);F.idle(65)
  F.press("A",2);F.idle(240)
  F.press("A",2);F.idle(120)
  local live,created
  for _=1,115 do
    live=pcBase()
    if live and (F.r8(live+19)&2)~=0 then created=true;break end
    F.press("A",2);F.idle(35)
  end
  F.check("original Birch starter selection boxes seventh Pokémon",
    created==true,"pc="..tostring(live))
  if not created then F.shot("birch_starter_choose_stalled");F.finish();return end
  F.check("original Hoenn starter is Treecko (#252)",boxSpecies(live)==252,
    "species="..boxSpecies(live))
  local valid,sum=boxChecksum(live)
  F.check("boxed Treecko encrypted secure payload checksum is valid",valid,
    "computed="..sum.." stored="..F.r16(live+28))
  F.check("starter choice is Hoenn Treecko index zero",var(0x4023)==0,
    "Hoenn VAR_STARTER_MON="..var(0x4023))
  F.check("six carried Pokémon remain present after Birch gift",
    F.r8(S.gPartiesCount)==6)
  F.check("original Birch rescue flag is set",flag(0x52))
  F.check("original global Pokémon possession flag is set",flag(0x948))
  -- The Zigzagoon rescue battle must actually start, then let the player
  -- fight with normal buttons; do not patch battle outcome or script vars.
  local battleSeen=(F.battleFlags()~=0 or F.battlers()>0)
  local lab=false
  for tick=1,620 do
    if at(1,4) then lab=true;break end
    if F.battleFlags()~=0 or F.battlers()>0 then battleSeen=true end
    F.press("A",2);F.idle(55)
    if tick%80==0 then
      F.L(("Birch fight tick=%d map=%s battlers=%d flags=%08x route=%d"):format(
        tick,where(),F.battlers(),F.battleFlags(),var(0x4060)))
      if F.reportCrash("birch_first_battle_"..tick) then break end
    end
  end
  F.check("original Zigzagoon rescue battle actually started",battleSeen)
  F.check("finished first battle and reached Birch's real Littleroot lab",
    lab,where().." state="..var(0x4060))
  if not lab then F.shot("birch_rescue_battle_stalled");F.finish();return end
  F.dismiss(55)
  F.check("Route 101 rescue story state advanced to completed 3",
    var(0x4060)==3,"state="..var(0x4060))
  F.check("starter bag consumption flag now set",flag(0x2bc))
  F.check("Birch rescue completion flag remains set",flag(0x52))
  local post=pcBase()
  F.check("global PC pointer remains valid after original rescue battle",post~=nil)
  if not post then F.finish();return end
  local stored=snap(post,80)
  local partyAfter=snap(S.gParties,600)
  F.check("boxed starter stays Treecko after original first battle",
    boxSpecies(post)==252)
  F.check("existing six-slot party remains after battle",
    F.r8(S.gPartiesCount)==6)

  -- Save through the production Start menu and battery flash, not a savestate.
  local free=F.ensureFree()
  F.check("normal field controls are available in Birch's lab",free,where())
  if not free then F.finish();return end
  F.press("Start",2);F.idle(100)
  local menu=F.selectStartIcon(7)
  F.check("original rescue permits normal in-game Save",menu)
  if not menu then F.finish();return end
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(90)
  F.press("A",2);F.idle(700)
  client.reboot_core()
  F.idle(240)
  local continued=F.boot(1,true)
  F.check("real battery Continue resumes original Birch lab",
    continued and at(1,4),where())
  if not continued then F.finish();return end
  local restored=pcBase()
  F.check("PC pointer is valid after real emulator reset/Continue",restored~=nil)
  if restored then
    local same,why=equal(restored,stored)
    F.check("boxed Treecko's complete 80-byte record survives flash reload",same,why)
    F.check("saved Treecko species survives flash reload",boxSpecies(restored)==252)
    local ok,_=boxChecksum(restored)
    F.check("saved Treecko checksum remains valid",ok)
  end
  local same,why=equal(S.gParties,partyAfter)
  F.check("all six party Pokémon persist byte-identical through Save/Continue",same,why)
  F.check("six carried slots survive saving Birch's original rescue",
    F.r8(S.gPartiesCount)==6)
  F.check("Hoenn rescue state survives real battery Continue",var(0x4060)==3)
  F.check("Birch rescue flag survives real battery Continue",flag(0x52))
  F.check("starter bag consumed flag survives real battery Continue",flag(0x2bc))
  F.check("Hoenn regional starter index survives real battery Continue",var(0x4023)==0)
  F.check("Save/Continue restores current v11 compatible save stamp",
    F.r8(F.sb2()+S.SaveBlock2.saveVersion)==S.SAVE_FORMAT_VERSION)
  F.shot("birch_treecko_rescue_saved_and_continued")
  F.finish()
end)
