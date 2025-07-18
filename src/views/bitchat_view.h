#pragma once

#include <gui/view.h>

typedef struct BitChatView BitChatView;

typedef void (*BitChatViewCallback)(void* context);

BitChatView* bitchat_view_alloc(void);
void bitchat_view_free(BitChatView* bitchat_view);
View* bitchat_view_get_view(BitChatView* bitchat_view);
void bitchat_view_set_callback(BitChatView* bitchat_view, BitChatViewCallback callback, void* context);