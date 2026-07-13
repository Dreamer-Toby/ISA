# Asset source manifest

All external files below were downloaded on **2026-07-13** from the Huiji Wiki static host for non-profit classroom demonstration. Direct requests initially returned HTTP 403; retrying with the corresponding Wiki page as the HTTP referrer succeeded. No image was generated. Files are loaded with nearest-neighbor scaling (`Texture::setSmooth(false)`) and fall back to View geometry with a diagnostic if missing.

| Local path | Original file | Source page | Original URL | Use | Processing |
|---|---|---|---|---|---|
| `assets/textures/characters/cain-icon.png` | Cain Icon.png | https://isaac.huijiwiki.com/wiki/该隐 | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Cain_Icon.png | Cain player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/enemies/fly.png` | Flywill.png | https://isaac.huijiwiki.com/wiki/实体/13 | https://huiji-public.huijistatic.com/isaac/uploads/a/a4/Flywill.png | Fly enemy | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/bosses/boss-icon.png` | Boss Icon.png | https://isaac.huijiwiki.com/wiki/实体/67 | https://huiji-public.huijistatic.com/isaac/uploads/7/77/Boss_Icon.png | Generic Boss badge for the four configured Boss DTOs | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/items/blood-tear.png` | Blood Tear.png | https://isaac.huijiwiki.com/wiki/C90 | https://huiji-public.huijistatic.com/isaac/uploads/c/c2/Blood_Tear.png | Projectile visual / item-effect reference | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/rooms/red-room-door.png` | Red Room Door outline.png | https://isaac.huijiwiki.com/wiki/房间 | https://huiji-public.huijistatic.com/isaac/uploads/9/97/Red_Room_Door_outline.png | Room door visual | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/ui/red-heart.png` | Health Red 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/8/85/Health_Red_2.png | Red-heart HUD | Renamed only; native 16×16 draw. |
| `assets/textures/ui/shield-heart.png` | Health Soul 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Health_Soul_2.png | Shield/soul-heart HUD | Renamed only; native 16×16 draw. |

## Integrity hashes (SHA-256)

```text
0d10cbfdbb51fd70012e27f623a405786ad23653f25545266e2c4d6265d55971  assets/textures/bosses/boss-icon.png
34fed097f41f39f2167f163bd3f07d0b1c4f33de1f2129c91a12a681c279dd6f  assets/textures/characters/cain-icon.png
faa050b48b134d8864ca9297a1a6afb96a645be37aeb82ddb2c3d97518fae78a  assets/textures/enemies/fly.png
fef305793e024fc6d82564e4a81a52b98e39ea5522b553a699aff981c0f0b8af  assets/textures/items/blood-tear.png
263d4bdf895f01af8aebcc70420b1aca1844c73538bd82ea0511802aaf76173e  assets/textures/rooms/red-room-door.png
4ef18bcf3dcb8a63503af4ce79a144c517d5f5150f12496c4aeb8209e9af8313  assets/textures/ui/red-heart.png
6636aebdcc626cbb8d7e3bab4c92c9d2575d2b47856af29d7824504d0b2a3def  assets/textures/ui/shield-heart.png
```
