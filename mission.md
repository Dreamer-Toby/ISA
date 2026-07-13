# Mission

- Created: 2026-07-13T10:48:03+08:00
- Git hash at creation: unborn
- Edit rule: change only after an explicit user instruction.
- Final rework plans allowed: 1

## Final Goal

Complete a project that satisfies every requirement in the initial task and plan, produces the requested deliverables, and verifies the result.

## Initial Task

你是 Codex Agent。你的任务是在指定 GitHub 仓库中，使用 C++17/20 + SFML，以严格 MVVM 架构完成一个《以撒的结合：重生》简化版课程项目。

本项目仅用于非盈利课程教学与作业展示，不用于商业发布或公开分发。你需要自主完成资料调研、架构设计、编码、调试、测试、资源整理、版本记录和答辩材料编写。整个过程应保持连贯、自我迭代，不要把项目拆成互不衔接的临时代码片段。

目标仓库：
https://github.com/Dreamer-Toby/ISA

资料来源：
https://isaac.huijiwiki.com/wiki/%E9%A6%96%E9%A1%B5

==================================================
一、仓库与执行方式
==================================================

1. 所有代码、资源、文档和版本记录都必须写入上述目标仓库。
2. 开始工作前，先检查仓库当前状态、已有代码、分支、构建系统、依赖和未提交修改，不得盲目覆盖已有内容。
3. 如果仓库已有实现，应在理解现有结构后增量修改；如果仓库为空，则从可编译的最小工程开始搭建。
4. 默认在独立开发分支中工作，例如：
   feature/mvvm-isaac-course-project
5. 不得修改或删除与本项目无关的文件，不得覆盖用户已有的未提交修改。
6. 每个阶段结束后必须保证仓库处于可检查状态：尽量可编译、可运行、可测试；若暂时无法通过，必须明确记录失败原因和下一步修复方案。
7. 不要把全部开发压缩成一个最终提交，不要 squash 掉中间迭代。老师需要检查 Agent 的真实优化过程。

==================================================
二、项目目标
==================================================

使用 C++17/20 + SFML 实现一个俯视角、房间制 roguelike 射击游戏，玩法参考《以撒的结合：重生》，但控制在课程项目可完成、可演示、可答辩的范围内。

核心玩法：
- 玩家在房间中移动、射击、躲避敌人、拾取道具
- 地图由多个房间组成，玩家通过门切换房间
- 简化为 3 层地图
- 每层包含普通房、宝箱房、商店房、隐藏房、Boss 房
- 打完 Boss 后有概率出现恶魔房
- 最后一层 Boss 可以设计为简化版“妈腿”
- 支持 3 到 4 个可选角色
- 支持约 6 种普通怪物和约 4 个 Boss
- 支持金币、炸弹、钥匙、主动道具、被动道具、饰品
- 血量分为红心和灰心/护盾，红心与护盾总上限为 12 颗心
- 游戏应具备开始菜单、角色选择、暂停、失败、通关或结算界面

优先保证：
1. 架构正确
2. 核心玩法完整
3. 工程能够稳定编译运行
4. 版本过程真实可查
5. 再增加动画、音效和视觉细节

不得为了追求原作全部内容而制造大量未完成系统。课程项目应做到范围受控、功能闭环。

==================================================
三、资料调研与 Wiki 子页面要求
==================================================

1. 不得只查看 Wiki 首页。必须根据实现内容主动打开并阅读必要的子页面，包括但不限于：
   - 角色及其基础属性
   - 普通怪物及攻击方式
   - Boss 及阶段行为
   - 道具、饰品、主动道具、被动道具
   - 房间类型、门、隐藏房、商店、宝箱房、恶魔房
   - 红心、魂心或护盾、金币、炸弹、钥匙等机制
   - HUD、小地图和界面表现
2. 只调研本简化版真正需要实现的内容，不要无目的遍历整个网站。
3. 在 docs/research/wiki-research.md 中持续记录：
   - 页面标题
   - 页面 URL
   - 查阅日期
   - 提取出的机制、数值或视觉参考
   - 最终是否采用
   - 为课程项目做了哪些简化
4. 从网页提取的信息必须转化为本项目自己的配置、规则和说明，不能把网页正文大段复制进代码或文档。
5. 若不同页面信息冲突，记录冲突并选择一个明确、可实现的课程版规则，不要让行为保持含糊。

