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
