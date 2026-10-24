-- Production mGBA acceptance: visit Oak's actual Bulbasaur ball in Kanto.
-- Travels through World Transit, Pallet bedroom, 1F, town and Oak's lab via
-- normal GBA inputs. Stages only the Oak lab's "choose a starter" story
-- state in disposable emulator memory, NOT the preceding long introduction.
-- Party fixture has six valid-format Pokémon. No debug warp, savestate or
-- user-owned SRAM involved.
local here=(debug.getinfo(1,"S").source:sub(2)):match("^(.*[/\\])") or ""
package.path=here.."?.lua;"..package.path
local S=dofile(assert(os.getenv("PW_TITLE_SYMBOLS"),"release symbols"))
local F=require("lib").new(S,"WorldKantoStarterBall")
local function at(g,m) return F.grp()==g and F.mapn()==m end
local function where() return ("group=%d map=%d (%d,%d)"):format(F.grp(),F.mapn(),F.pos()) end
local function oldParty()
  local a={}
  for i=0,599 do a[i+1]=F.r8(S.gParties+i) end
  return a
end
local function equalParty(a)
  for i=0,599 do
    if F.r8(S.gParties+i)~=a[i+1] then return false,"byte "..i end
  end
  return true,"all 600 bytes unchanged"
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
local function seedSix()
  for k,species in ipairs({1,4,7,25,152,258}) do
    local a=S.gParties+(k-1)*100
    for j=0,99 do F.w8(a+j,0) end
    F.w8(a+8,255);F.w8(a+20,255);F.w8(a+19,2)
    F.w16(a+32,species);F.w16(a+34,k==1 and 28 or 0)
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
local function pcAddress()
  local ptr=F.r32(S.gPokemonStoragePtr)
  if ptr<0x02000000 or ptr>=0x02040000 then return nil end
  return ptr+4 -- compiler-protected PokemonStorage.boxes offset
