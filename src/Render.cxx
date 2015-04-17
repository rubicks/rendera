/*
Copyright (c) 2015 Joe Davisson.

This file is part of Rendera.

Rendera is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Rendera is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Rendera; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#include <vector>

#include "Bitmap.H"
#include "Brush.H"
#include "Gui.H"
#include "Inline.H"
#include "Map.H"
#include "Math.H"
#include "Project.H"
#include "Render.H"
#include "Stroke.H"
#include "Tool.H"
#include "Undo.H"
#include "View.H"

namespace
{
  Bitmap *bmp;
  Map *map;
  Brush *brush;
  Stroke *stroke;
  View *view;
  int color;
  int trans;


  bool isEdge(Map *map, const int &x, const int &y)
  {
    if(x < 1 || x >= map->w - 1 || y < 1 || y >= map->h - 1)
      return 0;

    if( *(map->row[y - 1] + x) &&
        *(map->row[y] + x - 1) &&
        *(map->row[y] + x + 1) &&
        *(map->row[y + 1] + x) )
      return 0;
    else
      return 1;
  }

  inline int fdist(const int &x1, const int &y1, const int &x2, const int &y2)
  {
    const int dx = (x1 - x2);
    const int dy = (y1 - y2);

    return dx * dx + dy * dy;
  }

  inline int sdist(const int &x1, const int &y1,
                   const int &x2, const int &y2,
                   const int &edge, const int &trans)
  {
    float d = Math::sqrt(fdist(x1, y1, x2, y2));
    float s = (255 - trans) / (((3 << edge) >> 1) + 1);

    if(s < 1)
      s = 1;

    int temp = 255 - s * d;

    if(temp < trans)
      temp = trans;

    return temp;
  }

  inline void shrinkBlock(unsigned char *s0, unsigned char *s1,
                          unsigned char *s2, unsigned char *s3)
  {
    const int z = (*s0 << 0) + (*s1 << 1) + (*s2 << 2) + (*s3 << 3);

    switch(z)
    {
      case 0:
      case 15:
        return;
      case 7:
        *s1 = 0;
        *s2 = 0;
        return;
      case 11:
        *s0 = 0;
        *s3 = 0;
        return;
      case 13:
        *s0 = 0;
        *s3 = 0;
        return;
      case 14:
        *s1 = 0;
        *s2 = 0;
        return;
    }

    *s0 = 0;
    *s1 = 0;
    *s2 = 0;
    *s3 = 0;
  }

  inline void growBlock(unsigned char *s0, unsigned char *s1,
                        unsigned char *s2, unsigned char *s3)
  {
    int z = (*s0 << 0) + (*s1 << 1) + (*s2 << 2) + (*s3 << 3);

    switch (z)
    {
      case 0:
      case 15:
        return;
      case 1:
        *s1 = 1;
        *s2 = 1;
        return;
      case 2:
        *s0 = 1;
        *s3 = 1;
        return;
      case 4:
        *s0 = 1;
        *s3 = 1;
        return;
      case 8:
        *s1 = 1;
        *s2 = 1;
        return;
    }

    *s0 = 1;
    *s1 = 1;
    *s2 = 1;
    *s3 = 1;
  }

  int update(int pos)
  {
    // user cancelled operation
    if(Fl::get_key(FL_Escape))
    {
      Gui::getView()->drawMain(true);
      return -1;
    }

    if(!(pos % 50))
    {
      view->drawMain(true);
      Fl::check();
    }

    return 0;
  }

  void renderSolid()
  {
    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      unsigned char *p = map->row[y] + stroke->x1;

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if(*p++)
          bmp->setpixel(x, y, color, trans);
      }

      if(update(y) < 0)
        break;
    }
  }

  void renderAntialiased()
  {
    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      unsigned char *p = map->row[y] + stroke->x1;

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if(*p > 0)
          bmp->setpixel(x, y, color, scaleVal((255 - *p), trans));

        p++;
      }

      if(update(y) < 0)
        break;
    }
  }

  void renderCoarse()
  {
    float soft_trans = 255;
    const int j = (3 << brush->edge);
    float soft_step = (float)(255 - trans) / ((j >> 1) + 1);
    bool found = false;

    for(int i = 0; i < j; i++)
    {
      for(int y = stroke->y1 + (i & 1); y < stroke->y2; y += 2)
      {
        for(int x = stroke->x1 + (i & 1); x < stroke->x2; x += 2)
        {
          unsigned char *s0 = map->row[y] + x;
          unsigned char *s1 = map->row[y] + x + 1;
          unsigned char *s2 = map->row[y + 1] + x;
          unsigned char *s3 = map->row[y + 1] + x + 1;

          *s0 &= 1;
          *s1 &= 1;
          *s2 &= 1;
          *s3 &= 1;

          if(*s0 | *s1 | *s2 | *s3)
            found = true;

          const unsigned char d0 = *s0;
          const unsigned char d1 = *s1;
          const unsigned char d2 = *s2;
          const unsigned char d3 = *s3;

          shrinkBlock(s0, s1, s2, s3);

          if(!*s0 && d0)
            bmp->setpixel(x, y, color, soft_trans);
          if(!*s1 && d1)
            bmp->setpixel(x + 1, y, color, soft_trans);
          if(!*s2 && d2)
            bmp->setpixel(x, y + 1, color, soft_trans);
          if(!*s3 && d3)
            bmp->setpixel(x + 1, y + 1, color, soft_trans);
        }
      }

      if(!found)
        break;

      soft_trans -= soft_step;

      if(soft_trans < trans)
      {
        soft_trans = trans;

        for(int y = stroke->y1; y <= stroke->y2; y++)
        {
          for(int x = stroke->x1; x <= stroke->x2; x++)
          {
            if(map->getpixel(x, y))
              bmp->setpixel(x, y, color, soft_trans);
          }
        }

        return;
      }

      if(update(i) < 0)
        break;
    }
  }

  void renderFine()
  {
    int count = 0;

    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if((Project::map)->getpixel(x, y) && isEdge((Project::map), x, y))
        {
          stroke->edgecachex[count] = x;
          stroke->edgecachey[count] = y;
          count++;
          count &= 0xFFFFF;
        }
      }
    }

    if(count < 2)
      return;

    for(int y = stroke->y1; y < stroke->y2; y++)
    {
      unsigned char *p = map->row[y] + stroke->x1;

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if(*p == 0)
        {
          p++;
          continue;
        }

        int *cx = &stroke->edgecachex[0];
        int *cy = &stroke->edgecachey[0];
        int temp1 = fdist(x, y, *cx++, *cy++);
        int z = 0;

        for(int i = 1; i < count; i++)
        {
          const int dx = (x - *cx++);
          const int dy = (y - *cy++);
          const int temp2 = dx * dx + dy * dy;

          if(temp2 < temp1)
          {
            z = i;
            temp1 = temp2;
          }
        }

        bmp->setpixel(x, y, color, sdist(x, y,
                      stroke->edgecachex[z], stroke->edgecachey[z],
                      brush->edge, trans));

        p++;
      }

      if(update(y) < 0)
        break;
    }
  }

  void renderBlur()
  {
    const int amount = (brush->edge + 2) * (brush->edge + 2) + 1;
    std::vector<int> kernel(amount);
    int div = 0;

    // bell curve
    const int b = amount / 2;

    for(int x = 0; x < amount; x++)
    {
      kernel[x] = 255 * Math::exp(-((double)((x - b) * (x - b)) /
                                           ((b * b) / 2)));
      div += kernel[x];
    }

    Map temp((stroke->x2 - stroke->x1) + 1,
             (stroke->y2 - stroke->y1) + 1);

    // x direction
    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      unsigned char *p = temp.row[y - stroke->y1];

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        int val = 0;

        for(int i = 0; i < amount; i++)
        {
          val += map->getpixel(x - amount / 2 + i, y) * kernel[i];
        }

        val /= div;

        *p++ = (unsigned char)val;
      }
    }

    // y direction
    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      unsigned char *p = map->row[y] + stroke->x1;

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        int val = 0;

        for(int i = 0; i < amount; i++)
        {
          val += temp.getpixel(x - stroke->x1,
                               y - amount / 2 + i - stroke->y1) * kernel[i];
        }

        val /= div;

        *p++ = (unsigned char)val;
      }
    }

    // render
    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      unsigned char *p = map->row[y] + stroke->x1;

      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if(*p > 0)
          bmp->setpixel(x, y, color, scaleVal((255 - *p), trans));

        p++;
      }

      if(update(y) < 0)
        break;
    }
  }

  void renderWatercolor()
  {
    float soft_trans = trans;
    const int j = (3 << brush->edge);
    float soft_step = (float)(255 - trans) / ((j >> 1) + 1);
    bool found = false;
    int inc = 0;

    for(int y = stroke->y1; y <= stroke->y2; y++)
    {
      for(int x = stroke->x1; x <= stroke->x2; x++)
      {
        if(map->getpixel(x, y))
          bmp->setpixel(x, y, color, trans);
      }
    }

    for(int i = 0; i < j; i++)
    {
      inc++;

      for(int y = stroke->y1 + (inc & 1); y < stroke->y2 - 1; y += 2)
      {
        for(int x = stroke->x1 + (inc & 1); x < stroke->x2 - 1; x += 2)
        {
          int yy = y + !(Math::rnd() & 3);

          unsigned char *s0 = map->row[yy] + x;
          unsigned char *s1 = map->row[yy] + x + 1;
          unsigned char *s2 = map->row[yy + 1] + x;
          unsigned char *s3 = map->row[yy + 1] + x + 1;

          *s0 &= 1;
          *s1 &= 1;
          *s2 &= 1;
          *s3 &= 1;

          if(*s0 | *s1 | *s2 | *s3)
            found = true;

          const unsigned char d0 = *s0;
          const unsigned char d1 = *s1;
          const unsigned char d2 = *s2;
          const unsigned char d3 = *s3;

          growBlock(s0, s1, s2, s3);

          if(*s0 & !(Math::rnd() & 15))
          {
            *s0 = 1;
            *s1 = 1;
            *s2 = 1;
            *s3 = 1;
            inc--;
          }

          if(*s0 && !d0)
            bmp->setpixel(x, yy, color, soft_trans);
          if(*s1 && !d1)
            bmp->setpixel(x + 1, yy, color, soft_trans);
          if(*s2 && !d2)
            bmp->setpixel(x, yy + 1, color, soft_trans);
          if(*s3 && !d3)
            bmp->setpixel(x + 1, yy + 1, color, soft_trans);
        }
      }

      if(!found)
        break;

      soft_trans += soft_step;

      if(soft_trans > 255)
        break;

      if(update(i) < 0)
        break;
    }
  }

  void renderChalk()
  {
    float soft_trans = 255;
    const int j = (3 << brush->edge);
    float soft_step = (float)(255 - trans) / ((j >> 1) + 1);
    bool found = false;

    for(int i = 0; i < j; i++)
    {
      for(int y = stroke->y1 + (i & 1); y < stroke->y2; y += 2)
      {
        for(int x = stroke->x1 + (i & 1); x < stroke->x2; x += 2)
        {
          unsigned char *s0 = map->row[y] + x;
          unsigned char *s1 = map->row[y] + x + 1;
          unsigned char *s2 = map->row[y + 1] + x;
          unsigned char *s3 = map->row[y + 1] + x + 1;

          *s0 &= 1;
          *s1 &= 1;
          *s2 &= 1;
          *s3 &= 1;

          if(*s0 | *s1 | *s2 | *s3)
            found = true;

          unsigned char d0 = *s0;
          unsigned char d1 = *s1;
          unsigned char d2 = *s2;
          unsigned char d3 = *s3;

          shrinkBlock(s0, s1, s2, s3);

          int t = 0;

          if(!*s0 && d0)
          {
            t = (int)soft_trans + (Math::rnd() & 63) - 32;
            if(t < 0)
              t = 0;
            if(t > 255)
              t = 255;
            bmp->setpixel(x, y, color, t);
          }

          if(!*s1 && d1)
          {
            t = (int)soft_trans + (Math::rnd() & 63) - 32;
            if(t < 0)
              t = 0;
            if(t > 255)
              t = 255;
            bmp->setpixel(x + 1, y, color, t);
          }

          if(!*s2 && d2)
          {
            t = (int)soft_trans + (Math::rnd() & 63) - 32;
            if(t < 0)
              t = 0;
            if(t > 255)
              t = 255;
            bmp->setpixel(x, y + 1, color, t);
          }

          if(!*s3 && d3)
          {
            t = (int)soft_trans + (Math::rnd() & 63) - 32;
            if(t < 0)
              t = 0;
            if(t > 255)
              t = 255;
            bmp->setpixel(x + 1, y + 1, color, t);
          }
        }
      }

      if(!found)
        break;

      soft_trans -= soft_step;

      if(soft_trans < trans)
      {
        soft_trans = trans;
        for(int y = stroke->y1; y <= stroke->y2; y++)
        {
          for(int x = stroke->x1; x <= stroke->x2; x++)
          {
            if(map->getpixel(x, y))
            {
              int t = (int)soft_trans + (Math::rnd() & 63) - 32;
              if(t < 0)
                t = 0;
              if(t > 255)
                t = 255;
              bmp->setpixel(x, y, color, t);
            }
          }
        }

        return;
      }

      if(update(i) < 0)
        break;
    }
  }
}

void Render::begin()
{
  view = Gui::getView();
  bmp = Project::bmp;
  map = Project::map;
  brush = Project::brush.get();
  stroke = Project::stroke.get();
  color = brush->color;
  trans = brush->trans;

  int size = 1;

  // kludge for tools that grow outward
  switch(Gui::getPaintMode())
  {
    case WATERCOLOR:
      size = (3 << brush->edge);
      break;
    case BLUR:
      size = ((brush->edge + 2) * (brush->edge + 2) + 1) / 2 + 1;
      break;
  }

  stroke->x1 -= size;
  stroke->y1 -= size;
  stroke->x2 += size;
  stroke->y2 += size;
  stroke->clip();

  stroke->makeBlitRect(stroke->x1, stroke->y1,
                       stroke->x2, stroke->y2,
                       view->ox, view->oy, 1, view->zoom);

  Undo::push(stroke->x1,
             stroke->y1,
             (stroke->x2 - stroke->x1) + 1,
             (stroke->y2 - stroke->y1) + 1);

  view->rendering = true;

  switch(Gui::getPaintMode())
  {
    case SOLID:
      renderSolid();
      break;
    case ANTIALIASED:
      renderAntialiased();
      break;
    case COARSE:
      renderCoarse();
      break;
    case FINE:
      renderFine();
      break;
    case BLUR:
      renderBlur();
      break;
    case WATERCOLOR:
      renderWatercolor();
      break;
    case CHALK:
      renderChalk();
      break;
    default:
      break;
  }

  view->drawMain(true);
  view->rendering = false;
}

