#include "ui.h"

Widget
Widget_dummy ()
{
  Widget widget;

  widget.index = 0;
  widget.x = 0;
  widget.y = 0;
  widget.width = ScreenWidth ();
  widget.height = ScreenHeight ();
  widget.background_color = WHITE;
  widget.border_color = WHITE;
  widget.border_thickness = 0;
  widget.margin = 0;
  widget.padding = 0;
  widget.enabled = TRUE;

  return widget;  
}

Widget
Widget_new_box (_Bool horizontal)
{
  Widget widget = Widget_dummy ();

  widget.type = horizontal ? WIDGETTYPE_HBOX : WIDGETTYPE_VBOX;

  return widget;
}

Widget
Widget_new_label (char *text, int color, TextAlign align, ifont *font)
{
  Widget widget = Widget_dummy ();

  widget.label.text = text;
  widget.label.color = color;
  widget.label.align = align;
  widget.label.font = font;

  return widget;  
}

/* This function is FUCKING BROKEN for now. */
void
Widget_add_child (Widget *parent, Widget child)
{
  child.index = parent->children_count;
  child.parent = parent;
  int all_the_space = parent->margin + parent->padding;
  child.x = parent->x + all_the_space;
  child.y = parent->x + all_the_space;
  child.width = parent->width - all_the_space * 2;
  child.height = parent->height - all_the_space * 2;
  
  ++parent->children_count;
  parent->children = realloc
    (parent->children, parent->children_count
     * sizeof (Widget));
    
  if (parent->children == NULL)
    {
      FATAL ("realloc() is out of memory.");
    }
  else
    {
      parent->children[parent->children_count - 1] = child;
    }  
}

void
Widget_set_margin_and_padding (Widget *widget, int margin, int padding)
{
  widget->margin = margin;
  widget->padding = padding;
}

void
Widget_set_background_color (Widget *widget, int background_color)
{
  widget->background_color = background_color;
}

void
Widget_set_border (Widget *widget, int border_color, int border_thickness)
{
  widget->border_color = border_color;
  widget->border_thickness = border_thickness;
}

void Widget_draw (Widget *widget)
{
  int x = widget->x + widget->margin;
  int y = widget->y + widget->margin;
  int width = widget->width - widget->margin * 2;
  int height = widget->height - widget->margin * 2;

  switch (widget->type)
    {
    case WIDGETTYPE_HBOX:
    case WIDGETTYPE_VBOX:
      {
	FillArea (x, y, width, height, widget->background_color);
	DrawThickRect (x, y, width, height, widget->border_color,
		       widget->border_thickness);
      }
      break;
    default:
      break;
    }

  /* /\* After we're done with all that, we move on to the children. *\/ */
  /* if (widget->children != NULL) */
  /*   { */
  /*     for (int i = 0; i < widget->children_count; ++i) */
  /* 	{ */
  /* 	  Widget_draw (&widget->children[i]); */
  /* 	} */
  /*   } */
}
