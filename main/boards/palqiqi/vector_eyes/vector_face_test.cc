/**
 * @file vector_face_test.cc
 * @brief VectorFace 单元测试 - 验证随机视线控制
 * 
 * 测试内容：
 * 1. Look 表情时随机视线被禁用
 * 2. 切换回 Normal 时随机视线恢复
 * 
 * Requirements: 5.2, 5.3
 */

#include "vector_face.h"
#include "emotions.h"
#include <esp_log.h>
#include <cassert>

static const char* TAG = "VectorFaceTest";

namespace vector_eyes {
namespace test {

/**
 * @brief 测试 Look 表情时随机视线被禁用
 * 
 * 验证 Requirements 5.2:
 * WHEN a Look expression is active THEN the VectorEyes system SHALL 
 * disable random look behavior to maintain the fixed gaze direction
 */
bool TestLookEmotionDisablesRandomLook() {
    ESP_LOGI(TAG, "测试: Look 表情时随机视线被禁用");
    
    // 创建 VectorFace 实例
    VectorFace face(240, 240, 60);
    
    // 初始状态：随机视线应该是启用的
    bool initial_state = face.GetLookController().IsRandomLookEnabled();
    if (!initial_state) {
        ESP_LOGE(TAG, "  ✗ 初始状态随机视线应该启用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ 初始状态随机视线已启用");
    
    // 测试 LookLeft
    face.SetExpression(Emotion::LookLeft);
    if (face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ LookLeft: 随机视线应被禁用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ LookLeft: 随机视线已禁用");
    
    // 测试 LookRight
    face.SetExpression(Emotion::LookRight);
    if (face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ LookRight: 随机视线应被禁用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ LookRight: 随机视线已禁用");
    
    // 测试 LookUp
    face.SetExpression(Emotion::LookUp);
    if (face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ LookUp: 随机视线应被禁用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ LookUp: 随机视线已禁用");
    
    // 测试 LookDown
    face.SetExpression(Emotion::LookDown);
    if (face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ LookDown: 随机视线应被禁用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ LookDown: 随机视线已禁用");
    
    ESP_LOGI(TAG, "✓ Look 表情禁用随机视线测试通过");
    return true;
}

/**
 * @brief 测试切换回 Normal 时随机视线恢复
 * 
 * 验证 Requirements 5.3:
 * WHEN transitioning from a Look expression to Normal THEN the VectorEyes 
 * system SHALL re-enable random look behavior
 */
bool TestNormalEmotionRestoresRandomLook() {
    ESP_LOGI(TAG, "测试: 切换回 Normal 时随机视线恢复");
    
    // 创建 VectorFace 实例
    VectorFace face(240, 240, 60);
    
    // 先设置为 LookLeft 表情
    face.SetExpression(Emotion::LookLeft);
    if (face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ LookLeft 后随机视线应被禁用");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ LookLeft 后随机视线已禁用");
    
    // 切换回 Normal
    face.SetExpression(Emotion::Normal);
    if (!face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ 切换到 Normal 后随机视线应恢复");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ 切换到 Normal 后随机视线已恢复");
    
    // 测试从 LookUp 切换到 Happy
    face.SetExpression(Emotion::LookUp);
    face.SetExpression(Emotion::Happy);
    if (!face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ 从 LookUp 切换到 Happy 后随机视线应恢复");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ 从 LookUp 切换到 Happy 后随机视线已恢复");
    
    // 测试从 LookRight 切换到 Sad
    face.SetExpression(Emotion::LookRight);
    face.SetExpression(Emotion::Sad);
    if (!face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ 从 LookRight 切换到 Sad 后随机视线应恢复");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ 从 LookRight 切换到 Sad 后随机视线已恢复");
    
    // 测试从 LookDown 切换到 Angry
    face.SetExpression(Emotion::LookDown);
    face.SetExpression(Emotion::Angry);
    if (!face.GetLookController().IsRandomLookEnabled()) {
        ESP_LOGE(TAG, "  ✗ 从 LookDown 切换到 Angry 后随机视线应恢复");
        return false;
    }
    ESP_LOGI(TAG, "  ✓ 从 LookDown 切换到 Angry 后随机视线已恢复");
    
    ESP_LOGI(TAG, "✓ Normal 表情恢复随机视线测试通过");
    return true;
}

/**
 * @brief 测试非 Look 表情保持随机视线启用
 * 
 * 验证所有非 Look 表情都保持随机视线启用
 */
bool TestNonLookEmotionsKeepRandomLookEnabled() {
    ESP_LOGI(TAG, "测试: 非 Look 表情保持随机视线启用");
    
    VectorFace face(240, 240, 60);
    
    // 测试各种非 Look 表情
    const Emotion non_look_emotions[] = {
        Emotion::Normal,
        Emotion::Angry,
        Emotion::Glee,
        Emotion::Happy,
        Emotion::Sad,
        Emotion::Worried,
        Emotion::Focused,
        Emotion::Annoyed,
        Emotion::Surprised,
        Emotion::Skeptic,
        Emotion::Frustrated,
        Emotion::Unimpressed,
        Emotion::Sleepy,
        Emotion::Suspicious,
        Emotion::Squint,
        Emotion::Furious,
        Emotion::Scared,
        Emotion::Awe
    };
    
    const char* emotion_names[] = {
        "Normal", "Angry", "Glee", "Happy", "Sad", "Worried",
        "Focused", "Annoyed", "Surprised", "Skeptic", "Frustrated",
        "Unimpressed", "Sleepy", "Suspicious", "Squint", "Furious",
        "Scared", "Awe"
    };
    
    for (size_t i = 0; i < sizeof(non_look_emotions) / sizeof(non_look_emotions[0]); i++) {
        face.SetExpression(non_look_emotions[i]);
        if (!face.GetLookController().IsRandomLookEnabled()) {
            ESP_LOGE(TAG, "  ✗ %s: 随机视线应保持启用", emotion_names[i]);
            return false;
        }
    }
    
    ESP_LOGI(TAG, "  ✓ 所有非 Look 表情都保持随机视线启用");
    ESP_LOGI(TAG, "✓ 非 Look 表情保持随机视线启用测试通过");
    return true;
}

/**
 * @brief 运行所有 VectorFace 随机视线控制测试
 */
void RunAllTests() {
    ESP_LOGI(TAG, "==========================================");
    ESP_LOGI(TAG, "  VectorFace 随机视线控制单元测试");
    ESP_LOGI(TAG, "==========================================");
    
    int passed = 0;
    int failed = 0;
    
    if (TestLookEmotionDisablesRandomLook()) {
        passed++;
    } else {
        failed++;
    }
    
    if (TestNormalEmotionRestoresRandomLook()) {
        passed++;
    } else {
        failed++;
    }
    
    if (TestNonLookEmotionsKeepRandomLookEnabled()) {
        passed++;
    } else {
        failed++;
    }
    
    ESP_LOGI(TAG, "==========================================");
    ESP_LOGI(TAG, "  测试结果: %d 通过, %d 失败", passed, failed);
    ESP_LOGI(TAG, "==========================================");
}

} // namespace test
} // namespace vector_eyes

/**
 * @brief 外部调用入口
 * 
 * 在 app_main() 或其他地方调用此函数运行测试
 */
extern "C" void vector_face_test_run(void) {
    vector_eyes::test::RunAllTests();
}
