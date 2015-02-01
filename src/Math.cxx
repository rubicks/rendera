/* rendera/src/Math.cxx */

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

namespace
{
#include <math.h>

  double _cos   ( double a ){ return cos  ( a ); }
  double _exp   ( double a ){ return exp  ( a ); }
  double _sin   ( double a ){ return sin  ( a ); }
  double _sqrt  ( double a ){ return sqrt ( a ); }

  double _atan2 ( double y, double x ){ return atan2 ( y, x ); }
  double _pow   ( double b, double e ){ return pow   ( b, e ); }
}

#include "Math.H"

double Math::cos   ( double a ){ return _cos  ( a ); }
double Math::exp   ( double a ){ return _exp  ( a ); }
double Math::sin   ( double a ){ return _sin  ( a ); }
double Math::sqrt  ( double a ){ return _sqrt ( a ); }

double Math::atan2 ( double y, double x ){ return _atan2 ( y, x ); }
double Math::pow   ( double b, double e ){ return _pow   ( b, e ); }
