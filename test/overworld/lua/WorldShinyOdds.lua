-- Genuine GBA production Options / shiny-odds persistence test in mGBA.
-- Fresh save: default 1/128 is packed as 0 in SaveBlock2 at 0x16 bits 6..9 (four bits).
-- Select Always via LEFT wrap, then actually Save, reboot and Continue.
-- The runner uses disposable Verify1.gba/.sav; no user save is touched.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local S = dofile(assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols required"))
local F = require("lib").new(S, "WorldShinyOdds")

local function shinyOdds()
  return (F.r16(F.sb2()+0x16) >> 6) & 15
end
local function tap(key, pause)
  F.press(key,2)
  F.idle(pause or 25)
end

-- The *first* save and subsequent OVERWRITES use different confirmation
-- sequences. Three blind A taps successfully created the first save but left
-- the overwrite confirmation pending and the old value reloaded. Keep
-- confirming until the actual save callback releases field controls, then
-- restart the emulated GBA; a cancellation can never pass the reload check.
local function overwriteAndWait()
  tap("A",120) -- start-wheel Save icon
  for _=1,12 do
    tap("A",140)
    if F.ensureFree() then
      F.idle(180)
      return true
    end
  end
  return false
end

F.run(function()
  local boot=F.boot(100)
  F.check("new game boots to shared hub",boot)
  if not boot then F.finish();return end
  F.check("fresh options preserve legacy 1/128 default",shinyOdds()==0,
    "value="..shinyOdds())
  tap("Start",100)
  local opts=F.selectStartIcon(9) -- USM_ICO_OPTIONS
  F.check("production graphical wheel opens Options icon",opts)
  if not opts then F.finish();return end
  tap("A",250)
  -- MENUITEM_SHINYODDS has enum index 11 (option_menu.c). Scroll naturally.
  for i=1,11 do tap("Down",22) end
  tap("Left",65) -- wrap 0 -> 7, OPTIONS_SHINY_ODDS_ALWAYS
  tap("B",330) -- Task_OptionMenuSave commits the bitfield
  F.check("Options screen saves selectable Always shiny odds",shinyOdds()==7,
    "value="..shinyOdds())
  -- Options uses CB2_ReturnToFieldWithOpenMenu: the wheel reopens itself.
  -- Select the normal Save icon, avoiding a fixed cursor location.
  local save=F.selectStartIcon(7) -- USM_ICO_SAVE
  F.check("normal graphical Start provides Save after Options",save)
  if save then
    tap("A",90);tap("A",90);tap("A",700)
    client.reboot_core()
    F.idle(240)
    local resumed=F.boot(100)
    F.check("Continue reloads real battery save in Region Hub",resumed)
    if resumed then
      F.check("shiny odds survive Save/core reset/Continue",shinyOdds()==7,
        "persisted="..shinyOdds())
      F.check("shiny odds kept the v11 save-format stamp",
        F.r8(F.sb2()+S.SaveBlock2.saveVersion)==S.SAVE_FORMAT_VERSION)
      F.shot("always_odds_restored")
      -- Existing saved Always ID stays 7. The three new options carry IDs
      -- 10/9/8 but appear before Always in the player's natural menu order.
      -- Prove each via actual graphical Options selection and flash reload.
      local steps = {
        {id=10, label="1/2"}, {id=9, label="1/4"}, {id=8, label="1/8"}
      }
      for _, step in ipairs(steps) do
        tap("Start",100)
        local optionsAgain=F.selectStartIcon(9)
        F.check("Options stays accessible for "..step.label, optionsAgain)
        if not optionsAgain then break end
        tap("A",250)
        for i=1,11 do tap("Down",22) end
        tap("Left",65) -- Always -> 1/2 -> 1/4 -> 1/8
        tap("B",330)
        F.check("Options selects new "..step.label.." Shiny Odds",
          shinyOdds()==step.id,"value="..shinyOdds())
        local saveAgain=F.selectStartIcon(7)
        F.check("Save is available at "..step.label.." Shiny Odds",saveAgain)
        if not saveAgain then break end
        local saved = overwriteAndWait()
        F.check("existing save overwrite confirmation completes for "..step.label,saved)
        if not saved then break end
        client.reboot_core()
        F.idle(240)
        local reloaded=F.boot(100)
        F.check("Continue works at "..step.label.." Shiny Odds",reloaded)
        if not reloaded then break end
        F.check(step.label.." Shiny Odds survives Save/core reset/Continue",
          shinyOdds()==step.id,"persisted="..shinyOdds())
      end
    end
  end
  F.finish()
end)
