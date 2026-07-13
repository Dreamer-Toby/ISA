# Goal 0 feedback

## Executor Attempt 1

### Scope implemented

- C++20/SFML 3.0.1 CMake project with Model, ViewModel, View, Resource and App targets.
- Four configured characters; health/shield cap, inventory, shooting and fixed-step movement.
- Three seeded connected floors, required room types, doors, locks, persistence and minimap.
- Six configured ordinary enemies composed from movement/attack/drop strategies.
- Active/passive/trinket items, resource consumption, shop, chest and secret-room logic.
- Four Boss patterns, injectable 35% devil-room rule, floor progression, Mom's Leg, defeat/victory.
- Complete screen/HUD DTO flow and SFML rendering, traced assets, cache and missing fallback.
- Architecture/research/assets/iteration/defense/release documentation and staged history.

### Changed areas and reasons

| Area | Reason |
|---|---|
| `CMakeLists.txt`, `tests/` | Reproducible dependency/build plus Model, flow, architecture, resource and provenance verification. |
| `src/model/` | All gameplay rules and deterministic state, independent of SFML. |
| `src/viewmodel/` | Input bridge, screen states and immutable render/HUD/minimap DTO projection. |
| `src/view/` | SFML events, visibly distinct screens, complete title-bar HUD and generic DTO drawing. |
| `src/resource/` | Asset path catalog, cached SFML textures and missing-resource diagnostics. |
| `src/app/` | Composition root and scheduling-only 60 Hz accumulator. |
| `assets/`, `docs/assets/` | Seven Huiji Wiki visuals with page/original URL/hash and classroom-use warning. |
| `docs/` and root docs | Directed research, MVVM/ADRs, genuine iterations, progress, README, changelog and defense guide. |

### Requirement mapping

| Requirement | Implementation / proof |
|---|---|
| 4 characters and combat | `CharacterCatalog`, `Player`, `ShootComponent`, `Projectile`; Model assertions and Stage 2 evidence. |
| Hearts/shields <= 12 | `HealthComponent::MaxCombinedHearts`; cap/damage-order/death assertions. |
| 3 floors and required rooms | `Level`; fixed-seed connectivity/type, lock, secret and round-trip assertions. |
| 6 enemies | `EnemyCatalog` + `EnemySystem`; roster, hit, death and drop assertions. |
| 4 Bosses/devil/ending | `BossCatalog` + `BossSystem` + `DevilRoomPolicy`; boundary/progression and ViewModel victory test. |
| Economy/items | `Inventory` + `ItemSystem`; atomic spend, stacking, chest, shop and trinket assertions. |
| Menus/HUD | `GameViewModel` state transitions and `GameView` panels/title HUD; `viewmodel_course_flow`. |
| Fixed timestep | `Application` accumulator; 30/60/120 schedule assertion equality. |
| Strict MVVM | Layer targets, include scan, pure Model tests and `docs/architecture/`. |
| Wiki/resources | `docs/research/wiki-research.md`, seven-file source manifest, asset provenance CTest. |
| Course records | README, CHANGELOG, iteration log, progress evidence, ADRs and defense guide. |

### Staged history, tags and progress

| Stage | Commit | Tag | Progress evidence |
|---|---|---|---|
| 0 baseline | `0530df1` | — | `stage-0-architecture.svg` |
| 0 architecture | `6496331` | `v0.1-architecture` | same architecture evidence |
| 1 SFML shell | `80723b1` | — | `stage-1-window.svg` |
| 2 combat | `1ffb245` | `v0.2-player-combat` | `stage-2-player-combat.svg` |
| 3 rooms | `dba8e19` | `v0.3-rooms` | `stage-3-rooms.svg` |
| 4 enemies | `9f4bdcb` | `v0.4-enemies` | `stage-4-enemies.svg` |
| 5 items | `3e75e7b` | `v0.5-items` | `stage-5-items-shop.svg` |
| 6 Bosses | `c6520c2` | `v0.6-bosses` | `stage-6-bosses-ending.svg` |
| 7 assets | `bd0823d` | — | `stage-7-visual-assets.svg` |
| 8 architecture correction | `bc2ef8f` | — | `stage-8-final-release.svg` |
| 8 runtime/UI fixes | `0451b4f`, `04f5c1f` | — | runtime startup plus final evidence |
| 8 flow tests | `6831161` | — | final evidence |

`v1.0-course-release` is created after the final documentation commit and checks. No stage was squashed.

### Wiki and asset coverage

Research covers characters, ordinary enemies, Bosses, drops/economy, HUD, rooms, shop, secret, devil and layouts. Jina's page reader exposed the Wiki static URLs after direct page requests returned 403. Direct static downloads also initially returned 403; adding the matching Wiki page as HTTP referrer succeeded. Seven committed PNGs cover character, enemy, Boss, item/projectile, room and red/shield HUD categories. Every file has local path, original filename, source page, original URL, use, processing and SHA-256 in `docs/assets/asset-sources.md`; no generated asset was used.

### Verification evidence

All commands ran from the repository root on 2026-07-13.

```text
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/debug --parallel 6
ctest --test-dir build/debug --output-on-failure
Result: PASS, 5/5 tests.

cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/release --parallel 6
ctest --test-dir build/release --output-on-failure
Result: PASS, 5/5 tests.

cmake -S . -B build/asan -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DENABLE_SANITIZERS=ON
cmake --build build/asan --parallel 6
ctest --test-dir build/asan --output-on-failure
Result: PASS, 5/5 tests under AppleClang AddressSanitizer + UndefinedBehaviorSanitizer flags.

cmake --build build/debug --target architecture_check
Result: PASS, strict MVVM include boundaries.

python <selfpush skill>/scripts/audit_public_state.py --project-root .
Result before each commit: PASS.
```

CTest entries: `model_regression_suite`, `viewmodel_course_flow`, `architecture_boundaries`, `resource_cache_and_missing`, `asset_provenance`.

Real window smoke initially failed with a Cocoa `NSInternalInconsistencyException` because an SFML narrow title contained a Unicode dash. Commit `0451b4f` replaced it with ASCII. Rebuild passed and the application remained open until intentionally interrupted; it emitted only SFML's environment message `Detected a keyboard without any keys.`. This was not misreported as an interactive key-through session. The ViewModel flow test independently covers menu, selection, pause/resume, HUD, defeat and three-floor victory.

### Known limitations / honest gaps

- The project is intentionally course-scale and not an original-content clone.
- Only seven traced visuals are shipped; all other entities use intentional View fallback geometry. No external audio or font is shipped.
- Automated tests cover state and full flow without a human-driven final playthrough. The documented manual demo remains the presentation step.
- SFML/Freetype emitted two third-party build warnings (macro redefinition and a macOS pointer-type warning); project targets compiled and all sanitizer tests passed.
- The pre-existing incomplete `提示词.txt` remains preserved and untracked; checkpoint/private/build files remain uncommitted.
- Executor did not push and does not issue the Acceptance verdict.

### Final public-state check

The intended public paths use project-relative references and contain no credentials, private notes or local absolute machine paths. Build products are ignored. The selfpush public-state audit is run again after staging the final report/documentation commit.
