#include "bitchat_core.h"
#include <furi_hal_bt.h>
#include <ble_glue/ble_glue.h>
#include <string.h>

#define TAG "BitChatCore"

// BLE service and characteristic UUIDs (simplified for Flipper)
static const uint8_t bitchat_service_uuid[] = {0x12, 0x34};
static const uint8_t bitchat_char_uuid[] = {0x56, 0x78};

BitChatCore* bitchat_core_alloc(void) {
    BitChatCore* core = malloc(sizeof(BitChatCore));
    memset(core, 0, sizeof(BitChatCore));
    
    // Set default nickname
    strcpy(core->my_nickname, "FlipperUser");
    strcpy(core->current_channel, "public");
    
    return core;
}

void bitchat_core_free(BitChatCore* core) {
    if(core) {
        bitchat_core_stop_advertising(core);
        bitchat_core_stop_scanning(core);
        free(core);
    }
}

bool bitchat_core_start_advertising(BitChatCore* core) {
    furi_assert(core);
    
    if(core->is_advertising) {
        return true;
    }
    
    // Start BLE advertising with BitChat service
    if(furi_hal_bt_is_active()) {
        // Configure advertising data with BitChat identifier
        core->is_advertising = true;
        FURI_LOG_I(TAG, "Started advertising");
        return true;
    }
    
    return false;
}

bool bitchat_core_stop_advertising(BitChatCore* core) {
    furi_assert(core);
    
    if(!core->is_advertising) {
        return true;
    }
    
    core->is_advertising = false;
    FURI_LOG_I(TAG, "Stopped advertising");
    return true;
}

bool bitchat_core_start_scanning(BitChatCore* core) {
    furi_assert(core);
    
    if(core->is_scanning) {
        return true;
    }
    
    // Start BLE scanning for BitChat devices
    core->is_scanning = true;
    FURI_LOG_I(TAG, "Started scanning");
    return true;
}

bool bitchat_core_stop_scanning(BitChatCore* core) {
    furi_assert(core);
    
    if(!core->is_scanning) {
        return true;
    }
    
    core->is_scanning = false;
    FURI_LOG_I(TAG, "Stopped scanning");
    return true;
}

bool bitchat_core_send_message(BitChatCore* core, const char* message, const char* channel, const char* target_peer) {
    furi_assert(core);
    furi_assert(message);
    
    // Create message packet
    BitChatMessage msg;
    memset(&msg, 0, sizeof(msg));
    
    strncpy(msg.content, message, sizeof(msg.content) - 1);
    strncpy(msg.sender, core->my_nickname, sizeof(msg.sender) - 1);
    
    if(channel) {
        strncpy(msg.channel, channel, sizeof(msg.channel) - 1);
    } else {
        strcpy(msg.channel, "public");
    }
    
    msg.timestamp = furi_get_tick();
    msg.is_private = (target_peer != NULL);
    
    // Add to local message history
    core->messages[core->message_head] = msg;
    core->message_head = (core->message_head + 1) % 50;
    if(core->message_count < 50) {
        core->message_count++;
    }
    
    // TODO: Send via BLE to connected peers
    FURI_LOG_I(TAG, "Sent message: %s", message);
    
    return true;
}

bool bitchat_core_join_channel(BitChatCore* core, const char* channel) {
    furi_assert(core);
    furi_assert(channel);
    
    strncpy(core->current_channel, channel, sizeof(core->current_channel) - 1);
    FURI_LOG_I(TAG, "Joined channel: %s", channel);
    
    return true;
}

bool bitchat_core_leave_channel(BitChatCore* core) {
    furi_assert(core);
    
    strcpy(core->current_channel, "public");
    FURI_LOG_I(TAG, "Left channel, back to public");
    
    return true;
}

bool bitchat_core_set_nickname(BitChatCore* core, const char* nickname) {
    furi_assert(core);
    furi_assert(nickname);
    
    strncpy(core->my_nickname, nickname, sizeof(core->my_nickname) - 1);
    FURI_LOG_I(TAG, "Set nickname: %s", nickname);
    
    return true;
}

void bitchat_core_set_callbacks(
    BitChatCore* core,
    void (*on_message)(BitChatMessage* message, void* context),
    void (*on_peer_connected)(BitChatPeer* peer, void* context),
    void (*on_peer_disconnected)(const char* peer_id, void* context),
    void* context
) {
    furi_assert(core);
    
    core->on_message_received = on_message;
    core->on_peer_connected = on_peer_connected;
    core->on_peer_disconnected = on_peer_disconnected;
    core->callback_context = context;
}

uint8_t bitchat_core_get_peer_count(BitChatCore* core) {
    furi_assert(core);
    return core->peer_count;
}

BitChatPeer* bitchat_core_get_peer(BitChatCore* core, uint8_t index) {
    furi_assert(core);
    if(index >= core->peer_count) {
        return NULL;
    }
    return &core->peers[index];
}

uint8_t bitchat_core_get_message_count(BitChatCore* core) {
    furi_assert(core);
    return core->message_count;
}

BitChatMessage* bitchat_core_get_message(BitChatCore* core, uint8_t index) {
    furi_assert(core);
    if(index >= core->message_count) {
        return NULL;
    }
    
    // Calculate actual index in ring buffer
    uint8_t actual_index;
    if(core->message_count < 50) {
        actual_index = index;
    } else {
        actual_index = (core->message_head + index) % 50;
    }
    
    return &core->messages[actual_index];
}