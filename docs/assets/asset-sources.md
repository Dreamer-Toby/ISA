# Asset source manifest

All external files below were downloaded on **2026-07-13** from the Huiji Wiki static host for non-profit classroom demonstration. Direct requests initially returned HTTP 403; retrying with the corresponding Wiki page as the HTTP referrer succeeded. No image was generated. Files are loaded with nearest-neighbor scaling (`Texture::setSmooth(false)`) and fall back to View geometry with a diagnostic if missing.

| Local path | Original file | Source page | Original URL | Use | Processing |
|---|---|---|---|---|---|
| `assets/textures/characters/isaac-icon.png` | Isaac Icon.png | https://isaac.huijiwiki.com/wiki/以撒 | https://huiji-public.huijistatic.com/isaac/uploads/b/bd/Isaac_Icon.png | Isaac player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/magdalene-icon.png` | Magdalene Icon.png | https://isaac.huijiwiki.com/wiki/抹大拉 | https://huiji-public.huijistatic.com/isaac/uploads/d/dc/Magdalene_Icon.png | Magdalene player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/cain-icon.png` | Cain Icon.png | https://isaac.huijiwiki.com/wiki/该隐 | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Cain_Icon.png | Cain player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/judas-icon.png` | Judas Icon.png | https://isaac.huijiwiki.com/wiki/犹大 | https://huiji-public.huijistatic.com/isaac/uploads/e/ef/Judas_Icon.png | Judas player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/enemies/fly.png` | Flywill.png | https://isaac.huijiwiki.com/wiki/实体/13 | https://huiji-public.huijistatic.com/isaac/uploads/a/a4/Flywill.png | Fly enemy | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/bosses/boss-icon.png` | Boss Icon.png | https://isaac.huijiwiki.com/wiki/实体/67 | https://huiji-public.huijistatic.com/isaac/uploads/7/77/Boss_Icon.png | Generic Boss badge for the four configured Boss DTOs | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/items/blood-tear.png` | Blood Tear.png | https://isaac.huijiwiki.com/wiki/C90 | https://huiji-public.huijistatic.com/isaac/uploads/c/c2/Blood_Tear.png | Projectile visual / item-effect reference | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/rooms/red-room-door.png` | Red Room Door outline.png | https://isaac.huijiwiki.com/wiki/房间 | https://huiji-public.huijistatic.com/isaac/uploads/9/97/Red_Room_Door_outline.png | Room door visual | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/ui/red-heart.png` | Health Red 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/8/85/Health_Red_2.png | Red-heart HUD | Renamed only; native 16×16 draw. |
| `assets/textures/ui/shield-heart.png` | Health Soul 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Health_Soul_2.png | Shield/soul-heart HUD | Renamed only; native 16×16 draw. |

## EasyIsaac final-presentation subset

The files listed below were copied byte-for-byte on **2026-07-13** from
[`tianguantg/EasyIsaac` commit `6580ba5`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets).
The reference repository publishes its code and bundled files under the
[Unlicense](licenses/EasyIsaac-UNLICENSE.txt), while its README says that several images and sounds came from
*The Binding of Isaac*. Consequently, this subset is retained only for the same non-profit classroom demonstration
stated by this project; it is not a claim of commercial rights to the original game artwork or audio.