end
-- Validated ABI: global Hoenn/Kanto flags live in SaveBlock1; the
-- independent Kanto starter selection lives in SaveBlock3's regionVars.
local function globalFlag(id)
  local byte=F.r8(F.sb1()+S.SaveBlock1.flags+(id//8))
  return (byte & (1<<(id%8)))~=0
end
local function kantoStarterChoice()
  return F.r16(F.sb3()+S.SaveBlock3.regionVars+0x0c*2)
end
local function boxSpecies(base)
  local pid,otid=F.r32(base),F.r32(base+4)
  local s0={0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3}
  local decrypted=F.r32(base+32+12*s0[(pid%24)+1])~pid~otid
  -- Substruct0 packs 11 species bits followed by 5 tera-type bits.
  return decrypted & 0x7ff
end

-- Collision-aware controller navigation: explore real walkable metatiles,
-- never writes map coords or suppresses scripts. Each map has a bound on both
-- coordinate and total attempted D-pad presses; blocked rooms fail visibly.
local function navigate(srcG,srcM,dstG,dstM,width,height,goalX,goalY,tag)
  local seen,attempts={},0
  local function reached()
    if not at(dstG,dstM) then return false end
    -- When the goal is a walkable interaction tile *inside the same map*,
    -- location-only success would always be true at the starting tile.
    if srcG==dstG and srcM==dstM then
      local x,y=F.pos()
      return x==goalX and y==goalY
    end
    return true
  end
  local dirs={
    {key="Up",dx=0,dy=-1,undo="Down"},
    {key="Right",dx=1,dy=0,undo="Left"},
    {key="Down",dx=0,dy=1,undo="Up"},
    {key="Left",dx=-1,dy=0,undo="Right"}
  }
  local function scan(depth)
    if reached() then return true end
    if not at(srcG,srcM) or attempts>=width*height*5 or depth>width*height then return false end
    local x,y=F.pos()
    seen[x..","..y]=true
    local order={}
    for _,d in ipairs(dirs) do
      local nx,ny=x+d.dx,y+d.dy
      if nx>=0 and ny>=0 and nx<width and ny<height and not seen[nx..","..ny] then
        order[#order+1]=d
      end
    end
    table.sort(order,function(a,b)
      local ad=math.abs(goalX-(x+a.dx))+math.abs(goalY-(y+a.dy))
      local bd=math.abs(goalX-(x+b.dx))+math.abs(goalY-(y+b.dy))
      return ad<bd
    end)
    for _,d in ipairs(order) do
      local nx,ny=x+d.dx,y+d.dy
      if not seen[nx..","..ny] then
        attempts=attempts+1
        local stepped=F.step(d.key)
        F.idle(26)
        if reached() then return true end
        if stepped and at(srcG,srcM) then
          local ax,ay=F.pos()
          if ax==nx and ay==ny then
            if scan(depth+1) then return true end
            if not F.step(d.undo) then
              F.L("Cannot backtrack "..tag.." from "..ax..","..ay)
              return false
            end
            F.idle(26)
            if reached() then return true end
          end
        end
      end
    end
    return false
  end
  local ok=scan(0)
  F.L(("Explore %s: %d D-pad moves; success=%s %s"):format(tag,attempts,tostring(ok),where()))
  if not ok then F.shot(tag.."_navigation_failed") end
  return ok
end
local function gateKanto()
  if not F.route({{16,4},{11,4},{11,3}},"kanto_attendant") then return false end
  F.face("Up");F.press("A",3);F.idle(90)
  F.press("Up",2);F.idle(18);F.press("A",2);F.idle(70)
  for _=1,130 do
    if at(38,1) then F.idle(150);return true end
    F.press("A",2);F.idle(25)
  end
  return false
end
local function finishDialog()
  -- The town's first-visit narration fires at the Kanto outside arrival.
  -- Do not use debug scene skip; the in-game B button closes text.
  F.dismiss(65)
end

F.run(function()
  local boot=F.boot(100)
  F.check("new GBA adventure starts at World Transit",boot and at(100,0))
  if not boot then F.finish();return end
  local pc=pcAddress()
  F.check("global PC slot exists",pc~=nil)
  if not pc then F.finish();return end
  F.check("PC slot initially empty",(F.r8(pc+19)&2)==0)
  seedSix()
  -- Earlier travel and story scenes can legitimately change friendship.
  F.check("six carried team members staged",F.r8(S.gPartiesCount)==6)
  local arrived=gateKanto()
  F.check("actual Kanto attendant reaches Pallet bedroom",arrived,where())
  if not arrived then F.finish();return end

  -- Kanto scene vars live in its isolated SaveBlock3 region bank.
  -- Stage the lab's *entrance* (1), not the post-intro choice (2).
  -- The actual map's OnFrame script then moves Oak and the trainer into
  -- place, delivers the original dialogue, and changes state to 2 itself.
  -- Staging 2 at the doorway hit the leave-without-starter trigger at y=8
  -- and never physically reached the balls.
  local base=F.sb3()+S.SaveBlock3.regionVars
  F.w16(base+0x2E*2,1)
  F.w16(base+0x2F*2,1)
  F.check("Kanto lab introductory state staged in isolated save bank",
    F.r16(base+0x2E*2)==1)
  local house1=navigate(38,1,38,0,12,9,10,2,"kanto_bedroom_stairs")
  F.check("walked down original Pallet 2F staircase",house1,where())
  if not house1 then F.finish();return end
  local town=navigate(38,0,37,0,13,10,5,8,"kanto_house_exit")
  F.check("left house by normal door into Pallet Town",town,where())
  if not town then F.finish();return end
  finishDialog()
  local lab=navigate(37,0,38,3,24,20,16,13,"kanto_oak_lab_door")
  F.check("walked across Pallet Town into Oak's real lab",lab,where())
  if not lab then F.finish();return end
  -- Play the ACTUAL Professor Oak introduction inside the lab. Use
  -- normal A presses to advance speech, not RAM mutations or debug flags.
  local oakIntroComplete=false
  for _=1,175 do
    if F.r16(base+0x2E*2)==2 then
      F.idle(100)
      oakIntroComplete=true
      break
    end
    F.press("A",2);F.idle(45)
  end
  F.check("original Oak intro naturally advances lab to starter choice",
    oakIntroComplete and F.r16(base+0x2E*2)==2,where())
  if not oakIntroComplete then
    F.shot("kanto_oak_intro_stalled")
    F.finish()
    return
  end
  F.dismiss(20)

  -- Real object at (8,4); interact from walkable tile directly south.
  local reached=navigate(38,3,38,3,13,14,8,5,"kanto_bulbasaur")
  local x,y=F.pos()
  F.check("physically reached tile below Bulbasaur Poké Ball",reached and x==8 and y==5,where())
  if not reached then F.finish();return end
  -- New-game and world transitions can call MoveSaveBlocks_ResetHeap(),
  -- which ASLR-relocates gPokemonStoragePtr. Never use the Hub's old
  -- boxed-mon address after crossing into Kanto.
  local livePc=pcAddress()
  F.L(("Oak PC base initial=%08x live=%08x"):format(pc,livePc or 0))
  F.check("shared PC pointer valid after Kanto story transitions",livePc~=nil)
  if not livePc then F.finish();return end
  F.check("live Oak PC slot empty before starter gift",(F.r8(livePc+19)&2)==0)
  -- Gift event must preserve the six party records from this precise point.
  local walkedParty=oldParty()
  F.face("Up");F.press("A",2);F.idle(120)
  for _=1,95 do
    livePc=pcAddress()
    if livePc and (F.r8(livePc+19)&2)~=0 then break end
    F.press("A",2);F.idle(30)
  end
  livePc=pcAddress()
  F.check("shared PC pointer valid after Oak gift",livePc~=nil)
  if not livePc then F.finish();return end
  F.check("real Oak object script boxes seventh Pokémon",(F.r8(livePc+19)&2)~=0,where())
  diagnoseBox(livePc,1)
  F.check("boxed Oak starter decrypts to Bulbasaur #1",boxSpecies(livePc)==1,
    "decrypted="..boxSpecies(livePc))
  local checksumOk,calculated=boxChecksum(livePc)
  F.check("boxed Oak starter has intact encrypted checksum",checksumOk,
    ("stored=%04x calculated=%04x"):format(F.r16(livePc+28),calculated))
  local same,note=equalParty(walkedParty)
  F.check("six existing party Pokémon kept byte-for-byte during gift",same,note)
  F.check("full party remains six members",F.r8(S.gPartiesCount)==6)
  F.shot("kanto_oak_original_bulbasaur")

  -- Save the ORIGINAL NPC gift, not the generic Cheats-created fixture.
  -- All inputs use the actual field Save dialog and GBA core reboot.
  local free=F.dismiss(75)
  F.check("Oak gift dialog returns genuine field controls",free,where())
  F.check("original Oak starter acceptance records shared Pokédex-gift availability",
    globalFlag(0x948))
  F.check("original Oak starter acceptance records Pallet story progression",
    globalFlag(0xCD1))
  F.check("original Kanto starter selection records Bulbasaur ball index 0",
    kantoStarterChoice()==0,"index="..kantoStarterChoice())
  if free then
    local saveParty=oldParty()
    local saveBox={}
    for i=0,79 do saveBox[i+1]=F.r8(livePc+i) end
    F.press("Start",2);F.idle(100)
    local saveIcon=F.selectStartIcon(7)
    F.check("Save remains selectable following original Oak gift",saveIcon)
    if saveIcon then
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(90)
      F.press("A",2);F.idle(700)
      client.reboot_core();F.idle(240)
      local continued=F.boot(38,true)
      F.check("actual Continue restores Kanto after Oak gift",continued and at(38,3),where())
      if continued then
        local restored=pcAddress() -- the reset creates ANOTHER save-block offset
        F.check("shared PC pointer valid after Oak Save/Continue",restored~=nil)
        if restored then
          local bytesSame=true
          for i=0,79 do if F.r8(restored+i)~=saveBox[i+1] then bytesSame=false;break end end
          F.check("actual boxed Bulbasaur survives flash Save/Continue byte-identical",bytesSame)
          F.check("restored PC species is still Bulbasaur #1",boxSpecies(restored)==1)
          local checksumValid,checksum=boxChecksum(restored)
          F.check("restored Bulbasaur passes encrypted checksum",checksumValid,
            ("stored=%04x calculated=%04x"):format(F.r16(restored+28),checksum))
        end
        local partySame,note=equalParty(saveParty)
        F.check("all six existing Pokémon survive Oak Save/Continue",partySame,note)
        F.check("six-carried-party size survives Oak Save/Continue",F.r8(S.gPartiesCount)==6)
        F.check("Oak gift shared ownership flag survives Save/Continue",globalFlag(0x948))
        F.check("Oak Pallet storyline flag survives Save/Continue",globalFlag(0xCD1))
        F.check("Bulbasaur regional starter choice survives Save/Continue",kantoStarterChoice()==0)
      end
    end
  end
  F.finish()
end)
