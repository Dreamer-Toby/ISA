# Public Project Context

- Created: 2026-07-13T10:48:03+08:00
- Repository state at bootstrap: the supplied GitHub repository has no refs or files; the local directory was not a Git repository and contained one pre-existing, incomplete prompt copy that must be preserved.
- Development branch: `feature/mvvm-isaac-course-project`.
- Language and standard: C++20 (C++17-compatible design where practical).
- Build system: CMake with CTest; SFML is the required window/rendering dependency.
- Architecture invariant: the primary dependency direction is `View -> ViewModel -> Model`. Model uses pure C++ types and has no SFML dependency. View does not include Model headers.
- Runtime convention: App owns composition and a fixed-timestep main loop; game rules stay in Model; ViewModel translates commands and produces render DTOs; View and Resource own SFML rendering/resources.
- Project verification must include Debug and Release builds, CTest model tests, an automated dependency-boundary check, resource-missing behavior, and a non-interactive smoke path suitable for CI/headless machines.
- Documentation must include research sources and adopted simplifications, per-asset provenance, MVVM architecture and ADRs, iteration history, progress evidence, README, changelog, and a course-defense guide.
- Delivery convention: meaningful stages retain separate commits and milestone tags. The whole `goal0` is accepted only after all stages and deliverables are independently verified.
- Public state must use project-relative paths and contain no usernames, credentials, absolute machine paths, or private operational notes.