==================================================
四、美术资源要求
==================================================

本项目以后述要求为准：允许并优先直接从上述灰机 Wiki 及其必要子页面抓取课程作业所需的图片资源，不再以自制占位图作为默认最终方案。

1. Agent 应主动打开对应子页面，定位角色、怪物、Boss、道具、房间元素、HUD 等所需图片并下载。
2. 抓取的资源必须整理到清晰目录，例如：

/assets
  /textures
    /characters
    /enemies
    /bosses
    /items
    /rooms
    /ui
  /audio
  /fonts

3. 每个外部资源必须记录来源。在 docs/assets/asset-sources.md 中至少写明：
   - 本地文件路径
   - 原始文件名
   - 来源页面 URL
   - 图片原始 URL（能够获取时）
   - 在游戏中的用途
   - 是否经过裁剪、缩放、透明化、调色或重新切片
4. 不得把资源散落在源码目录，也不得使用无法追踪来源的随机文件。
5. 下载后应进行必要的尺寸统一、透明背景处理、像素缩放或 Sprite Sheet 切分，但所有处理步骤应记录在 Agent 日志中。
6. 如果特定资源在 Wiki 上确实找不到、无法下载或质量无法使用，才允许调用图像生成工具生成替代素材；生成资源也必须记录提示词、生成日期、用途和后处理方式。
7. 资源加载、纹理缓存、动画帧和 Sprite 只允许存在于 View 或资源层，不能污染 Model。
8. README 中必须明确说明：项目仅用于非盈利课程教学展示，外部素材来源见 docs/assets/asset-sources.md，不得将素材随意用于商业发布。

==================================================
五、视觉风格
==================================================

整体参考《以撒的结合：重生》的俯视角房间、低分辨率像素/卡通风格和暗色地下室氛围。

UI 至少显示：
- 红心
- 灰心或护盾
- 金币
- 炸弹
- 钥匙
- 当前主动道具
- 小地图
- 当前层数或房间状态

视觉表现必须与游戏逻辑解耦：
- 贴图、颜色、动画、Sprite、Texture 只允许出现在 View 或资源层
- Model 层不能依赖 sf::RenderWindow、sf::Sprite、sf::Texture 等渲染对象
- 不能为了画面方便把业务逻辑写进 View
- Model 不得通过贴图名称、动画帧编号等视觉信息判断游戏规则

==================================================
六、MVVM 架构要求
==================================================

本项目必须严格使用 MVVM。

唯一允许的主要依赖方向：

View -> ViewModel -> Model

禁止反向依赖：
- Model 不能依赖 ViewModel
- Model 不能依赖 View
- ViewModel 不能依赖 View
- View 不能直接依赖 Model

1. Model 层职责

Model 层负责所有游戏业务逻辑和核心状态，包括：
- Player、Enemy、Boss、Projectile、Room、Level、Item
- 玩家移动、射击、受伤、死亡
- 敌人 AI、Boss 状态机
- 子弹、碰撞、伤害结算
- 房间生成、地图生成、房间切换
- 道具效果、拾取物、掉落规则
- 商店、宝箱、钥匙、炸弹、隐藏房逻辑

Model 层要求：
- 不依赖 SFML 渲染对象
- 不 include View 或 ViewModel
- 尽量使用纯 C++ 类型保存游戏状态
- 可以使用自定义 Vec2、Rect、Direction 等通用类型
- 所有核心游戏规则必须放在 Model 层
- 即使不创建窗口，也应能够对主要规则进行单元测试

2. ViewModel 层职责

ViewModel 层是 View 和 Model 之间的桥梁，负责：
- 从 Model 读取状态
- 转换成 View 可以直接绘制的 RenderDTO / DisplayState
- 接收 View 传来的输入命令
- 调用 Model 接口完成业务操作
- 管理菜单、暂停、HUD、角色选择等界面状态

ViewModel 层要求：
- 可以调用 Model
- 不写核心业务规则
- 不直接绘制 SFML 图形
- 不持有 sf::RenderWindow
- 尽量向 View 暴露简单、只读、稳定的数据结构
- 输入命令通过 InputCommand、ActionState 等结构传入

错误示例：
- ViewModel 中直接计算敌人 AI
- ViewModel 中直接做碰撞检测
- ViewModel 中直接生成房间
- ViewModel 中直接决定道具掉落

