#include "../bitchat_app.h"
#include "bitchat_scene.h"

typedef enum {
    BitChatMainMenuChat,
    BitChatMainMenuPeers,
    BitChatMainMenuSettings,
} BitChatMainMenuItem;

static void bitchat_scene_main_menu_callback(void* context, uint32_t index) {
    BitChatApp* app = context;
    
    switch(index) {
        case BitChatMainMenuChat:
            scene_manager_next_scene(app->scene_manager, BitChatSceneChat);
            break;
        case BitChatMainMenuPeers:
            scene_manager_next_scene(app->scene_manager, BitChatScenePeerList);
            break;
        case BitChatMainMenuSettings:
            scene_manager_next_scene(app->scene_manager, BitChatSceneSettings);
            break;
    }
}

void bitchat_scene_main_menu_on_enter(void* context) {
    BitChatApp* app = context;
    Submenu* submenu = app->submenu;
    
    submenu_add_item(submenu, "Chat", BitChatMainMenuChat, bitchat_scene_main_menu_callback, app);
    submenu_add_item(submenu, "Peers", BitChatMainMenuPeers, bitchat_scene_main_menu_callback, app);
    submenu_add_item(submenu, "Settings", BitChatMainMenuSettings, bitchat_scene_main_menu_callback, app);
    
    submenu_set_selected_item(submenu, 0);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, BitChatViewSubmenu);
}

bool bitchat_scene_main_menu_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void bitchat_scene_main_menu_on_exit(void* context) {
    BitChatApp* app = context;
    submenu_reset(app->submenu);
}