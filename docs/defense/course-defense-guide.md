# Course defense guide

## Why MVVM

The project must keep a testable game even when no window or art is available. MVVM gives the pure C++ Model sole ownership of rules, makes ViewModel a narrow command/snapshot bridge, and confines SFML to View/Resource/App. CMake target edges and an include scanner enforce this rather than relying on folder names.

## Responsibilities and direction

`GameView` polls SFML and creates `InputCommand`. `GameViewModel` handles menu state, forwards a plain `GameplayInput`, then translates `SessionSnapshot` into `DisplayState`. `GameSession` and its Model systems perform movement, cooldowns, AI, collision, drops, room generation, item effects, Boss phases and progression. View draws generic DTOs. No reverse dependency exists.

## One complete input path

Holding `D` and `Right Arrow` sets movement/shooting vectors in `GameView::inputCommand`. At each 1/60-second accumulator step, `Application` calls `GameViewModel::update`. ViewModel forwards values to `GameSession::update`; Player moves and ShootComponent may create a Projectile. Collision/Enemy/Boss systems update it. ViewModel copies positions and HUD values into DTOs, and View renders them. Rendering never mutates the Player.

## Extending content

- Character: append one `CharacterDefinition`; Player components consume it and generic View DTOs need no change.
- Enemy: append one `EnemyDefinition` combining existing movement, attack and drop strategies.
- Item: append an `ItemDefinition` and one Model-owned effect case. UI only receives display text.
- Boss: append a definition/pattern and spawn rule; View still renders a generic Boss entity.

## Real iteration and architecture correction

History moves from research/architecture to SFML shell, combat, rooms, enemies, items, Boss ending, traced assets and release tests. Stage 8 review found literal asset paths in `GameView`. Texture objects were isolated correctly, but path catalog responsibility belonged to Resource. Commit `bc2ef8f` moved every mapping to `resource::AssetCatalog`; architecture and tests were rerun. Stage 1 had also exposed an overly broad architecture regex that mistook `viewmodel/` for a Model include; it was narrowed to actual include directives before that stage commit.

## Test strategy

Model assertions cover the 12-heart cap/damage order, death, projectile hit/destruction, clear-gated doors, key/bomb/coin success/failure, passive stacking, shop/chest, secret reveal, Boss/dead/devil/floor boundaries, fixed-seed connectivity and 30/60/120 FPS-equivalent fixed steps. A ViewModel flow test covers menu, selection, pause/resume, HUD, defeat and three-floor victory. Resource tests verify pointer-identical caching, missing diagnostics and one-to-one provenance. Debug, Release and supported sanitizer builds are recorded in Executor feedback.

## Scope simplification

The original game's catalogs, exact DLC formulas, layout pools, angel/deal conditions and heart taxonomy become four characters, six enemies, four Bosses, six-node floors, one 35% devil rule and red/shield hearts. This keeps a closed architecture/gameplay demonstration without pretending to be a full clone.

## Demonstration order

1. Start and choose characters to show configured stat differences.
2. Move/shoot; pause/resume; show HUD and minimap.
3. Clear a normal room and observe transition/drop.
4. Spend key/coin/bomb at chest, shop and hidden entrance; show item state.
5. Show four Boss definitions and finish floor three against Mom's Leg.
6. Trigger defeat separately and present deterministic tests/architecture scan.

Likely questions: “Why not ECS?” — unnecessary for this bounded roster. “Does View include Model?” — the scan rejects it. “What if assets are absent?” — Resource reports it and View falls back. “How is randomness tested?” — floors use seeds and devil rooms accept a unit roll.