正确示例：
- ViewModel 接收 move、shoot、useBomb 等命令
- ViewModel 调用 GameSession.update(dt, input)
- ViewModel 从 GameSession 读取状态并生成 RenderDTO

3. View 层职责

View 层负责 SFML 相关内容：
- 创建或使用窗口
- 采集键盘事件
- 绘制角色、怪物、房间、子弹和 UI
- 播放简单动画或音效
- 根据 ViewModel 提供的 RenderDTO 绘制画面

View 层要求：
- View 只能依赖 ViewModel
- View 不允许直接 include Player、Enemy、Room、Item 等 Model 类
- View 不允许直接修改游戏业务状态
- View 不允许直接调用 Model
- View 中可以使用 sf::RenderWindow、sf::Sprite、sf::Texture、sf::Shape

4. App / Common / Resource 层职责

App 层负责：
- 创建窗口
- 组装 Model、ViewModel、View
- 运行主循环
- 调用 handleEvents、update、render

Common 层只放：
- 常量
- 数学类型
- 通用工具
- 枚举类型

Resource 层负责：
- Texture、Font、SoundBuffer 等资源加载与缓存
- 资源路径映射
- Sprite Sheet 和动画元数据

App / Common / Resource 层要求：
- 不允许写具体游戏业务逻辑
- 不允许处理玩家移动规则
- 不允许处理敌人 AI
- 不允许处理碰撞检测
- 不允许处理掉落规则
- 不允许处理房间生成规则

==================================================
七、SFML 主循环要求
==================================================

SFML 主循环必须保持干净，只做调度：

while (window.isOpen()) {
    pollEvent();
    update(dt);
    render();
}

主循环中禁止直接写：
- 玩家移动
- 敌人生成
- 碰撞检测
- 道具结算
- 房间逻辑
- ViewModel 业务逻辑
- Boss 行为
- 掉落规则

应采用稳定的时间步策略，避免不同帧率下移动速度和攻击节奏明显不同。若使用 fixed timestep 或 accumulator，应在架构文档中解释。

==================================================
八、推荐项目结构
==================================================

可按照以下结构设计，并允许在不破坏 MVVM 的前提下合理调整：

/src
  /app
    Application.h/.cpp

  /view
    GameView.h/.cpp
    HudView.h/.cpp
    MenuView.h/.cpp
    CharacterSelectView.h/.cpp

  /viewmodel
    GameViewModel.h/.cpp
    HudViewModel.h/.cpp
    MenuViewModel.h/.cpp
    RenderDTO.h
    InputCommand.h
    DisplayState.h

  /model
    GameSession.h/.cpp
    Player.h/.cpp
    Character.h/.cpp
    CharacterStats.h
    Enemy.h/.cpp
    EnemyStats.h
    Boss.h/.cpp
    Projectile.h/.cpp
    Room.h/.cpp
    Level.h/.cpp
    Item.h/.cpp
    Inventory.h/.cpp
    HealthComponent.h/.cpp
    ShootComponent.h/.cpp
    CollisionSystem.h/.cpp
    ProjectileSystem.h/.cpp
    EnemySystem.h/.cpp
    ItemSystem.h/.cpp
    RoomGenerator.h/.cpp
    MoveStrategy.h/.cpp
    AttackStrategy.h/.cpp
    DeathDropRule.h/.cpp
    BossStateMachine.h/.cpp

  /resource
    ResourceManager.h/.cpp
    AnimationLibrary.h/.cpp
    AssetCatalog.h/.cpp

  /common
    MathTypes.h
    Constants.h
    Direction.h
    GameTypes.h

/assets
/tests
/docs
  /architecture
  /research
  /assets
  /agent
  /progress

==================================================
九、人物和怪物设计方式
==================================================

优先使用接口、抽象类、组合、策略和配置驱动设计，而不是把每个角色和怪物写死。

人物设计：
- CharacterStats 表示基础数值
- HealthComponent 管理红心、灰心、血量上限
- ShootComponent 管理射速、伤害、子弹速度
- Inventory 管理金币、炸弹、钥匙、主动道具、被动道具、饰品
- 特殊能力通过 Ability 或 ItemEffect 接入

示例角色：
- 以撒：均衡角色
- 抹大拉：血量高、移速低
- 该隐：速度较快、幸运较高
- 犹大：血量低、伤害高

