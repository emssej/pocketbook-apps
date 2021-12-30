#include "notepad.h"

typedef struct
UINotepadData
{
  int last_x, last_y, line_thickness, color;
  bool pressure_sensitivity_emulation_on, eraser_on;
} UINotepadData;

void
notepad_start_drawing (UIWidget *widget, UIEvent event)
{
  UINotepadData *data = (UINotepadData *) widget->data;
  
  data->last_x = event.arg1;
  data->last_y = event.arg2;
}

void
notepad_drawing (UIWidget *widget, UIEvent event)
{
  UINotepadData *data = (UINotepadData *) widget->data;

  int line_thickness = data->line_thickness;
  int thickness = line_thickness;
  int color = data->color;

  if (data->eraser_on)
    {
      color = 3;
      line_thickness = 25;
    }
  
  if (data->pressure_sensitivity_emulation_on)
    {
      /* We calculate this so that we can emulate pen pressure
	 sensitivity. */
      int a = event.arg1 - data->last_x;
      int b = event.arg2 - data->last_y;
      float distance = fabs (sqrt (a * a + b * b));
      float speed = (float) distance / (line_thickness * 5);
      thickness = (float) line_thickness - speed;

      if (thickness < 1)
	{
	  thickness = 1;
	}
    }
  
  SmoothThickLine (data->last_x, data->last_y, event.arg1, event.arg2,
		   color * 0x111111 * 5, thickness);
      
  int start_x = (event.arg1 < data->last_x ? event.arg1 : data->last_x) - thickness;
  int start_y = (event.arg2 < data->last_y ? event.arg2 : data->last_y) - thickness;
  int end_x = (event.arg1 > data->last_x ? event.arg1 : data->last_x) + thickness;
  int end_y = (event.arg2 > data->last_y ? event.arg2 : data->last_y) + thickness;
      
  PartialUpdate (start_x, start_y, end_x - start_x, end_y - start_y);
      
  data->last_x = event.arg1;
  data->last_y = event.arg2;
}

UIWidget *
UINotepad_new ()
{
  UIWidget *widget = UIWidget_new ();

  UINotepadData *data = malloc (sizeof (UINotepadData));
  *data = (UINotepadData) {
    .last_x = 0, .last_y = 0, .line_thickness = 3, .color = 0,
    .pressure_sensitivity_emulation_on = true, .eraser_on = false
  };
  widget->data = data;

  widget->onpointerdown = notepad_start_drawing;
  widget->onpointermove = notepad_drawing;
  
  return widget;
}

bool *
UINotepad_get_pressure_bool (UIWidget *widget)
{
  UINotepadData *data = (UINotepadData *) widget->data;
  return &data->pressure_sensitivity_emulation_on;
}

int *
UINotepad_get_thickness_int (UIWidget *widget)
{
  UINotepadData *data = (UINotepadData *) widget->data;
  return &data->line_thickness;
}

int *
UINotepad_get_color_int (UIWidget *widget)
{
  UINotepadData *data = (UINotepadData *) widget->data;
  return &data->color;
}

bool *
UINotepad_get_eraser_bool (UIWidget *widget)
{
  UINotepadData *data = (UINotepadData *) widget->data;
  return &data->eraser_on;  
}
