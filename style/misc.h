/*
 * Copyright 2003, Sandro Giessl <ceebx@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __MISC_H
#define __MISC_H

TQColor blendColors(const TQColor &backgroundColor, const TQColor &foregroundColor);
TQColor alphaBlendColors(const TQColor &backgroundColor, const TQColor &foregroundColor, const int alpha);
TQImage tintImage(const TQImage &img, const TQColor &tintColor);
TQImage setImageOpacity(const TQImage &img, const uint &p);
bool blend( const TQImage & upper, const TQImage & lower, TQImage & output);

#endif // __MISC_H
