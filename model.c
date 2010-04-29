/*
Copyright (C) 2003 - Derek John Evans 

This file is part of Yeti3D Portable Engine

Yeti3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 2003 - Derek J. Evans <derek@theteahouse.com.au>
Prepared for public release: 10/24/2003 - Derek J. Evans <derek@theteahouse.com.au>
*/

#include "model.h"

/******************************************************************************/

#undef CODE_IN_IWRAM 
#define CODE_IN_IWRAM 

void CODE_IN_IWRAM md2_tmap(vertex_t* p, int n, YETI_ROM u16* texture, int textsize, framebuffer_t* dst)
{
  int lt_i = 0, lt_x = 0, lt_xx = 0, lt_u = 0, lt_uu = 0, lt_v = 0, lt_vv = 0;
  int rt_i = 0, rt_x = 0, rt_xx = 0, rt_u = 0, rt_uu = 0, rt_v = 0, rt_vv = 0;
  int x1 = 0, x2 = 0, i = 0, j = 0, u = 0, uu = 0, v = 0, vv = 0;
  int lt_length = 0, rt_length = 0;
  int y1 = 999999, y2 = -999999;

  for (i = n; i--;)
  {
    p[i].d = fixup(p[i].y);
    if (p[i].y < y1) {y1 = p[i].y; lt_i = rt_i = i;}
    if (p[i].y > y2) {y2 = p[i].y;}
  }

  if ((y1 = fixup(y1)) < YETI_VIEWPORT_Y1) y1 = YETI_VIEWPORT_Y1;
  if ((y2 = fixup(y2)) > YETI_VIEWPORT_Y2) y2 = YETI_VIEWPORT_Y2;

  for (; y1 < y2; y1++)
  {
    if (--lt_length <= 0)
    {
      do {
        i = lt_i--; if (lt_i < 0) lt_i = n - 1;
      } while ((lt_length = p[lt_i].d - p[i].d) <= 0);

      j = reciprocal[lt_length];

      lt_xx = r2i((p[lt_i].x - (lt_x = p[i].x)) * j);
      lt_uu = r2i((p[lt_i].u - (lt_u = p[i].u)) * j);
      lt_vv = r2i((p[lt_i].v - (lt_v = p[i].v)) * j);
    }
    if (--rt_length <= 0)
    {
      do {
        i = rt_i++; if (rt_i >= n) rt_i = 0;
      } while ((rt_length = p[rt_i].d - p[i].d) <= 0);

      j = reciprocal[rt_length];

      rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * j);
      rt_uu = r2i((p[rt_i].u - (rt_u = p[i].u)) * j);
      rt_vv = r2i((p[rt_i].v - (rt_v = p[i].v)) * j);
    }
    if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) x1 = YETI_VIEWPORT_X1;
    if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) x2 = YETI_VIEWPORT_X2;

    if ((i = x2 - x1) > 0)
    {
      u16* fb = &dst->pixels[y1][x1];

      j = reciprocal[i];
      uu = r2i((rt_u - (u = lt_u)) * j); u -= uu;
      vv = r2i((rt_v - (v = lt_v)) * j); v -= vv;

#define AFFINE(I) fb[I] = texture[(f2i(v+=vv) << textsize) + f2i(u+=uu)];
      AFFINE_LOOP;
#undef  AFFINE
    }
    lt_x += lt_xx; lt_u += lt_uu; lt_v += lt_vv;
    rt_x += rt_xx; rt_u += rt_uu; rt_v += rt_vv;
  }
}

void CODE_IN_IWRAM md2_clipped_poly(yeti_t* yeti, polyclip_t src, int n, u16* skin)
{
  int i;
  polygon_t p;

  for (i = n; i--;)
  {
    p[i].x = src[i]->sx;
    p[i].y = src[i]->sy;
    p[i].u = src[i]->u;
    p[i].v = src[i]->v;
  }

  md2_tmap(p, n, skin, 8, yeti->viewport.back);
}

void CODE_IN_IWRAM md2_unclipped_poly(yeti_t* yeti, polyclip_t p, int n, u16* skin)
{
  int i;

  for (i = n; i--;)
  {
    if (
       (p[i]->x + p[i]->z) < 0 ||
       (p[i]->z - p[i]->x) < 0 ||
       (p[i]->y + p[i]->z) < 0 ||
       (p[i]->z - p[i]->y) < 0 )
    {
      polyclip_t a, b;

      for (i = n; i--; p[i]->d = p[i]->x + p[i]->z); n = polygon_clip(yeti, a, p, n); // Left
      for (i = n; i--; a[i]->d = a[i]->z - a[i]->x); n = polygon_clip(yeti, b, a, n); // Right
      for (i = n; i--; b[i]->d = b[i]->y + b[i]->z); n = polygon_clip(yeti, a, b, n); // Top
      for (i = n; i--; a[i]->d = a[i]->z - a[i]->y); n = polygon_clip(yeti, b, a, n); // Bottom

      p = b;
      
      break;
    }
  }
  if (n > 2) md2_clipped_poly(yeti, p, n, skin);
}




