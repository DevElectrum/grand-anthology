-- Bind the disposable E6 .sav to the frozen XGBA headless core.
-- mCoreLoadFile() does NOT autoload a .sav in headless-main.c: the GUI does.
-- The 0x0F memory window otherwise sees no valid E6 superblock.
local path = assert(os.getenv("A2_E6_SAVE_PATH"), "Missing disposable E6 save path")
local file = assert(io.open(path, "rb"), "Disposable E6 save image must already exist")
local size = assert(file:seek("end"))
file:close()
assert(size == 16777216, "Refusing non-16MiB E6 image")
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