怪物设计：
- EnemyStats 表示血量、速度、接触伤害等
- MoveStrategy 表示追踪、随机游走、保持距离等移动方式
- AttackStrategy 表示接触伤害、定时射击、冲刺等攻击方式
- DeathDropRule 表示死亡掉落
- Boss 可以继承或组合 Enemy，并额外使用 BossPhase / BossStateMachine

新增角色或怪物时应做到：
- 不需要修改 View
- 不需要修改 App 主循环
- 不需要修改 Common 层
- 尽量只新增 Model 类、配置或策略类
- View 通过通用 RenderDTO 自动绘制新增实体

==================================================
十、Codex Agent 自我迭代与版本留痕
==================================================

老师要求检查 Agent 每一步如何优化，因此以下内容是硬性要求，不是可选建议。

1. 所有实质性改版必须留下独立 Git 提交。
2. 禁止完成全部代码后再伪造或补写中间过程。
3. 不得把多个阶段压进一个含糊的“大提交”。
4. 每个版本至少包含：
   - 本轮目标
   - 修改前存在的问题
   - 采用的方案
   - 实际修改文件
   - 架构影响
   - 测试方法与结果
   - 尚未解决的问题
   - 下一轮计划
5. 建议的提交粒度：
   - chore: initialize SFML project and build system
   - docs: add MVVM architecture design
   - feat: add player movement vertical slice
   - feat: add shooting and projectile model
   - refactor: move collision rules from viewmodel to model
   - feat: add room graph and transitions
   - feat: add enemy strategies and drops
   - feat: add items shop and inventory
   - feat: add bosses floor progression and ending
   - test: add model tests and architecture checks
   - docs: finalize agent log and defense guide
6. 在 docs/agent/agent-iteration-log.md 中持续记录每一次迭代。每条记录至少包含：
   - 迭代编号
   - 日期和时间
   - 对应 commit hash
   - 目标
   - 发现的问题
   - 方案比较
   - 实际优化
   - 编译结果
   - 测试结果
   - 截图或演示证据路径
   - 下一步
7. 在 CHANGELOG.md 中维护面向老师和使用者的版本变化摘要。
8. 对重要架构决策使用 ADR，放入 docs/architecture/decisions/，例如：
   - 为什么采用 MVVM
   - 为什么 Model 不依赖 SFML
   - 为什么使用策略模式扩展敌人
   - 为什么通过 RenderDTO 隔离 View 与 Model
9. 每个主要阶段创建一个可识别的 tag，例如：
   - v0.1-architecture
   - v0.2-player-combat
   - v0.3-rooms
   - v0.4-enemies
   - v0.5-items
   - v0.6-bosses
   - v1.0-course-release
10. 不要为了制造“迭代感”故意提交低质量代码。每次改版必须有真实目的、真实差异和真实验证。
11. 如果一次实现已经正确，也应记录当时的设计依据和验证结果，而不是故意破坏再修复。
12. 发现架构违规时，必须单独进行重构提交，并在日志中说明违规位置、原因和迁移方式。
13. 最终提交历史必须能够清楚展示：最小可运行版本 -> 核心玩法 -> 房间地图 -> 敌人与道具 -> Boss 与完整流程 -> 测试和整理。

==================================================
十一、阶段实施顺序
==================================================

Agent 应连续推进项目，但每个阶段都必须产生可检查版本，不需要每完成一个小文件就等待人工确认。

阶段 0：仓库检查、需求冻结、Wiki 调研计划、构建环境检查
阶段 1：架构设计、文件结构、CMake、SFML 窗口、主循环、MVVM 骨架
阶段 2：玩家移动、射击、子弹、HUD，形成第一个可玩垂直切片
阶段 3：房间、门、地图生成、房间切换、小地图
阶段 4：普通敌人、AI 策略、碰撞、伤害、死亡、掉落
阶段 5：道具、金币、炸弹、钥匙、宝箱、商店、隐藏房
阶段 6：Boss、恶魔房、楼层推进、最终 Boss、失败与结算
阶段 7：视觉资源替换、动画、音效、性能与体验优化
阶段 8：架构自查、测试、Bug 修复、代码整理、README 和答辩材料

每个阶段完成时都必须：
1. 编译
2. 运行或执行测试
3. 更新 Agent 日志
4. 更新 CHANGELOG
5. 创建独立 commit
6. 必要时创建 tag
7. 保存至少一份进度截图到 docs/progress/

==================================================
十二、每次生成或修改代码前后的强制自查
==================================================

