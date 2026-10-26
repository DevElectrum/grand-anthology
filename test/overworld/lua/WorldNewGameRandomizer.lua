-- Real fresh-save New Game configuration test using release GBA ROM in mGBA.
-- All input is routed through the normal title, Options and Oak introduction.
-- No user's battery save is used: mgba-run.sh always clones a disposable ROM.
local here = (debug.getinfo(1, "S").source:sub(2)):match("^(.*[/\\])") or ""
package.path = here .. "?.lua;" .. package.path
local S = dofile(assert(os.getenv("PW_TITLE_SYMBOLS"), "release symbols required"))
local F = require("lib").new(S, "WorldNewGameRandomizer")

local function tap(key, pause)
    F.press(key, 2)
    F.idle(pause or 24)
end

local function untilCallback(cb, frames)
    for i = 1, frames do
        if F.cb2() == cb then return true end
        F.idle(1)
    end
    return false
end

local function config()
    local block = F.sb2()
    local packed = F.r16(block + 0x16)
    return {
        categories = F.r8(block + 0x92) & 0xF8,
        seed = F.r16(block + 0x96),
        mode = (packed >> 10) & 3,
        locked = (packed >> 12) & 1,
        version = (packed >> 13) & 7
    }
end

local function goToOption(index)
    for i = 1, index do tap("Down", 8) end
end

local function openNewGame()
    tap("A", 145)
    local conf = config()
    return conf.seed == 2026 and conf.locked == 0 and conf.version == 1
end

F.run(function()
    F.idle(330)
    local titleReady = untilCallback(S.MainCB2_WorldTitleScreen, 6000)
    F.check("fresh GBA starts on real title", titleReady)
    if not titleReady then F.finish(); return end
    -- Like WorldTitle, wait for the title's entrance/fog animation to settle.
    -- It can ignore START while the initial animation still owns input.
    F.idle(1150)
    local mainMenu = false
    for attempt = 1, 4 do
        F.press("Start", 1)
        if untilCallback(S.CB2_MainMenu, 400) then
            mainMenu = true
            break
        end
        F.idle(220)
    end
    F.idle(90)
    F.check("title opens real main menu", mainMenu,
        string.format("cb2=%08X", F.cb2()))
    if not mainMenu then F.shot("main_menu_transition_failed") end
    if not mainMenu then F.finish(); return end

    F.check("New Game enters unlocked Adventure Settings", openNewGame())
    F.shot("adventure_settings")

    -- The preset is row 13, with twenty other option rows after the normal
    -- gameplay configuration fields. Exercise native menu input, not RAM edits.
    goToOption(13)
    tap("Right")
    local full = config()
    F.check("Full preset enables all five randomizer categories",
        full.categories == 0xF8, "flags=" .. full.categories)
    F.check("Full preset enables static Full Chaos", full.mode == 2,
        "mode=" .. full.mode)

    -- Advance to seed row 20 and modify it in-place.
    goToOption(7)
    tap("Right")
    F.check("seed editing changes 2026 to 2027", config().seed == 2027,
        "seed=" .. config().seed)

    -- B must cancel without starting Oak's introduction.
    F.press("B", 2)
    local returned = untilCallback(S.CB2_MainMenu, 400)
    F.idle(90)
    F.check("B cancels configuration back to main menu", returned)
    F.check("cancel restores previous unsaved configuration",
        config().seed ~= 2027 and config().categories ~= 0xF8)

    -- Start a second new game and repeat the setup, this time confirming it.
    F.check("returning to New Game starts with fresh defaults", openNewGame())
    goToOption(13)
    tap("Right")
    goToOption(7)
    tap("Right")
    tap("Start", 95)
    local beforeIntro = config()
    F.check("START confirms and locks seed and categories before Oak",
        beforeIntro.seed == 2027 and beforeIntro.categories == 0xF8
        and beforeIntro.mode == 2 and beforeIntro.locked == 1)

    -- Original introduction includes name and outfit selections. Reuse the
    -- proven full-ROM navigation cadence of WorldFirstBoot.lua.
    local reached, frames = false, 0
    for i = 1, 120000 do
        local phase = (i - 1) % 30
        if phase < 3 then joypad.set({A = true})
        elseif phase >= 15 and phase < 17 then joypad.set({Start = true})
        else joypad.set({}) end
        emu.frameadvance()
        if i % 30 == 0 and F.ow() then
            reached, frames = true, i
            break
        end
    end
    joypad.set({})
    F.check("confirmed settings lead through character creation to Hub",
        reached and F.grp() == 100, "frames=" .. frames)
    if not reached then F.finish(); return end

    F.idle(180)
    local saved = config()
    F.check("randomizer survives new-save initialization",
        saved.seed == 2027 and saved.categories == 0xF8
        and saved.mode == 2 and saved.locked == 1 and saved.version == 1,
        string.format("seed=%d flags=%d mode=%d lock=%d version=%d",
          saved.seed, saved.categories, saved.mode, saved.locked, saved.version))
    F.shot("randomized_hub")

    tap("Start", 110)
    local foundOptions = F.selectStartIcon(9)
    F.check("in-game graphical menu opens Options", foundOptions)
    if foundOptions then
        tap("A", 200)
        goToOption(20)
        tap("Right")
        tap("R")
        local locked = config()
        F.check("Options cannot reroll the locked seed",
            locked.seed == 2027 and locked.categories == 0xF8
            and locked.mode == 2 and locked.locked == 1)
        tap("B", 280)
    end
    F.finish()
end)
