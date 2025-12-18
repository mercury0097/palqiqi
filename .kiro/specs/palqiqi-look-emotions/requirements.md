# Palqiqi 方向看表情需求文档

## Introduction

为 Palqiqi 机器人的矢量眼睛系统添加四个方向看的表情预设（Look Left、Look Right、Look Up、Look Down），这些表情将作为独立的表情类型，可以通过 `SetEmotion()` 接口调用，并映射到 Normal 表情的变体。

## Glossary

- **VectorEyes**: 矢量眼睛系统，使用参数化方式绘制眼睛表情
- **EyeConfig**: 眼睛配置结构，包含位置偏移、大小、斜度、圆角等参数
- **Emotion**: 表情枚举类型
- **LookController**: 现有的视线控制器，控制眼睛看向的方向
- **眼距**: 两只眼睛之间的水平距离

## Requirements

### Requirement 1: Look Left 表情

**User Story:** As a user, I want the robot to display a "look left" expression, so that the robot appears to be looking to its left side.

#### Acceptance Criteria

1. WHEN the system receives a "look_left" emotion command THEN the VectorEyes system SHALL display both eyes shifted to the left with the left eye enlarged
2. WHEN displaying Look Left expression THEN the VectorEyes system SHALL reduce the inter-eye distance to create a focused look effect
3. WHEN transitioning to Look Left expression THEN the VectorEyes system SHALL animate smoothly from the current expression within 300ms

### Requirement 2: Look Right 表情

**User Story:** As a user, I want the robot to display a "look right" expression, so that the robot appears to be looking to its right side.

#### Acceptance Criteria

1. WHEN the system receives a "look_right" emotion command THEN the VectorEyes system SHALL display both eyes shifted to the right with the right eye enlarged
2. WHEN displaying Look Right expression THEN the VectorEyes system SHALL reduce the inter-eye distance to create a focused look effect
3. WHEN transitioning to Look Right expression THEN the VectorEyes system SHALL animate smoothly from the current expression within 300ms

### Requirement 3: Look Up 表情

**User Story:** As a user, I want the robot to display a "look up" expression, so that the robot appears to be looking upward.

#### Acceptance Criteria

1. WHEN the system receives a "look_up" emotion command THEN the VectorEyes system SHALL display both eyes shifted upward
2. WHEN displaying Look Up expression THEN the VectorEyes system SHALL maintain symmetric eye sizes
3. WHEN transitioning to Look Up expression THEN the VectorEyes system SHALL animate smoothly from the current expression within 300ms

### Requirement 4: Look Down 表情

**User Story:** As a user, I want the robot to display a "look down" expression, so that the robot appears to be looking downward.

#### Acceptance Criteria

1. WHEN the system receives a "look_down" emotion command THEN the VectorEyes system SHALL display both eyes shifted downward
2. WHEN displaying Look Down expression THEN the VectorEyes system SHALL maintain symmetric eye sizes
3. WHEN transitioning to Look Down expression THEN the VectorEyes system SHALL animate smoothly from the current expression within 300ms

### Requirement 5: 表情映射与接口兼容

**User Story:** As a developer, I want the new look expressions to be accessible through the existing SetEmotion interface, so that I can use them without modifying the calling code.

#### Acceptance Criteria

1. WHEN the system receives emotion name "look_left", "look_right", "look_up", or "look_down" THEN the VectorEyes system SHALL map these to the corresponding Look expression presets
2. WHEN a Look expression is active THEN the VectorEyes system SHALL disable random look behavior to maintain the fixed gaze direction
3. WHEN transitioning from a Look expression to Normal THEN the VectorEyes system SHALL re-enable random look behavior

### Requirement 6: 视觉效果一致性

**User Story:** As a user, I want the look expressions to match the existing visual style, so that they appear consistent with other expressions.

#### Acceptance Criteria

1. WHEN displaying any Look expression THEN the VectorEyes system SHALL use the same eye color as the Normal expression
2. WHEN displaying any Look expression THEN the VectorEyes system SHALL maintain the rounded rectangle eye shape consistent with Normal expression
3. WHEN displaying Look Left or Look Right THEN the VectorEyes system SHALL make the eye closer to the look direction appear 10-20% larger than the other eye