每次实质性修改前，先检查：
- 本次需求属于哪一层？
- 是否已经存在可复用接口？
- 是否会造成 View、ViewModel、Model 的反向依赖？
- 是否需要先补充测试？

每次修改后，必须回答并记录：
- 是否严格遵守 View -> ViewModel -> Model？
- View 是否直接 include 或调用了 Model？
- ViewModel 是否写入了本应属于 Model 的核心游戏规则？
- App/Common/Resource 是否写入了具体游戏规则？
- 主循环中是否塞入了具体业务细节？
- Model 是否错误依赖了 SFML 渲染对象？
- 新增人物或怪物是否可以通过配置、接口或策略扩展？
- 新资源是否登记来源？
- 当前版本是否能够编译？
- 测试是否通过？
- 本次 commit 是否准确描述了改动？

如果发现违反，先修正设计和代码，再进入下一阶段。不得只在文字中声称“符合 MVVM”而不实际检查 include、依赖和职责。

建议增加自动化架构检查，例如：
- 搜索 model 目录是否 include SFML Graphics 或 View/ViewModel 头文件
- 搜索 view 目录是否 include Model 具体类
- 检查 ViewModel 是否出现碰撞、掉落、AI 等核心规则实现
- 使用 CMake/CTest 执行 Model 层单元测试

==================================================
十三、测试和质量要求
==================================================

至少对以下 Model 逻辑提供可重复测试：
- 红心与护盾上限
- 伤害和死亡
- 子弹命中与销毁
- 房间清空后开门
- 钥匙、炸弹和金币消耗
- 道具属性叠加
- 商店购买
- 隐藏房进入条件
- Boss 死亡和楼层推进
- 地图生成的基本连通性

同时进行：
- Debug 构建
- Release 构建
- 无资源或资源缺失时的错误处理
- 多次进入/离开房间的稳定性测试
- 帧率变化下的移动与射击一致性检查
- 内存、悬空引用和资源重复加载检查

不得用“能启动窗口”代替完整测试。

==================================================
十四、最终交付物
==================================================

最终仓库至少包含：
1. 可编译运行的源码
2. CMakeLists.txt 和依赖说明
3. assets 目录
4. tests 目录
5. README.md
6. CHANGELOG.md
7. docs/architecture/mvvm-architecture.md
8. docs/architecture/decisions/ 下的 ADR
9. docs/research/wiki-research.md
10. docs/assets/asset-sources.md
11. docs/agent/agent-iteration-log.md
12. docs/progress/ 下的阶段截图
13. docs/defense/course-defense-guide.md
14. 清晰、连续、未被压扁的 Git 提交历史
15. 阶段 tag 与最终 v1.0-course-release tag

README 至少说明：
- 项目目标
- 已实现玩法
- 操作方式
- 构建与运行方法
- MVVM 结构概览
- 资源来源说明
- 当前限制

答辩材料至少说明：
- 为什么使用 MVVM
- 三层职责与依赖方向
- 一个输入如何经过 View -> ViewModel -> Model 再形成 RenderDTO
- 如何新增角色、怪物和道具
- Agent 的主要迭代过程
- 至少一个真实架构问题及其修复
- 测试策略
- 项目简化了原作哪些内容

==================================================
十五、开始执行时的输出要求
==================================================

开始时先输出并写入文档：
1. 仓库现状检查
2. 已有文件和构建方式
3. 总体架构说明
4. MVVM 各层职责说明
5. 依赖方向图
6. 文件结构
7. 核心类设计
8. 人物扩展接口设计
9. 怪物扩展接口设计
10. SFML 主循环伪代码
11. 输入、更新、渲染流程
12. Wiki 调研页面计划
13. 美术资源抓取计划
14. 分阶段版本和 commit 计划
15. 测试计划

完成上述设计并提交第一个可检查版本后，继续按照阶段顺序自主实现，不要在每个小步骤都停下来等待确认。

但必须遵循以下停止条件：
- 需要删除或覆盖用户已有重要代码
- 发现目标仓库存在无法判断归属的未提交修改
- 需要真实账号、密钥或隐私信息
- 构建环境或依赖完全不可用，且无法通过仓库内修改解决

除这些情况外，应自主推进、测试、记录和提交，直到形成可演示的课程项目。


## Constraints

TODO

## Proof Target

TODO

## Expected Deliverables

TODO
