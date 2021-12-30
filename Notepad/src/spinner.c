#include "spinner.h"

typedef struct
UISpinnerData
{
  char *text;
  int *number, min, max;
} UISpinnerData;

void
on_subtract (UIEvent event)
{
  UIWidget_flash (event.widget);
  
  UISpinnerData *data = (UISpinnerData*) event.widget->parent->data;
  
  *data->number = MAX (data->min, *data->number - 1);

  /* Now, I gotta admit, this isn't clean, but it'll do. */
  UIWidget *value = event.widget->parent->children[1];
  value->text = malloc (strlen (data->text) + floor (log10 (abs (*data->number))) + 1);
  sprintf (value->text, "%s%d", data->text, *data->number);

  UIWidget_draw (value);
}

void
on_add (UIEvent event)
{
  UIWidget_flash (event.widget);
  
  UISpinnerData *data = (UISpinnerData*) event.widget->parent->data;
  
  *data->number = MIN (data->max, *data->number + 1);

  /* Now, I gotta admit, this isn't clean, but it'll do. */
  UIWidget *value = event.widget->parent->children[1];
  value->text = malloc (strlen (data->text) + floor (log10 (abs (*data->number))) + 1);
  sprintf (value->text, "%s%d", data->text, *data->number);

  UIWidget_draw (value);
}

void
dummy (UIEvent event)
{
  UIWidget_flash (event.widget);
}

UIWidget *
UISpinner_new (int *number, int min, int max, ifont *font, char *text)
{
  UIWidget *container = UIWidget_new ();
  container->horizontal = true;

  UISpinnerData *data = malloc (sizeof (UISpinnerData));
  *data = (UISpinnerData) {
    .text = text,
    .number = number,
    .min = min,
    .max = max    
  };
  container->data = data;

  UIWidget *subtract = UIWidget_new ();
  subtract->font = font;
  subtract->text = "<";
  subtract->ratio = 0.1;
  subtract->onpointerdown = on_subtract;
  
  UIWidget_add_child (container, subtract);
  
  UIWidget *value = UIWidget_new ();
  value->font = font;
  value->ratio = 0.8;
  value->onpointerdown = dummy;

  value->text = malloc (strlen (text) + floor (log10 (abs (*number))) + 1);
  sprintf (value->text, "%s%d", text, *number);

  UIWidget_add_child (container, value);
  
  UIWidget *add = UIWidget_new ();
  add->font = font;
  add->text = ">";
  add->ratio = 0.1;
  add->onpointerdown = on_add;

  UIWidget_add_child (container, add);
  
  return container;
}

