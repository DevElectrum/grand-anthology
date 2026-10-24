-- Real mGBA Johto Chikorita ball-script test with six occupied party slots.
-- Traverse World Transit and actual house/town/lab geometry, then press A
-- on Elm's original Poké Ball. Stage lab's "choose now" story state in
-- temporary test RAM: this does NOT test the preceding Elm/Mom dialogue.
-- All saved files and Pokémon fixtures are disposable, never the user's.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols"))
local F=require("lib").new(S,"WorldJohtoStarterBall")
local function at(g,m) return F.grp()==g and F.mapn()==m end
local function loc() return ("g%d m%d (%d,%d)"):format(F.grp(),F.mapn(),F.pos()) end
local function snap(a,n)
  local out={}
  for i=0,n-1 do out[i+1]=F.r8(a+i) end
  return out
end
local function equal(a,old)
  for i=1,#old do
    if old[i]~=F.r8(a+i-1) then return false,"byte "..(i-1) end
  end
  return true,"all original bytes unchanged"
end

-- Independently validate the unencrypted header against all 24 decrypted
-- secure halfwords. A matching species alone is not proof of an intact mon.
local function boxChecksum(base)
  local key=F.r32(base) ~ F.r32(base+4)
  local sum=0
  for word=0,11 do
    local decrypted=F.r32(base+32+word*4) ~ key
    sum=(sum+(decrypted & 0xffff)+((decrypted >> 16) & 0xffff)) & 0xffff
  end
  return sum==F.r16(base+28),sum
end
local function diagnoseBox(base, expected)
  local pid,otid=F.r32(base),F.r32(base+4)
  local offsets={0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3}
  F.L(("Box diagnostic expected=%d PID=%08x OTID=%08x checksum=%04x hasSpecies=%d")
    :format(expected,pid,otid,F.r16(base+28),F.r8(base+19)))
  for i=0,3 do
    local word=F.r32(base+32+12*i)
    F.L(("  secure[%d] raw=%08x decrypted=%08x decodedSpecies=%d")
      :format(i,word,word ~ pid ~ otid,(word ~ pid ~ otid)&0x7ff))
  end
  F.L(("  calculated substruct0 index=%d"):format(offsets[(pid%24)+1]))
