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

