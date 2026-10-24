# Contributing to Pokémon Grand Anthology

Thank you for considering a contribution. This is a **small, unofficial,
non-commercial, experimental fan project** based on
[Pokémon World](https://github.com/evilchinesefood/PKMN-World).
We prioritize a casual, approachable Kanto → Johto → Hoenn adventure over
competitive difficulty or strict deadlines.

## Before proposing changes

- Read [README.md](README.md), [CREDITS.md](CREDITS.md) and
  [current scope and status](docs/PUBLIC_STATUS.md).
- Existing P0 scope and gameplay decisions are frozen unless the maintainer
  reopens them. Prefer targeted fixes and tested additions over unrelated rewrites.
- Discuss large feature changes in an issue first. The project is a hobby:
  response times and acceptance are not guaranteed.
- Clearly disclose the origin of significant borrowed art, audio or code,
  including project, author, source link and applicable permission/license.
  Preserve all third-party notices and credits.
- If using AI assistance, review the output and disclose its involvement.
  AI-generated work can be incorrect or include third-party material.

## Bug reports

Please include a minimal reproduction, exact commit, emulator and version,
platform, whether the save is new or migrated, steps performed, expected
and observed behavior, and logs *after removing sensitive details*.
Do not attach copyrighted base ROMs, compiled ROM images, complete game
asset rips, personal save files or credentials.

## Development and verification

Contribute against the published repository's documented default branch or a
maintainer-designated development branch. Private development branches and the
inherited upstream baseline are not part of a public source snapshot.
See [INSTALL.md](INSTALL.md) for the modern ARM build toolchain.

Source validation, compilation and actual emulator gameplay are separate
acceptance levels; do not describe one as another. A species mentioned in
an encounter or evolution parser is **not** verified catchable until tested.
Preserve the existing normal 420-slot save layout unless storage
compatibility work is explicitly approved.

## Non-commercial and no-ROM rules

Do not upload or distribute compiled `.gba` files, saves with sensitive
information, proprietary game dumps or ROM-derived archives through this
repository. No paid builds, downloads, donation-gated access or commercialization.
Keep inherited credits and [third-party license notices](THIRD_PARTY_NOTICES.md)
intact. These project rules do not grant permission over Pokémon IP or
externally authored assets.

