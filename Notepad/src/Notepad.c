#include <math.h>

#include "ui/base.h"
#include "ui/notepad.h"
#include "ui/spinner.h"
#include "ui/switch.h"

/* Some global stuff. */

ifont *default_font;

/* The UI elements. */

UIWidget *current_root, *notepad_root, *settings_root;

/* 

   The meat.

*/

void
settings (UIWidget *widget, UIEvent event)
{
  (void) event; (void) widget;

  notepad_root->image = BitmapFromScreen
    (notepad_root->x, notepad_root->y, notepad_root->w, notepad_root->h);
  
  current_root = settings_root;
  UIWidget_draw (current_root);
}

void
notepad (UIWidget *widget, UIEvent event)
{
  (void) event; (void) widget;
  
  current_root = notepad_root;
  UIWidget_draw (current_root);
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
	
	notepad_root = UINotepad_new ();
	notepad_root->onkeydown[IV_KEY_MENU] = settings;

	settings_root = UIWidget_new ();
	settings_root->onkeydown[IV_KEY_MENU] = notepad;

	UIWidget *cl = UISpinner_new
	  (UINotepad_get_color_int (notepad_root), 0, 3, default_font,
	   "Color: ");
	cl->ratio = 0.1;
	
	UIWidget *lt = UISpinner_new
	  (UINotepad_get_thickness_int (notepad_root), 1, 10, default_font,
	   "Line thickness: ");
	lt->ratio = 0.1;

	UIWidget *em = UISwitch_new
	  (UINotepad_get_eraser_bool (notepad_root), default_font,
	   "Eraser: ");
	em->ratio = 0.1;

	UIWidget *ps = UISwitch_new
	  (UINotepad_get_pressure_bool (notepad_root), default_font,
	   "Pressure sensitivity emulation: ");
	ps->ratio = 0.1;

	UIWidget_add_child (settings_root, cl);
	UIWidget_add_child (settings_root, lt);
	UIWidget_add_child (settings_root, em);
	UIWidget_add_child (settings_root, ps);

	current_root = notepad_root;
	
	/* Don't touch anything else. */

	/* Since we don't want a panel, we have to tell InkView this
	   application is NOT a reader. */
	SetCurrentApplicationAttribute (APPLICATION_READER, 0);

	UIWidget_draw (current_root);
      }
      break;
    }

  UIWidget_handle_event (current_root, (UIEvent) {
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
