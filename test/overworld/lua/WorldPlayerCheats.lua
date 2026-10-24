-- Real release-ROM mGBA: the normal graphical Start wheel must expose
-- the restricted PLAYER Cheats menu, and B must exit without any raw dev UI.
-- This deliberately uses the saved numeric icon slot 11, previously DEBUG:
-- the source validator enforces that this is an alias, not a new save field.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local S = dofile(assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols missing"))
local F = require("lib").new(S, "WorldPlayerCheats")

F.run(function()
  if not F.boot(100) then
    F.check("new-game hub boot",false)
    F.finish()
    return
  end
  F.check("normal release boot starts in hub",F.grp()==100 and F.mapn()==0)
  F.press("Start",2)
  F.idle(100)
  local selected=F.selectStartIcon(11) -- USM_ICO_CHEATS, alias of old DEBUG
  F.check("normal release graphical wheel exposes player Cheats",selected)
  if selected then
    F.press("A",2)
    F.idle(100)
    F.check("Cheats enters locked field-menu controls, not normal walking",
      not F.ensureFree())
    local closed=false
    for i=1,12 do
      F.press("B",2)
      F.idle(35)
      if F.ensureFree() then closed=true;break end
    end
    F.check("Cheats can exit safely to the same World Transit map",
      closed and F.grp()==100 and F.mapn()==0)
    F.shot("cheats_wheel")
    -- Open the production wheel a second time and actually create a Pokémon.
    -- The root Cheat list begins at Quick Pokémon, which calls the game's
    -- normal ScriptGiveMon(species, level, ITEM_NONE). A genuine addition to
    -- the player's party is more discriminating than checking only a menu.
    if closed then
      F.press("Start",2); F.idle(100)
      local again=F.selectStartIcon(11)
      F.check("player Cheats remains available after closing and reopening",again)
      if again then
        F.press("A",2); F.idle(120) -- open curated CHEATS list
        F.press("A",2); F.idle(120) -- Quick Pokémon (first option)
        F.press("A",2); F.idle(120) -- species 1 (Bulbasaur)
        F.press("A",2); F.idle(200) -- level 1 -> ScriptGiveMon
        local count=F.r8(S.gPartiesCount)
        F.check("Quick Pokémon actually gives one party Pokémon",
          count==1,"count="..count)
        F.check("created Pokémon marks the first BoxPokemon slot occupied",
          (F.r8(S.gParties+19) & 2) ~= 0)
        F.dismiss(30)
        F.check("Quick Pokémon returns field control to player",
          F.ensureFree() and F.grp()==100 and F.mapn()==0)
        F.shot("quick_pokemon_added")
      end
    end
  end
  F.finish()
end)
