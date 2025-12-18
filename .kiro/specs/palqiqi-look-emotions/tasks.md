# Palqiqi 方向看表情实现任务

## Implementation Plan

- [ ] 1. 添加表情枚举和预设
  - [x] 1.1 在 emotions.h 中添加 LookLeft, LookRight, LookUp, LookDown 枚举值
    - 在 Awe 之后、COUNT 之前添加四个新枚举
    - _Requirements: 5.1_
  - [x] 1.2 在 eye_presets.h 中添加 Look Left 预设（左右眼不对称）
    - 创建 Preset_LookLeft_Left: offset_x=-25, height=82, width=66
    - 创建 Preset_LookLeft_Right: offset_x=-20, height=75, width=60
    - _Requirements: 1.1, 1.2, 6.2, 6.3_
  - [x] 1.3 在 eye_presets.h 中添加 Look Right 预设（左右眼不对称）
    - 创建 Preset_LookRight_Left: offset_x=20, height=75, width=60
    - 创建 Preset_LookRight_Right: offset_x=25, height=82, width=66
    - _Requirements: 2.1, 2.2, 6.2, 6.3_
  - [x] 1.4 在 eye_presets.h 中添加 Look Up 预设（对称）
    - 创建 Preset_LookUp: offset_y=-20, 其他参数与 Normal 相同
    - _Requirements: 3.1, 3.2, 6.2_
  - [x] 1.5 在 eye_presets.h 中添加 Look Down 预设（对称）
    - 创建 Preset_LookDown: offset_y=20, 其他参数与 Normal 相同
    - _Requirements: 4.1, 4.2, 6.2_
  - [x] 1.6 更新 GetPreset(), GetPresetLeft(), GetPresetRight() 函数
    - 在 switch 语句中添加四个新表情的 case
    - LookLeft 和 LookRight 需要返回不同的左右眼预设
    - _Requirements: 1.1, 2.1, 3.1, 4.1_
  - [x] 1.7 更新 IsAsymmetricEmotion() 函数
    - 添加 LookLeft 和 LookRight 为不对称表情
    - _Requirements: 1.1, 2.1_
  - [ ] 1.8 编写属性测试验证预设参数
    - **Property 1: Horizontal look expressions shift eyes and enlarge closer eye**
    - **Property 2: Vertical look expressions maintain symmetric eye sizes**
    - **Property 3: Vertical look expressions shift eyes in correct direction**
    - **Property 5: Look expressions maintain rounded rectangle shape**
    - **Validates: Requirements 1.1, 2.1, 3.1, 3.2, 4.1, 4.2, 6.2, 6.3**

- [ ] 2. 更新 VectorFace 支持 Look 表情
  - [x] 2.1 修改 VectorFace::SetExpression() 处理 Look 表情的随机视线控制
    - 检测是否为 Look 表情
    - Look 表情时调用 look_controller_.SetRandomLookEnabled(false)
    - 非 Look 表情时恢复 look_controller_.SetRandomLookEnabled(true)
    - _Requirements: 5.2, 5.3_
  - [x] 2.2 编写单元测试验证随机视线控制
    - 测试 Look 表情时随机视线被禁用
    - 测试切换回 Normal 时随机视线恢复
    - _Requirements: 5.2, 5.3_

- [ ] 3. 更新表情名称映射
  - [x] 3.1 在 palqiqi_vector_eye_display.cc 的 emotion_name_maps_ 中添加映射
    - 添加 {"look_left", vector_eyes::Emotion::LookLeft}
    - 添加 {"look_right", vector_eyes::Emotion::LookRight}
    - 添加 {"look_up", vector_eyes::Emotion::LookUp}
    - 添加 {"look_down", vector_eyes::Emotion::LookDown}
    - _Requirements: 5.1_
  - [ ] 3.2 编写属性测试验证表情名称映射
    - **Property 4: Emotion name mapping returns correct Look emotion**
    - **Validates: Requirements 5.1**

- [x] 4. Checkpoint - 确保所有测试通过
  - Ensure all tests pass, ask the user if questions arise.

- [x] 5. 集成测试和验证
  - [x] 5.1 编译验证
    - 确保代码编译通过，无警告
    - _Requirements: 所有_
  - [x] 5.2 功能验证
    - 通过 SetEmotion("look_left") 等调用验证表情显示正确
    - 验证表情过渡动画平滑
    - _Requirements: 1.3, 2.3, 3.3, 4.3_

- [x] 6. Final Checkpoint - 确保所有测试通过
  - Ensure all tests pass, ask the user if questions arise.
