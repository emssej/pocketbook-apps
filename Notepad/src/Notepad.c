#include <math.h>

#include "spinner.h"
#include "ui.h"

/* Some global stuff. */

ifont *default_font;

int last_x, last_y;
  
int line_thickness = 3;
int color = BLACK;

_Bool pressure_sensitivity_emulation_on = 1;

/* The UI elements. */

UIWidget *current_root, *notepad_root;
UIWidget *thickness_info;

/* 

   The meat.

*/

void
notepad_start_drawing (UIEvent event)
{
  last_x = event.arg1;
  last_y = event.arg2;
}

void
notepad_drawing (UIEvent event)
{
  int thickness = line_thickness;

  if (pressure_sensitivity_emulation_on)
    {
      /* We calculate this so that we can emulate pen pressure
	 sensitivity. */
      int a = event.arg1 - last_x;
      int b = event.arg2 - last_y;
      float distance = fabs (sqrt (a * a + b * b));
      float speed = (float) distance / (line_thickness * 5);
      thickness = (float) line_thickness - speed;

      if (thickness < 1)
	{
	  thickness = 1;
	}
    }

  SmoothThickLine (last_x, last_y, event.arg1, event.arg2, color, thickness);
      
  int start_x = (event.arg1 < last_x ? event.arg1 : last_x) - thickness;
  int start_y = (event.arg2 < last_y ? event.arg2 : last_y) - thickness;
  int end_x = (event.arg1 > last_x ? event.arg1 : last_x) + thickness;
  int end_y = (event.arg2 > last_y ? event.arg2 : last_y) + thickness;
      
  PartialUpdate (start_x, start_y, end_x - start_x, end_y - start_y);
      
  last_x = event.arg1;
  last_y = event.arg2;
}

int
main_handler (int event_type, int arg1, int arg2)
{
  switch (event_type)
    {
    case EVT_INIT:
      {
	default_font = OpenFont (DEFAULTFONT, 48, true);

	/* Put your GUI stuff here. */
	
	notepad_root = UIWidget_new ();
	notepad_root->onpointerdown = notepad_start_drawing;
	notepad_root->onpointermove = notepad_drawing;

	current_root = UIWidget_new ();

	UIWidget_add_child
	  (current_root, UISpinner_new (&line_thickness, 1, 10, default_font,
					"Line thickness: "));
	
	/* Don't touch anything else. */

	/* Since we don't want a panel, we have to tell InkView this
	   application is NOT a reader. */
	SetCurrentApplicationAttribute (APPLICATION_READER, 0);

	UIWidget_draw (current_root);
      }
      break;
    }

  UIWidget_handle_event ((UIEvent) {
      .widget = current_root,
      .event_type = event_type,
      .arg1 = arg1,
      .arg2 = arg2
    });
  
  return 0;
}

int
main ()
{
  InkViewMain (main_handler);
  
  return 0;
}
