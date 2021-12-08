#include "ui.h"

UIWidget
UIWidget_new ()
{
  UIWidget widget;
  
  widget.index = 0;
  widget.parent = NULL;
  widget.children_count = 0;
  widget.children = NULL;
  widget.onpointerdown = NULL;
  widget.onpointermove = NULL;
  widget.onpointerup = NULL;
  widget.horizontal = FALSE;
  widget.ratio = 1;
  widget.background_color = WHITE;
  widget.foreground_color = BLACK;
  widget.font = NULL;
  widget.text = NULL;
  widget.image = NULL;
  widget.x = 0;
  widget.y = 0;
  widget.width = ScreenWidth ();
  widget.height = ScreenHeight ();

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
UIWidget_draw (UIWidget *widget, _Bool with_children)
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
  
  if (with_children)
    {
      for (int i = 0; i < widget->children_count; ++i)
	{
	  UIWidget_draw (widget->children[i], with_children);
	}
    }

  PartialUpdate (widget->x, widget->y, widget->width, widget->height);
}

/* 

   This is where the error is. But why? The for loop?

 */

void
UIWidget_handle_event (UIWidget *widget, int event_type, int arg1, int arg2)
{
  _Bool inrect = INRECT (widget->x, widget->y, widget->width, widget->height,
			 arg1, arg2);

  _Bool fallthrough = TRUE;

  if (inrect)
    {
      switch (event_type)
	{
	case EVT_POINTERDOWN:
	  if (widget->onpointerdown != NULL)
	    {
	      /* widget->onpointerdown (widget); */
	      fallthrough = FALSE;
	    }
	  break;
	case EVT_POINTERMOVE:
 	  if (widget->onpointermove != NULL)
	    {
	      /* widget->onpointermove (widget); */
	      fallthrough = FALSE;
	    }
	  break;
	case EVT_POINTERUP:
	  if (widget->onpointerup != NULL)
	    {
	      /* widget->onpointerup (widget); */
	      fallthrough = FALSE;
	    }
	  break;
	default:
	  break;
	}
    }

  if (fallthrough)
    {
      for (int i = 0; i < widget->children_count; ++i)
	{
	  UIWidget_handle_event (widget->children[i], event_type, arg1, arg2);
	}
    }
}
