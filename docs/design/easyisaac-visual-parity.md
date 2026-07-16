# EasyIsaac visual parity target

## Baselines

- Midterm backup: tag `v1.0-midterm-backup` at the accepted SFML/MVVM release.
- Final-development branch: `feature/easyisaac-final-replica`.
- Visual reference: `tianguantg/EasyIsaac` at commit `6580ba5`.
- Reference license: Unlicense. Its README also identifies several images and sounds as originating from *The Binding of Isaac*, so imported presentation assets remain restricted to this non-profit classroom demonstration and are recorded in the asset manifest.

## In scope

1. Alternating paper start screen with a prominent start prompt.
2. Paper main menu with a red highlighted selection and `RUN / RANKINGS / BACK` choices.
3. Paper character selection that retains all four configured characters.
4. Dark basement room art scaled to the existing 960-wide Model coordinate system.
5. Runtime conversion of the reference's EasyX foreground/background mask pairs into cached SFML alpha textures.
6. Reference-style player, tear, fly, obstacle, pickup, health, pause, item-banner and ending visuals.
7. Presentation-only bob, pulse, shadow, projectile spin, screen fade and damage flash effects.
8. Reference-style HUD: hearts at the top, stats on the left, time/room at the top, floor at the bottom, inventory and minimap on the right.
9. Pause scroll, defeat testament and victory paper screens.
10. A rank-style paper page for the final course run evidence.
11. Optional, failure-tolerant menu/gameplay and event audio owned by View/Resource only.

## Out of scope

- Replacing SFML with EasyX or making the final build Windows-only.
- Copying the reference's global-state architecture, gameplay code, collision rules or single-room limitation.
- Removing the current four-character, six-enemy, three-Boss, two-floor, item/economy or MVVM implementation.
- Claiming that third-party game art or audio is newly licensed for commercial redistribution.

## Acceptance checks

- The midterm tag and original development branch remain remotely available.
- Start, menu, character select, gameplay, pause, defeat/victory and rank presentations are visibly distinct and match the reference composition.
- GameView consumes DTOs only; Model remains SFML-free and tests/architecture checks pass.
- Every imported asset is traceable to the reference commit and its original repository path.
- Debug and Release builds and full CTest pass.
- Fresh runtime captures show the final paper menu and gameplay HUD.
