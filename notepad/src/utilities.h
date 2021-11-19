#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <inkview.h>

#define FALSE 0
#define TRUE 1

#define COLOR_BLUE     "\x1B[34m"
#define COLOR_RED      "\x1B[31m"
#define COLOR_YELLOW   "\x1B[33m"
#define COLOR_RESET    "\x1B[0m"

#define INFO(fmt, args...) (fprintf (stderr, COLOR_BLUE "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args))
#define WARN(fmt, args...) (fprintf (stderr, COLOR_YELLOW "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args))
#define FATAL(fmt, args...) (fprintf (stderr, COLOR_RED "%s:%d:%s(): " COLOR_RESET fmt "\n", __FILE__, __LINE__, __func__, ##args), exit (-1))

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

  while (TRUE)
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
  thickness = thickness < 1 ? 1 : thickness;
  for (int i = 0; i < thickness; ++i)
    {
      DrawRect (x + i, y + i, width - i * 2, height - i * 2, color);
    }
}
