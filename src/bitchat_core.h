#pragma once

#include <furi.h>
#include <bt/bt_service/bt.h>
#include <ble_glue/ble_glue.h>

#define BITCHAT_SERVICE_UUID 0x1234
#define BITCHAT_CHAR_UUID 0x5678
#define BITCHAT_MAX_MESSAGE_SIZE 244
#define BITCHAT_MAX_PEERS 10

typedef struct {
    char peer_id[16];
    char nickname[32];
    uint32_t last_seen;
    bool is_connected;
} BitChatPeer;

typedef struct {
    char content[BITCHAT_MAX_MESSAGE_SIZE];
    char sender[32];
    char channel[32];
    uint32_t timestamp;
    bool is_private;
} BitChatMessage;

typedef struct {
    // Bluetooth state
    bool is_advertising;
    bool is_scanning;
    
    // Peers
    BitChatPeer peers[BITCHAT_MAX_PEERS];
    uint8_t peer_count;
    
    // Messages
    BitChatMessage messages[50]; // Ring buffer
    uint8_t message_head;
    uint8_t message_count;
    
    // Current state
    char my_nickname[32];
    char current_channel[32];
    
    // Callbacks
    void (*on_message_received)(BitChatMessage* message, void* context);
    void (*on_peer_connected)(BitChatPeer* peer, void* context);
    void (*on_peer_disconnected)(const char* peer_id, void* context);
    void* callback_context;
    
} BitChatCore;

BitChatCore* bitchat_core_alloc(void);
void bitchat_core_free(BitChatCore* core);

bool bitchat_core_start_advertising(BitChatCore* core);
bool bitchat_core_stop_advertising(BitChatCore* core);
bool bitchat_core_start_scanning(BitChatCore* core);
bool bitchat_core_stop_scanning(BitChatCore* core);

bool bitchat_core_send_message(BitChatCore* core, const char* message, const char* channel, const char* target_peer);
bool bitchat_core_join_channel(BitChatCore* core, const char* channel);
bool bitchat_core_leave_channel(BitChatCore* core);
bool bitchat_core_set_nickname(BitChatCore* core, const char* nickname);

void bitchat_core_set_callbacks(
    BitChatCore* core,
    void (*on_message)(BitChatMessage* message, void* context),
    void (*on_peer_connected)(BitChatPeer* peer, void* context),
    void (*on_peer_disconnected)(const char* peer_id, void* context),
    void* context
);

uint8_t bitchat_core_get_peer_count(BitChatCore* core);
BitChatPeer* bitchat_core_get_peer(BitChatCore* core, uint8_t index);
uint8_t bitchat_core_get_message_count(BitChatCore* core);
BitChatMessage* bitchat_core_get_message(BitChatCore* core, uint8_t index);