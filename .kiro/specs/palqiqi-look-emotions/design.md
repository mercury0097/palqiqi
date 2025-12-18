# Palqiqi 方向看表情设计文档

## Overview

本设计为 Palqiqi 机器人的矢量眼睛系统添加四个方向看的表情预设（LookLeft、LookRight、LookUp、LookDown）。这些表情将作为独立的 Emotion 枚举值，通过现有的 `SetEmotion()` 接口调用。

根据参考图片分析：
- **Look Left/Right**: 两眼同时水平移动，靠近看向方向的眼睛变大，眼距变小
- **Look Up/Down**: 两眼同时垂直移动，保持对称大小

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                 PalqiqiVectorEyeDisplay                     │
│                                                             │
│  SetEmotion("look_left") ──┐                               │
│  SetEmotion("look_right")──┼──► MapEmotionName() ──► face_->SetExpression()
│  SetEmotion("look_up") ────┤                               │
│  SetEmotion("look_down") ──┘                               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      VectorFace                             │
│                                                             │
│  SetExpression(Emotion::LookLeft)                          │
│       │                                                     │
│       ├──► GetPresetLeft(LookLeft) ──► left_eye_.TransitionTo()
│       └──► GetPresetRight(LookLeft) ──► right_eye_.TransitionTo()
│                                                             │
│  + 禁用 LookController 随机视线（Look表情时）              │
└─────────────────────────────────────────────────────────────┘
```

## Components and Interfaces

### 1. emotions.h 修改

添加四个新的表情枚举值：

```cpp
enum class Emotion {
    // ... 现有表情 ...
    Awe,
    
    // 新增方向看表情
    LookLeft,
    LookRight,
    LookUp,
    LookDown,
    
    COUNT  // 表情总数
};
```

### 2. eye_presets.h 修改

添加四个方向看的预设配置：

```cpp
// ============ Look Left: 左右眼不对称 ============
// 左眼 (变大，向左偏移)
constexpr EyeConfig Preset_LookLeft_Left = {
    .offset_x = -25,    // 向左偏移
    .offset_y = 0,
    .height = 82,       // 比Normal大约10%
    .width = 66,
    // ... 其他参数与Normal相似
};

// 右眼 (正常大小，向左偏移)
constexpr EyeConfig Preset_LookLeft_Right = {
    .offset_x = -20,    // 向左偏移（比左眼少一点，形成眼距变小效果）
    .offset_y = 0,
    .height = 75,       // Normal大小
    .width = 60,
    // ...
};

// Look Right, Look Up, Look Down 类似结构
```

### 3. VectorFace 修改

在 `SetExpression()` 中处理 Look 表情时禁用随机视线：

```cpp
void VectorFace::SetExpression(Emotion emotion) {
    // 检查是否是 Look 表情
    bool is_look_emotion = (emotion == Emotion::LookLeft || 
                            emotion == Emotion::LookRight ||
                            emotion == Emotion::LookUp || 
                            emotion == Emotion::LookDown);
    
    // Look 表情时禁用随机视线
    look_controller_.SetRandomLookEnabled(!is_look_emotion);
    
    // 应用表情预设...
}
```

### 4. PalqiqiVectorEyeDisplay 修改

在表情名称映射表中添加新映射：

```cpp
const EmotionNameMap emotion_name_maps_[] = {
    // ... 现有映射 ...
    
    // 方向看表情
    {"look_left", vector_eyes::Emotion::LookLeft},
    {"look_right", vector_eyes::Emotion::LookRight},
    {"look_up", vector_eyes::Emotion::LookUp},
    {"look_down", vector_eyes::Emotion::LookDown},
    
    {nullptr, vector_eyes::Emotion::Normal}
};
```

## Data Models

### EyeConfig 预设参数设计

基于参考图片和现有 Normal 预设，设计四个方向看的参数：

| 参数 | Normal | LookLeft_L | LookLeft_R | LookRight_L | LookRight_R | LookUp | LookDown |
|------|--------|------------|------------|-------------|-------------|--------|----------|
| offset_x | 0 | -25 | -20 | 20 | 25 | 0 | 0 |
| offset_y | 0 | 0 | 0 | 0 | 0 | -20 | 20 |
| height | 75 | 82 | 75 | 75 | 82 | 75 | 75 |
| width | 60 | 66 | 60 | 60 | 66 | 60 | 60 |
| radius_top | 12 | 12 | 12 | 12 | 12 | 12 | 12 |
| radius_bottom | 12 | 12 | 12 | 12 | 12 | 12 | 12 |

说明：
- Look Left: 左眼放大10%，两眼都向左偏移，左眼偏移更多形成眼距变小效果
- Look Right: 右眼放大10%，两眼都向右偏移，右眼偏移更多
- Look Up/Down: 两眼对称，只有垂直偏移

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: Horizontal look expressions shift eyes and enlarge closer eye
*For any* horizontal look expression (LookLeft or LookRight), both eyes should be shifted in the look direction, and the eye closer to the look direction should be 10-20% larger than the other eye.
**Validates: Requirements 1.1, 2.1, 6.3**

### Property 2: Vertical look expressions maintain symmetric eye sizes
*For any* vertical look expression (LookUp or LookDown), both eyes should have identical width and height values.
**Validates: Requirements 3.2, 4.2**

### Property 3: Vertical look expressions shift eyes in correct direction
*For any* vertical look expression, both eyes should have offset_y values that match the look direction (negative for up, positive for down).
**Validates: Requirements 3.1, 4.1**

### Property 4: Emotion name mapping returns correct Look emotion
*For any* look direction string ("look_left", "look_right", "look_up", "look_down"), the MapEmotionName function should return the corresponding Emotion enum value.
**Validates: Requirements 5.1**

### Property 5: Look expressions maintain rounded rectangle shape
*For any* Look expression preset, the radius_top and radius_bottom values should be within ±20% of the Normal preset values to maintain visual consistency.
**Validates: Requirements 6.2**

## Error Handling

1. **未知表情名称**: 如果传入未知的表情名称，`MapEmotionName()` 返回 `Emotion::Normal`
2. **空指针检查**: `SetEmotion()` 在 emotion 或 face_ 为空时直接返回
3. **过渡动画**: 如果在过渡中设置新表情，会中断当前过渡并开始新的过渡

## Testing Strategy

### Property-Based Testing

使用 ESP-IDF 的 Unity 测试框架进行属性测试。由于嵌入式环境限制，使用简化的属性测试方法：

1. **Property 1 测试**: 验证 LookLeft/LookRight 预设的眼睛大小比例
2. **Property 2 测试**: 验证 LookUp/LookDown 预设的对称性
3. **Property 3 测试**: 验证垂直方向偏移的正确性
4. **Property 4 测试**: 验证表情名称映射的正确性
5. **Property 5 测试**: 验证圆角值的一致性

### Unit Tests

1. **预设值验证**: 验证每个 Look 预设的具体参数值
2. **表情切换测试**: 验证从 Normal 切换到 Look 表情的行为
3. **随机视线控制测试**: 验证 Look 表情时随机视线被禁用

### Integration Tests

1. **端到端测试**: 通过 `SetEmotion("look_left")` 调用，验证最终眼睛配置正确
2. **过渡动画测试**: 验证表情切换时的平滑过渡
