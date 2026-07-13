# Active Goal

- Goal ID: goal0
- Activated: 2026-07-13T11:03:00+08:00
- Status: ACTIVE
- Final supplemental rework plans remaining after this plan: 1

## Objective

Build and verify the complete C++20 + SFML course project described by `mission.md` on `feature/mvvm-isaac-course-project`: a playable, room-based roguelike with strict `View -> ViewModel -> Model` dependencies, traceable research/assets, genuine staged history, tests, documentation, and release evidence.

The intentionally bounded content is four selectable characters (Isaac, Magdalene, Cain, Judas), six normal enemy kinds, four bosses (the floor-three final boss is a simplified Mom's Leg), three floors, and at least one complete implementation of every required room, inventory, item, UI, and progression category. The Commander may authorize at most one final supplemental rework plan, so required work must not be deferred.

## In Scope

1. Start menu, character select, play, pause, failure, victory/results states.
2. Player movement, four-direction shooting, projectile lifetime/collision, damage, death, invulnerability, pickups, and room traversal.
3. Red hearts and shield/soul hearts with a combined maximum of 12.
4. Three connected floors. Every floor has normal, treasure, shop, secret, and boss rooms; doors, locks, room persistence, clear-to-open behavior, transitions, and minimap.
5. Six configured normal enemies using composable movement, attack, and drop strategies.
6. Four bosses using a phase/state machine, boss death, injectable/testable devil-room probability, floor progression, simplified Mom's Leg, final completion.
7. Coins, bombs, keys, active items, passive items, trinkets, chests, shops, purchasing, attribute stacking, and secret-room reveal/entry.
8. HUD for hearts, shields, coins, bombs, keys, active item, minimap, floor, and room state.
9. Directed research of necessary Huiji Wiki subpages beyond the homepage. Record title, page/source URLs, access date, extracted rule or visual reference, adoption decision, simplification, and conflicts in `docs/research/wiki-research.md` without copying long prose.
10. Traceable Wiki visual assets organized under `assets/`; every external file must be mapped in `docs/assets/asset-sources.md` to its local path, original name, page URL, original asset URL when available, use, and processing. Only use a generated replacement after a real lookup/download failure and record prompt/date/use/processing.
11. A real staged Git history, progress evidence, changelog, Agent iteration log, architecture and ADRs, README, and course-defense guide.

## Out of Scope

- Full original-game content or exact balance; multiplayer, online services, achievements, controller completeness, editors, ECS/plugin frameworks, commercial distribution, pixel-perfect replication, installers, or speculative abstractions.
- Deliberately introducing defects to simulate iteration.

## Architecture Contract

- Model is pure C++ and owns all game rules. It cannot depend on SFML, ViewModel, or View.
- ViewModel receives `InputCommand`/`ActionState`, invokes Model, and exposes stable read-only `RenderDTO`/`DisplayState`. It contains no rendering, AI, collision, generation, drop, or other core rules.
- View owns SFML input/rendering and depends only on ViewModel APIs/DTOs; it cannot include or call concrete Model types.
- Resource owns textures, fonts, sound buffers, animation metadata, caching, and missing-resource handling, but no game rules.
- App is the composition root and clean `pollEvent -> fixed update -> render` scheduler, with no gameplay rules.
- Common contains only generic types, constants, enums, and utilities.
- CMake target boundaries plus an automated dependency scan must enforce the contract. New characters/enemies/items must not require View, App-loop, or Common changes.

## Required Structure and Deliverables

- Root CMake project, `src/{app,common,model,viewmodel,view,resource}`, `assets/`, `tests/`, README, CHANGELOG.
- `docs/architecture/mvvm-architecture.md` and ADRs for MVVM, Model/SFML isolation, RenderDTO, enemy strategies/Boss state, fixed timestep, and resource boundaries.
- `docs/research/wiki-research.md`, `docs/assets/asset-sources.md`, `docs/agent/agent-iteration-log.md`, `docs/progress/`, and `docs/defense/course-defense-guide.md`.
- README covers purpose, implemented gameplay, controls, build/run/test, MVVM overview, asset notice, and limitations. It states non-profit classroom use, points to the source manifest, and warns against commercial reuse.
- Defense guide explains MVVM choice and responsibilities, a View-to-Model-to-DTO input path, extension of character/enemy/item, real iterations, at least one naturally discovered architecture issue and its real fix, testing, and simplifications.

## Stage and History Plan

Each stage must finish in a reviewable state with real differences, build/test evidence where applicable, an Agent-log and CHANGELOG update, at least one progress image or honest architecture/evidence capture in `docs/progress/`, and a distinct commit. Never squash or fabricate hashes/evidence.

0. Inspect/freeze requirements, toolchain and research/resource plan; create baseline docs. Commits include `chore: establish course project baseline and research plan` and `docs: add MVVM architecture design`; tag `v0.1-architecture`.
1. Add C++20 CMake/SFML dependency, MVVM targets/shell, window, resource layer, minimal DTO/input, fixed-timestep loop, smoke and architecture checks. Commit `chore: initialize C++20 SFML build and MVVM shell`.
2. Add four-character selection, player components, movement, shooting/projectiles, HUD, pause, and timestep tests. Commit `feat: add character selection and player combat vertical slice`; tag `v0.2-player-combat`.
3. Add connected room graph, required room kinds, doors, transitions, persistence, and minimap. Commit `feat: add connected room graph doors and minimap`; tag `v0.3-rooms`.
4. Add six enemy configurations, strategies, collision, damage, death, drops, and clear-room loop. Commit `feat: add enemy strategies collision and drops`; tag `v0.4-enemies`.
5. Add inventory/economy, all item categories, shops, chests, purchases, and secret rooms. Commit `feat: add inventory items shop chests and secret rooms`; tag `v0.5-items`.
6. Add four bosses, devil room, three-floor progression, simplified Mom's Leg, failure and ending. Commit `feat: add bosses devil room floor progression and ending`; tag `v0.6-bosses`.
7. Integrate final traced Wiki visuals, sizing/transparency/nearest-neighbor processing, simple animation/audio only after the loop works, cache and missing-resource handling. Commit `feat: integrate traced visual assets and presentation polish`.
8. Add/finish regression and architecture tests, Debug/Release/sanitizer verification, real fixes, final docs and release evidence. Commits include `test: add model regression and architecture checks` and `docs: finalize course defense guide and release evidence`; tag `v1.0-course-release` only after the release state is complete. Any naturally required architecture migration gets a separate `refactor:` commit.

Because goal0 is one whole-project goal, these stage commits remain implementation checkpoints. Acceptance evaluates the complete goal and history once Executor reports completion; Commander archives accepted state afterward.

## Measurable Acceptance Criteria

1. Debug and Release configure/build succeed with C++20 and the documented SFML dependency; the executable has a headless/noninteractive smoke path or equivalent testable startup behavior.
2. Full CTest passes and contains effective assertions for: combined heart/shield cap and damage order; damage/death; projectile hit/destruction; clear-room doors; key/bomb/coin consumption success/failure; passive stacking; shop purchase; secret-room condition; boss death/devil probability/floor progression; deterministic map connectivity and required room types.
3. Architecture check returns zero, CMake links only permitted directions, Model has no SFML/View/ViewModel include, View has no Model include, and ViewModel has no View or SFML drawing dependency. Code review confirms rules are in Model and the App loop is scheduling-only.
4. Four characters, six enemies, four distinct bosses, all five required room categories per floor, three floors, final Mom's Leg, devil room, all item/inventory categories, all required menus, and all HUD fields exist in actual code and can occur in a deterministic/demo flow, not just documentation.
5. Same fixed-duration inputs simulated at 30/60/120 render FPS yield movement and shot counts within a documented tolerance; multiple room round trips preserve cleared state.
6. Missing noncritical assets cause a clear diagnostic or recorded visual fallback rather than an unhandled crash; Resource cache does not recreate a resource for the same path.
7. Wiki research covers every implemented content/mechanic/visual category with the required fields. Every committed external asset has a manifest row and every manifest path exists; no images/audio are scattered in source.
8. Stage 0-8 commits, required tags, log entries, CHANGELOG entries, and progress files are real and temporally consistent. Agent log records objective, prior problem, options, actual change, architecture effect, build/test results, evidence path, unresolved items, next step, and MVVM self-check for each iteration.
9. README, architecture doc/ADRs, research, asset manifest, Agent log, progress evidence, defense guide, build/test instructions, and limitations are complete and mutually consistent.
10. Debug and Release CTest, architecture scan, resource trace check, missing-resource check, room re-entry check, framerate check, and Address/Undefined sanitizer checks (when supported) are recorded honestly. Manual/demo smoke covers menu, selection, combat, clear/open/transition, pickup, shop/chest/secret, boss, floor change, final ending, pause, and failure.
11. Public-state audit passes; no private state, credentials, absolute local paths, build products, or unrelated user changes are committed. The pre-existing incomplete prompt copy is preserved and only committed if its purpose is explicitly documented.
12. `feedback/feedback0.md` lists changed files/reasons, mission mapping, stage commits/tags/screenshots, research/assets coverage, exact verification commands/results, limits, and audit evidence without claiming Acceptance.

## Verification Commands

Use equivalent documented commands if the generator requires adjustment:

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/debug --parallel
ctest --test-dir build/debug --output-on-failure

cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/release --parallel
ctest --test-dir build/release --output-on-failure

cmake -S . -B build/asan -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DENABLE_SANITIZERS=ON
cmake --build build/asan --parallel
ctest --test-dir build/asan --output-on-failure

cmake --build build/debug --target architecture_check
```

Also run the project's resource-provenance checker, application/demo smoke, public-state audit, `git status`, history/tag inspection, and manual visual checks. Before any push, re-query the remote; do not force or overwrite unexpected history.

## Risks and Controls

- Pin one SFML major/minor strategy and do not mix APIs. Prefer system SFML when found; otherwise use a reproducible CMake fetch.
- Inject seeds/RNG for maps and devil rooms.
- If Wiki blocks or lacks an asset, record the failed URL/date before using a documented generated replacement.
- Do not claim screenshots, sanitizer results, gameplay, or source provenance that was not actually obtained.
- If a graphics/audio environment is unavailable, keep Model/CTest independent and report the genuine remaining evidence gap; do not substitute a unit test for a visual screenshot.
- Re-check remote refs before publishing. Escalate unexpected remote history, authentication, branch protection, or any need for rewriting history.

## Executor Report Contract

Append attempts to `feedback/feedback0.md`. Include changed files/reasons, requirement mapping, stage commit/tag/progress table, Wiki/resource coverage, exact commands with exit results, known limitations/blockers, and audit results. Implement only this goal, preserve unrelated changes, do not push, do not declare Acceptance, and do not overwrite Acceptance notes.
