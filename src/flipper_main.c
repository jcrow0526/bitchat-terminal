#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/text_box.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <bt/bt_service/bt.h>
#include <ble_glue/ble_glue.h>

#include "bitchat_app.h"
#include "scenes/bitchat_scene.h"
#include "views/bitchat_view.h"

#define TAG "BitChat"

static bool bitchat_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    BitChatApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool bitchat_app_back_event_callback(void* context) {
    furi_assert(context);
    BitChatApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void bitchat_app_tick_event_callback(void* context) {
    furi_assert(context);
    BitChatApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

BitChatApp* bitchat_app_alloc() {
    BitChatApp* app = malloc(sizeof(BitChatApp));

    // Initialize GUI
    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&bitchat_scene_handlers, app);
    
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, bitchat_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, bitchat_app_back_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, bitchat_app_tick_event_callback, 100);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Initialize views
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, BitChatViewSubmenu, submenu_get_view(app->submenu));

    app->text_input = text_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, BitChatViewTextInput, text_input_get_view(app->text_input));

    app->text_box = text_box_alloc();
    view_dispatcher_add_view(app->view_dispatcher, BitChatViewTextBox, text_box_get_view(app->text_box));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, BitChatViewWidget, widget_get_view(app->widget));

    // Initialize BitChat core
    app->bitchat = bitchat_core_alloc();
    
    // Initialize storage
    app->storage = furi_record_open(RECORD_STORAGE);
    
    // Initialize notifications
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Initialize Bluetooth
    app->bt = furi_record_open(RECORD_BT);
    
    return app;
}

void bitchat_app_free(BitChatApp* app) {
    furi_assert(app);

    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, BitChatViewSubmenu);
    submenu_free(app->submenu);

    view_dispatcher_remove_view(app->view_dispatcher, BitChatViewTextInput);
    text_input_free(app->text_input);

    view_dispatcher_remove_view(app->view_dispatcher, BitChatViewTextBox);
    text_box_free(app->text_box);

    view_dispatcher_remove_view(app->view_dispatcher, BitChatViewWidget);
    widget_free(app->widget);

    // Free GUI
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    // Close records
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_BT);

    // Free BitChat core
    bitchat_core_free(app->bitchat);

    free(app);
}

int32_t bitchat_app(void* p) {
    UNUSED(p);
    
    FURI_LOG_I(TAG, "Starting BitChat");
    
    BitChatApp* app = bitchat_app_alloc();
    
    // Start with main menu scene
    scene_manager_next_scene(app->scene_manager, BitChatSceneStart);
    
    view_dispatcher_run(app->view_dispatcher);
    
    bitchat_app_free(app);
    
    return 0;
}