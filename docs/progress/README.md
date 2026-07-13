# Runtime progress evidence

The historical SVG files remain explicitly labelled architecture/design evidence; they are not screenshots. Acceptance correction added the following genuine macOS captures from the corrected SFML application. They supplement the earlier diagrams without pretending to have been captured from old commits.

| Capture | Actual state shown | Capture method |
|---|---|---|
| `runtime-start-menu.png` | Start menu rendered inside the SFML window | `screencapture -l <CGWindowID>` against the live `isaac_course_game` window. |
| `runtime-character-select.png` | Character select with current Isaac name and HP/SPD/DMG/LUCK | Live app launched with `--evidence-character-select`, which freezes only the UI input for reproducible capture. |
| `runtime-gameplay-hud.png` | Gameplay room, character texture, projectile, door, hearts, minimap and in-window counters/state | Live app after normal menu input. |
| `runtime-paused.png` | Pause overlay over the gameplay view | Live app after a posted Escape key event. |

Capture date: 2026-07-13. The window was identified through `CGWindowListCopyWindowInfo`; only that application window was captured, avoiding unrelated desktop content. The deterministic nonvisual full-run evidence is the `deterministic_course_demo` CTest.
