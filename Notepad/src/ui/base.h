#pragma once

#include <math.h>

#include "../utilities.h"

typedef struct
UIEvent
{
  int event_type, arg1, arg2;
} UIEvent;

/* The UIWidget has no type or anything, it just contains every
   possible thing you can have in a widget. That way it is much less
   complicated, and actually more versatile too. */
typedef struct
UIWidget
{
  /* Index is the index of this UIWidget in the PARENT's array of
     CHILDREN. */
  int index;

  size_t children_count;
  struct UIWidget *parent, **children;

  void (*onpointerdown) (struct UIWidget *self, UIEvent event);
  void (*onpointermove) (struct UIWidget *self, UIEvent event);
  void (*onpointerup) (struct UIWidget *self, UIEvent event);
  void (*onkeydown[34]) (struct UIWidget *self, UIEvent event);
  void (*ondraw) (struct UIWidget *self, UIEvent event);
  
  bool horizontal;		/* If it's TRUE, this UIWidget's
				   CHILDREN are arranged
				   horizontally. If it's FALSE,
				   vertically.*/
  float ratio;			/* This describes how much height (or
				   width) this UIWidget will have in
				   relation to it's PARENT. Every
				   RATIO of every child of a PARENT
				   should add up to 1. */
  
  /* Soon™ */
  /* int margin, padding; */

  int background_color, foreground_color;

  ifont *font;
  char *text;

  /* ibitmap *BitmapFromScreen(int x, int y, int w, int h); */
  /* int SavePNG(const char *path, const ibitmap *bmp); */
  /* ibitmap *LoadPNG(const char *path, int dither); */
  /* void DrawBitmap(int x, int y, const ibitmap *b); */
  ibitmap *image;

  /* And to add some versatility... */
  void *data;
  
  /* Things that aren't set by the user, but by the internal
     functions. */
  int x, y, w, h;
} UIWidget;

UIWidget *UIWidget_new ();
void UIWidget_terminate (UIWidget *widget);

int UIWidget_add_child (UIWidget *parent, UIWidget *child);
void UIWidget_remove_children (UIWidget *parent);

#define UIWidget_draw(widget, ...) UIWidget_draw_internal(widget, (true, ##__VA_ARGS__))
void UIWidget_draw_internal (UIWidget *widget, bool update);
void UIWidget_handle_event (UIWidget *widget, UIEvent event);

/* Utility functions. */

inline void
UIWidget_flash (UIWidget *widget)
{
  FlashArea (widget->x, widget->y, widget->w, widget->h);
}
