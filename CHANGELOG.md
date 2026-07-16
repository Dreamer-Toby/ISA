# Changelog

## Unreleased

### Stage 0 — project baseline

- Froze the course-scale scope and strict MVVM dependency rules.
- Recorded toolchain, repository, Wiki research, asset and verification plans.
- Added architecture design and initial decision records.

### Stage 1 — runnable MVVM shell

- Added a pinned SFML 3.0.1 CMake dependency and C++20 layer targets.
- Added an SFML window, scheduling-only fixed-step loop, commands, DTOs and fallback drawing.
- Added Model smoke and strict include-boundary checks.

### Stage 2 — player combat vertical slice

- Added four configured characters, health/shield, inventory and shooting components.
- Added movement, projectile lifetime, character selection, pause state and HUD DTOs.
- Added heart cap, damage/death, projectile and render-rate consistency tests.

### Stage 3 — rooms and minimap

- Added deterministic connected floors with normal, treasure, shop, secret and Boss rooms.
- Added clear-to-open traversal, key/bomb gates, persistent room visits and minimap DTOs.

### Stage 4 — enemies and clear loop

- Added six configured enemies composed from movement, attack and drop strategies.
- Added enemy/projectile/contact collision, death, pickups and room clear behavior.

### Stage 5 — items and economy

- Added active, passive and trinket item categories with Model-owned effects.
- Added chest key use, shop purchase, passive stacking and secret-room trinket flow.

### Stage 6 — bosses and ending

- Added four Boss definitions and timed two-phase state patterns, including Mom's Leg.
- Added deterministic devil-room chance, three-floor progression, defeat and victory states.

### Stage 7 — traced visuals and resource robustness

- Integrated seven Huiji Wiki images across character, enemy, Boss, item, room and HUD categories.
- Added nearest-neighbor cached loading, clear missing-resource fallback diagnostics and provenance tests.

### Stage 8 — course release

- Added end-to-end ViewModel flow tests and finalized regression, provenance and architecture checks.
- Corrected asset-path ownership into Resource `AssetCatalog` after final architecture review.
- Completed README, defense material and release verification evidence.

### Acceptance correction

- Added integrated uncleared-room door coverage and a no-shortcut deterministic full course demo.
- Rendered every HUD field inside the window and exposed current character stats during selection.
- Added all four character icons, dedicated implemented-item research and per-content failed-asset lookup records.
- Added genuine SFML window captures for start, selection, gameplay HUD and pause states.

### Stage 9 — EasyIsaac final presentation

- Froze the accepted midterm build at `v1.0-midterm-backup` and moved final work to `feature/easyisaac-final-replica`.
- Added paper start/menu/rank/selection/ending screens, basement room art, masked character/enemy/projectile/item sprites, reference-style HUD and pause scroll.
- Added runtime EasyX mask-pair conversion, cached SFML Audio event effects, transition/damage feedback and optional hitbox inspection.
- Preserved the strict MVVM dependency direction and the full four-character, six-enemy, four-Boss, three-floor course loop.
- Added menu/rank/quick-run tests, mask/audio resource tests, complete imported-asset provenance and reproducible final framebuffer captures.

### Stage 10 - illustrated doors and mission guidance

- Imported four door images and six character portraits from the user-provided local material library. The View now draws directional normal, treasure, locked, Boss and trapdoor states, plus distinct Cain, Judas and Magdalene heads.
- Added pure Model door snapshots, contextual ViewModel mission text, per-floor reward reset and regression coverage for hidden, locked, sealed and cleared exits.
