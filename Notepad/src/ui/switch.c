#include "switch.h"

typedef struct
UISwitchData
{
  bool *boolean;
  char *text;
} UISwitchData;

void
on_press (UIWidget *widget, UIEvent event)
{
  (void) event;
  
  UIWidget_flash (widget);
  
  UISwitchData *data = (UISwitchData *) widget->data;

  *data->boolean = !*data->boolean;
  sprintf (widget->text, "%s%s", data->text, *data->boolean ? "yes" : "no");

  UIWidget_draw (widget);
}

UIWidget *
UISwitch_new (bool *boolean, ifont *font, char *text)
{
  UIWidget *widget = UIWidget_new ();
  widget->font = font;
  
  UISwitchData *data = malloc (sizeof (UISwitchData));
  *data = (UISwitchData) {
    .boolean = boolean,
    .text = text
  };
  widget->data = data;

  widget->text = malloc (strlen (text) + 4);
  sprintf (widget->text, "%s%s", text, *boolean ? "yes" : "no");

  widget->onpointerdown = on_press;

  return widget;
}
