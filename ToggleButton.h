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

#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include "rendera.h"

class ToggleButton : public Fl_Button
{
public:
  ToggleButton(Fl_Group *, int, int, int, int, const char *, const char *);
  virtual ~ToggleButton();

  virtual int handle(int);

  int var;
  Fl_Group *group;
  Fl_PNG_Image *image;
protected:
  virtual void draw();
};

#endif
