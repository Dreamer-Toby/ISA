# ADR 0005: resource provenance and cache boundary

Status: accepted

SFML textures/fonts/sounds live in Resource/View only and are cached by normalized path. Missing optional resources produce a diagnostic and View fallback geometry. Every external file is mapped in the asset manifest.

