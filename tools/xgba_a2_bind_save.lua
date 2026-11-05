-- Bind the disposable E6 .sav to the frozen XGBA headless core.
-- mCoreLoadFile() does NOT autoload a .sav in headless-main.c: the GUI does.
-- The 0x0F memory window otherwise sees no valid E6 superblock.
local path = assert(os.getenv("A2_E6_SAVE_PATH"), "Missing disposable E6 save path")
local file = assert(io.open(path, "rb"), "Disposable E6 save image must already exist")
local size = assert(file:seek("end"))
file:close()
-- The accepted XGBA emulator writes a separate 16-byte RTC trailer after
-- its 16MiB E6 data area. The canonical Python auditor validates both
-- permitted lengths and the trailer independently, so this is a strict
-- transport size check, not a relaxed or lossy save format.
assert(size == 16777216 or size == 16777232,
       "Refusing E6 save with invalid data/trailer length")
assert(emu:loadSaveFile(path, false), "Unable to load disposable E6 save")

local header = {
    0x58, 0x47, 0x42, 0x41, 0x45, 0x36, 0x53, 0,
    2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1,
    0xB0, 0x2B, 0x5B, 0x12
}
for offset = 0, #header - 1 do
    local seen = emu:read8(0x0F000000 + offset)
    assert(seen == header[offset + 1],
           "E6 guest save header mismatch at byte " .. tostring(offset)
           .. " expected=" .. tostring(header[offset + 1])
           .. " actual=" .. tostring(seen))
end
print("A2_XGBA_E6_SAVE_BOUND verified_header_bytes=24")
io.stdout:flush()
