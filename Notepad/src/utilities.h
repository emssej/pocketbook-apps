#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <inkview.h>

#define COLOR_BLUE     "\x1B[34m"
#define COLOR_RED      "\x1B[31m"
#define COLOR_YELLOW   "\x1B[33m"
#define COLOR_RESET    "\x1B[0m"

#define LAMBDA(type, body) ({ type _ body _; })

#define INFO(fmt, args...) (fprintf (stderr, COLOR_BLUE "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args))
#define WARN(fmt, args...) (fprintf (stderr, COLOR_YELLOW "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args))
#define FATAL(fmt, args...) (fprintf (stderr, COLOR_RED "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args), exit (-1))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define INRECT(x, y, w, h, X, Y) ((X > x) && (X < w) && (Y > y) && (Y < h))

inline void
SmoothThickLine (int x0, int y0, int x1, int y1, int color, int thickness)
{
  /* Safety check. */
  thickness = thickness < 1 ? 1 : thickness;
  
  int dx = abs (x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  while (true)
    {
      DrawCircle (x0, y0, thickness, color);

      if (x0 == x1 && y0 == y1)
	{
	  break;
	}

      e2 = err * 2;

      if (e2 >= dy)
	{
	  err += dy;
	  x0 += sx;
	}

      if (e2 <= dx)
	{
	  err += dx;
	  y0 += sy;
	}
    }  
}

inline void
DrawThickRect (int x, int y, int width, int height, int color, int thickness)
{
  /* Just a safety check. */
  thickness = thickness < 0 ? 0 : thickness;
  for (int i = 0; i < thickness; ++i)
    {
      DrawRect (x + i, y + i, width - i * 2, height - i * 2, color);
    }
}

inline void
DrawTextRectCentered (int x, int y, int w, int h, char *text, int flags)
{
  int hc = TextRectHeight (w, text, flags);
  DrawTextRect (x, y + h / 2 - hc / 2, w, h, text, flags);
}

/* int last_x, last_y; */
  
/* int line_thickness = 3; */
/* int color = BLACK; */

/* _Bool pressure_sensitivity_emulation_on = 1; */

/* void */
/* pointermove_callback (int event_type, int x, int y) */
/* { */
/*   if (event_type == EVT_POINTERMOVE) */
/*     { */
/*       int thickness = line_thickness; */

/*       if (pressure_sensitivity_emulation_on) */
/* 	{ */
/* 	  /\* We calculate this so that we can emulate pen pressure */
/* 	     sensitivity. *\/ */
/* 	  int a = x - last_x; */
/* 	  int b = y - last_y; */
/* 	  float distance = fabs (sqrt (a * a + b * b)); */
/* 	  float speed = (float) distance / (line_thickness * 5); */
/* 	  thickness = (float) line_thickness - speed; */

/* 	  if (thickness < 1) */
/* 	    { */
/* 	      thickness = 1; */
/* 	    } */
/* 	} */

/*       SmoothThickLine (last_x, last_y, x, y, color, thickness); */
      
/*       int start_x = (x < last_x ? x : last_x) - thickness; */
/*       int start_y = (y < last_y ? y : last_y) - thickness; */
/*       int end_x = (x > last_x ? x : last_x) + thickness; */
/*       int end_y = (y > last_y ? y : last_y) + thickness; */
      
/*       PartialUpdate (start_x, start_y, end_x - start_x, end_y - start_y); */
      
/*       last_x = x; */
/*       last_y = y; */
/*     } */
/* } */
