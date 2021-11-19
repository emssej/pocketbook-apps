#pragma once

#include <stdlib.h>
#include <math.h>

#include <inkview.h>

#include "utilities.h"

typedef enum
WidgetType
  {
    WIDGETTYPE_VBOX,
    WIDGETTYPE_HBOX,
    WIDGETTYPE_LABEL
  } WidgetType;

/* Should text alignment be separate to the margin and padding? I don't think so. */

typedef enum
TextAlign
  {
    TEXT_ALIGN_TOP_LEFT,
    TEXT_ALIGN_TOP_RIGHT,
    TEXT_ALIGN_TOP_CENTER,
    TEXT_ALIGN_CENTER_LEFT,
    TEXT_ALIGN_CENTER_CENTER,
    TEXT_ALIGN_CENTER_RIGHT,
    TEXT_ALIGN_BOTTOM_LEFT,
    TEXT_ALIGN_BOTTOM_CENTER,
    TEXT_ALIGN_BOTTOM_RIGHT
  } TextAlign;

/* The huge struct that the whole UI system relies on. */

typedef struct
Widget
{
  WidgetType type;
  /* Index is the position in the CHILDREN array of the PARENT.  */
  int index;
  /* If WIDTH or HEIGHT is 0, it fills the remaining space. If the
     widget has no parent, then it uses ScreenWidth () and
     ScreenHeight (). */
  int x, y, width, height;
  /* If it's disabled, both it and it's children are invisible, and
     neither does it or its children take events. */
  _Bool enabled;
  
  void (*pointerdown_callback) (struct Widget *self);
  void (*pointerup_callback) (struct Widget *self);
  void (*pointermove_callback) (struct Widget *self);

  struct Widget *parent;
  size_t children_count;
  struct Widget *children;

  /* Visual stuff that applies to all widgets. For now, at least. */
  int background_color;
  int border_color, border_thickness;
  int margin, padding;
  
  /* This union contains all the widget types. */
  union
  {
    struct
    {
      ifont *font;
      char *text;
      int color;
      TextAlign align;
    } label;
  };
} Widget;

/* The root widget is always a box. */
Widget Widget_new_box (_Bool horizontal);
Widget Widget_new_label (char *text, int color, TextAlign align, ifont *font);

void Widget_add_child (Widget *parent, Widget child);

void Widget_set_margin_and_padding (Widget *widget, int margin, int padding);
void Widget_set_background_color (Widget *widget, int background);
void Widget_set_border (Widget *widget, int border_color, int border_thickness);

void Widget_draw (Widget *widget);
