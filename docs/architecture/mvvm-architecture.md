# MVVM architecture

## Dependency rule

```text
SFML events -> View -> ViewModel -> Model
                  ^        |
                  +-- immutable RenderDTO

App creates the objects and schedules pollEvent -> fixed update -> render.
Resource is used only by App/View and owns SFML resource objects.
```

The direction describes compile-time knowledge. Model never knows ViewModel, View or SFML. ViewModel knows Model interfaces but contains no game rules. View knows only ViewModel commands and DTOs, never concrete Model types.

## Responsibilities

- `src/common`: generic `Vec2`, rectangles, directions and shared enums only.
- `src/model`: player, health, shooting, inventory, enemies, projectiles, collision, drops, rooms, map generation, items, bosses and progression.
- `src/viewmodel`: input commands, screen state and conversion of Model snapshots to render DTOs.
- `src/view`: SFML event mapping and rendering of generic entity/room/HUD DTOs.
- `src/resource`: texture/font/sound cache, path catalog and fallback diagnostics; no gameplay decisions.
- `src/app`: composition root, window lifetime and accumulator loop only.

## Fixed timestep

Rendering may occur at any rate. `Application` accumulates frame time and calls `GameViewModel::update(1/60, input)` once per fixed step, with a frame-time clamp to prevent a spiral after a breakpoint. Model speeds and cooldowns therefore operate in seconds and produce equivalent movement/shooting at 30, 60 and 120 rendered FPS.

## Input-to-render flow

1. View polls SFML and creates an `InputCommand` containing held movement and edge-triggered actions.
2. ViewModel applies UI-state transitions and passes gameplay commands to `GameSession::update`.
3. Model updates movement, AI, attacks, collision, drops, doors and progression.
4. ViewModel builds a stable `DisplayState`/`RenderDTO` snapshot.
5. View draws the snapshot; it cannot modify gameplay state.

## Extension points

- A character is a `CharacterDefinition` entry consumed by Player; View receives the same generic player DTO.
- An enemy combines an `EnemyDefinition` with a movement and attack strategy enum/factory; View renders a generic entity kind.
- A boss uses a Model-owned phase state machine and a generic boss DTO.
- An item is an `ItemDefinition` plus a Model-owned effect kind. UI displays DTO strings and never applies effects.

## Enforcement

CMake exposes one target per layer and links only permitted directions. `tests/architecture/check_architecture.cmake` scans includes for reverse dependencies and SFML leakage. Model tests link only Model, proving rules operate without a window.