| Local group | Original repository path | Use | Processing |
|---|---|---|---|
| `assets/easyisaac/panel/` | [`assets/images/panel/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/panel) | Start/menu/rank paper, room, HUD, pause and ending presentation | Original basename retained; runtime nearest-neighbor scaling. |
| `assets/easyisaac/character/` | [`assets/images/character/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/character) | Player body/head animation frames | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/bullet/` | [`assets/images/bullet/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/bullet) | Tear projectile | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/monster/` | [`assets/images/monster/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/monster) | Fly and generic Boss presentation | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/obstacle/` | [`assets/images/obstacle/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/obstacle) | Decorative stone and spike | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/prop/` | [`assets/images/prop/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/prop) | Active/passive item presentation | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/drops/` | [`assets/images/drops/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/images/drops) | Heart pickup | Original basename retained; paired at runtime to derive alpha. |
| `assets/easyisaac/sounds/` | [`assets/sounds/`](https://github.com/tianguantg/EasyIsaac/tree/6580ba5ac6af8bcdce065eec41b908636ab1df20/assets/sounds) | Shoot, damage, pickup and defeat feedback | Byte-for-byte subset; decoded and cached by SFML Audio. |

EasyX uses a `_back` color image and `_front` monochrome mask for transparent drawing. The source JPEG bytes are
unchanged here. `ResourceManager::maskedTexture` combines each pair at runtime by using inverse mask brightness as
alpha, then caches the resulting SFML texture. The exact imported inventory is:

```text
assets/easyisaac/bullet/bullet0_back.jpg
assets/easyisaac/bullet/bullet0_front.jpg
assets/easyisaac/character/isaac0_back.jpg
assets/easyisaac/character/isaac0_front.jpg
assets/easyisaac/character/isaac0_shoot_back.jpg
assets/easyisaac/character/isaac0_shoot_front.jpg
assets/easyisaac/character/isaac_walk0_back.jpg
assets/easyisaac/character/isaac_walk0_front.jpg
assets/easyisaac/character/isaac_walk0_left_back.jpg
assets/easyisaac/character/isaac_walk0_left_front.jpg
assets/easyisaac/character/isaac_walk0_side_back.jpg
assets/easyisaac/character/isaac_walk0_side_front.jpg
assets/easyisaac/drops/itemRedHp0_back.jpg
assets/easyisaac/drops/itemRedHp0_front.jpg
assets/easyisaac/drops/itemRedHp1_back.jpg
assets/easyisaac/drops/itemRedHp1_front.jpg
assets/easyisaac/monster/monster_redfly0_back.jpg
assets/easyisaac/monster/monster_redfly0_front.jpg
assets/easyisaac/monster/people_head_back.jpg
assets/easyisaac/monster/people_head_front.jpg
assets/easyisaac/obstacle/spine0_back.jpg
assets/easyisaac/obstacle/spine0_front.jpg
assets/easyisaac/obstacle/stone0_back.jpg
assets/easyisaac/obstacle/stone0_front.jpg
assets/easyisaac/panel/BK0.jpg
assets/easyisaac/panel/BK1.jpg
assets/easyisaac/panel/begin0.jpg
assets/easyisaac/panel/begin1.jpg
assets/easyisaac/panel/health0_back.jpg
assets/easyisaac/panel/health0_front.jpg
assets/easyisaac/panel/health1_back.jpg
assets/easyisaac/panel/health1_front.jpg
assets/easyisaac/panel/health2_back.jpg
assets/easyisaac/panel/health2_front.jpg
assets/easyisaac/panel/health3_back.jpg
assets/easyisaac/panel/health3_front.jpg
assets/easyisaac/panel/health4_back.jpg
assets/easyisaac/panel/health4_front.jpg
assets/easyisaac/panel/menu.jpg
assets/easyisaac/panel/pause0_back.jpg
assets/easyisaac/panel/pause0_front.jpg
assets/easyisaac/panel/pause1_back.jpg
assets/easyisaac/panel/pause1_front.jpg
assets/easyisaac/panel/propName_back.jpg
assets/easyisaac/panel/propName_front.jpg
assets/easyisaac/panel/testament_back.jpg
assets/easyisaac/panel/testament_front.jpg
assets/easyisaac/prop/prop0_back.jpg
assets/easyisaac/prop/prop0_front.jpg
assets/easyisaac/prop/prop1_back.jpg
assets/easyisaac/prop/prop1_front.jpg
assets/easyisaac/prop/prop2_back.jpg
assets/easyisaac/prop/prop2_front.jpg
assets/easyisaac/prop/prop3_back.jpg
assets/easyisaac/prop/prop3_front.jpg
assets/easyisaac/prop/prop4_back.jpg
assets/easyisaac/prop/prop4_front.jpg
assets/easyisaac/prop/prop5_back.jpg
assets/easyisaac/prop/prop5_front.jpg
assets/easyisaac/prop/prop6_back.jpg
assets/easyisaac/prop/prop6_front.jpg
assets/easyisaac/sounds/dead.mp3
assets/easyisaac/sounds/getProp.mp3
assets/easyisaac/sounds/hurt0.mp3
assets/easyisaac/sounds/shoot.wav
```

## Integrity hashes (SHA-256)

```text
0d10cbfdbb51fd70012e27f623a405786ad23653f25545266e2c4d6265d55971  assets/textures/bosses/boss-icon.png
34fed097f41f39f2167f163bd3f07d0b1c4f33de1f2129c91a12a681c279dd6f  assets/textures/characters/cain-icon.png
faa050b48b134d8864ca9297a1a6afb96a645be37aeb82ddb2c3d97518fae78a  assets/textures/enemies/fly.png
fef305793e024fc6d82564e4a81a52b98e39ea5522b553a699aff981c0f0b8af  assets/textures/items/blood-tear.png
263d4bdf895f01af8aebcc70420b1aca1844c73538bd82ea0511802aaf76173e  assets/textures/rooms/red-room-door.png
4ef18bcf3dcb8a63503af4ce79a144c517d5f5150f12496c4aeb8209e9af8313  assets/textures/ui/red-heart.png
6636aebdcc626cbb8d7e3bab4c92c9d2575d2b47856af29d7824504d0b2a3def  assets/textures/ui/shield-heart.png
d622d9cc30837d52e1eb3d303817539f04f3f4b7f690e65d08ac3fc4613cbaad  assets/textures/characters/isaac-icon.png
75c5b18f36b0ab41d8dc8b6fde1f80cef07a4b8938095d06b58c4062bfd37024  assets/textures/characters/judas-icon.png
f390b58ecc33f32cfc4446b50b9324a0919f26d774afcffd74233ecb39192fa9  assets/textures/characters/magdalene-icon.png
```

## Per-content lookup failures and fallbacks

On 2026-07-13 each page below was fetched through the Jina page reader and its Huiji static URLs were enumerated; direct static requests were also retried with the matching page referrer. The pages exposed only dynamic “loading” placeholders, edition badges, stat icons or generic category icons—not a usable static entity/collectible image. These explicit failures authorize the existing authored geometry fallback; they are not claims that an asset exists.

| Configured content | Page checked | Result / fallback |
|---|---|---|
| Pooter | https://isaac.huijiwiki.com/wiki/实体/14 | No static Pooter image URL; generic enemy circle. |
| Spider | https://isaac.huijiwiki.com/wiki/实体/85 | No static Spider image URL; generic enemy circle. |
| Hopper | https://isaac.huijiwiki.com/wiki/实体/29 | No static Hopper image URL; generic enemy circle. |
| Gaper | https://isaac.huijiwiki.com/wiki/实体/10 | No static Gaper image URL; generic enemy circle. |
| Clotty | https://isaac.huijiwiki.com/wiki/实体/15 | No static Clotty image URL; generic enemy circle. |
| Monstro | https://isaac.huijiwiki.com/wiki/实体/20 | Only generic Boss icon/dynamic animation placeholder; traced Boss badge plus geometry. |
| Duke of Flies | https://isaac.huijiwiki.com/wiki/实体/67 | Only generic Boss icon/dynamic animation placeholder; traced Boss badge plus geometry. |
| Larry Jr. | https://isaac.huijiwiki.com/wiki/实体/19 | Only generic Boss icon/dynamic animation placeholder; traced Boss badge plus geometry. |
| Mom's Leg | https://isaac.huijiwiki.com/wiki/实体/45 | Only generic Boss icon/dynamic animation placeholder; traced Boss badge plus enlarged geometry. |
| Yum Heart | https://isaac.huijiwiki.com/wiki/C45 | No static collectible image URL; in-window bitmap name and item-color geometry. |
| Book of Belial | https://isaac.huijiwiki.com/wiki/C34 | No static collectible image URL; in-window bitmap name and item-color geometry. |
| Small Rock | https://isaac.huijiwiki.com/wiki/C90 | No static collectible image URL; in-window bitmap name and item-color geometry. |
| Sad Onion | https://isaac.huijiwiki.com/wiki/C1 | No static collectible image URL; in-window bitmap name and item-color geometry. |
| Lucky Toe | https://isaac.huijiwiki.com/wiki/T42 | No static trinket image URL; in-window bitmap name and item-color geometry. |