end
-- Original engine FlagGet dispatches Johto 0x6000..0x63ff to SaveBlock3,
-- NOT to global SaveBlock1 flags. This tests the real isolated bank.
local function globalFlag(id)
  local byte=F.r8(F.sb1()+S.SaveBlock1.flags+(id//8))
  return (byte & (1<<(id%8)))~=0
end
local function johtoFlag(offset)
  local byte=F.r8(F.sb3()+S.SaveBlock3.johtoFlags+(offset//8))
  return (byte & (1<<(offset%8)))~=0
end
local function johtoStarterChoice()
  return F.r16(F.sb3()+S.SaveBlock3.regionVars+0xbc*2)
end
local function seedSix()
  for k,s in ipairs({1,4,7,25,152,258}) do
    local p=S.gParties+100*(k-1)
    for i=0,99 do F.w8(p+i,0) end
    F.w8(p+8,255);F.w8(p+20,255);F.w8(p+19,2)
    F.w16(p+32,s);F.w16(p+34,k==1 and 28 or 0)
    F.w16(p+36,125);F.w8(p+41,70);F.w16(p+42,1)
    local sum=0
    for i=32,78,2 do sum=(sum+F.r16(p+i))%65536 end
    F.w16(p+28,sum)
    F.w8(p+84,5);F.w8(p+85,255)
    F.w16(p+86,20);F.w16(p+88,20)
    for i=90,98,2 do F.w16(p+i,10) end
  end
  F.w8(S.gPartiesCount,6)
end
local function waitMap(g,m)
  for _=1,20 do
    if at(g,m) then F.idle(110); return true end
    F.idle(30)
  end
  return at(g,m)
end
local function gate()
  if not F.route({{16,4},{16,3}},"johto_gate") then return false end
  F.face("Up");F.press("A",3);F.idle(90)
  F.press("Up",2);F.idle(18);F.press("A",2)
  for _=1,130 do
    if at(76,4) then F.idle(150);return true end
    F.press("A",2);F.idle(22)
  end
  return false
end
-- Navigate *actual collision* in the cramped 11x9 Johto bedroom.
-- Axis-first routes hit the bedroom furniture at (6,6). This bounded DFS
-- uses genuine D-pad presses and backtracks on the same tile; it never
-- teleports, edits map coordinates, disables collision, or assumes a route.
local function downstairs()
  local dirs={
    {key="Right",dx=1,dy=0,back="Left"},
    {key="Up",dx=0,dy=-1,back="Down"},
    {key="Down",dx=0,dy=1,back="Up"},
    {key="Left",dx=-1,dy=0,back="Right"}
  }
  local seen={}
  local attempted=0
  local function walk(depth)
    if at(76,3) then return true end
    if not at(76,4) or depth>95 or attempted>390 then return false end
    local x,y=F.pos()
    seen[x..","..y]=true
    local order={}
    for _,d in ipairs(dirs) do
      local nx,ny=x+d.dx,y+d.dy
      if nx>=0 and nx<11 and ny>=0 and ny<9
          and not seen[nx..","..ny] then
        order[#order+1]=d
      end
    end
    table.sort(order,function(a,b)
      local da=math.abs(9-(x+a.dx))+math.abs(2-(y+a.dy))
      local db=math.abs(9-(x+b.dx))+math.abs(2-(y+b.dy))
      return da<db
    end)
    for _,d in ipairs(order) do
      local nx,ny=x+d.dx,y+d.dy
      if not seen[nx..","..ny] then
        attempted=attempted+1
        local moved=F.step(d.key)
        if at(76,3) then F.idle(130);return true end
        if at(76,4) and moved then
          local ax,ay=F.pos()
          if ax==nx and ay==ny then
            if walk(depth+1) then return true end
            if not F.step(d.back) then
              F.L("Stairs DFS cannot return from "..nx..","..ny)
              return false
            end
          end
        end
      end
    end
    return false
  end
  local ok=walk(0)
  F.L(("Johto bedroom collision search: %d moves, found stairs=%s; %s")
    :format(attempted,tostring(ok),loc()))
  if not ok then F.shot("johto_bedroom_search_failed") end
  return ok
end
local function leaveHouse()
  for _=1,60 do F.press("A",2);F.idle(24) end
  if not F.route({{9,7}},"johto_mom_house") then return false end
  -- Johto house layout has the interior door at y=8 and the border exit
  -- directly below at y=9. A single step previously stopped at (9,8)
  -- without entering New Bark; allow the actual outer-door crossing,
  -- confirming each position and map transition instead of assuming warp.
  for n=1,3 do
    if at(75,0) then return true end
    local was=loc()
    local stepped=F.step("Down")
    F.idle(120)
    F.L(("Johto house exit step %d: moved=%s %s -> %s"):format(
      n,tostring(stepped),was,loc()))
    if waitMap(75,0) then return true end
    if not at(76,3) or not stepped then break end
  end
  F.shot("johto_house_exit_diagnostic")
  return false
end
-- Traverse New Bark Town by the actual GBA collision map. The first
-- arrival also shows the region's welcome narration, so clear text before
-- attempting movement. Axis-first walking from the house at (20,12) to
-- Elm's lab is blocked by a building/fence and is not a valid test route.
local function reachLab()
  F.L("Town start "..loc())
  F.dismiss(55)
  F.idle(60)
  local visited,steps={},0
  local directions={
    {key="Left", dx=-1,dy=0,back="Right"},
    {key="Up",   dx=0, dy=-1,back="Down"},
    {key="Down", dx=0, dy=1,back="Up"},
    {key="Right",dx=1, dy=0,back="Left"}
  }
  local function explore(depth)
    if at(76,0) then return true end
    if not at(75,0) or depth>440 or steps>=850 then return false end
    local x,y=F.pos()
    visited[x..","..y]=true
    local options={}
    for _,d in ipairs(directions) do
      local nx,ny=x+d.dx,y+d.dy
      if nx>=0 and nx<30 and ny>=0 and ny<39
          and not visited[nx..","..ny] then
        options[#options+1]=d
      end
    end
    table.sort(options,function(a,b)
      local aa=math.abs(10-(x+a.dx))+math.abs(9-(y+a.dy))
      local bb=math.abs(10-(x+b.dx))+math.abs(9-(y+b.dy))
      return aa<bb
    end)
    for _,d in ipairs(options) do
      local nx,ny=x+d.dx,y+d.dy
      if not visited[nx..","..ny] then
        steps=steps+1
        local moved=F.step(d.key)
        F.idle(28)
        if at(76,0) then F.idle(110);return true end
        if moved and at(75,0) then
          local ax,ay=F.pos()
          if ax==nx and ay==ny then
            if explore(depth+1) then return true end
            if not F.step(d.back) then
              F.L("Town route cannot backtrack from "..ax..","..ay)
              return false
            end
            F.idle(28)
          end
        end
      end
    end
    return false
  end
  local arrived=explore(0)
  F.L(("Elm lab navigation: %d controller steps success=%s %s")
    :format(steps,tostring(arrived),loc()))
  if not arrived then F.shot("johto_elm_lab_navigation_failed") end
  return arrived
end

F.run(function()
  local boot=F.boot(100)
  F.check("actual new game starts in World Transit",boot and at(100,0))
  if not boot then F.finish();return end
  local pcptr=F.r32(S.gPokemonStoragePtr)
  local pc=(pcptr>=0x02000000 and pcptr<0x02040000) and pcptr+4 or nil
  F.check("shared PC first slot exists",pc~=nil)
  if not pc then F.finish();return end
  F.check("shared PC first slot starts empty",(F.r8(pc+19)&2)==0)
  seedSix()
  -- Travel may legitimately advance friendship, so gift integrity is tested later.
  F.check("full six-Pokémon carried party staged",F.r8(S.gPartiesCount)==6)
  local crossed=gate()
  F.check("real Johto attendant sends trainer to bedroom",crossed,loc())
  if not crossed then F.finish();return end
  -- Johto story bank is isolated in SaveBlock3. Stage only the already
  -- completed clock/mother segment. Leave Elm's lab state at 0, so its
  -- ACTUAL trigger/intro offers the first Pokémon and advances state to 1.
  -- Pre-staging 1 from the doorway hit the exit-prevention script at y=8.
  local labState=F.sb3()+S.SaveBlock3.regionVars+0x81*2
  local townState=F.sb3()+S.SaveBlock3.regionVars+0x80*2
  F.w16(labState,0)
  F.w16(townState,2)
  F.check("Elm's real initial lab scene left pending",F.r16(labState)==0)
  local stair=downstairs()
  F.check("original upstairs/downstairs map transition works",stair,loc())
  if not stair then F.finish();return end
  local town=leaveHouse()
  F.check("player walks out into New Bark Town",town,loc())
  if not town then F.finish();return end
  local lab=reachLab()
  F.check("player enters Elm's actual laboratory",lab,loc())
  if not lab then F.finish();return end
  -- Cross the lab's genuine (6,7) introductory trigger while its scene
  -- var is 0. Elm's original dialogue, PC message and scripted movement
  -- should then advance the lab to choice-ready state 1.
  local enteredIntro=F.route({{6,9},{6,7}},"elm_original_intro")
  F.L("Original Elm lab intro approached="..tostring(enteredIntro).." "..loc())
  local offered=false
  for _=1,190 do
    if F.r16(labState)==1 then F.idle(110);offered=true;break end
    F.press("A",2);F.idle(44)
  end
  F.check("original Professor Elm intro naturally offers starter",
    offered and F.r16(labState)==1,loc())
  if not offered then F.shot("elm_original_intro_stalled");F.finish();return end
  F.dismiss(24)
  local reached=F.route({{6,6},{8,6},{8,5}},"chikorita_ball")
  F.check("walked to the original Chikorita Poké Ball",reached,loc())
  if not reached then F.finish();return end
  -- Save blocks intentionally relocate during new-game/region setup.
  -- Resolve gPokemonStoragePtr again after traversing from the Hub:
  -- the pointer cached before world travel can point into obsolete EWRAM.
  local livePcPtr=F.r32(S.gPokemonStoragePtr)
  local livePc=(livePcPtr>=0x02000000 and livePcPtr<0x02040000) and livePcPtr+4 or nil
  F.L(("Elm PC base initial=%08x live=%08x"):format(pc,livePc or 0))
  F.check("shared PC pointer valid after Johto story transitions",livePc~=nil)
  if not livePc then F.finish();return end
  F.check("live Elm PC slot empty before starter gift",(F.r8(livePc+19)&2)==0)
  -- Snapshot immediately before the gift, not before traveling.
  local walkedParty=snap(S.gParties,600)
  F.face("Up");F.press("A",2);F.idle(90)
  local function liveBox()
    local ptr=F.r32(S.gPokemonStoragePtr)
    if ptr<0x02000000 or ptr>=0x02040000 then return nil end
    return ptr+4
  end
  for _=1,70 do
    livePc=liveBox()
    if livePc and (F.r8(livePc+19)&2)~=0 then break end
    F.press("A",2);F.idle(27)
  end
  livePc=liveBox()
  F.check("shared PC pointer valid after Elm gift",livePc~=nil)
  if not livePc then F.finish();return end
  F.check("actual Elm ball script puts seventh mon in PC",(F.r8(livePc+19)&2)~=0,loc())
  diagnoseBox(livePc,152)
  local function elmSpecies(base)
    local pid,otid=F.r32(base),F.r32(base+4)
    local offsets={0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3}
    local offset=offsets[(pid%24)+1]*12
    return (F.r32(base+32+offset) ~ pid ~ otid) & 0x7ff
  end
  F.check("Elm gift is Chikorita species 152",elmSpecies(livePc)==152,
    "encrypted BoxPokemon: decrypted="..elmSpecies(livePc))
  local checksumOk,calculated=boxChecksum(livePc)
  F.check("boxed Elm starter has intact encrypted checksum",checksumOk,
    ("stored=%04x calculated=%04x"):format(F.r16(livePc+28),calculated))
  local unchanged,why=equal(S.gParties,walkedParty)
  F.check("all six existing Pokémon are untouched by gift event",unchanged,why)
  F.check("gift does not exceed party size six",F.r8(S.gPartiesCount)==6)
  F.shot("johto_real_starter_ball")

  -- Genuine Elm-ball Save/reset/Continue, not a RAM savestate or cheat gift.
  local free=F.dismiss(75)
  F.check("Elm gift dialog returns genuine field controls",free,loc())
  F.check("Elm acceptance sets shared player ownership flag",globalFlag(0x948))
  F.check("Elm acceptance sets isolated Johto starter chosen flag",johtoFlag(0x38))
  F.check("Johto starter selection records Chikorita ball index 0",
    johtoStarterChoice()==0,"index="..johtoStarterChoice())
  if free then
    local savedParty=snap(S.gParties,600)
    local savedBox=snap(livePc,80)
    F.press("Start",2);F.idle(100)
    local saveIcon=F.selectStartIcon(7)
    F.check("Save remains selectable following original Elm gift",saveIcon)
    if saveIcon then
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(700)
      client.reboot_core();F.idle(240)
      local continued=F.boot(76,true)
      F.check("actual Continue restores Johto after Elm gift",continued and at(76,0),loc())
      if continued then
        local restored=liveBox() -- GBA reboot assigns new save-block offset
        F.check("shared PC pointer valid after Elm Save/Continue",restored~=nil)
        if restored then
          local same,note=equal(restored,savedBox)
          F.check("actual boxed Chikorita survives flash Save/Continue byte-identical",same,note)
          F.check("restored PC species is still Chikorita #152",elmSpecies(restored)==152)
          local checksumValid,checksum=boxChecksum(restored)
          F.check("restored Chikorita passes encrypted checksum",checksumValid,
            ("stored=%04x calculated=%04x"):format(F.r16(restored+28),checksum))
        end
        local same,note=equal(S.gParties,savedParty)
        F.check("all six existing Pokémon survive Elm Save/Continue",same,note)
        F.check("six-carried-party size survives Elm Save/Continue",F.r8(S.gPartiesCount)==6)
        F.check("Elm gift shared ownership flag survives Save/Continue",globalFlag(0x948))
        F.check("Elm Johto starter chosen flag survives Save/Continue",johtoFlag(0x38))
        F.check("Chikorita regional starter choice survives Save/Continue",johtoStarterChoice()==0)
      end
    end
  end
  F.finish()
end)
