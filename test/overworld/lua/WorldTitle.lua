-- Grand Anthology's approved 240x160 indexed title image, native GBA video
-- allocation and input transitions. All emulation uses a disposable ROM/save.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local symbolFile = os.getenv("PW_TITLE_SYMBOLS")
local S = symbolFile and dofile(symbolFile) or require("symbols")
local F = require("lib").new(S, "WorldTitle")

local function waitFor(callback, frames)
  joypad.set({})
  for _ = 1, frames do
    if F.cb2() == callback then return true end
    emu.frameadvance()
  end
  return false
end

local function title(reboot)
  if reboot then client.reboot_core(); F.idle(1) end
  local ready = waitFor(S.MainCB2_WorldTitleScreen, 6000)
  F.check("boot reaches Grand Anthology title", ready, string.format("cb2=%08X", F.cb2()))
  if not ready then F.shot("boot_failure"); F.finish(); return false end
  F.idle(100)
  return true
end

local function readResource(filename)
  local path = here .. "../../../graphics/title_screen/world/" .. filename
  local file = assert(io.open(path, "rb"), path)
  local bytes = file:read("*a")
  file:close()
  return bytes
end

local function matchesFile(address, filename)
  local data = readResource(filename)
  for i = 1, #data do
    if F.r8(address + i - 1) ~= data:byte(i) then
      return false, string.format("%s differs at offset %d", filename, i-1)
    end
  end
  return true, #data .. " native bytes match"
end

local function checkResource(name, address, filename)
  local good, details = matchesFile(address, filename)
  F.check(name, good, details)
end

local function decodeTitleTiles()
  local data = readResource("anthology_title_tiles.bin.lz")
  assert(data:byte(1) == 0x10, "expected GBA LZ10 tile graphics")
  local expected = data:byte(2) + (data:byte(3) << 8) + (data:byte(4) << 16)
  assert(expected == 601 * 64, "unexpected approved artwork tile count")
  local out, position = {}, 5
  while #out < expected do
    local flags = assert(data:byte(position)); position = position + 1
    for bit = 7, 0, -1 do
      if #out >= expected then break end
      if (flags & (1 << bit)) ~= 0 then
        local a, b = data:byte(position, position + 1)
        assert(b, "truncated compressed tile pair")
        position = position + 2
        local length = (a >> 4) + 3
        local distance = (((a & 15) << 8) | b) + 1
        assert(distance <= #out, "invalid tile backreference")
        for _ = 1, length do
          out[#out+1] = out[#out-distance+1]
        end
      else
        out[#out+1] = assert(data:byte(position))
        position = position + 1
      end
    end
  end
  return out
end

local function checkNativeArt()
  local tiles = decodeTitleTiles()
  local ok, detail = true, "601 native tiles exactly match VRAM"
  for i = 1, #tiles do
    if F.r8(0x06000000 + i-1) ~= tiles[i] then
      ok, detail = false, string.format("tile pixel byte %d differs", i-1)
      break
    end
  end
  F.check("approved title's 601 decompressed tiles are in VRAM", ok, detail)
  checkResource("approved tilemap loaded on BG0", 0x0600F000, "anthology_title_map.bin")
  checkResource("approved 256-color title palette loaded", 0x05000000, "anthology_title_palette.bin")
end

local function stoppedWave()
  return F.r8(S.gScanlineEffect + 21) == 0
end

local function noDuplicatePromptSprites()
  local pieces = 0
  for i = 0, 127 do
    local a0, a1 = F.r16(0x07000000 + i*8), F.r16(0x07000002 + i*8)
    if (a0 & 0xE300) == 0x4000 and (a1 & 0xC000) == 0x4000 and (a0 & 255) < 160 then
      pieces = pieces + 1
    end
  end
  F.check("no overlaid World PRESS START sprite strip", pieces == 0, "32x8 sprite pieces=" .. pieces)
end

F.run(function()
  if not title(false) then return end
  F.check("GBA mode 0 with BG0 and sprites, no fog/logo BGs", F.r16(0x04000000) == 0x1140)
  F.check("BG0 uses 8bpp charbase 0 / screenbase 30", F.r16(0x04000008) == 0x1E80)
  F.check("BG1 and BG2 remain disabled", F.r16(0x0400000A) == 0 and F.r16(0x0400000C) == 0)
  F.check("former fog alpha blend is disabled", F.r16(0x04000050) == 0)
  checkNativeArt()
  noDuplicatePromptSprites()
  F.shot("grand_anthology_title")

  F.idle(1024)
  checkResource("artwork remains stable without World fog cycle", 0x0600F000, "anthology_title_map.bin")
  checkResource("palette remains stable", 0x05000000, "anthology_title_palette.bin")
  F.press("Start", 1)
  F.check("Start opens main menu", waitFor(S.CB2_MainMenu, 300))
  F.idle(90)
  F.check("scanline effects stopped before main menu", stoppedWave())
  F.shot("main_menu")

  if not title(true) then return end
  F.press("A", 1)
  F.check("A opens main menu", waitFor(S.CB2_MainMenu, 300))

  if not title(true) then return end
  joypad.set({B = true, Select = true, Left = true})
  for _ = 1, 5 do emu.frameadvance() end
  F.idle(90)
  F.check("locked RTC chord does not invoke quickstart", F.cb2() == S.MainCB2_WorldTitleScreen)
  joypad.set({B = true, Select = true, Up = true})
  emu.frameadvance()
  F.check("clear-save chord reaches confirmation", waitFor(S.CB2_InitClearSaveDataScreen, 120))
  F.idle(90)
  F.check("scanline effects stopped before clear-save", stoppedWave())
  F.shot("clear_save_confirmation")
  F.press("B", 1)
  if not title(false) then return end
  F.check("canceling clear-save restores Grand Anthology title", F.r16(0x04000000) == 0x1140)

  local left = false
  for _ = 1, 12000 do
    F.idle(1)
    if F.cb2() ~= S.MainCB2_WorldTitleScreen then left = true; break end
  end
  F.check("title music completion returns to intro", left)
  if left then
    if not title(false) then return end
    checkResource("title replay restores correct palette", 0x05000000, "anthology_title_palette.bin")
    F.shot("title_after_intro_loop")
  end
  F.press("Select", 1)
  F.idle(180)
  F.check("SELECT alone does not start new game", F.cb2() == S.MainCB2_WorldTitleScreen)
  F.shot("select_disabled")
  F.finish()
end)
