# Asset source manifest

The project downloaded the Wiki files in the first table on **2026-07-13** for non-profit classroom demonstration. Direct requests returned HTTP 403; a retry with the matching Wiki page as the HTTP referrer succeeded. Except for the user-directed door derivatives documented below, the project generated no replacement images. `ResourceManager` loads each file with nearest-neighbor scaling (`Texture::setSmooth(false)`), and the View draws fallback geometry when a file is missing.

| Local path | Original file | Source page | Original URL | Use | Processing |
|---|---|---|---|---|---|
| `assets/textures/characters/isaac-icon.png` | Isaac Icon.png | https://isaac.huijiwiki.com/wiki/以撒 | https://huiji-public.huijistatic.com/isaac/uploads/b/bd/Isaac_Icon.png | Isaac player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/magdalene-icon.png` | Magdalene Icon.png | https://isaac.huijiwiki.com/wiki/抹大拉 | https://huiji-public.huijistatic.com/isaac/uploads/d/dc/Magdalene_Icon.png | Magdalene player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/cain-icon.png` | Cain Icon.png | https://isaac.huijiwiki.com/wiki/该隐 | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Cain_Icon.png | Cain player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/characters/judas-icon.png` | Judas Icon.png | https://isaac.huijiwiki.com/wiki/犹大 | https://huiji-public.huijistatic.com/isaac/uploads/e/ef/Judas_Icon.png | Judas player marker | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/enemies/fly.png` | Flywill.png | https://isaac.huijiwiki.com/wiki/实体/13 | https://huiji-public.huijistatic.com/isaac/uploads/a/a4/Flywill.png | Fly enemy | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/bosses/boss-icon.png` | Boss Icon.png | https://isaac.huijiwiki.com/wiki/实体/67 | https://huiji-public.huijistatic.com/isaac/uploads/7/77/Boss_Icon.png | Generic Boss badge for the four configured Boss DTOs | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/items/blood-tear.png` | Blood Tear.png | https://isaac.huijiwiki.com/wiki/C90 | https://huiji-public.huijistatic.com/isaac/uploads/c/c2/Blood_Tear.png | Projectile visual / item-effect reference | Renamed only; runtime nearest-neighbor scale. |
| `assets/textures/rooms/red-room-door.png` | Red Room Door outline.png | https://isaac.huijiwiki.com/wiki/房间 | https://huiji-public.huijistatic.com/isaac/uploads/9/97/Red_Room_Door_outline.png | Legacy Red Room reference; no longer used for ordinary exits | Renamed only. |
| `assets/textures/ui/red-heart.png` | Health Red 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/8/85/Health_Red_2.png | Red-heart HUD | Renamed only; native 16×16 draw. |
| `assets/textures/ui/shield-heart.png` | Health Soul 2.png | https://isaac.huijiwiki.com/wiki/HUD | https://huiji-public.huijistatic.com/isaac/uploads/6/6a/Health_Soul_2.png | Shield/soul-heart HUD | Renamed only; native 16×16 draw. |

## User-directed treasure-room and ordinary wooden doors

On **2026-07-17** the user supplied `codex-clipboard-aa0204a7-0a57-47c6-9865-552146fa73ad.png`. The user later clarified that this gold style identifies exits whose destination is a treasure room, rather than ordinary exits. The four gold assets retain the selected source pixels: a deterministic mask removes pixels outside the door crop, then lossless pixel transforms produce the four directions. The ordinary wooden doorway was produced from the previous door draft through a user-directed image edit: keep the silhouette, replace the material with dark walnut and dull iron, and leave a near-black open passage. A flat chroma-key background was removed to alpha with the installed image helper. These derivatives have no external URL or license record and remain limited to this non-profit classroom demo.

| Local path | User-provided source | Use | Processing |
|---|---|---|---|
| `assets/textures/rooms/treasure-door-down.png` | `codex-clipboard-aa0204a7-0a57-47c6-9865-552146fa73ad.png` | Down exit leading to a treasure room | Deterministic crop and alpha mask; retained RGB pixels are copied from the user image without repainting. |
| `assets/textures/rooms/treasure-door-up.png` | `codex-clipboard-aa0204a7-0a57-47c6-9865-552146fa73ad.png` | Up exit leading to a treasure room | Exact vertical pixel mirror of `treasure-door-down.png`. |
| `assets/textures/rooms/treasure-door-left.png` | `codex-clipboard-aa0204a7-0a57-47c6-9865-552146fa73ad.png` | Left exit leading to a treasure room | Exact clockwise quarter-turn of `treasure-door-down.png`. |
| `assets/textures/rooms/treasure-door-right.png` | `codex-clipboard-aa0204a7-0a57-47c6-9865-552146fa73ad.png` | Right exit leading to a treasure room | Exact horizontal pixel mirror of `treasure-door-left.png`. |
| `assets/textures/rooms/wood-door.png` | Built-in image edit of the previous door draft | Non-treasure, non-Boss exits in the four-room layout | Precise material edit to dark walnut, dull iron and an open black tunnel; flat chroma-key background converted to alpha; runtime direction rotation and nearest-neighbor scaling. |

