/**
******************************************************************************
*
* @file       pointlatlng.cpp
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2012.
* @author     Tau Labs, http://taulabs.org, Copyright (C) 2013
* @brief      
* @see        The GNU Public License (GPL) Version 3
* @defgroup   TLMapWidget
* @{
* 
*****************************************************************************/
/* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
* for more details.
* 
* You should have received a copy of the GNU General Public License along 
* with this program; if not, see <http://www.gnu.org/licenses/>
*/
#include "pointlatlng.h"

 
namespace internals {
PointLatLng PointLatLng::Empty=PointLatLng();
PointLatLng::PointLatLng():lat(0),lng(0),empty(true)
{

}

bool operator==(PointLatLng const& lhs,PointLatLng const& rhs)
{
   return ((lhs.Lng() == rhs.Lng()) && (lhs.Lat() == rhs.Lat()));
}

bool operator!=(PointLatLng const& left, PointLatLng const& right)
{
   return !(left == right);
}
PointLatLng operator+(PointLatLng pt, SizeLatLng sz)
{
   return PointLatLng::Add(pt, sz);
}

PointLatLng operator-(PointLatLng pt, SizeLatLng sz)
{
   return PointLatLng::Subtract(pt, sz);
}
}
