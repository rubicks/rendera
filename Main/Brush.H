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

#ifndef BRUSH_H
#define BRUSH_H

class Brush
{
public:
  Brush();
  ~Brush();

  void make(int, int);

  int *solidx, *solidy;
  int *hollowx, *hollowy;
  int solid_count;
  int hollow_count;
  int size;
  int shape;
  int edge;
  int color;
  int trans;
  int blend;
  int aa;
};

#endif

