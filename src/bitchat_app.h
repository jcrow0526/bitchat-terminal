#pragma once

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/text_box.h>
#include <gui/modules/widget.h>
#include <notification/notification.h>
#include <storage/storage.h>
#include <bt/bt_service/bt.h>

#include "bitchat_core.h"

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    
    // Views
    Submenu* submenu;
    TextInput* text_input;
    TextBox* text_box;
    Widget* widget;
    
    // Services
    Storage* storage;
    NotificationApp* notifications;
    Bt* bt;
    
    // BitChat core
    BitChatCore* bitchat;
    
    // App state
    char nickname[32];
    char current_channel[32];
    char text_buffer[256];
    bool is_connected;
    
} BitChatApp;

typedef enum {
    BitChatViewSubmenu,
    BitChatViewTextInput,
    BitChatViewTextBox,
    BitChatViewWidget,
} BitChatView;

typedef enum {
    BitChatEventStartScan,
    BitChatEventStopScan,
    BitChatEventSendMessage,
    BitChatEventJoinChannel,
    BitChatEventLeaveChannel,
    BitChatEventSetNickname,
} BitChatEvent;