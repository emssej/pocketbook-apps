#include <math.h>

#include "src/event.h"
#include "src/ui.h"

Widget ui_root;

EventHandler event_handler;
  
ifont *default_font;

int last_x, last_y;
  
int line_thickness = 3;
int color = BLACK;

_Bool pressure_sensitivity_emulation_on = 1;

/* 

   The meat.

*/

int
main_handler (int event_type, int arg1, int arg2)
{
  EventHandler_update (&event_handler, event_type, arg1, arg2);
  
  return 0;
}

/* 

   Callbacks.

*/


void
init_callback (int event_type, int arg1, int arg2)
{
  if (event_type == EVT_INIT)
    {
      default_font = OpenFont (DEFAULTFONT, 48, TRUE);

      ui_root = Widget_new_box (FALSE);
      Widget_set_margin_and_padding (&ui_root, 64, 0);
      Widget_set_background_color (&ui_root, LGRAY);
      Widget_set_border (&ui_root, BLACK, 6);

      /* Widget subwidget = Widget_new_box (FALSE); */
      /* Widget_set_margin_and_padding (&subwidget, 16, 0); */
      /* Widget_set_background_color (&subwidget, LGRAY); */
      /* Widget_set_border (&ui_root, DGRAY, 3); */

      /* Widget_add_child (&ui_root, subwidget); */
      
      /* Since we don't want a panel, we have to tell InkView this
	 application is NOT a reader. */
      SetCurrentApplicationAttribute (APPLICATION_READER, 0);

      ClearScreen ();

      Widget_draw (&ui_root);

      FullUpdate ();
    }
}

void
exit_callback (int event_type, int arg1, int arg2)
{
  if (event_type == EVT_EXIT)
    {
    }
}

void
pointerdown_callback (int event_type, int x, int y)
{
  if (event_type == EVT_POINTERDOWN)
    {
    }
}

void
pointermove_callback (int event_type, int x, int y)
{
  if (event_type == EVT_POINTERMOVE)
    {
      /* int thickness = line_thickness; */

      /* if (pressure_sensitivity_emulation_on) */
      /* 	{ */
      /* 	  /\* We calculate this so that we can emulate pen pressure */
      /* 	     sensitivity. *\/ */
      /* 	  int a = x - last_x; */
      /* 	  int b = y - last_y; */
      /* 	  float distance = fabs (sqrt (a * a + b * b)); */
      /* 	  float speed = (float) distance / (line_thickness * 5); */
      /* 	  thickness = (float) line_thickness - speed; */

      /* 	  if (thickness < 1) */
      /* 	    { */
      /* 	      thickness = 1; */
      /* 	    } */
      /* 	} */

      /* SmoothThickLine (last_x, last_y, x, y, color, thickness); */
      
      /* int start_x = (x < last_x ? x : last_x) - thickness; */
      /* int start_y = (y < last_y ? y : last_y) - thickness; */
      /* int end_x = (x > last_x ? x : last_x) + thickness; */
      /* int end_y = (y > last_y ? y : last_y) + thickness; */
      
      /* PartialUpdate (start_x, start_y, end_x - start_x, end_y - start_y); */
      
      /* last_x = x; */
      /* last_y = y; */
    }
}

int
main ()
{  
  EventHandler_add_EventCallback (&event_handler, init_callback);
  EventHandler_add_EventCallback (&event_handler, exit_callback);
  EventHandler_add_EventCallback (&event_handler, pointermove_callback);
  EventHandler_add_EventCallback (&event_handler, pointerdown_callback);
  
  InkViewMain (main_handler);
  
  return 0;
}
