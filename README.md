# ISA — strict-MVVM course roguelike

ISA is a bounded, room-based C++20/SFML 3 game inspired by *The Binding of Isaac: Rebirth*. It is made only for non-profit classroom teaching and assignment demonstration. External artwork remains the property of its respective creators; each file is traced in [the asset manifest](docs/assets/asset-sources.md) and must not be reused for commercial release.

## Version roles

- `v1.0-midterm-backup` is the frozen, accepted midterm version.
- `feature/easyisaac-final-replica` is the final presentation branch. It reproduces the paper menus, basement room, HUD, masked sprites, pause scroll and event sounds of [`tianguantg/EasyIsaac`](https://github.com/tianguantg/EasyIsaac) while retaining this project's larger gameplay loop and strict SFML/MVVM architecture.

The final branch does not depend on EasyX and remains portable beyond Windows. EasyIsaac's paired EasyX color/mask JPEGs are converted to cached alpha textures at runtime by the Resource layer.

## Implemented gameplay

- Alternating start page, selectable main menu, rankings, four-character selection (Isaac, Magdalene, Cain, Judas), pause, defeat and victory screens.
- Fixed-step movement, four-direction tears, enemy projectiles, collision, invulnerability, red hearts and shields (combined cap: 12).
- Three deterministic connected floors. Each has normal, treasure, shop, secret and Boss rooms, with persistent visits, doors and minimap.
- Six configured enemies: Fly, Pooter, Spider, Hopper, Gaper and Clotty, composed from movement/attack/drop strategies.
- Four Bosses: Monstro; Duke of Flies and Larry Jr. together on floor two; simplified Mom's Leg on floor three.
- Coins, bombs, keys, pickups, chest, shop, hidden entrance, active items, stacking passives and trinkets.
- A testable 35% post-Boss devil-room roll and a complete three-floor ending.
- HUD DTOs for red/shield hearts, coins, bombs, keys, active item, minimap, floor and room state.
- EasyIsaac-style room art, characters, tears, flies, props and feedback sounds with presentation-only animation and hitbox inspection.

## Controls

| Action | Key |
|---|---|
| Confirm menu / interact | Enter or Space on paper screens |
| Navigate main menu | W / S |
| Change character | A / D |
| Move | W A S D |
| Shoot | Arrow keys |
| Pause / resume | Escape |
| Use bomb at a secret wall | E |
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
