#include "event.h"

void
EventHandler_add_EventCallback (EventHandler *event_handler,
				EventCallback callback)
{
  ++event_handler->callbacks_count;
  event_handler->callbacks = realloc
    (event_handler->callbacks, event_handler->callbacks_count
     * sizeof (EventCallback));
    
  if (event_handler->callbacks == NULL)
    {
      FATAL ("realloc() is out of memory.");
    }
  else
    {
      event_handler->callbacks[event_handler->callbacks_count - 1] = callback;
    }
}

void
EventHandler_update (EventHandler *event_handler, int event_type, int arg1,
		     int arg2)
{
  for (int i = 0; i < event_handler->callbacks_count; ++i)
    {
      event_handler->callbacks[i] (event_type, arg1, arg2);
    }
}
