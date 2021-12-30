#include "ui.h"

UIWidget *
UIWidget_new ()
{
  UIWidget *widget = malloc (sizeof (UIWidget));

  *widget = (UIWidget) {
    .ratio = 1,
    .background_color = WHITE,
    .foreground_color = BLACK,
    .w = ScreenWidth (),
    .h = ScreenHeight ()
  };
  
  return widget;
}

void
UIWidget_terminate (UIWidget *widget)
{
  /* Fix this crap. */
  for (size_t i = 0; i < widget->children_count; ++i)
    {
      free (widget->children[i]);
    }

  free (widget->children);
  free (widget);
}

void
UIWidget_calculate_dimensions (UIWidget *widget)
{
  if (widget->parent != NULL)
    {
      widget->x = widget->parent->x;
      widget->y = widget->parent->y;
      widget->w = widget->parent->w * (widget->parent->horizontal
				       ? widget->ratio : 1);
      widget->h = widget->parent->h * (widget->parent->horizontal ? 1
				       : widget->ratio);
      
      if (widget->parent->horizontal && widget->index != 0)
	{
	  widget->x = widget->parent->children[widget->index - 1]->x
	    + widget->parent->children[widget->index - 1]->w;
	}

      if (!widget->parent->horizontal && widget->index != 0)
	{
	  widget->y = widget->parent->children[widget->index - 1]->y
	    + widget->parent->children[widget->index - 1]->h;
	}
    }

  /* Just in case, we do it for the children too. */
  for (size_t i = 0; i < widget->children_count; ++i)
    {
      UIWidget_calculate_dimensions (widget->children[i]);
    }
}  

int
UIWidget_add_child (UIWidget *parent, UIWidget *child)
{
  child->index = parent->children_count++;

  if ((parent->children = realloc (parent->children, parent->children_count
				   * sizeof (UIWidget *))) == NULL)
    {
      return 0;
    }

  child->parent = parent;
  parent->children[parent->children_count - 1] = child;

  UIWidget_calculate_dimensions (child);

  return 1;
}

void
UIWidget_draw_internal (UIWidget *widget, bool update)
{
  FillArea (widget->x, widget->y, widget->w, widget->h,
	    widget->background_color);

  if (widget->image != NULL)
    {
      TileBitmap (widget->x, widget->y, widget->w, widget->h, widget->image);
    }
  
  if (widget->font != NULL && widget->text != NULL)
    {      
      SetFont (widget->font, widget->foreground_color);

      DrawTextRectCentered (widget->x, widget->y, widget->w, widget->h,
			    widget->text, ALIGN_CENTER);
    }

  for (size_t i = 0; i < widget->children_count; ++i)
    {
      UIWidget_draw_internal (widget->children[i], false);
    }

  if (update)
    {
      PartialUpdate (widget->x, widget->y, widget->w, widget->h);
    }

  if (widget->ondraw != NULL)
    {
      widget->ondraw ((UIEvent) { .widget = widget });
    }
}

void
UIWidget_handle_event (UIEvent event)
{
  bool in = INRECT (event.widget->x, event.widget->y, event.widget->w, event.widget->h,
		    event.arg1, event.arg2);

  switch (event.event_type)
    {
    case EVT_POINTERDOWN:
      {
	if (event.widget->onpointerdown != NULL && in)
	  {
	    event.widget->onpointerdown (event);
	  }
      }
      break;
    case EVT_POINTERMOVE:
      if (event.widget->onpointermove != NULL && in)
	{
	  event.widget->onpointermove (event);
	}
      break;
    case EVT_POINTERUP:
      if (event.widget->onpointerup != NULL && in)
	{
	  event.widget->onpointerup (event);
	}
      break;
    case EVT_KEYDOWN:
      for (int i = 0; i < 0x39; ++i)
	{
	  if (event.widget->onkeydown[i] != NULL)
	    {
	      event.widget->onkeydown[i] (event);
	    }
	}
    default:
      break;
    }

  for (size_t i = 0; i < event.widget->children_count; ++i)
    {
      event.widget = event.widget->children[i];
      UIWidget_handle_event (event);
    }
}
