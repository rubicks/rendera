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

#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Group.H>

#include "InputFloat.H"

InputFloat::InputFloat(Fl_Group *g, int x, int y, int w, int h,
                       const char *text, Fl_Callback *cb)
: Fl_Float_Input(x, y, w, h, 0)
{
  group = g;
  var = 0;
  if(cb)
    callback(cb, &var);
  maximum_size(5);
  labelsize(12);
  textsize(12);
  label(text);
  when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
  resize(group->x() + x, group->y() + y, w, h);
}

InputFloat::~InputFloat()
{
}

void InputFloat::center()
{
  int ww = 0, hh = 0;

  this->measure_label(ww, hh);

  resize(parent()->w() / 2 - (ww + w()) / 2 + ww, y(), w(), h());
}

int InputFloat::limitValue(double min, double max)
{
  char str[8];
  double val = std::atof(value());

  if(val < min)
  {
    snprintf(str, sizeof(str), "%.2f", min);
    value(str);
    return -1;
  }

  if(val > max)
  {
    snprintf(str, sizeof(str), "%.2f", max);
    value(str);
    return -1;
  }

  return 0;
}