## User-provided local material library

The user added the source files below to `以撒的结合素材库/` on **2026-07-15**. The files came without an external URL or license record. This project keeps them inside the non-profit classroom demo; add the original source and license before redistributing them. The import copied each PNG byte for byte into the existing `assets/textures/` structure.

| Local path | User-provided source file | Use | Processing |
|---|---|---|---|
| `assets/textures/rooms/boss-door.png` | `以撒的结合素材库/门/Bossroom.png` | Boss-room exits | Renamed and copied byte for byte; runtime rotation and nearest-neighbor scale. |
| `assets/textures/rooms/treasure-door.png` | `以撒的结合素材库/门/Itemroomdoor.png` | Legacy item-room reference; no longer used at runtime | Renamed and copied byte for byte. |
| `assets/textures/rooms/locked-treasure-door.png` | `以撒的结合素材库/门/Lockeditemroomdoor.png` | Legacy locked-item-room reference; no longer used at runtime | Renamed and copied byte for byte. |
| `assets/textures/rooms/trapdoor.png` | `以撒的结合素材库/门/Secret_Trap_Door.png` | Post-Boss floor exit | Renamed and copied byte for byte; runtime nearest-neighbor scale. |
| `assets/textures/characters/cain-portrait.png` | `以撒的结合素材库/人物/该隐.png` | Cain selection and gameplay head | Renamed and copied byte for byte; runtime nearest-neighbor scale. |
| `assets/textures/characters/cain-portrait-closed.png` | `以撒的结合素材库/人物/该隐_闭眼.png` | Cain blink frame | Renamed and copied byte for byte; runtime nearest-neighbor scale. |
| `assets/textures/characters/judas-portrait.png` | `以撒的结合素材库/人物/犹大.png` | Judas selection and gameplay head | Renamed and copied byte for byte; runtime nearest-neighbor scale. |
| `assets/textures/characters/judas-portrait-closed.png` | `以撒的结合素材库/人物/犹大_闭眼.png` | Judas blink frame | Renamed and copied byte for byte; runtime nearest-neighbor scale. |
| `assets/textures/characters/magdalene-portrait.png` | `以撒的结合素材库/人物/犹大拉.png` | Magdalene selection and gameplay head | Mapped to the existing Magdalene roster entry, renamed and copied byte for byte. |
| `assets/textures/characters/magdalene-portrait-closed.png` | `以撒的结合素材库/人物/犹大拉_闭眼.png` | Magdalene blink frame | Mapped to the existing Magdalene roster entry, renamed and copied byte for byte. |

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
df2657766a81bcf57a734d1fe9ef15c0ebd0fdace89ca4ecea72e3ea3c9fdc1e  assets/textures/rooms/wood-door.png
eabcee887cfc7771a14ac5a5f2951628d254b7b907b87afa9d2d31b0f7534377  assets/textures/rooms/treasure-door-down.png
5ce2362879defe53f0df8a02ee3b497c3ed6213a61465b435f0f5f89a3e05223  assets/textures/rooms/treasure-door-up.png
3ddba8517253fcf7d2d49bcc9322fa257314c28b78d0f48ce15c98616d7f3e2d  assets/textures/rooms/treasure-door-left.png
147676973ccde0234a38b333059e64421dd362c18b6f320efe5e40ca9ef9c50f  assets/textures/rooms/treasure-door-right.png
e0f16223e6bc52cb6cef75f578bb7b239f4a198f3803012a5a45e2b4b8808d0b  assets/textures/rooms/boss-door.png
1b3bd858d0ca68350a545f4059af8086293680479f0f2e7b0a5b9dc4ee6d8c1e  assets/textures/rooms/treasure-door.png
b7503b7b367537809781618fedf890198f4ff5d893aee1d2519ae2fa129db8f8  assets/textures/rooms/locked-treasure-door.png
a6127a34b98e4794de991a9bd6bfae7aaa57fc9b90f556266fc304333d6a9c71  assets/textures/rooms/trapdoor.png
fc80d9a941e2369e52f2bc46217eec46f45e725dc495a7964e20ddcff59c5bc9  assets/textures/characters/cain-portrait.png
5f12efb8e786d3afb6802b7e4fbad16d240c5a0f9594c146e0658f881e9f9b07  assets/textures/characters/cain-portrait-closed.png
dc036c0def876455f3a82399a0fd3c11a41a796c380954f16d13406ca39a9a9f  assets/textures/characters/judas-portrait.png
8f5ea5e8c45396157b08c71d22c447d5f764e20819add6b8841b761483f5cabb  assets/textures/characters/judas-portrait-closed.png
a8813e423162ce1699c0ff782bd22404b922789d8a7ce8aa57a3fea9f57e4f57  assets/textures/characters/magdalene-portrait.png
d4e993b852633f6b5526b2e0e28a28ddae9156151d368f2ea442068f1e8927d7  assets/textures/characters/magdalene-portrait-closed.png
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
