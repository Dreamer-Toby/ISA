# ISA — MVVM course roguelike

This repository contains a bounded, non-profit classroom project inspired by *The Binding of Isaac: Rebirth*. It uses C++20 and SFML with a strict `View -> ViewModel -> Model` dependency direction. External artwork is for classroom demonstration only; provenance is recorded in [the asset manifest](docs/assets/asset-sources.md) and must not be reused commercially.

The planned release contains four characters, six ordinary enemies, four bosses, three floors, combat, room traversal, inventory/economy, shops, treasure and secret rooms, a devil-room roll, HUD, menus, pause, defeat and victory.

## Repository status

The upstream repository was empty when work began. The project is being implemented in genuine, reviewable stages on `feature/mvvm-isaac-course-project`. Build and control instructions are added with the executable in Stage 1.

## Architecture

`View` translates SFML events into commands and draws DTOs. `ViewModel` coordinates screen state and projects the pure C++ `Model` into render DTOs. The `Model` owns every gameplay rule and has no SFML dependency. See [the architecture design](docs/architecture/mvvm-architecture.md).

## Current limitations

This is a course-scale interpretation rather than a full clone. Online play, save synchronization, achievements, controller completeness and the original game's full content are intentionally excluded.

