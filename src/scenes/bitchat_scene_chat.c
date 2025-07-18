#include "../bitchat_app.h"
#include "bitchat_scene.h"
#include <gui/modules/text_box.h>

static void bitchat_scene_chat_update_display(BitChatApp* app) {
    FuriString* chat_text = furi_string_alloc();
    
    // Add header
    furi_string_printf(chat_text, "BitChat - %s\n", app->bitchat->current_channel);
    furi_string_cat_printf(chat_text, "Nickname: %s\n", app->bitchat->my_nickname);
    furi_string_cat_printf(chat_text, "Peers: %d\n\n", bitchat_core_get_peer_count(app->bitchat));
    
    // Add recent messages
    uint8_t msg_count = bitchat_core_get_message_count(app->bitchat);
    uint8_t start_idx = msg_count > 10 ? msg_count - 10 : 0;
    
    for(uint8_t i = start_idx; i < msg_count; i++) {
        BitChatMessage* msg = bitchat_core_get_message(app->bitchat, i);
        if(msg) {
            furi_string_cat_printf(chat_text, "<%s> %s\n", msg->sender, msg->content);
        }
    }
    
    text_box_set_text(app->text_box, furi_string_get_cstr(chat_text));
    furi_string_free(chat_text);
}

void bitchat_scene_chat_on_enter(void* context) {
    BitChatApp* app = context;
    
    bitchat_scene_chat_update_display(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, BitChatViewTextBox);
}

bool bitchat_scene_chat_on_event(void* context, SceneManagerEvent event) {
    BitChatApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle custom events like new messages
        bitchat_scene_chat_update_display(app);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        // Refresh display periodically
        bitchat_scene_chat_update_display(app);
        consumed = true;
    }
    
    return consumed;
}

void bitchat_scene_chat_on_exit(void* context) {
    BitChatApp* app = context;
    text_box_reset(app->text_box);
}