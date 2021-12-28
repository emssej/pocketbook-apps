#include "ui.h"

UIWidget *
UIWidget_new ()
{
  UIWidget *widget = malloc (sizeof (UIWidget));

  *widget = (UIWidget) {
    .ratio = 1,
    .background_color = WHITE,
    .foreground_color = BLACK,
    .width = ScreenWidth (),
    .height = ScreenHeight ()
  };
  
  return widget;
}

int
UIWidget_add_child (UIWidget *parent, UIWidget *child)
{
  child->index = parent->children_count;
  ++parent->children_count;
  parent->children = realloc (parent->children, sizeof (UIWidget *)
			      * parent->children_count);

  if (parent->children == NULL)
    {
      return 0;
    }

  /* Here, we calculate all the x's, y's, widths and heights. */

  if (parent->horizontal)
    {
      child->x = parent->x;
      child->y = parent->y;
      child->width = parent->width * child->ratio;
      child->height = parent->height;

      if (child->index != 0)
	{
	  child->x = parent->children[child->index - 1]->x
	    + parent->children[child->index - 1]->width;
	}
    }
  else
    {
      child->x = parent->x;
      child->y = parent->y;
      child->width = parent->width;
      child->height = parent->height * child->ratio;

      if (child->index != 0)
	{
	  child->y = parent->children[child->index - 1]->y
	    + parent->children[child->index - 1]->height;
	}
    }
  
  parent->children[parent->children_count - 1] = child;
  
  return 1;
}

void
UIWidget_remove_children (UIWidget *parent)
{
  parent->children_count = 0;
  parent->children = NULL;
}

void
UIWidget_draw (UIWidget *widget)
{
  FillArea (widget->x, widget->y, widget->width, widget->height,
	    widget->background_color);
  
  if (widget->font != NULL && widget->text != NULL)
    {      
      SetFont (widget->font, widget->foreground_color);

      /* I wish to know why we can't use TextRectHeight, which would
	 make our centering more accurate, but I have yet to find
	 answers, so we just get our height from the font struct. */
      DrawTextRect (widget->x, widget->y + widget->height / 2
		    - widget->font->height, widget->width, widget->height,
		    widget->text, ALIGN_CENTER);
    }

  if (widget->image != NULL)
    {
      TileBitmap (widget->x, widget->y, widget->width, widget->height,
		  widget->image);
    }

  for (int i = 0; i < widget->children_count; ++i)
    {
      UIWidget_draw (widget->children[i]);
    }

  PartialUpdate (widget->x, widget->y, widget->width, widget->height);
}

void
UIWidget_handle_event (UIWidget *widget, int event_type, int arg1, int arg2)
{
  bool in = INRECT (widget->x, widget->y, widget->width, widget->height, arg1,
		    arg2);

  if (in)
    {
      switch (event_type)
	{
	case EVT_POINTERDOWN:
	  if (widget->onpointerdown != NULL)
	    {
	      widget->onpointerdown (widget);
	    }
	  else
	    {
	      for (int i = 0; i < widget->children_count; ++i)
		{
		  UIWidget_handle_event (widget->children[i], event_type, arg1,
					 arg2);
		}
	    }
	  break;
	case EVT_POINTERMOVE:
	  if (widget->onpointermove != NULL)
	    {
	      widget->onpointermove (widget);
	    }
	  else
	    {
	      for (int i = 0; i < widget->children_count; ++i)
		{
		  UIWidget_handle_event (widget->children[i], event_type, arg1,
					 arg2);
		}
	    }

	  break;
	case EVT_POINTERUP:
	  if (widget->onpointerup != NULL)
	    {
	      widget->onpointerup (widget);
	    }
	  else
	    {
	      for (int i = 0; i < widget->children_count; ++i)
		{
		  UIWidget_handle_event (widget->children[i], event_type, arg1,
					 arg2);
		}
	    }

	  break;
	default:
	  break;
	}
    }
}
