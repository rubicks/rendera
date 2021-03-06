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
#include "Clone.H"
#include "Gui.H"
#include "Map.H"
#include "Project.H"
#include "Tool.H"
#include "Undo.H"
#include "View.H"

namespace
{
  std::vector<Bitmap *> undo_stack(10); 
  int undo_levels = undo_stack.size();
  std::vector<bool> undo_resized(undo_levels); 
  int undo_current = undo_levels - 1;
}

void Undo::init()
{
  for(int i = 0; i < undo_levels; i++)
  {
    if(undo_stack[i])
      delete undo_stack[i];
    undo_stack[i] = new Bitmap(8, 8);
    undo_resized[i] = false;
  }

  undo_current = undo_levels - 1;
}

void Undo::push()
{
  int x1 = Project::bmp->cl;
  int y1 = Project::bmp->ct;
  int x2 = Project::bmp->cr;
  int y2 = Project::bmp->cb;

  int w = (x2 - x1) + 1;
  int h = (y2 - y1) + 1;

  if(undo_current < 0)
  {
    undo_current = 0;

    Bitmap *temp_bmp = undo_stack[undo_levels - 1];
    bool temp_resized = undo_resized[undo_levels - 1];

    for(int i = undo_levels - 1; i > 0; i--)
    {
      undo_stack[i] = undo_stack[i - 1];
      undo_resized[i] = undo_resized[i - 1];
    }

    undo_stack[0] = temp_bmp;
    undo_resized[0] = temp_resized;
  }

  undo_resized[undo_current] = true;

  delete undo_stack[undo_current];
  undo_stack[undo_current] = new Bitmap(w, h);
  undo_stack[undo_current]->x = x1;
  undo_stack[undo_current]->y = y1;

  Project::bmp->blit(undo_stack[undo_current], x1, y1, 0, 0, w, h);
  undo_current--;
}

void Undo::push(int x, int y, int w, int h)
{
  // store full image if wrap is enabled
  if(Clone::wrap)
  {
    push();
    return;
  }

  int x1 = x;
  int y1 = y;
  int x2 = x + w - 1;
  int y2 = y + h - 1;

  if(x1 < Project::bmp->cl)
    x1 = Project::bmp->cl;
  if(y1 < Project::bmp->ct)
    y1 = Project::bmp->ct;
  if(x2 > Project::bmp->cr)
    x2 = Project::bmp->cr;
  if(y2 > Project::bmp->cb)
    y2 = Project::bmp->cb;

  w = (x2 - x1) + 1;
  h = (y2 - y1) + 1;

  if(undo_current < 0)
  {
    undo_current = 0;

    Bitmap *temp_bmp = undo_stack[undo_levels - 1];
    bool temp_resized = undo_resized[undo_levels - 1];

    for(int i = undo_levels - 1; i > 0; i--)
    {
      undo_stack[i] = undo_stack[i - 1];
      undo_resized[i] = undo_resized[i - 1];
    }

    undo_stack[0] = temp_bmp;
    undo_resized[0] = temp_resized;
  }

  undo_resized[undo_current] = false;

  delete undo_stack[undo_current];
  undo_stack[undo_current] = new Bitmap(w, h);
  undo_stack[undo_current]->x = x1;
  undo_stack[undo_current]->y = y1;

  Project::bmp->blit(undo_stack[undo_current], x1, y1, 0, 0, w, h);
  undo_current--;
}

void Undo::pop()
{
  if(undo_current >= undo_levels - 1)
    return;

  undo_current++;

  if(undo_resized[undo_current])
  {
    int w = undo_stack[undo_current]->w;
    int h = undo_stack[undo_current]->h;

    Project::newImage(w, h);

    int ox = Gui::getView()->ox;
    int oy = Gui::getView()->oy;

    if(ox < 0)
      ox = 0;
    if(ox > w - 1)
      ox = w - 1;
    if(oy < 0)
      oy = 0;
    if(oy > h - 1)
      oy = h - 1;

    Gui::getView()->ox = ox;
    Gui::getView()->oy = oy;

    undo_stack[undo_current]->blit(Project::bmp, 0, 0,
                                   Project::bmp->overscroll,
                                   Project::bmp->overscroll,
                                   undo_stack[undo_current]->w,
                                   undo_stack[undo_current]->h);
  }
  else
  {
    undo_stack[undo_current]->blit(Project::bmp, 0, 0, 
                                   undo_stack[undo_current]->x,
                                   undo_stack[undo_current]->y,
                                   undo_stack[undo_current]->w,
                                   undo_stack[undo_current]->h);
  }

  Project::tool->reset();
  Gui::getView()->drawMain(true);
}

void Undo::free()
{
  for(int i = 0; i < undo_levels; i++)
    if(undo_stack[i])
      delete undo_stack[i];
}

