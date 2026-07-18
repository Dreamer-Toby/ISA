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

## Why the final version does not switch to EasyX

EasyIsaac is the final visual reference, not the new architecture. EasyX is Windows/Visual-C++ specific, while the accepted project already uses SFML 3 and a testable MVVM boundary. Replacing the renderer would add platform risk and encourage copying the reference's global-state design without improving gameplay. The final branch instead imports a small traced asset subset, converts EasyX's `_back`/`_front` mask pairs to alpha textures inside Resource, and reproduces the paper menus, basement room, HUD and pause composition in `GameView`. Later gameplay corrections still follow the same boundary: collision/items/progression stay in Model, input selection stays in ViewModel, and asset choice stays in View.

## Test strategy

Model assertions cover the 12-heart cap/damage order, exact half-heart traps, rock/player/projectile collision, complete run reset, Breakfast drop effects, exact four-room floors, one visible treasure item per floor, complementary Sad Onion/Wiggle Worm rewards, sine trajectories, clear-gated doors, Boss/two-floor boundaries and 30/60/120 FPS-equivalent fixed steps. A ViewModel flow test covers the three-option main menu, rankings, selection, stable pause selection, pause exit/resume, HUD, defeat/restart and two-floor victory. Resource tests verify pointer-identical texture/audio caching, differently sized EasyX mask pairs, missing diagnostics and one-to-one provenance. Debug and Release builds plus the architecture scan are release gates.

## Scope simplification

The original game's catalogs, exact DLC formulas, layout pools, angel/deal conditions and heart taxonomy become four characters, six enemies, three Bosses, two four-room floors and half-heart-aware red/shield health. This keeps a closed architecture/gameplay demonstration without pretending to be a full clone.

## Demonstration order

1. Start and choose characters to show configured stat differences.
2. Move/shoot; use both stable pause choices; show HUD and minimap.
3. Walk into a rock, shoot it, and step on a trap to show physical volume and half-heart damage.
4. Collect a monster-dropped Breakfast to demonstrate one added container plus one healed heart.
5. Enter both treasure rooms to show one Sad Onion and one Wiggle Worm in opposite floor order, then finish the floor-two dual-Boss encounter.
6. Trigger defeat separately and present deterministic tests/architecture scan.

Likely questions: “Why not ECS?” — unnecessary for this bounded roster. “Does View include Model?” — the scan rejects it. “What if assets are absent?” — Resource reports it and View falls back. “How is randomness tested?” — a run seed selects the first treasure and the second is its deterministic complement.
