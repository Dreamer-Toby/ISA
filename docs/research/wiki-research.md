# Directed Huiji Wiki research

Access date for every entry below: **2026-07-13**. The Wiki intermittently returned HTTP 403 to direct automated page requests, so research used indexed copies of the named pages where necessary. No Wiki prose is copied into the project. Values below are the course rules selected after reading the references.

| Page | URL | Extracted reference | Adopted course rule / simplification |
|---|---|---|---|
| 首页 | https://isaac.huijiwiki.com/wiki/首页 | Navigation taxonomy for characters, entities, rooms and items. | Used only to plan directed subpage research. |
| 该隐 | https://isaac.huijiwiki.com/wiki/该隐 | Faster character with a damage modifier; luck affects drops. | Cain uses high speed/luck and two starting hearts. Exact original formulas are simplified. |
| 抹大拉 | https://isaac.huijiwiki.com/wiki/抹大拉 | High health and low movement speed; healing active item association. | Magdalene starts with four hearts, moves slowly and carries Yum Heart. |
| 犹大 | https://isaac.huijiwiki.com/wiki/犹大 | Low-health, high-damage identity and coin association. | Judas starts with one heart, high damage and three coins. |
| 成就（以撒/人物索引） | https://isaac.huijiwiki.com/wiki/成就 | Character roster and unlock context. | Isaac is the balanced baseline; unlock rules are omitted. |
| 掉落物 | https://isaac.huijiwiki.com/wiki/掉落物 | Coins buy goods, bombs damage/open secret entrances, keys open locked doors/chests. | Each resource is an integer; use fails atomically when insufficient. |
| HUD | https://isaac.huijiwiki.com/wiki/HUD | Health, active item, coins, bombs, keys and map are grouped in the HUD. | Show red hearts, shields, all resources, active item, minimap, floor and room state. |
| 房间 | https://isaac.huijiwiki.com/wiki/房间 | Named special-room taxonomy and door relationships. | Every floor has normal, treasure, shop, secret and boss rooms. |
| 商店 | https://isaac.huijiwiki.com/wiki/商店 | Shop is normally locked and contains coin-priced goods. | Shop entry consumes one key; one passive item costs 5 coins. |
| 隐藏房 | https://isaac.huijiwiki.com/wiki/隐藏房 | Secret entrances are revealed using bombs and connect to non-boss rooms. | A bomb at an adjacent normal-room wall reveals the secret room permanently. |
| 恶魔房 | https://isaac.huijiwiki.com/wiki/恶魔房 | May appear after clearing a boss. | A deterministic injectable roll below 35% creates a devil room; detailed angel/deal rules are omitted. |
| 布局 | https://isaac.huijiwiki.com/wiki/布局 | Floor/room layout categories. | Use a small connected graph with deterministic seeds rather than original layout pools. |
| 美味的心 | https://isaac.huijiwiki.com/wiki/C45 | Active healing item associated with Magdalene; restores red-heart health. | `Yum Heart` heals one whole course heart. Original charge/synergy details are omitted. |
| 彼列之书 | https://isaac.huijiwiki.com/wiki/C34 | Active book grants temporary damage, with the Wiki listing +2 damage in the current room. | `Book of Belial` grants +2 damage when used; room-duration/charge and repeated-use DLC differences are simplified. |
| 小石头 | https://isaac.huijiwiki.com/wiki/C90 | Passive offensive item whose identity is a damage increase with tear-side tradeoffs in the original. | `Small Rock` stacks +1 damage; original speed/stat side effects are omitted. |
| 悲伤洋葱 | https://isaac.huijiwiki.com/wiki/C1 | Passive tears/rate increase. | `Sad Onion` multiplies shot interval by 0.85 with a safe lower bound; exact tears formula is simplified. |
| 幸运脚趾 | https://isaac.huijiwiki.com/wiki/T42 | Trinket described by the Wiki data table as luck up and affecting reward odds. | `Lucky Toe` occupies the trinket slot and adds +1 course luck; detailed machine/drop tables are omitted. |

## Enemy and boss research matrix

The implemented roster deliberately uses broad behavior references rather than original numeric replication.

| Content | Wiki page | Mechanic retained | Course simplification |
|---|---|---|---|
| Fly / Pooter | https://isaac.huijiwiki.com/wiki/苍蝇 ; https://isaac.huijiwiki.com/wiki/吐根蝇 | chase or keep-distance shooting | circles with one movement and one attack strategy |
| Spider / Hopper | https://isaac.huijiwiki.com/wiki/蜘蛛 ; https://isaac.huijiwiki.com/wiki/跳蛛 | erratic/hopping pursuit | deterministic wander or dash |
| Gaper / Clotty | https://isaac.huijiwiki.com/wiki/裂口怪 ; https://isaac.huijiwiki.com/wiki/血块 | pursuit and projectile pressure | chase or stationary radial shot |
| Monstro | https://isaac.huijiwiki.com/wiki/萌死戳 | leap/shot rhythm | two timed phases |
| Duke of Flies | https://isaac.huijiwiki.com/wiki/苍蝇公爵 | summons/protective orbit identity | radial projectiles and faster second phase |
| Larry Jr. | https://isaac.huijiwiki.com/wiki/小拉里 | segmented charge identity | axis-aligned dash state |
| Mom | https://isaac.huijiwiki.com/wiki/妈妈 | final stomping attack | simplified Mom's Leg warning/stomp cycle |

## Conflict decisions

- The Wiki's exact stats vary by edition/DLC. This project uses stable, documented course values and retains only the role identity.
- Original health displays include many heart types. The course version has red hearts plus one shield/soul-heart pool, capped together at 12 hearts.
- Original devil-room probability depends on many run conditions. The course version uses one injectable 35% roll after a boss.
- Original maps use large layout pools. The course version guarantees five required room types in a small connected graph on each of three floors.
- Item values differ across editions and use the original game's nonlinear stat formulas. The five dedicated rows above freeze transparent course values rather than mixing editions.
