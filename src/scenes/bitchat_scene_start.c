#include "../bitchat_app.h"
#include "bitchat_scene.h"

void bitchat_scene_start_on_enter(void* context) {
    BitChatApp* app = context;
    
    // Initialize BitChat core
    bitchat_core_set_callbacks(
        app->bitchat,
        NULL, // message callback
        NULL, // peer connected callback  
        NULL, // peer disconnected callback
        app
    );
    
    // Start advertising and scanning
    bitchat_core_start_advertising(app->bitchat);
    bitchat_core_start_scanning(app->bitchat);
    
    // Move to main menu
    scene_manager_next_scene(app->scene_manager, BitChatSceneMainMenu);
}

bool bitchat_scene_start_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void bitchat_scene_start_on_exit(void* context) {
    UNUSED(context);
}