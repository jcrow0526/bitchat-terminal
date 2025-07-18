#include "bitchat_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const bitchat_on_enter_handlers[])(void*) = {
#include "bitchat_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const bitchat_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "bitchat_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const bitchat_on_exit_handlers[])(void* context) = {
#include "bitchat_scene_config.h"
};
#undef ADD_SCENE

// Initialize scene handlers configuration
const SceneManagerHandlers bitchat_scene_handlers = {
    .on_enter_handlers = bitchat_on_enter_handlers,
    .on_event_handlers = bitchat_on_event_handlers,
    .on_exit_handlers = bitchat_on_exit_handlers,
    .scene_num = BitChatSceneNum,
};