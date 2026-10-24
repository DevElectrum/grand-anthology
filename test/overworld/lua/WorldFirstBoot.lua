-- Full-roster original GBA release smoke: boot an authentic *fresh* save
-- through the real title/main menu into the shared Region Hub. This is not
-- native Linux, a debug warp, a forged save, or a stubbed game engine.
-- The harness copies the ROM to a throwaway Verify1.gba with no save.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path

local symbolPath = assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols path required")
local S = dofile(symbolPath)
local F = require("lib").new(S, "WorldFirstBoot")

F.run(function()
  -- A new game has dialogue and a menu on the way into the hub.
  -- Use the established SmokeBoot input cadence, but here avoid the
  -- original full-symbol profile, which includes LTO-elided debug helpers.
  client.speedmode(800)
  F.idle(300)
  local reached, frames = false, 0
  for i = 1, 120000 do
    local phase = (i - 1) % 30
    if phase < 3 then
      joypad.set({A = true})
    elseif phase >= 15 and phase < 17 then
      joypad.set({Start = true})
    else
      joypad.set({})
    end
    emu.frameadvance()
    if i % 30 == 0 and F.ow() then
      reached, frames = true, i
      break
    end
  end
  joypad.set({})
  F.check("fresh full-roster ROM reaches real GBA overworld", reached, "frames=" .. frames)
  if reached then
    F.idle(180)
    F.check("new game enters the shared Region Hub group", F.grp() == 100, "group=" .. F.grp())
    local x, y = F.pos()
    F.check("new game starts at hub crest (16,4)", x == 16 and y == 4, ("x=%d y=%d"):format(x, y))
    local count = F.r8(S.gPartiesCount)
    F.check("new trainer starts with no Pokémon", count == 0, "party=" .. count)
    F.shot("hub_first_boot")
  end
  F.finish()
end)
