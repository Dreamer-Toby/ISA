# ADR 0005: resource provenance and cache boundary

Status: accepted

SFML textures/fonts/sounds live in Resource/View only and are cached by normalized path. Missing optional resources produce a diagnostic and View fallback geometry. Every external file is mapped in the asset manifest.

Stage 8 review found that Stage 7 still placed literal asset paths in `GameView`. Commit `refactor: centralize asset paths in resource layer` introduced `AssetCatalog`; View now asks that catalog for appearance paths while continuing to own Sprite construction. This is the project's recorded real architecture-boundary correction.
