#pragma once

#include <inkview.h>

#include "utilities.h"

typedef void (*EventCallback) (int event_type, int arg1, int arg2);

typedef struct
EventHandler
{
  size_t callbacks_count;
  EventCallback *callbacks;
} EventHandler;

void EventHandler_add_EventCallback (EventHandler *event_handler,
				     EventCallback callback);
void EventHandler_update (EventHandler *event_handler, int event_type,
			  int arg1, int arg2);
