# Agent iteration log

Commit hashes are recorded only after the referenced commit exists; stage tags provide stable evidence without self-referential fake hashes.

## Iteration 0 — 2026-07-13 11:10 +08:00

- Objective: inspect the empty repository, freeze scope, research required Wiki areas and design strict MVVM.
- Prior problem: no product files, build system, history, dependency decision or architecture boundary existed.
- Options: start coding immediately, or establish auditable constraints first. Selected a short design baseline because architecture and provenance are hard requirements.
- Actual change: README/CHANGELOG skeletons, research matrix, source manifest, architecture design, five ADRs and progress evidence.
- Architecture effect: defines and forbids reverse dependencies before targets exist.
- Verification: repository/branch, remote heads, CMake 4.3.2 and Apple Clang 21 inspected; Wiki pages accessed through search/indexed copies; public-state audit scheduled before commit.
- Evidence: `docs/progress/stage-0-architecture.svg`.
- Unresolved: SFML is not installed system-wide; Stage 1 will pin FetchContent. Direct Wiki binary requests returned HTTP 403 and will be retried before Stage 7.
- Next: add the C++20/SFML MVVM shell and automated architecture check.
- MVVM self-check: no production layers exist yet; documentation consistently specifies View -> ViewModel -> Model and pure Model.

## Iteration 1 — 2026-07-13 11:24 +08:00

- Objective: create the minimum compilable/runnable C++20 + SFML MVVM shell.
- Prior problem: no build targets, executable, input path, DTO path or enforcement existed.
- Options: require a machine package or fetch a pinned dependency. Selected `find_package` with pinned SFML 3.0.1 fallback for reproducibility.
- Actual change: per-layer libraries, SFML window, event mapper, 60 Hz accumulator, pure Model session, ViewModel snapshot, resource cache and checks.
- Architecture effect: the CMake graph and include scan now enforce the documented dependency direction.
- Verification: Debug configure/build succeeded; two CTest tests and `architecture_check` passed. The first scan falsely matched `viewmodel/` as `model/`; its pattern was narrowed to actual include directives before this commit.
- Evidence: `docs/progress/stage-1-window.svg` is an honest reconstruction of the fallback geometry drawn by the Stage 1 View, not a claim of an external asset.
- Unresolved: gameplay remains the intentional shell; binary Wiki assets remain pending.
- Next: add character selection, components, shooting/projectiles and HUD.
- MVVM self-check: Model contains no SFML; View includes only ViewModel/Resource; ViewModel owns no collision/AI/drop rules; App loop only schedules.

## Iteration 2 — 2026-07-13 11:38 +08:00

- Objective: deliver the first playable movement/shooting slice with four real character configurations.
- Prior problem: the shell had only a moving dot and placeholder HUD state.
- Options: per-character subclasses or a small catalog plus components. Selected catalog/components to meet extension requirements without an unused framework.
- Actual change: character catalog, Player, health/shield, inventory, shooting, projectile lifecycle, selection/pause transitions, HUD and tests.
- Architecture effect: combat timing and health rules remain entirely in Model; ViewModel only selects and projects.
- Verification: Debug build/CTest and architecture check; 30/60/120 simulated render schedules produce equal fixed-step positions and shot counts.
- Evidence: `docs/progress/stage-2-player-combat.svg` faithfully shows the View fallback shapes.
- Unresolved: enemies and collision arrive in Stage 4; HUD uses shapes until final resource work.
- Next: deterministic connected room graph, doors, persistence and minimap.
- MVVM self-check: no SFML in Model; no Model includes in View; no business rule in App/Resource/ViewModel.

## Iteration 3 — 2026-07-13 11:48 +08:00

- Objective: add connected rooms, gates, persistence and minimap.
- Prior problem: the player was confined to one visual rectangle.
- Options: a general procedural graph or a small seeded topology. Selected a seeded six-node topology because it guarantees all course room types and remains testable.
- Actual change: Level/Room graph, adjacency, key/bomb entry, clear requirement, transitions, persistent visited state and minimap projection.
- Architecture effect: generation and entry costs live in Model; View sees generic minimap DTOs.
- Verification: connectivity, required types, secret/key consumption and repeated round-trip persistence assertions plus full Debug CTest/architecture scan.
- Evidence: `docs/progress/stage-3-rooms.svg` is the exact course topology diagram.
- Unresolved: combat rooms are populated in Stage 4.
- Next: six enemy definitions, strategies, collision, drops and clear loop.
- MVVM self-check: Room/Level rules are pure Model; ViewModel contains projection only.

