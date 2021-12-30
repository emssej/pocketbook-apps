#include "base.h"

void
UIWidget_debug (UIWidget *widget)
{
  char debug[256];
  sprintf (debug, "x: %d y: %d w: %d h: %d index: %d has parent: %s children count: %d",
	   widget->x, widget->y, widget->w, widget->h,
	   widget->index, widget->parent != NULL ? "yes" : "no", widget->children_count);

  DEBUGMSG (debug);
}

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
  parent->children[child->index] = child;

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
      widget->ondraw (widget, (UIEvent) {});
    }
}

void
UIWidget_handle_event (UIWidget *widget, UIEvent event)
{
  bool in = INRECT (widget->x, widget->y, widget->w, widget->h,
		    event.arg1, event.arg2);

  switch (event.event_type)
    {
    case EVT_POINTERDOWN:
      {
	if (widget->onpointerdown != NULL && in)
	  {
	    widget->onpointerdown (widget, event);
	  }
      }
      break;
    case EVT_POINTERMOVE:
      if (widget->onpointermove != NULL && in)
	{
	  widget->onpointermove (widget, event);
	}
      break;
    case EVT_POINTERUP:
      if (widget->onpointerup != NULL && in)
	{
	  widget->onpointerup (widget, event);
	}
      break;
    case EVT_KEYDOWN:
      if (widget->onkeydown[event.arg1] != NULL)
	{
	  widget->onkeydown[event.arg1] (widget, event);
	}
    default:
      break;
    }

  for (size_t i = 0; i < widget->children_count; ++i)
    {
      UIWidget_handle_event (widget->children[i], event);
    }
}
