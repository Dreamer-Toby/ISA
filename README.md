# ISA — MVVM course roguelike

This repository contains a bounded, non-profit classroom project inspired by *The Binding of Isaac: Rebirth*. It uses C++20 and SFML with a strict `View -> ViewModel -> Model` dependency direction. External artwork is for classroom demonstration only; provenance is recorded in [the asset manifest](docs/assets/asset-sources.md) and must not be reused commercially.

The planned release contains four characters, six ordinary enemies, four bosses, three floors, combat, room traversal, inventory/economy, shops, treasure and secret rooms, a devil-room roll, HUD, menus, pause, defeat and victory.

## Build and run

SFML 3.0.1 is used. CMake first tries an installed SFML 3 and otherwise downloads the pinned release. A network connection is therefore required for the first configure when SFML is absent.

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/debug --parallel
ctest --test-dir build/debug --output-on-failure
./build/debug/isaac_course_game
```

Controls: `Enter` advances menus, `WASD` moves, arrow keys shoot, `Esc` pauses, `E` uses a bomb and `Space` uses the active item.

## Architecture

`View` translates SFML events into commands and draws DTOs. `ViewModel` coordinates screen state and projects the pure C++ `Model` into render DTOs. The `Model` owns every gameplay rule and has no SFML dependency. See [the architecture design](docs/architecture/mvvm-architecture.md).

## Current limitations

This is a course-scale interpretation rather than a full clone. Online play, save synchronization, achievements, controller completeness and the original game's full content are intentionally excluded.
