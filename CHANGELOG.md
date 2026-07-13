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
