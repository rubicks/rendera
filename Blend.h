/*
Copyright (c) 2014 Joe Davisson.

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

#ifndef BLEND_H
#define BLEND_H

class Bitmap;

#include "rendera.h"

namespace Blend
{
  enum
  {
    TRANS,
    DARKEN,
    LIGHTEN,
    COLORIZE,
    ALPHA_ADD,
    ALPHA_SUB,
    SMOOTH,
    SMOOTH_COLOR,
    INVERT
  };
 
  void set(const int &);
  void target(Bitmap *, const int &, const int &);
  int current(const int &, const int &, const int &);
  int invert(const int &, const int &, const int &);
  int trans(const int &, const int &, const int &);
  int transAll(const int &, const int &, const int &);
  int darken(const int &, const int &, const int &);
  int lighten(const int &, const int &, const int &);
  int colorize(const int &, const int &, const int &);
  int keepLum(const int &, const int &);
  int alphaAdd(const int &, const int &, const int &);
  int alphaSub(const int &, const int &, const int &);
  int smooth(const int &, const int &, const int &);
  int smoothColor(const int &, const int &, const int &);
  void rgbToHsv(const int &, const int &, const int &, int *, int *, int *);
  void hsvToRgb(const int &, const int &, const int &, int *, int *, int *);
}

#endif

