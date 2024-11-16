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

#include <tqcolor.h>
#include <tqimage.h>
#include <tqpixmap.h>

#include "misc.h"

TQColor alphaBlendColors(const TQColor &bgColor, const TQColor &fgColor, const int a)
{

    // normal button...
    TQRgb rgb = bgColor.rgb();
    TQRgb rgb_b = fgColor.rgb();
    int alpha = a;
    if(alpha>255) alpha = 255;
    if(alpha<0) alpha = 0;
    int inv_alpha = 255 - alpha;

    TQColor result  = TQColor( tqRgb(tqRed(rgb_b)*inv_alpha/255 + tqRed(rgb)*alpha/255,
                                  tqGreen(rgb_b)*inv_alpha/255 + tqGreen(rgb)*alpha/255,
                                  tqBlue(rgb_b)*inv_alpha/255 + tqBlue(rgb)*alpha/255) );

    return result;
}

TQImage tintImage(const TQImage &img, const TQColor &tintColor) {
	
	TQImage *result = new TQImage(img.width(), img.height(), 32, 0, TQImage::IgnoreEndian);
	result->setAlphaBuffer( true );
	uint *data = (unsigned int*) img.bits();
	uint *resultData = (unsigned int*) result->bits();
	uint total = img.width()*img.height();
	for ( uint current = 0 ; current < total ; ++current ) {
		resultData[ current ] = tqRgba( tintColor.red(), tintColor.green(), tintColor.blue(), tqAlpha( data[ current ] ));
	}
	return *result;
}

TQImage setImageOpacity(const TQImage &img, const uint &p) {
	TQImage *result = new TQImage(img.width(), img.height(), 32, 0, TQImage::IgnoreEndian);
	result->setAlphaBuffer( true );
	uint *data = (unsigned int*) img.bits();
	uint *resultData = (unsigned int*) result->bits();
	uint alpha;
	uint total = img.width()*img.height();
	for ( uint current = 0 ; current < total ; ++current ) {
		alpha = tqAlpha( data[ current ] ) * p / 100;
		resultData[ current ] = tqRgba( tqRed( data[ current ] ), tqGreen( data[ current ] ), tqBlue( data[ current ] ), alpha );
	}
	return *result;
}

bool blend( const TQImage & upper, const TQImage & lower, TQImage & output)
// adopted from kimageeffect::blend - that is not endian safe and cannot handle complex alpha combinations...
{
	if
		(
		  upper.width()  > lower.width()  ||
		  upper.height() > lower.height() ||
		  upper.depth() != 32             ||
		  lower.depth() != 32
		)
		return false;
	
	output = lower.copy();
	
	uchar *i, *o;
	uint a, ab, ac;
	uint col;
	uint w = upper.width();
	int row(upper.height() - 1);
	
	do
	{
		i = upper.scanLine(row);
		o = output.scanLine(row);
		
		col = w << 2;
		
		--col;
		
		do
		{
#ifdef WORDS_BIGENDIAN
			while (!(a = i[col-3]) && (col != 3))
#else
				while (!(a = i[col]) && (col != 3))
#endif
				{
					--col; --col; --col; --col;
				}
#ifdef WORDS_BIGENDIAN
			if ((ab = o[col-3]))
#else
				if ((ab = o[col]))
#endif
				{
					ac = 65025 - (255 - a) * (255 - ab);
#ifndef WORDS_BIGENDIAN
					o[col]= (ac+127)/255;
					--col;
#endif
					o[col] = ((255 - a) * o[col] * ab + a * i[col] * 255 + 127) / ac;
					--col;
					o[col] = ((255 - a) * o[col] * ab + a * i[col] * 255 + 127) / ac;
					--col;
					o[col] = ((255 - a) * o[col] * ab + a * i[col]  * 255 + 127) / ac;
#ifdef WORDS_BIGENDIAN
				--col;
				o[col]= (ac+127)/255;
#endif
				}
			else
			{
				o[col] = i[col]; --col;
				o[col] = i[col]; --col;
				o[col] = i[col]; --col;
				o[col] = i[col];
			}
		} while (col--);
	} while (row--);
	return true;
}