## Iteration 4 — 2026-07-13 12:00 +08:00

- Objective: complete ordinary combat rooms with six distinguishable enemies.
- Prior problem: room traversal had no opposition or clear condition.
- Options: subclasses per enemy or configured strategy composition. Selected composition to keep behavior data-driven and small.
- Actual change: six definitions, five movement strategies, three attack strategies, three drops, collisions, invulnerability, pickup collection and clear-to-open integration.
- Architecture effect: AI, collision and drop selection are confined to Model's EnemySystem.
- Verification: roster count, projectile hit/destruction, damage/death, drop and clear assertions plus project checks.
- Evidence: `docs/progress/stage-4-enemies.svg` reflects generic View entities rather than claiming copied artwork.
- Unresolved: item effects and special rooms are next.
- Next: shop/chest/secret interactions and all inventory categories.
- MVVM self-check: View receives only generic entity DTOs; ViewModel has no strategy/collision/drop implementation.

## Iteration 5 — 2026-07-13 12:12 +08:00

- Objective: close the inventory/economy loop across treasure, shop and secret rooms.
- Prior problem: resource counters existed but special rooms had no item outcomes.
- Options: broad item scripting or a fixed course catalog. Selected five definitions and four effect kinds to demonstrate every required category without speculative scripting.
- Actual change: active healing/damage, stacking passives, luck trinket, chest key cost, shop coin purchase and secret reward.
- Architecture effect: ItemSystem applies all effects to Model components; View/HUD only display strings and counts.
- Verification: resource success/failure, passive stacking, chest, shop and trinket assertions plus project checks.
- Evidence: `docs/progress/stage-5-items-shop.svg` documents the three interaction loops.
- Unresolved: Boss progression/devil room and ending remain.
- Next: four Boss state definitions, three-floor progression, defeat and victory.
- MVVM self-check: item prices/effects and interaction state remain in Model; no SFML or reverse includes introduced.

## Iteration 6 — 2026-07-13 12:25 +08:00

- Objective: finish the three-floor run with four Bosses, devil room, defeat and victory.
- Prior problem: the map had a Boss room but no Boss encounter or terminal flow.
- Options: subclass state machines or one explicit pattern-driven state record. Selected the latter to keep four distinct, auditable behaviors.
- Actual change: Monstro, Duke, Larry Jr. and Mom's Leg definitions; health phases; attacks; deterministic devil rule; floor and screen progression.
- Architecture effect: all patterns, probability and progression are Model state; View renders generic Boss DTOs.
- Verification: roster/final identity, floor-two dual spawn, death, 35% probability boundaries, devil insertion and floor advance tests.
- Evidence: `docs/progress/stage-6-bosses-ending.svg` maps the real encounter ordering.
- Unresolved: traced visuals, final robustness checks and defense docs.
- Next: integrate traceable assets/fallback cache behavior, then release verification.
- MVVM self-check: Boss AI/state/progression remain Model-only; App loop unchanged.

## Iteration 7 — 2026-07-13 12:42 +08:00

- Objective: replace key fallback shapes with traceable Wiki visuals and verify resource behavior.
- Prior problem: direct Wiki/static requests returned HTTP 403 and the runtime used only authored geometry.
- Options: generated replacements or retry authenticated-looking page navigation. The static host succeeded with its source page as referrer, so no generation was needed.
- Actual change: seven original files, per-file URLs/hashes, runtime sprite integration, nearest-neighbor cache, missing diagnostics and provenance/cache tests.
- Architecture effect: SFML Texture/Sprite remains only in Resource/View; Model is unchanged.
- Verification: file type/hash checks, manifest cross-check, cache pointer identity and missing-file diagnostic test.
- Evidence: `docs/progress/stage-7-visual-assets.svg` embeds the actual committed files.
- Unresolved: final Debug/Release/sanitizer and course documentation pass.
- Next: comprehensive release verification and defense guide.
- MVVM self-check: asset names influence appearance only; no game rule branches on texture state.
