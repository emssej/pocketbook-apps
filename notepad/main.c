#include <math.h>

#include "src/ui.h"

/* Some global stuff. */

ifont *default_font;

/* The UI elements. */

UIWidget current_root;

/* 

   The meat.

*/

void
onpointerdown (UIWidget *self)
{
  InvertArea (self->x, self->y, self->width, self->height);
  PartialUpdate (self->x, self->y, self->width, self->height);
  InvertArea (self->x, self->y, self->width, self->height);
  PartialUpdate (self->x, self->y, self->width, self->height);
}

int
main_handler (int event_type, int arg1, int arg2)
{
  (void) arg1; (void) arg2;
  
  switch (event_type)
    {
    case EVT_INIT:
      {
	default_font = OpenFont (DEFAULTFONT, 48, TRUE);

	/* Put your GUI stuff here. */

	current_root = UIWidget_new ();

	UIWidget button1 = UIWidget_new ();
	button1.font = default_font;
	button1.text = "Click me and stuff!";
	button1.onpointerdown = onpointerdown;
	button1.ratio = 0.6;
	button1.background_color = LGRAY;

	UIWidget button2 = UIWidget_new ();
	button2.font = default_font;
	button2.text = "Click me!";
	button2.onpointerdown = onpointerdown;
	button2.ratio = 0.4;
	button1.background_color = BLACK;
	button1.foreground_color = WHITE;
	
	UIWidget_add_child (&current_root, &button1);
	UIWidget_add_child (&current_root, &button2);
	
	/* Don't touch anything else. */

	/* Since we don't want a panel, we have to tell InkView this
	   application is NOT a reader. */
	SetCurrentApplicationAttribute (APPLICATION_READER, 0);

	UIWidget_draw (&current_root, TRUE);
      }
      break;
    }

  UIWidget_handle_event (&current_root, event_type, arg1, arg2);
  
  return 0;
}

int
main ()
{
  InkViewMain (main_handler);
  
  return 0;
}
