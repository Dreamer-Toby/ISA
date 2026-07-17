# ISA — strict-MVVM course roguelike

ISA is a bounded, room-based C++20/SFML 3 game inspired by *The Binding of Isaac: Rebirth*. It is made only for non-profit classroom teaching and assignment demonstration. External artwork remains the property of its respective creators; each file is traced in [the asset manifest](docs/assets/asset-sources.md) and must not be reused for commercial release.

## Version roles

- `v1.0-midterm-backup` is the frozen, accepted midterm version.
- `feature/easyisaac-final-replica` is the final presentation branch. It reproduces the paper menus, basement room, HUD, masked sprites, pause scroll and event sounds of [`tianguantg/EasyIsaac`](https://github.com/tianguantg/EasyIsaac) while retaining this project's larger gameplay loop and strict SFML/MVVM architecture.

The final branch does not depend on EasyX and remains portable beyond Windows. EasyIsaac's paired EasyX color/mask JPEGs are converted to cached alpha textures at runtime by the Resource layer.

## Implemented gameplay

- Alternating start page, a `RUN / RANKINGS / BACK` main menu, four-character selection (Isaac, Magdalene, Cain, Judas), stable two-option pause, defeat and victory screens.
- Fixed-step movement, four-direction tears, enemy projectiles, collision, invulnerability, half-heart-accurate red hearts and shields (combined cap: 12).
- Two connected floors with an intentionally fixed four-room layout: one start room, one monster room, one treasure room and one Boss room, with persistent visits, illustrated doors, combat seals and a minimap.
- Six configured enemies: Fly, Pooter, Spider, Hopper, Gaper and Clotty, composed from movement/attack/drop strategies.
- Three Bosses: Monstro on floor one; Duke of Flies and Larry Jr. together as the final floor-two encounter.
- Model-owned rock and spike-trap volumes: rocks block players and tears, while traps deal exactly half a heart with normal invulnerability.
- Monster milk drops are real Breakfast items: each adds one heart container and heals one whole red heart. Every treasure room visibly offers exactly one Sad Onion or Wiggle Worm; floor one chooses randomly and floor two always gives the other item.
- A complete two-floor ending with no shop, secret, devil or extra generated rooms. Starting after defeat creates a fresh run.
- HUD DTOs for full/half/empty red hearts, shields, coins, bombs, keys, active item, minimap, floor and room state.
- EasyIsaac-style room art, characters, tears, flies, props and feedback sounds with presentation-only animation and hitbox inspection.

## Controls

| Action | Key |
|---|---|
| Confirm menu / interact / descend | Enter or Space on paper screens |
| Navigate main or pause menu | W / S |
| Change character | A / D |
| Move | W A S D |
| Shoot | Arrow keys |
| Pause / resume directly | Escape |
| Confirm Continue / Exit while paused | Enter |
| Use active item | Space |
| Toggle hitbox overlay | U |

## Build, run and test

Prerequisites: CMake 3.24+, a C++20 compiler and network access on the first configure if SFML 3 is not installed. CMake first tries system SFML 3, then fetches pinned SFML 3.0.1.

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/debug --parallel
ctest --test-dir build/debug --output-on-failure
./build/debug/isaac_course_game

cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/release --parallel
ctest --test-dir build/release --output-on-failure
```

Run from the repository root so the documented `assets/` paths resolve. Missing optional textures or sounds produce diagnostics; the View falls back to geometry or continues without audio rather than crashing.

## MVVM overview

The only primary dependency direction is `View -> ViewModel -> Model`:

- Model is pure C++ and owns player, AI, collision, rooms, items, Boss patterns and progression.
- ViewModel accepts commands, invokes Model and builds immutable display DTOs; it neither renders nor owns rules.
- View maps SFML input and draws only DTOs, sprites or fallback geometry.
- Resource owns asset paths/cache; App is only the composition root and fixed-step scheduler.

See [the architecture document](docs/architecture/mvvm-architecture.md), [decision records](docs/architecture/decisions/) and [course defense guide](docs/defense/course-defense-guide.md).

## Evidence and limitations

Research is in [wiki-research.md](docs/research/wiki-research.md), the final visual boundary in [easyisaac-visual-parity.md](docs/design/easyisaac-visual-parity.md), staged evidence in [docs/progress](docs/progress/), and genuine iterations in [agent-iteration-log.md](docs/agent/agent-iteration-log.md). This course version intentionally omits the original game's full roster, items, DLC rules, large layout pools, online play, saves, achievements, controller completeness, installers and pixel-perfect balance. It ships only a small, traced EasyIsaac visual/audio subset and no external font.
