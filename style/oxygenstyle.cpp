/* Oxygen widget style for TDE
   
   Copyright (C) 2024 Glen Harpring

   eventually meant to look similar to Oxygen from KDE4
   but not (currently) using any code directly from Oxygen

   based on Plastik:

   Copyright (C) 2003 Sandro Giessl <ceebx@users.sourceforge.net>

   based on the KDE style "dotNET":

   Copyright (C) 2001-2002, Chris Lee <clee@kde.org>
                            Carsten Pfeiffer <pfeiffer@kde.org>
                            Karol Szwed <gallium@kde.org>
   Drawing routines completely reimplemented from KDE3 HighColor, which was
   originally based on some stuff from the KDE2 HighColor.

   based on drawing routines of the style "Keramik":

   Copyright (c) 2002 Malte Starostik <malte@kde.org>
             (c) 2002,2003 Maksim Orlovich <mo002j@mail.rochester.edu>
   based on the KDE3 HighColor Style
   Copyright (C) 2001-2002 Karol Szwed      <gallium@kde.org>
             (C) 2001-2002 Fredrik H�glund  <fredrik@kde.org>
   Drawing routines adapted from the KDE2 HCStyle,
   Copyright (C) 2000 Daniel M. Duley       <mosfet@kde.org>
             (C) 2000 Dirk Mueller          <mueller@kde.org>
             (C) 2001 Martijn Klingens      <klingens@kde.org>
   Progressbar code (no longer) based on TDEStyle,
   Copyright (C) 2001-2002 Karol Szwed <gallium@kde.org>

   using some routines from Domino:
   Copyright (C) 2006 Michael Lentner <michaell@gmx.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */

#include <tqimage.h>
#include <tqstylefactory.h>
#include <tqpointarray.h>
#include <tqpainter.h>
#include <tqtabbar.h>
#include <tqprogressbar.h>
#include <tqcheckbox.h>
#include <tqcombobox.h>
#include <tqcleanuphandler.h>
#include <tqheader.h>
#include <tqlineedit.h>
#include <tqlistbox.h>
#include <tqlistview.h>
#include <tqscrollbar.h>
#include <tqstyleplugin.h>
#include <tqpushbutton.h>
#include <tqtabwidget.h>
#include <tqtimer.h>
#include <tqtoolbutton.h>
#include <tqtoolbar.h>
#include <tqmenubar.h>
#include <tqpopupmenu.h>
#include <tqdrawutil.h>
#include <tqapplication.h>
#include <tqvariant.h>
#include <tqradiobutton.h>
#include <tqregion.h>
#include <tqmainwindow.h>
#include <tqslider.h>
#include <tqsettings.h>
#include <kpixmap.h>

#include <math.h>

#include "oxygenstyle.h"
#include "oxygenstyle.moc"
#include "data.h"
#include "misc.h"

// some bitmaps for the radio button so it's easier to handle the circle stuff...
// 13x13
static const unsigned char radiobutton_mask_bits[] = {
   0xf8, 0x03, 0xfc, 0x07, 0xfe, 0x0f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f,
   0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xfe, 0x0f, 0xfc, 0x07,
   0xf8, 0x03};
static const unsigned char radiobutton_contour_bits[] = {
   0xf0, 0x01, 0x0c, 0x06, 0x02, 0x08, 0x02, 0x08, 0x01, 0x10, 0x01, 0x10,
   0x01, 0x10, 0x01, 0x10, 0x01, 0x10, 0x02, 0x08, 0x02, 0x08, 0x0c, 0x06,
   0xf0, 0x01};
static const unsigned char radiobutton_aa_inside_bits[] = {
   0x00, 0x00, 0x10, 0x01, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x04, 0x10, 0x01,
   0x00, 0x00};
static const unsigned char radiobutton_aa_outside_bits[] = {
   0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x02};
static const unsigned char radiobutton_highlight1_bits[] = {
   0x00, 0x00, 0xf0, 0x01, 0x1c, 0x07, 0x04, 0x04, 0x06, 0x0c, 0x02, 0x08,
   0x02, 0x08, 0x02, 0x08, 0x06, 0x0c, 0x04, 0x04, 0x1c, 0x07, 0xf0, 0x01,
   0x00, 0x00};
static const unsigned char radiobutton_highlight2_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x18, 0x03, 0x08, 0x02, 0x04, 0x04,
   0x04, 0x04, 0x04, 0x04, 0x08, 0x02, 0x18, 0x03, 0xe0, 0x00, 0x00, 0x00,
   0x00, 0x00};
// check mark
const uint CHECKMARKSIZE = 9; // 9x9
static const unsigned char checkmark_aa_bits[] = {
   0x45, 0x01, 0x28, 0x00, 0x11, 0x01, 0x82, 0x00, 0x44, 0x00, 0x82, 0x00,
   0x11, 0x01, 0x28, 0x00, 0x45, 0x01};
static const unsigned char checkmark_dark_bits[] = {
   0x82, 0x00, 0x45, 0x01, 0xaa, 0x00, 0x54, 0x00, 0x28, 0x00, 0x74, 0x00,
   0xea, 0x00, 0xc5, 0x01, 0x82, 0x00};
static const unsigned char checkmark_light_bits[] = {
   0x00, 0xfe, 0x82, 0xfe, 0x44, 0xfe, 0x28, 0xfe, 0x10, 0xfe, 0x08, 0xfe,
   0x04, 0xfe, 0x02, 0xfe, 0x00, 0xfe};
static const unsigned char checkmark_tristate_bits[] = {
   0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0xff, 0x01,
   0x00, 0x00, 0xff, 0x01, 0x00, 0x00};
// radio mark
const uint RADIOMARKSIZE = 9; // 9x9
static const unsigned char radiomark_aa_bits[] = {
   0x00, 0x00, 0x44, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x82, 0x00, 0x44, 0x00, 0x00, 0x00};
static const unsigned char radiomark_dark_bits[] = {
   0x00, 0x00, 0x38, 0x00, 0x44, 0x00, 0xf2, 0x00, 0xfa, 0x00, 0xfa, 0x00,
   0x7c, 0x00, 0x38, 0x00, 0x00, 0x00};
static const unsigned char radiomark_light_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x04, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// popupmenu item constants...
static const int itemHMargin = 6;
static const int itemFrame = 2;
static const int arrowHMargin = 6;
static const int rightBorder = 12;

// -- Style Plugin Interface -------------------------
class OxygenStylePlugin : public TQStylePlugin
{
    public:
        OxygenStylePlugin() {}
        ~OxygenStylePlugin() {}

        TQStringList keys() const {
            return TQStringList() << "Oxygen";
        }

        TQStyle* create( const TQString& key ) {
            if (key.lower() == "oxygen")
                return new OxygenStyle;
            return 0;
        }
};

TDE_EXPORT_PLUGIN( OxygenStylePlugin )
// -- end --

OxygenStyle::OxygenStyle() : TDEStyle( AllowMenuTransparency, ThreeButtonScrollBar),
    kickerMode(false),
    kornMode(false),
    flatMode(false)
{
    horizontalLine = 0;
    verticalLine = 0;

    TQSettings settings;
    _contrast = settings.readNumEntry("/Qt/KDE/contrast", 6);
    settings.beginGroup("/oxygenstyle/Settings");
    _scrollBarLines = settings.readBoolEntry("/scrollBarLines", false);
    _animateProgressBar = settings.readBoolEntry("/animateProgressBar", true);
    _drawToolBarSeparator = settings.readBoolEntry("/drawToolBarSeparator", false);
    _drawToolBarItemSeparator = settings.readBoolEntry("/drawToolBarItemSeparator", true);
    _drawFocusRect = settings.readBoolEntry("/drawFocusRect", true);
    _drawTriangularExpander = settings.readBoolEntry("/drawTriangularExpander", false);
    _inputFocusHighlight = settings.readBoolEntry("/inputFocusHighlight", true);
    _customOverHighlightColor = settings.readBoolEntry("/customOverHighlightColor", false);
    _overHighlightColor.setNamedColor( settings.readEntry("/overHighlightColor", "black") );
    _customFocusHighlightColor = settings.readBoolEntry("/customFocusHighlightColor", false);
    _focusHighlightColor.setNamedColor( settings.readEntry("/focusHighlightColor", "black") );
    _customCheckMarkColor = settings.readBoolEntry("/customCheckMarkColor", false);
    _checkMarkColor.setNamedColor( settings.readEntry("/checkMarkColor", "black") );
    settings.endGroup();

    // setup pixmap cache...
    pixmapCache = new TQIntCache<CacheEntry>(150000, 499);
    pixmapCache->setAutoDelete(true);

    if ( _animateProgressBar )
    {
        animationTimer = new TQTimer( this );
        connect( animationTimer, TQ_SIGNAL(timeout()), this, TQ_SLOT(updateProgressPos()) );
    }
}


void OxygenStyle::updateProgressPos()
{
    TQProgressBar* pb;
    //Update the registered progressbars.
    TQMap<TQWidget*, int>::iterator iter;
    bool visible = false;
    for (iter = progAnimWidgets.begin(); iter != progAnimWidgets.end(); ++iter)
    {   
        if ( !::tqt_cast<TQProgressBar*>(iter.key()) )
            continue;
        
        pb = dynamic_cast<TQProgressBar*>(iter.key());
        if( iter.key() -> isEnabled() )
        {
            // update animation Offset of the current Widget
            //iter.data() = (iter.data() + 1) % 20;
            
            int prog = 100 * pb->progress() / pb->totalSteps();
            
            if( iter.data() != prog )
            {
                if( iter.data() < prog )
                {
                    iter.data() += 4;
                    if( iter.data() > prog ) iter.data() = prog;
                }
                else
                {
                    iter.data() -= 4;
                    if( iter.data() < prog ) iter.data() = prog;
                }
                
                iter.key()->update();
            }
        }
        if (iter.key()->isVisible())
            visible = true;
    }
    if (!visible)
        animationTimer->stop();
}


OxygenStyle::~OxygenStyle()
{
    delete pixmapCache;
    delete horizontalLine;
    delete verticalLine;
}

#define CHECKBGPIXMAP(a) ( bgWidgets[(a)] && bgWidgets[(a)] == (a)->backgroundPixmap() )

void OxygenStyle::applicationPolish(const TQStyleControlElementData &ceData, ControlElementFlags, void *ptr)
{
    if (ceData.widgetObjectTypes.contains("TQApplication")) {
	TQApplication *app = reinterpret_cast<TQApplication*>(ptr);

	if (!qstrcmp(app->argv()[0], "kicker"))
		kickerMode = true;
	else if (!qstrcmp(app->argv()[0], "korn"))
		kornMode = true;
    }
}

void OxygenStyle::polish(const TQStyleControlElementData &ceData, ControlElementFlags elementFlags, void *ptr)
{
    if (ceData.widgetObjectTypes.contains("TQWidget")) {
	TQWidget *widget = reinterpret_cast<TQWidget*>(ptr);

	if( !strcmp(widget->name(), "__tdehtml") ) { // is it a tdehtml widget...?
		tdehtmlWidgets[widget] = true;
		connect(widget, TQ_SIGNAL(destroyed(TQObject*)), this, TQ_SLOT(tdehtmlWidgetDestroyed(TQObject*)));
	}
	
	// use tqt_cast where possible to check if the widget inheits one of the classes. might improve
	// performance compared to TQObject::inherits()
	if ( ::tqt_cast<TQPushButton*>(widget) || ::tqt_cast<TQComboBox*>(widget) ||
		::tqt_cast<TQSpinWidget*>(widget) || ::tqt_cast<TQSlider*>(widget) ||
		::tqt_cast<TQCheckBox*>(widget) || ::tqt_cast<TQRadioButton*>(widget) ||
		::tqt_cast<TQToolButton*>(widget) || widget->inherits("TQSplitterHandle") )
	{
	//         widget->setBackgroundMode(PaletteBackground);
		installObjectEventHandler(ceData, elementFlags, ptr, this);
	} else if (::tqt_cast<TQLineEdit*>(widget)) {
		installObjectEventHandler(ceData, elementFlags, ptr, this);
	} else if (::tqt_cast<TQTabBar*>(widget)) {
		widget->setMouseTracking(true);
		installObjectEventHandler(ceData, elementFlags, ptr, this);
	} else if (::tqt_cast<TQPopupMenu*>(widget)) {
		widget->setBackgroundMode( NoBackground );
	}
    else if( ::tqt_cast<TQListView*>(widget) )
    {
        /*
        widget->setErasePixmap(*result);*/
        
        installObjectEventHandler(ceData, elementFlags, ptr, this);
        
    }
    else if ( 
        !kickerMode && widget->isTopLevel()
        && ( widget->isDialog() || ::tqt_cast<TQMainWindow*>(widget) || widget->isModal() )
        && !widget->backgroundPixmap()
    )
    {
        TQImage aImg(widget->rect().width(),widget->rect().height(),32);
        TQPixmap * result = new TQPixmap(aImg);
        TQPainter p(result);
        p.fillRect(widget->rect(), widget->colorGroup().background());
        renderWindowBackground( &p, widget->rect(), widget->colorGroup() );
        
        bgWidgets[widget] = result;
        
        // apparently this function is deprecated,
        // but the suggested replacement has the wrong behavior
        // we need to only add this gradient to the top level widget
        // (and then maybe pull from it later)
        widget->setBackgroundPixmap(*result);
        //widget->setErasePixmap(*result);
        
        // we'll update it on resize
        installObjectEventHandler(ceData, elementFlags, ptr, this);
    } else if ( !qstrcmp(widget->name(), "tde toolbar widget") ) {
		installObjectEventHandler(ceData, elementFlags, ptr, this);
	}
    
    /*{
        
    }*/
    /*
    if( bgWidgets[widget->topLevelWidget()] )
    {
        TQImage aImg(widget->rect().width(),widget->rect().height(),32);
        TQImage bImg(bgWidgets[widget->topLevelWidget()]->convertToImage());
        
        bitBlt(&aImg, 0, 0, 
            &bImg, 
            widget->geometry().x(), widget->geometry().y(),
            widget->rect().width(), widget->rect().height()
        );
        
        TQPixmap * result = new TQPixmap(aImg);
        widget->setErasePixmap(*result);
    }
    // ok we're not doing it this way, not gonna work
    */
	
	if( _animateProgressBar && ::tqt_cast<TQProgressBar*>(widget) )
	{
		installObjectEventHandler(ceData, elementFlags, ptr, this);
		progAnimWidgets[widget] = 0;
		connect(widget, TQ_SIGNAL(destroyed(TQObject*)), this, TQ_SLOT(progressBarDestroyed(TQObject*)));
		if (!animationTimer->isActive())
		animationTimer->start( 50, false );
	}
    }

    TDEStyle::polish(ceData, elementFlags, ptr);
}

void OxygenStyle::unPolish(const TQStyleControlElementData &ceData, ControlElementFlags elementFlags, void *ptr)
{
    if (ceData.widgetObjectTypes.contains("TQWidget")) {
	TQWidget *widget = reinterpret_cast<TQWidget*>(ptr);

	if( !strcmp(widget->name(), "__tdehtml") ) { // is it a tdehtml widget...?
		tdehtmlWidgets.remove(widget);
	}
	
	// use tqt_cast to check if the widget inheits one of the classes.
	if ( ::tqt_cast<TQPushButton*>(widget) || ::tqt_cast<TQComboBox*>(widget) ||
		::tqt_cast<TQSpinWidget*>(widget) || ::tqt_cast<TQSlider*>(widget) ||
		::tqt_cast<TQCheckBox*>(widget) || ::tqt_cast<TQRadioButton*>(widget) ||
		::tqt_cast<TQToolButton*>(widget) || ::tqt_cast<TQLineEdit*>(widget) ||
		widget->inherits("TQSplitterHandle") )
	{
		removeObjectEventHandler(ceData, elementFlags, ptr, this);
	}
	else if (::tqt_cast<TQTabBar*>(widget)) {
		widget->setMouseTracking(false);
		removeObjectEventHandler(ceData, elementFlags, ptr, this);
	} else if (::tqt_cast<TQPopupMenu*>(widget)) {
		widget->setBackgroundMode( PaletteBackground );
	} else if ( !qstrcmp(widget->name(), "tde toolbar widget") ) {
		removeObjectEventHandler(ceData, elementFlags, ptr, this);
	}
    else if( CHECKBGPIXMAP(widget) )
    {
        widget->setBackgroundPixmap(0L);
        removeObjectEventHandler(ceData, elementFlags, ptr, this);
    }
    // TODO
	
	if ( ::tqt_cast<TQProgressBar*>(widget) )
	{
		progAnimWidgets.remove(widget);
	}
    }

    TDEStyle::unPolish(ceData, elementFlags, ptr);
}

void OxygenStyle::tdehtmlWidgetDestroyed(TQObject* obj)
{
    tdehtmlWidgets.remove(static_cast<TQWidget*>(obj));
}

void OxygenStyle::progressBarDestroyed(TQObject* obj)
{
    progAnimWidgets.remove(static_cast<TQWidget*>(obj));
}

void OxygenStyle::renderContour(TQPainter *p,
                                const TQRect &r,
                                const TQColor &backgroundColor,
                                const TQColor &contour,
                                const uint flags) const
{
    if((r.width() <= 0)||(r.height() <= 0))
        return;

    const bool drawLeft = flags&Draw_Left;
    const bool drawRight = flags&Draw_Right;
    const bool drawTop = flags&Draw_Top;
    const bool drawBottom = flags&Draw_Bottom;
    const bool disabled = flags&Is_Disabled;
    const bool alphaBlend = flags&Draw_AlphaBlend;

    TQColor contourColor;
    if (disabled) {
        contourColor = backgroundColor.dark(150);
    } else {
        contourColor = contour;
    }

// sides
    p->setPen( alphaBlendColors(backgroundColor, contourColor, 50) );
    if(drawLeft)
        p->drawLine(r.left(), drawTop?r.top()+2:r.top(), r.left(), drawBottom?r.bottom()-2:r.bottom());
    if(drawRight)
        p->drawLine(r.right(), drawTop?r.top()+2:r.top(), r.right(), drawBottom?r.bottom()-2:r.bottom());
    if(drawTop)
        p->drawLine(drawLeft?r.left()+2:r.left(), r.top(), drawRight?r.right()-2:r.right(), r.top());
    if(drawBottom)
        p->drawLine(drawLeft?r.left()+2:r.left(), r.bottom(), drawRight?r.right()-2:r.right(), r.bottom());

// edges
    const int alphaAA = 110; // the alpha value for anti-aliasing...

    // first part...
    p->setPen(alphaBlendColors(backgroundColor, contourColor, 50) );
    if(drawLeft && drawTop) {
        switch(flags&Round_UpperLeft) {
            case false:
                p->drawPoint(r.left()+1, r.top());
                p->drawPoint(r.left(), r.top()+1);
                break;
            default:
                p->drawPoint(r.left()+1, r.top()+1);
        }
    }
    if(drawLeft && drawBottom) {
        switch(flags&Round_BottomLeft) {
            case false:
                p->drawPoint(r.left()+1, r.bottom());
                p->drawPoint(r.left(), r.bottom()-1);
                break;
            default:
                p->drawPoint(r.left()+1, r.bottom()-1);
        }
    }
    if(drawRight && drawTop) {
        switch(flags&Round_UpperRight) {
            case false:
                p->drawPoint(r.right()-1, r.top());
                p->drawPoint(r.right(), r.top()+1);
                break;
            default:
                p->drawPoint(r.right()-1, r.top()+1);
        }
    }
    if(drawRight && drawBottom) {
        switch(flags&Round_BottomRight) {
            case false:
                p->drawPoint(r.right()-1, r.bottom());
                p->drawPoint(r.right(), r.bottom()-1);
                break;
            default:
                p->drawPoint(r.right()-1, r.bottom()-1);
        }
    }

    // second part... fill edges in case we don't paint alpha-blended
    p->setPen( backgroundColor );
    if (!alphaBlend) {
        if(flags&Round_UpperLeft && drawLeft && drawTop) {
            p->drawPoint( r.x(), r.y() );
        }
        if(flags&Round_BottomLeft && drawLeft && drawBottom) {
            p->drawPoint( r.x(), r.bottom() );
        }
        if(flags&Round_UpperRight && drawRight && drawTop) {
            p->drawPoint( r.right(), r.y() );
        }
        if(flags&Round_BottomRight && drawRight && drawBottom) {
            p->drawPoint( r.right(), r.bottom() );
        }
    }

    // third part... anti-aliasing...
    if(drawLeft && drawTop) {
        switch(flags&Round_UpperLeft) {
            case false:
                renderPixel(p,TQPoint(r.left(),r.top()),alphaAA,contourColor,backgroundColor,alphaBlend);
                break;
            default:
                renderPixel(p,TQPoint(r.left()+1,r.top()),alphaAA,contourColor,backgroundColor,alphaBlend);
                renderPixel(p,TQPoint(r.left(),r.top()+1),alphaAA,contourColor,backgroundColor,alphaBlend);
        }
    }
    if(drawLeft && drawBottom) {
        switch(flags&Round_BottomLeft) {
            case false:
                renderPixel(p,TQPoint(r.left(),r.bottom()),alphaAA,contourColor,backgroundColor,alphaBlend);
                break;
            default:
                renderPixel(p,TQPoint(r.left()+1,r.bottom()),alphaAA,contourColor,backgroundColor,alphaBlend);
                renderPixel(p,TQPoint(r.left(),r.bottom()-1),alphaAA,contourColor,backgroundColor,alphaBlend);
        }
    }
    if(drawRight && drawTop) {
        switch(flags&Round_UpperRight) {
            case false:
                renderPixel(p,TQPoint(r.right(),r.top()),alphaAA,contourColor,backgroundColor,alphaBlend);
                break;
            default:
                renderPixel(p,TQPoint(r.right()-1,r.top()),alphaAA,contourColor,backgroundColor,alphaBlend);
                renderPixel(p,TQPoint(r.right(),r.top()+1),alphaAA,contourColor,backgroundColor,alphaBlend);
        }
    }
    if(drawRight && drawBottom) {
        switch(flags&Round_BottomRight) {
            case false:
                renderPixel(p,TQPoint(r.right(),r.bottom()),alphaAA,contourColor,backgroundColor,alphaBlend);
                break;
            default:
                renderPixel(p,TQPoint(r.right()-1,r.bottom()),alphaAA,contourColor,backgroundColor,alphaBlend);
                renderPixel(p,TQPoint(r.right(),r.bottom()-1),alphaAA,contourColor,backgroundColor,alphaBlend);
        }
    }

}

void OxygenStyle::renderMask(TQPainter *p,
                              const TQRect &r,
                              const TQColor &color,
                              const uint flags) const
{
    if((r.width() <= 0)||(r.height() <= 0))
        return;

    const bool roundUpperLeft = flags&Round_UpperLeft;
    const bool roundUpperRight = flags&Round_UpperRight;
    const bool roundBottomLeft = flags&Round_BottomLeft;
    const bool roundBottomRight = flags&Round_BottomRight;


    p->fillRect (TQRect(r.x()+1, r.y()+1, r.width()-2, r.height()-2) , color);

    p->setPen(color);
    // sides
    p->drawLine(roundUpperLeft?r.x()+1:r.x(), r.y(),
                roundUpperRight?r.right()-1:r.right(), r.y() );
    p->drawLine(roundBottomLeft?r.x()+1:r.x(), r.bottom(),
                roundBottomRight?r.right()-1:r.right(), r.bottom() );
    p->drawLine(r.x(), roundUpperLeft?r.y()+1:r.y(),
                r.x(), roundBottomLeft?r.bottom()-1:r.bottom() );
    p->drawLine(r.right(), roundUpperLeft?r.y()+1:r.y(),
                r.right(), roundBottomLeft?r.bottom()-1:r.bottom() );
}

void OxygenStyle::renderSurface(TQPainter *p,
                                 const TQRect &r,
                                 const TQColor &backgroundColor,
                                 const TQColor &buttonColor,
                                 const TQColor &highlightColor,
                                 int intensity,
                                 const uint flags) const
{
    if((r.width() <= 0)||(r.height() <= 0))
        return;

    const bool disabled = flags&Is_Disabled;

    const bool drawLeft = flags&Draw_Left;
    const bool drawRight = flags&Draw_Right;
    const bool drawTop = flags&Draw_Top;
    const bool drawBottom = flags&Draw_Bottom;
    const bool roundUpperLeft = flags&Round_UpperLeft;
    const bool roundUpperRight = flags&Round_UpperRight;
    const bool roundBottomLeft = flags&Round_BottomLeft;
    const bool roundBottomRight = flags&Round_BottomRight;
    const bool sunken = flags&Is_Sunken;
    const bool horizontal = flags&Is_Horizontal;
    bool highlight = false,
        highlightLeft = false,
        highlightRight = false,
        highlightTop = false,
        highlightBottom = false;
    // only highlight if not sunken & not disabled...
    if(!sunken && !disabled) {
        highlight = (flags&Is_Highlight);
        highlightLeft = (flags&Highlight_Left);
        highlightRight = (flags&Highlight_Right);
        highlightTop = (flags&Highlight_Top);
        highlightBottom = (flags&Highlight_Bottom);
    }

    TQColor baseColor = alphaBlendColors(backgroundColor, disabled?backgroundColor:buttonColor, 10);
    if (disabled) {
        intensity = 2;
    } else if (highlight) {
        // blend this _slightly_ with the background
        baseColor = alphaBlendColors(baseColor, highlightColor, 240);
    } else if (sunken) {
        // enforce a common sunken-style...
        baseColor = baseColor.dark(110+intensity);
        intensity = _contrast/2;
    }
// some often needed colors...
    // 1 more intensive than 2 and 3.
    const TQColor colorTop1 = alphaBlendColors(baseColor,
                    sunken?baseColor.dark(100+intensity*2):baseColor.light(100+intensity*2), 80);
    const TQColor colorTop2 = alphaBlendColors(baseColor,
                    sunken?baseColor.dark(100+intensity):baseColor.light(100+intensity), 80);
    const TQColor colorBottom1 = alphaBlendColors(baseColor,
                        sunken?baseColor.light(100+intensity*2):baseColor.dark(100+intensity*2), 80);
    const TQColor colorBottom2 = alphaBlendColors(baseColor,
                        sunken?baseColor.light(100+intensity):baseColor.dark(100+intensity), 80);

// sides
    if (drawLeft) {
        if (horizontal) {
            int height = r.height();
            if (roundUpperLeft || !drawTop) height--;
            if (roundBottomLeft || !drawBottom) height--;
            renderGradient(p, TQRect(r.left(), (roundUpperLeft&&drawTop)?r.top()+1:r.top(), 1, height),
                            colorTop1, baseColor);
        } else {
            p->setPen(colorTop1 );
            p->drawLine(r.left(), (roundUpperLeft&&drawTop)?r.top()+1:r.top(),
                        r.left(), (roundBottomLeft&&drawBottom)?r.bottom()-1:r.bottom() );
        }
    }
    if (drawRight) {
        if (horizontal) {
            int height = r.height();
            // TODO: there's still a bogus in it: when edge4 is Thick
            //       and we don't whant to draw the Top, we have a unpainted area
            if (roundUpperRight || !drawTop) height--;
            if (roundBottomRight || !drawBottom) height--;
            renderGradient(p, TQRect(r.right(), (roundUpperRight&&drawTop)?r.top()+1:r.top(), 1, height),
                            baseColor, colorBottom1);
        } else {
            p->setPen(colorBottom1 );
            p->drawLine(r.right(), (roundUpperRight&&drawTop)?r.top()+1:r.top(),
                        r.right(), (roundBottomRight&&drawBottom)?r.bottom()-1:r.bottom() );
        }
    }
    if (drawTop) {
        if (horizontal) {
            p->setPen(colorTop1 );
            p->drawLine((roundUpperLeft&&drawLeft)?r.left()+1:r.left(), r.top(),
                        (roundUpperRight&&drawRight)?r.right()-1:r.right(), r.top() );
        } else {
            int width = r.width();
            if (roundUpperLeft || !drawLeft) width--;
            if (roundUpperRight || !drawRight) width--;
            renderGradient(p, TQRect((roundUpperLeft&&drawLeft)?r.left()+1:r.left(), r.top(), width, 1),
                            colorTop1, colorTop2);
        }
    }
    if (drawBottom) {
        if (horizontal) {
            p->setPen(colorBottom1 );
            p->drawLine((roundBottomLeft&&drawLeft)?r.left()+1:r.left(), r.bottom(),
                        (roundBottomRight&&drawRight)?r.right()-1:r.right(), r.bottom() );
        } else {
            int width = r.width();
            if (roundBottomLeft || !drawLeft) width--;
            if (roundBottomRight || !drawRight) width--;
            renderGradient(p, TQRect((roundBottomLeft&&drawLeft)?r.left()+1:r.left(), r.bottom(), width, 1),
                            colorBottom2, colorBottom1);
        }
    }

// button area...
    int width = r.width();
    int height = r.height();
    if (drawLeft) width--;
    if (drawRight) width--;
    if (drawTop) height--;
    if (drawBottom) height--;
    renderGradient(p, TQRect(drawLeft?r.left()+1:r.left(), drawTop?r.top()+1:r.top(), width, height),
                    colorTop2, colorBottom2, horizontal);


// highlighting...
    if(highlightTop) {
        p->setPen(alphaBlendColors(colorTop1 , highlightColor, 80) );
        p->drawLine((roundUpperLeft&&drawLeft)?r.left()+1:r.left(), r.top(),
                    (roundUpperRight&&drawRight)?r.right()-1:r.right(), r.top() );
        p->setPen(alphaBlendColors(colorTop2 , highlightColor, 150) );
        p->drawLine(highlightLeft?r.left()+1:r.left(), r.top()+1,
                    highlightRight?r.right()-1:r.right(), r.top()+1 );
    }
    if(highlightBottom) {
        p->setPen(alphaBlendColors(colorBottom1 , highlightColor, 80) );
        p->drawLine((roundBottomLeft&&drawLeft)?r.left()+1:r.left(), r.bottom(),
                    (roundBottomRight&&drawRight)?r.right()-1:r.right(), r.bottom() );
        p->setPen(alphaBlendColors(colorBottom2 , highlightColor, 150) );
        p->drawLine(highlightLeft?r.left()+1:r.left(), r.bottom()-1,
                    highlightRight?r.right()-1:r.right(), r.bottom()-1 );
    }
    if(highlightLeft) {
        p->setPen(alphaBlendColors(colorTop1 , highlightColor, 80) );
        p->drawLine(r.left(), (roundUpperLeft&&drawTop)?r.top()+1:r.top(),
                    r.left(), (roundBottomLeft&&drawBottom)?r.bottom()-1:r.bottom() );
        p->setPen(alphaBlendColors(colorTop2 , highlightColor, 150) );
        p->drawLine(r.left()+1, highlightTop?r.top()+1:r.top(),
                    r.left()+1, highlightBottom?r.bottom()-1:r.bottom() );
    }
    if(highlightRight) {
        p->setPen(alphaBlendColors(colorBottom1 , highlightColor, 80) );
        p->drawLine(r.right(), (roundUpperRight&&drawTop)?r.top()+1:r.top(),
                    r.right(), (roundBottomRight&&drawBottom)?r.bottom()-1:r.bottom() );
        p->setPen(alphaBlendColors(colorBottom2 , highlightColor, 150) );
        p->drawLine(r.right()-1, highlightTop?r.top()+1:r.top(),
                    r.right()-1, highlightBottom?r.bottom()-1:r.bottom() );
    }
}

void OxygenStyle::renderPixel(TQPainter *p,
            const TQPoint &pos,
            const int alpha,
            const TQColor &color,
            const TQColor &background,
            bool fullAlphaBlend) const
{    
    if(fullAlphaBlend)
    // full alpha blend: paint into an image with alpha buffer and convert to a pixmap ...
    {
        TQRgb rgb = color.rgb();
        // generate a quite unique key -- use the unused width field to store the alpha value.
        CacheEntry search(cAlphaDot, alpha, 0, rgb);
        int key = search.key();

        CacheEntry *cacheEntry;
        if( (cacheEntry = pixmapCache->find(key)) ) {
            if( search == *cacheEntry ) { // match! we can draw now...
                if(cacheEntry->pixmap)
                    p->drawPixmap(pos, *(cacheEntry->pixmap) );
                return;
            } else { //Remove old entry in case of a conflict!
                pixmapCache->remove( key );
            }
        }


        TQImage aImg(1,1,32); // 1x1
        aImg.setAlphaBuffer(true);
        aImg.setPixel(0,0,tqRgba(tqRed(rgb),tqGreen(rgb),tqBlue(rgb),alpha));
        TQPixmap *result = new TQPixmap(aImg);

        p->drawPixmap(pos, *result);

        // add to the cache...
        CacheEntry *toAdd = new CacheEntry(search);
        toAdd->pixmap = result;
        bool insertOk = pixmapCache->insert( key, toAdd, result->depth()/8);
        if(!insertOk)
            delete result;
    } else
    // don't use an alpha buffer: calculate the resulting color from the alpha value, the fg- and the bg-color.
    {
        TQRgb rgb_a = color.rgb();
        TQRgb rgb_b = background.rgb();
        int a = alpha;
        if(a>255) a = 255;
        if(a<0) a = 0;
        int a_inv = 255 - a;

        TQColor res  = TQColor( tqRgb(tqRed(rgb_b)*a_inv/255 + tqRed(rgb_a)*a/255,
                              tqGreen(rgb_b)*a_inv/255 + tqGreen(rgb_a)*a/255,
                              tqBlue(rgb_b)*a_inv/255 + tqBlue(rgb_a)*a/255) );
        p->setPen(res);
        p->drawPoint(pos);
    } 
}

void OxygenStyle::renderButton(TQPainter *p,
                               const TQRect &r,
                               const TQColorGroup &g,
                               bool sunken,
                               bool mouseOver,
                               bool horizontal,
                               bool enabled,
                               bool tdehtmlMode) const
{
    // small fix for the kicker buttons...
    if(kickerMode) enabled = true;

    const TQPen oldPen( p->pen() );

    uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
    if(!enabled) contourFlags|=Is_Disabled;
    if(tdehtmlMode) contourFlags|=Draw_AlphaBlend;

    uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
    if(horizontal) surfaceFlags|=Is_Horizontal;
    if(!enabled) surfaceFlags|=Is_Disabled;
    else {
        if(sunken) surfaceFlags|=Is_Sunken;
        else {
            if(mouseOver) {
                surfaceFlags|=Is_Highlight;
                if(horizontal) {
                    surfaceFlags|=Highlight_Top;
                    surfaceFlags|=Highlight_Bottom;
                } else {
                    surfaceFlags|=Highlight_Left;
                    surfaceFlags|=Highlight_Right;
                }
            }
        }
    }

    if (!flatMode) {
        contourFlags |= Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight;
        surfaceFlags |= Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight;

        renderContour(p, TQRect(r.left(), r.top(), r.width(), r.height()-1), g.background(), getColor(g,ButtonContour),
                    contourFlags);
        renderSurface(p, TQRect(r.left()+1, r.top()+1, r.width()-2, r.height()-3),
                    g.background(), g.button(), getColor(g,MouseOverHighlight), _contrast, surfaceFlags);

        
        if(!kickerMode)
        {
            TQColor c1(g.background().dark(120));
            
            p->setBrush(TQt::NoBrush);
            p->setPen(c1);
            //p->drawArc(TQRect(r.width()-6, r.height()-6, 3, 3), 225*16, 180*16);
            p->drawLine(r.x()+2, r.height()-1, r.width()-4, r.height()-1);

        }

    } else {
        renderContour(p, r, g.background(), g.button().dark(105+_contrast*3),
                      contourFlags);
        renderSurface(p, TQRect(r.left()+1, r.top()+1, r.width()-2, r.height()-2),
                      g.background(), g.button(), getColor(g,MouseOverHighlight), _contrast/2, surfaceFlags);

        flatMode = false;
    }

    p->setPen(oldPen);
}

void OxygenStyle::renderDot(TQPainter *p,
                             const TQPoint &point,
                             const TQColor &baseColor,
                             const bool thick,
                             const bool sunken) const
{
    const TQColor topColor = alphaBlendColors(baseColor, sunken?baseColor.dark(130):baseColor.light(150), 70);
    const TQColor bottomColor = alphaBlendColors(baseColor, sunken?baseColor.light(150):baseColor.dark(130), 70);
    p->setPen(topColor );
    p->drawLine(point.x(), point.y(), point.x()+1, point.y());
    p->drawPoint(point.x(), point.y()+1);
    p->setPen(bottomColor );
    if(thick) {
        p->drawLine(point.x()+1, point.y()+2, point.x()+2, point.y()+2);
        p->drawPoint(point.x()+2, point.y()+1);
    } else {
        p->drawPoint(point.x()+1, point.y()+1);
    }
}

void OxygenStyle::renderGradient(TQPainter *painter,
                                  const TQRect &rect,
                                  const TQColor &c1,
                                  const TQColor &c2,
                                  bool horizontal) const
{
    if((rect.width() <= 0)||(rect.height() <= 0))
        return;

    // generate a quite unique key for this surface.
    CacheEntry search(cGradientTile,
                      horizontal ? 0 : rect.width(),
                      horizontal ? rect.height() : 0,
                      c1.rgb(), c2.rgb(), horizontal );
    int key = search.key();

    CacheEntry *cacheEntry;
    if( (cacheEntry = pixmapCache->find(key)) ) {
        if( search == *cacheEntry ) { // match! we can draw now...
            if(cacheEntry->pixmap) {
                painter->drawTiledPixmap(rect, *(cacheEntry->pixmap) );
            }
            return;
        } else {
            // Remove old entry in case of a conflict!
            // This shouldn't happen very often, see comment in CacheEntry.
            pixmapCache->remove(key);
        }
    }

    // there wasn't anything matching in the cache, create the pixmap now...
    TQPixmap *result = new TQPixmap(horizontal ? 10 : rect.width(),
                                  horizontal ? rect.height() : 10);
    TQPainter p(result);

    int r_w = result->rect().width();
    int r_h = result->rect().height();
    int r_x, r_y, r_x2, r_y2;
    result->rect().coords(&r_x, &r_y, &r_x2, &r_y2);

    int rDiff, gDiff, bDiff;
    int rc, gc, bc;

    int x, y;

    rDiff = ( c2.red())   - (rc = c1.red());
    gDiff = ( c2.green()) - (gc = c1.green());
    bDiff = ( c2.blue())  - (bc = c1.blue());

    int rl = rc << 16;
    int gl = gc << 16;
    int bl = bc << 16;

    int rdelta = ((1<<16) / (horizontal ? r_h : r_w)) * rDiff;
    int gdelta = ((1<<16) / (horizontal ? r_h : r_w)) * gDiff;
    int bdelta = ((1<<16) / (horizontal ? r_h : r_w)) * bDiff;

    // these for-loops could be merged, but the if's in the inner loop
    // would make it slow
    if(horizontal) {
        for ( y = 0; y < r_h; y++ ) {
            rl += rdelta;
            gl += gdelta;
            bl += bdelta;

            p.setPen(TQColor(rl>>16, gl>>16, bl>>16));
            p.drawLine(r_x, r_y+y, r_x2, r_y+y);
        }
    } else {
        for( x = 0; x < r_w; x++) {
            rl += rdelta;
            gl += gdelta;
            bl += bdelta;

            p.setPen(TQColor(rl>>16, gl>>16, bl>>16));
            p.drawLine(r_x+x, r_y, r_x+x, r_y2);
        }
    }

    p.end();

    // draw the result...
    painter->drawTiledPixmap(rect, *result);

    // insert into cache using the previously created key.
    CacheEntry *toAdd = new CacheEntry(search);
    toAdd->pixmap = result;
    bool insertOk = pixmapCache->insert( key, toAdd, result->width()*result->height()*result->depth()/8 );

    if(!insertOk)
        delete result;
}

void OxygenStyle::renderWindowBackground( TQPainter * p, const TQRect &r, const TQColorGroup &cg ) const
{
    renderWindowBackground(p,r,r,cg);
}

void OxygenStyle::renderWindowBackground( TQPainter * p, const TQRect &r, const TQRect &w_r, const TQColorGroup &cg, int fx, int fy ) const
{
    
    int w_w = w_r.width();
    int w_h = w_r.height();
    
    int r_w = r.width();
    int r_h = r.height();
    int r_x, r_y, r_x2, r_y2;
    r.coords(&r_x, &r_y, &r_x2, &r_y2);

    int rDiff, gDiff, bDiff;
    int rc, gc, bc;

    int x, y;
    
    // TODO: fix these values
    //TQColor c1(cg.background());
    //TQColor c2(cg.background().light(117));
    TQColor c1(cg.background().dark(94));
    TQColor c2(255,255,255); // FIXME: probably also calculated?
    //TQColor c3(cg.background().dark(112));
    TQColor c3(193,187,184); // FIXME: calculate color
    
    bool horizontal = true;

    rDiff = ( c2.red())   - (rc = c1.red());
    gDiff = ( c2.green()) - (gc = c1.green());
    bDiff = ( c2.blue())  - (bc = c1.blue());

    /*int rl = rc << 16;
    int gl = gc << 16;
    int bl = bc << 16;

    int rdelta = ((1<<16) / (horizontal ? r_h : r_w)) * rDiff;
    int gdelta = ((1<<16) / (horizontal ? r_h : r_w)) * gDiff;
    int bdelta = ((1<<16) / (horizontal ? r_h : r_w)) * bDiff;

*/

    int rDiff2, gDiff2, bDiff2;
    rDiff2 = ( c3.red())   - c1.red();
    gDiff2 = ( c3.green()) - c1.green();
    bDiff2 = ( c3.blue())  - c1.blue();

    int hw = r_w/2;
    int hh = r_h/2;
    
    float dd = 0;
    float ddelta = 1.f / hw;
    /*for( x = 0; x < hw; x++)
    {
        dd += ddelta;

        for ( y = r_h; y > 0; --y )
        {
            rl += rdelta*dd;
            gl += gdelta*dd;
            bl += bdelta*dd;
            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
        }
        
        rl = rc << 16;
        gl = gc << 16;
        bl = bc << 16;
    }
    
    for( ; x < r_w; x++)
    {
        dd -= ddelta;

        for ( y = r_h; y > 0; --y )
        {
            rl += rdelta*dd;
            gl += gdelta*dd;
            bl += bdelta*dd;
            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
        }
        
        rl = rc << 16;
        gl = gc << 16;
        bl = bc << 16;
    }*/
    
    // origin of the radial gradient
    int cx = w_w/2, cy = -72;
    int cy2 = std::min(280, (w_h-50));
    
    for( y = r_h; y >= 0; --y )
    {
        if( y < cy2 )
        {
            // simple vertical gradient
            float s = cy2 - y;
            
            rc = c3.red()   - roundf( rDiff2 * s / cy2 );
            gc = c3.green() - roundf( gDiff2 * s / cy2 );
            bc = c3.blue()  - roundf( bDiff2 * s / cy2 );
            
            p->setPen(TQColor(rc, gc, bc));
            p->drawLine(r_x, r_y+y, r_x+r_w, r_y+y);
        }
        else
        {
            // the rest of the window
            // TODO: could just be a rect
            p->setPen(c3);
            p->drawLine(r_x, r_y+y, r_x+r_w, r_y+y);
        }
        
        if( (fy+y) < 100 )
        {
            // it was not so hard to write radial gradient code
            // after all!!!
            // I was previously imagining some ridiculous sin/cos loops
            // but the solution was much simpler and more deterministic
            
            for( x = r_w; x >= 0; --x)
            {
                // with vertical scaling
                // to try to match real Oxygen
                int xs = (fx+x) - cx, ys = (((fy+y)+20)*3.5) - cy;
                float s = sqrtf( xs*xs + ys*ys );
                
                // closest value I've found
                #define SIZE 308
                if( s < SIZE )
                {
                    //TQColor c3(c1);
                    
                    rc = c2.red();
                    gc = c2.green();
                    bc = c2.blue();
                    
                    rc -= roundf( rDiff * s / ((float)SIZE) );
                    gc -= roundf( gDiff * s / ((float)SIZE) );
                    bc -= roundf( bDiff * s / ((float)SIZE) );
                    
                    
                    // TODO: blend with other vertical gradient
                    /*float s2 = cy2 - y;
                    rc = c3.red()   - roundf( rDiff2 * s2 / cy2 ) - roundf( rDiff * s / 308.f );
                    gc = c3.green() - roundf( gDiff2 * s2 / cy2 ) - roundf( gDiff * s / 308.f );
                    bc = c3.blue()  - roundf( bDiff2 * s2 / cy2 ) - roundf( bDiff * s / 308.f );
                    */
                    
                    p->setPen(TQColor(rc, gc, bc));
                    p->drawPoint(x, y);
                }
            }
        }
    }
    
    // TODO: some caching maybe?
    // actually, if we are now setting this as topLevelWindow pixmap
    // we could just pull rectangles from that in the future maybe

    //p.end();
}

void OxygenStyle::renderPanel(TQPainter *p,
                              const TQRect &r,
                              const TQColorGroup &g,
                              const bool pseudo3d,
                              const bool sunken) const
{
    int x, x2, y, y2, w, h;
    r.rect(&x,&y,&w,&h);
    r.coords(&x, &y, &x2, &y2);

    if (kickerMode &&
            p->device() && p->device()->devType() == TQInternal::Widget &&
            TQCString(static_cast<TQWidget*>(p->device())->className()) == "FittsLawFrame") {
    //  Stolen wholesale from Keramik. I don't like it, but oh well.
        if (sunken) {
            const TQCOORD corners[] = { x2, y, x2, y2, x, y2, x, y };
            p->setPen(g.background().dark());
            p->drawConvexPolygon(TQPointArray(4, corners));
            p->setPen(g.background().light());
            p->drawPolyline(TQPointArray(4, corners), 0, 3);
        } else {
            const TQCOORD corners[] = { x, y2, x, y, x2, y, x2, y2 };
            p->setPen(g.background().dark());
            p->drawPolygon(TQPointArray(4, corners));
            p->setPen(g.background().light());
            p->drawPolyline(TQPointArray(4, corners), 0, 3);
        }
    } else {
        renderContour(p, r, g.background(), getColor(g, PanelContour) );

        if(pseudo3d) {
            if (sunken) {
                p->setPen(getColor(g, PanelDark) );
            } else {
                p->setPen(getColor(g, PanelLight) );
            }
            p->drawLine(r.left()+2, r.top()+1, r.right()-2, r.top()+1);
            p->drawLine(r.left()+1, r.top()+2, r.left()+1, r.bottom()-2);
            if (sunken) {
                p->setPen(getColor(g, PanelLight) );
            } else {
                p->setPen(getColor(g, PanelDark) );
            }
            p->drawLine(r.left()+2, r.bottom()-1, r.right()-2, r.bottom()-1);
            p->drawLine(r.right()-1, r.top()+2, r.right()-1, r.bottom()-2);
        }
    }
}

void OxygenStyle::renderMenuBlendPixmap( KPixmap &pix, const TQColorGroup &cg,
    const TQPopupMenu* /* popup */ ) const
{
    pix.fill( cg.background().light(105) );
}

void OxygenStyle::renderTab(TQPainter *p,
                            const TQRect &r,
                            const TQColorGroup &g,
                            bool mouseOver,
                            const bool selected,
                            const bool bottom,
                            const TabPosition pos,
                            const bool triangular,
                            const bool cornerWidget) const
{
    const bool reverseLayout = TQApplication::reverseLayout();

    const bool isFirst = (pos == First) || (pos == Single);
    const bool isLast = (pos == Last);
    const bool isSingle = (pos == Single);

    if (selected) {
    // is selected

    // the top part of the tab which is nearly the same for all positions
        TQRect Rc; // contour
        if (!bottom) {
            if (isFirst && !cornerWidget && !reverseLayout) {
                Rc = TQRect(r.x(), r.y(), r.width()-1, r.height()-3);
            } else if (isFirst && !cornerWidget && reverseLayout) {
                Rc = TQRect(r.x()+1, r.y(), r.width()-1, r.height()-3);
            } else {
                Rc = TQRect(r.x()+1, r.y(), r.width()-2, r.height()-3);
            }
        } else {
            if (isFirst && !cornerWidget && !reverseLayout) {
                Rc = TQRect(r.x(), r.y()+3, r.width()-1, r.height()-3);
            } else if (isFirst && !cornerWidget && reverseLayout) {
                Rc = TQRect(r.x()+1, r.y()+3, r.width()-1, r.height()-3);
            } else {
                Rc = TQRect(r.x()+1, r.y()+3, r.width()-2, r.height()-3);
            }
        }
        const TQRect Rs(Rc.x()+1, bottom?Rc.y():Rc.y()+1, Rc.width()-2, Rc.height()-1); // the resulting surface
        // the area where the fake border shoudl appear
        const TQRect Rb(r.x(), bottom?r.top():Rc.bottom()+1, r.width(), r.height()-Rc.height() );

        uint contourFlags = Draw_Left|Draw_Right;
        if(!bottom) {
            contourFlags |= Draw_Top|Round_UpperLeft|Round_UpperRight;
        } else {
            contourFlags |= Draw_Bottom|Round_BottomLeft|Round_BottomRight;
        }
        renderContour(p, Rc,
                      g.background(), getColor(g,PanelContour),
                      contourFlags);

        // surface
        if(!bottom) {
            p->setPen(getColor(g,PanelLight) );
            p->drawLine(Rs.x()+1, Rs.y(), Rs.right()-1, Rs.y() );
            renderGradient(p, TQRect(Rs.x(), Rs.y()+1, 1, Rs.height()-1),
                           getColor(g,PanelLight), getColor(g,PanelLight2));
            renderGradient(p, TQRect(Rs.right(), Rs.y()+1, 1, Rs.height()-1),
                            getColor(g,PanelDark), getColor(g,PanelDark2));
        } else {
            p->setPen(alphaBlendColors(g.background(), g.background().dark(160), 100) );
            p->drawLine(Rs.x()+1, Rs.bottom(), Rs.right()-1, Rs.bottom() );
            renderGradient(p, TQRect(Rs.x(), Rs.y(), 1, Rs.height()-1),
                            getColor(g,PanelLight), getColor(g,PanelLight2));
            renderGradient(p, TQRect(Rs.right(), Rs.y(), 1, Rs.height()-1),
                            getColor(g,PanelDark), getColor(g,PanelDark2));
        }

    // some "position specific" paintings...
        // draw parts of the inactive tabs around...
        if(!isSingle) {
            p->setPen(alphaBlendColors(g.background(), getColor(g, ButtonContour), 50) );
            if( (!isFirst&&!reverseLayout) || (!isLast&&reverseLayout) ) {
                p->drawPoint(r.left(), bottom?(triangular?r.bottom()-2:r.bottom()-3):(triangular?r.top()+2:r.top()+3) );
                renderSurface(p, TQRect(r.left(), bottom?r.top()+3:(triangular?r.top()+3:r.top()+4), 1, (triangular?r.height()-6:r.height()-7) ),
                            g.background(), g.button(), getColor(g,MouseOverHighlight), _contrast,
                            Draw_Top|Draw_Bottom|Is_Horizontal);
            }
            if( (!isLast&&!reverseLayout) || (!isFirst&&reverseLayout) ) {
                p->drawPoint(r.right(), bottom?(triangular?r.bottom()-2:r.bottom()-3):(triangular?r.top()+2:r.top()+3) );
                renderSurface(p, TQRect(r.right(), bottom?r.top()+3:(triangular?r.top()+3:r.top()+4), 1, (triangular?r.height()-6:r.height()-7) ),
                            g.background(), g.button(), getColor(g,MouseOverHighlight), _contrast,
                            Draw_Top|Draw_Bottom|Is_Horizontal);
            }
        }
        // left connection from the panel border to the tab. :)
        if(isFirst && !reverseLayout && !cornerWidget) {
            p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
            p->drawLine(Rb.x(), Rb.y(), Rb.x(), Rb.bottom() );
            p->setPen(getColor(g,PanelLight) );
            p->drawLine(Rb.x()+1, Rb.y(), Rb.x()+1, Rb.bottom() );
        } else if(isFirst && reverseLayout && !cornerWidget) {
            p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
            p->drawLine(Rb.right(), Rb.y(), Rb.right(), Rb.bottom() );
            p->setPen(getColor(g,PanelDark) );
            p->drawLine(Rb.right()-1, Rb.y(), Rb.right()-1, Rb.bottom() );
        }
        // rounded connections to the panel...
        if(!bottom) {
            // left
            if( (!isFirst && !reverseLayout) || (reverseLayout) || (isFirst && !reverseLayout && cornerWidget) ) {
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.x(), Rb.y());
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x(), Rb.y()+1);
                p->drawPoint(Rb.x()+1, Rb.y());
            }
            // right
            if( (!reverseLayout) || (!isFirst && reverseLayout) || (isFirst && reverseLayout && cornerWidget) ) {
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.right(), Rb.y());
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right(), Rb.y()+1);
                p->drawPoint(Rb.right()-1, Rb.y());
            }
        } else {
            // left
            if( (!isFirst && !reverseLayout) || (reverseLayout) || (isFirst && !reverseLayout && cornerWidget) ) {
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.x(), Rb.bottom());
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x(), Rb.bottom()-1);
                p->drawPoint(Rb.x()+1, Rb.bottom());
            }
            // right
            if( (!reverseLayout) || (!isFirst && reverseLayout) || (isFirst && reverseLayout && cornerWidget) ) {
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.right(), Rb.bottom());
                p->setPen( alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right(), Rb.bottom()-1);
                p->drawPoint(Rb.right()-1, Rb.bottom());
            }
        }

    } else {
    // inactive tabs

    // the top part of the tab which is nearly the same for all positions
        TQRect Rc; // contour
        if (isFirst&&reverseLayout ) {
            Rc = TQRect(r.x()+1, (bottom?r.y()+2:(triangular?r.y()+2:r.y()+3)), r.width()-2, (triangular?r.height()-4:r.height()-5) );
        } else {
            Rc = TQRect(r.x()+1, (bottom?r.y()+2:(triangular?r.y()+2:r.y()+3)), r.width()-1, (triangular?r.height()-4:r.height()-5) );
        }
        TQRect Rs; // the resulting surface
        if ( (isFirst&&!reverseLayout) || (isLast&&reverseLayout) ) {
            Rs = TQRect(Rc.x()+1, bottom?Rc.y():Rc.y()+1, Rc.width()-2, Rc.height()-1);
        } else {
            Rs = TQRect(Rc.x(), bottom?Rc.y():Rc.y()+1, Rc.width()-1, Rc.height()-1);
        }
        // the area where the fake border shoudl appear
        const TQRect Rb(r.x(), bottom?r.y():Rc.bottom()+1, r.width(), 2 );

        uint contourFlags;
        if(!bottom) {
            if ( (isFirst&&!reverseLayout) || (isLast&&reverseLayout) ) {
                contourFlags = Draw_Left|Draw_Right|Draw_Top|Round_UpperLeft;
            } else if ( (isLast&&!reverseLayout) || (isFirst&&reverseLayout) ) {
                contourFlags = Draw_Right|Draw_Top|Round_UpperRight;
            } else {
                contourFlags = Draw_Right|Draw_Top;
            }
        } else {
            if ( (isFirst&&!reverseLayout) || (isLast&&reverseLayout) ) {
                contourFlags = Draw_Left|Draw_Right|Draw_Bottom|Round_BottomLeft;
            } else if ( (isLast&&!reverseLayout) || (isFirst&&reverseLayout) ) {
                contourFlags = Draw_Right|Draw_Bottom|Round_BottomRight;
            } else {
                contourFlags = Draw_Right|Draw_Bottom;
            }
        }
        renderContour(p, Rc,
                        g.background(), getColor(g, ButtonContour),
                        contourFlags);

        uint surfaceFlags = Is_Horizontal;
        if(mouseOver) {
            surfaceFlags |= (bottom?Highlight_Bottom:Highlight_Top);
            surfaceFlags |= Is_Highlight;
        }
        if ( (isFirst&&!reverseLayout) || (isLast&&reverseLayout) ) {
            if(!bottom)
                surfaceFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_UpperLeft;
            else
                surfaceFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_BottomLeft;
        } else if ( (isLast&&!reverseLayout) || (isFirst&&reverseLayout) ) {
            if(!bottom)
                surfaceFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight;
            else
                surfaceFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_BottomRight;
        } else {
            surfaceFlags |= Draw_Top|Draw_Bottom;
        }
        renderSurface(p, Rs,
                        g.background(), g.button(), getColor(g,MouseOverHighlight), _contrast,
                        surfaceFlags);

    // some "position specific" paintings...
        // fake parts of the panel border
        if(!bottom) {
            p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
            p->drawLine(Rb.x(), Rb.y(), ((isLast&&!reverseLayout)||(isFirst&&reverseLayout&&cornerWidget))?Rb.right():Rb.right()-1, Rb.y());
            p->setPen(getColor(g,PanelLight) );
            p->drawLine(Rb.x(), Rb.y()+1, ((isLast&&!reverseLayout)||(isFirst&&reverseLayout&&cornerWidget))?Rb.right():Rb.right()-1, Rb.y()+1 );
        } else {
            p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
            p->drawLine(Rb.x(), Rb.bottom(), ((isLast&&!reverseLayout)||(isFirst&&reverseLayout&&cornerWidget))?Rb.right():Rb.right()-1, Rb.bottom());
            p->setPen(getColor(g,PanelDark) );
            p->drawLine(Rb.x(), Rb.bottom()-1, ((isLast&&!reverseLayout)||(isFirst&&reverseLayout&&cornerWidget))?Rb.right():Rb.right()-1, Rb.bottom()-1 );
        }
        // fake the panel border edge for tabs which are aligned left-most
        // (i.e. only if there is no widget in the corner of the tabwidget!)
        if(isFirst&&!reverseLayout&&!cornerWidget)
        // normal layout
        {
            if (!bottom) {
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.x()+1, Rb.y()+1 );
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x(), Rb.y()+1 );
                p->setPen(g.background() );
                p->drawPoint(Rb.x(), Rb.y() );
                p->setPen(alphaBlendColors( alphaBlendColors(g.background(), getColor(g, ButtonContour), 50), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x()+1, Rb.y() );
            } else {
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.x()+1, Rb.bottom()-1 );
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x(), Rb.bottom()-1 );
                p->setPen(g.background() );
                p->drawPoint(Rb.x(), Rb.bottom() );
                p->setPen(alphaBlendColors( alphaBlendColors(g.background(), getColor(g, ButtonContour), 50), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.x()+1, Rb.bottom() );
            }
        } else if(isFirst&&reverseLayout&&!cornerWidget)
        // reverse layout
        {
            if (!bottom) {
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.right()-1, Rb.y()+1 );
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right(), Rb.y()+1 );
                p->setPen(g.background() );
                p->drawPoint(Rb.right(), Rb.y() );
                p->setPen(alphaBlendColors( alphaBlendColors(g.background(), getColor(g, ButtonContour), 50), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right()-1, Rb.y() );
            } else {
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 50) );
                p->drawPoint(Rb.right()-1, Rb.bottom()-1 );
                p->setPen(alphaBlendColors(g.background(), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right(), Rb.bottom()-1 );
                p->setPen(g.background() );
                p->drawPoint(Rb.right(), Rb.bottom() );
                p->setPen(alphaBlendColors( alphaBlendColors(g.background(), getColor(g, ButtonContour), 50), getColor(g,PanelContour), 150) );
                p->drawPoint(Rb.right()-1, Rb.bottom() );
            }
        }
    }
}

void OxygenStyle::drawTDEStylePrimitive(TDEStylePrimitive kpe,
                                      TQPainter *p,
                                      const TQStyleControlElementData &ceData,
                                      ControlElementFlags elementFlags,
                                      const TQRect &r,
                                      const TQColorGroup &cg,
                                      SFlags flags,
                                      const TQStyleOption& opt,
                                      const TQWidget* widget) const
{
    // some "global" vars...
    const bool enabled = (flags & Style_Enabled);

//  SLIDER
//  ------
    switch( kpe ) {
        /*case KPE_SliderGroove: {
            bool horizontal = ceData.orientation == TQt::Horizontal;

            if (horizontal) {
                int center = r.y()+r.height()/2;
                p->fillRect(r.left(), r.top()+center-2, r.width(), 4, cg.background().dark(130));
                renderContour(p, TQRect(r.left(), center-2, r.width(), 4),
                              cg.background(), cg.background().dark(enabled?150:130),
                              Draw_Left|Draw_Right|Draw_Top|Draw_Bottom);
            } else {
                int center = r.x()+r.width()/2;
                renderContour(p, TQRect(center-2, r.top(), 4, r.height()),
                              cg.background(), cg.background().dark(enabled?150:130),
                              Draw_Left|Draw_Right|Draw_Top|Draw_Bottom);
            }
            break;
        }*/
        case KPE_SliderGroove: {
            
            bool horizontal = ((const TQSlider*)widget)->orientation() == Horizontal;
            
            TQRect re;
            if(horizontal)
                re.setRect(r.left(), r.y()+r.height()/2-3, r.width(), 4);
            else
                re.setRect(r.x()+r.width()/2-3, r.top(), 4, r.height());
            
            bool parentBgPix = widget->parentWidget() ? widget->parentWidget()->paletteBackgroundPixmap() : 0;
            TQString hori = horizontal ? "1" : "0";
            TQString trans = parentBgPix ? "1" : "0";
            TQPixmap* sliderGroove;
            if(!(sliderGroove = TQPixmapCache::find(TQString::number(cg.background().pixel(), 16)+"sliderGroove"+hori+trans))) {

                if(parentBgPix) { // ### the pix are transparent!
                    if(!horizontal) {
                        TQWMatrix m;
                        m.rotate(-90);
                        sliderGroove = new TQPixmap(qembed_findImage("slGHBody").xForm(m));
                    }
                    else
                        sliderGroove = new TQPixmap(qembed_findImage("slGHBody"));
                }
                else {
                    const TQColor c1 = alphaBlendColors(TQColor(0,0,0), cg.background(), 11);
                    const TQColor c2 = alphaBlendColors(TQColor(53,53,53), cg.background(), 155);
                    const TQColor c3 = alphaBlendColors(TQColor(67,67,67), cg.background(), 146);
                    const TQColor c4 = alphaBlendColors(TQColor(83,83,83), cg.background(), 140);
                    const TQColor c5 = alphaBlendColors(TQColor(101,101,101), cg.background(), 134);
                    const TQColor c6 = alphaBlendColors(TQColor(254,254,254), cg.background(), 43);
                    const TQColor c7 = alphaBlendColors(TQColor(255,255,255), cg.background(), 9);
                    
                    sliderGroove = new TQPixmap(7, 7);
                    TQPainter painter(sliderGroove);
                    TQRect rr = sliderGroove->rect();
                    
                    if (horizontal) {
                        painter.setPen(c1);
                        painter.drawLine(0, 0, 7, 0);
                        painter.setPen(c2);
                        painter.drawLine(0, 1, 7, 1);
                        painter.setPen(c3);
                        painter.drawLine(0, 2, 7, 2);
                        painter.setPen(c4);
                        painter.drawLine(0, 3, 7, 3);
                        painter.setPen(c5);
                        painter.drawLine(0, 4, 7, 4);
                        painter.setPen(c6);
                        painter.drawLine(0, 5, 7, 5);
                        painter.setPen(c7);
                        painter.drawLine(0, 6, 7, 6);
                    }
                    else {
                        painter.setPen(c1);
                        painter.drawLine(0, 0, 0, 7);
                        painter.setPen(c2);
                        painter.drawLine(1, 0, 1 , 7);
                        painter.setPen(c3);
                        painter.drawLine(2, 0, 2 , 7);
                        painter.setPen(c4);
                        painter.drawLine(3, 0, 3, 7);
                        painter.setPen(c5);
                        painter.drawLine(4, 0, 4, 7);
                        painter.setPen(c6);
                        painter.drawLine(5, 0, 5, 7);
                        painter.setPen(c7);
                        painter.drawLine(6, 0, 6, 7);
                    }
                }
                TQPixmapCache::insert(TQString::number(cg.background().pixel(), 16)+"sliderGroove"+hori+trans, sliderGroove);
            }
            
            if (horizontal) {
                TQPixmap* slGh;
                if(parentBgPix || !(slGh = TQPixmapCache::find(TQString::number(cg.background().pixel(), 16)+"slGh"))) { // rounded edges
                    
                    if(parentBgPix)
                        slGh = new TQPixmap(qembed_findImage("slGh"));
                    else {
                        slGh = new TQPixmap(6, 7);
                        slGh->fill(cg.background());
                        TQPixmap pix = qembed_findImage("slGh");
                        bitBlt(slGh, 0, 0, &pix, 0, 0, -1, -1, TQt::CopyROP);
                    }
                    TQPixmapCache::insert(TQString::number(cg.background().pixel(), 16)+"slGh",slGh );
                }
                
                bitBlt(p->device(), re.x(), re.y(), slGh, 0, 0, 3, 7, TQt::CopyROP);
                bitBlt(p->device(), r.width()-3, re.y(), slGh, 3, 0, 3, 7, TQt::CopyROP);
                p->drawTiledPixmap(re.x()+3, re.y(), r.width()-6, 7, *sliderGroove);
            }
            else {
                TQPixmap* slGv;
                if(parentBgPix || !(slGv = TQPixmapCache::find(TQString::number(cg.background().pixel(), 16)+"slGv"))) {
                    if(parentBgPix)
                        slGv = new TQPixmap(qembed_findImage("slGv"));
                    else {
                        slGv = new TQPixmap(7, 6);
                        slGv->fill(cg.background());
                        TQPixmap pix = qembed_findImage("slGv");
                        bitBlt(slGv, 0, 0, &pix, 0, 0, -1, -1, TQt::CopyROP);
                    }
                    TQPixmapCache::insert(TQString::number(cg.background().pixel(), 16)+"slGv",slGv );
                }
                bitBlt(p->device(), re.x(), re.y(), slGv, 0, 0, 7, 3, TQt::CopyROP);
                bitBlt(p->device(), re.x(), re.height()-3, slGv, 0, 3, 7, 3, TQt::CopyROP);
                p->drawTiledPixmap(re.x(), re.y()+3, 7, r.height()-6, *sliderGroove);
            }
            
            break;
        }

        case KPE_SliderHandle: {
                const TQSlider* slider = (const TQSlider*)widget;
                
                int y, x;
                if(slider->orientation() == Horizontal) {
                    x = r.x();
                    y =r.y()+r.height()/2-8+1;
                }
                else {
                    y = r.y()+1;
                    x = r.x()+r.width()/2-8;
                }
                
                TQPixmap* handle;
                if(!(handle = TQPixmapCache::find("sliderHandle"+TQString::number(cg.button().pixel(), 16)))) {
                    TQPixmap insidePix =  qembed_findImage("sliderHandle2inside"); // a filled antialiased circle to draw the gradients on
                    TQPainter painter(&insidePix);
                    flatMode=true;
                    renderSurface(&painter, TQRect(1, 0, 13, 15), cg.background(), cg.button(), cg.button(), Is_Horizontal);
                    flatMode=false;
                    TQImage inside = insidePix.convertToImage();
                    
                    TQImage circle = tintImage(qembed_findImage("sliderHandle2circle"),
                            alphaBlendColors(black, cg.button(), 110));
                    
                    TQImage shadow =  qembed_findImage("sliderHandle2shadow");
                    blend(shadow, inside, inside);
                    blend(circle, inside, inside);
                    handle = new TQPixmap(inside);
                    TQPixmapCache::insert("sliderHandle"+TQString::number(cg.button().pixel(), 16), handle );
                }
                bitBlt(p->device(), x, y, handle, 0, 0, -1, -1, TQt::CopyROP);
                
                break;
            }


        case KPE_ListViewExpander: {
            int radius = (r.width() - 4) / 2;
            int centerx = r.x() + r.width()/2;
            int centery = r.y() + r.height()/2;

            renderContour(p, r, cg.base(), cg.dark(), Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight );

            p->setPen( cg.text() );
            if(!_drawTriangularExpander)
            {
                // plus or minus
                p->drawLine( centerx - radius, centery, centerx + radius, centery );
                if ( flags & Style_On ) // Collapsed = On
                    p->drawLine( centerx, centery - radius, centerx, centery + radius );
            } else if(_drawTriangularExpander) {
              if( flags & Style_On )
                drawPrimitive(PE_ArrowRight, p, ceData, elementFlags, TQRect(r.x()+1,r.y()+1,r.width(),r.height()), cg,ButtonContour, flags);
              if( flags & Style_Off )
                drawPrimitive(PE_ArrowDown, p, ceData, elementFlags, TQRect(r.x()+1,r.y()+1,r.width(),r.height()), cg,ButtonContour, flags);
            }

            break;
        }

    // copied and slightly modified from TDEStyle.
    case KPE_ListViewBranch: {
        // Typical Windows style listview branch element (dotted line).

        // Create the dotline pixmaps if not already created
        if ( !verticalLine )
        {
            // make 128*1 and 1*128 bitmaps that can be used for
            // drawing the right sort of lines.
            verticalLine   = new TQBitmap( 1, 129, true );
            horizontalLine = new TQBitmap( 128, 1, true );
            TQPointArray a( 64 );
            TQPainter p2;
            p2.begin( verticalLine );

            int i;
            for( i=0; i < 64; i++ )
                a.setPoint( i, 0, i*2+1 );
            p2.setPen( color1 );
            p2.drawPoints( a );
            p2.end();
            TQApplication::flushX();
            verticalLine->setMask( *verticalLine );

            p2.begin( horizontalLine );
            for( i=0; i < 64; i++ )
                a.setPoint( i, i*2+1, 0 );
            p2.setPen( color1 );
            p2.drawPoints( a );
            p2.end();
            TQApplication::flushX();
            horizontalLine->setMask( *horizontalLine );
        }

        p->setPen( cg.mid() );

        if (flags & Style_Horizontal)
        {
            int point = r.x();
            int other = r.y();
            int end = r.x()+r.width();
            int thickness = r.height();

            while( point < end )
            {
                int i = 128;
                if ( i+point > end )
                    i = end-point;
                p->drawPixmap( point, other, *horizontalLine, 0, 0, i, thickness );
                point += i;
            }

        } else {
            int point = r.y();
            int other = r.x();
            int end = r.y()+r.height();
            int thickness = r.width();
            int pixmapoffset = (flags & Style_NoChange) ? 0 : 1;	// ### Hackish

            while( point < end )
            {
                int i = 128;
                if ( i+point > end )
                    i = end-point;
                p->drawPixmap( other, point, *verticalLine, 0, pixmapoffset, thickness, i );
                point += i;
            }
        }

        break;
    }

        default:
            TDEStyle::drawTDEStylePrimitive(kpe, p, ceData, elementFlags, r, cg, flags, opt, widget);
    }
}


void OxygenStyle::drawPrimitive(PrimitiveElement pe,
                                TQPainter *p,
                                const TQStyleControlElementData &ceData,
                                ControlElementFlags elementFlags,
                                const TQRect &r,
                                const TQColorGroup &cg,
                                SFlags flags,
                                const TQStyleOption &opt ) const
{
    bool down   = flags & Style_Down;
    bool on     = flags & Style_On;
    bool sunken = flags & Style_Sunken;
    bool horiz  = flags & Style_Horizontal;
    const bool enabled = flags & Style_Enabled;
    const bool mouseOver = flags & Style_MouseOver;

    bool hasFocus = flags & Style_HasFocus;

    int x = r.x();
    int y = r.y();
    int w = r.width();
    int h = r.height();

    int x2, y2;
    r.coords(&x, &y, &x2, &y2);


    switch(pe) {
    // BUTTONS
    // -------
        case PE_FocusRect: {
            if(_drawFocusRect)
                p->drawWinFocusRect( r );
            break;
        }

        case PE_HeaderSectionMenu:
        case PE_HeaderSection: {
            // the taskbar buttons seems to be painted with PE_HeaderSection but I
            // want them look like normal buttons (at least for now. :) )
            if(!kickerMode) {
                // detect if this is the left most header item
                bool isFirst = false;
                TQHeader *header = dynamic_cast<TQHeader*>(p->device() );
                if (header) {
                    isFirst = header->mapToIndex(header->sectionAt(r.x() ) ) == 0;
                }

                uint contourFlags = Draw_Right|Draw_Top|Draw_Bottom;
                if (isFirst)
                    contourFlags |= Draw_Left;
                if(!enabled) contourFlags|=Is_Disabled;
                renderContour(p, r, cg.background(), getColor(cg,ButtonContour),
                                contourFlags);

                uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
                if(!enabled) surfaceFlags|=Is_Disabled;
                else {
                    if(on||down) surfaceFlags|=Is_Sunken;
                    else {
                        if(mouseOver) {
                            surfaceFlags|=Is_Highlight|Highlight_Top|Highlight_Bottom;
                        }
                    }
                }
                renderSurface(p, TQRect(isFirst?r.left()+1:r.left(), r.top()+1, isFirst?r.width()-2:r.width()-1, r.height()-2),
                                cg.background(), cg.button(), getColor(cg,MouseOverHighlight), _contrast,
                                surfaceFlags);

                break;
            }
        }
        case PE_ButtonBevel:
        case PE_ButtonTool:
        case PE_ButtonDropDown:
        case PE_ButtonCommand: {
            bool tdehtmlMode = opt.isDefault() ? false : tdehtmlWidgets.contains(opt.widget());
            renderButton(p, r, cg, (on||down), mouseOver, true, enabled, tdehtmlMode );
            break;
        }

        case PE_ButtonDefault: {
            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                    Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight;
            if(!enabled) contourFlags|=Is_Disabled;
            renderContour(p, r, cg.background(), cg.background().dark(120),
                    contourFlags);
            break;
        }

        case PE_SpinWidgetPlus:
        case PE_SpinWidgetMinus: {
            p->setPen( cg.buttonText() );

            int l = TQMIN( w-2, h-2 );
            // make the length even so that we get a nice symmetric plus...
            if(l%2 != 0)
                --l;
            TQPoint c = r.center();

            p->drawLine( c.x()-l/2, c.y(), c.x()+l/2, c.y() );
            if ( pe == PE_SpinWidgetPlus ) {
                p->drawLine( c.x(), c.y()-l/2, c.x(), c.y()+l/2 );
            }
            break;
        }

        case PE_ScrollBarSlider: {
            const WidgetState s = enabled?(down?IsPressed:IsEnabled):IsDisabled;
            const TQColor surface = getColor(cg, DragButtonSurface, s);

            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            if(!enabled) contourFlags|=Is_Disabled;
            renderContour(p, r, cg.background(), getColor(cg, DragButtonContour, s),
                    contourFlags);

            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            if(!horiz) surfaceFlags|=Is_Horizontal;
            if(!enabled) surfaceFlags|=Is_Disabled;
            if(r.height() >= 4)
                renderSurface(p, TQRect(r.left()+1, r.top()+1, r.width()-2, r.height()-2),
                        cg.background(), surface, cg.background(),
                        _contrast+3, surfaceFlags);

            // set contour-like color for the case _scrollBarLines is set and we paint lines instead of dots.
            p->setPen(alphaBlendColors(cg.background(), surface.dark(enabled?140:120), 50) );

            const int d = 4;
            int n = ((horiz?r.width():r.height())-8)/d;
            if(n>5) n=5;
            /*if(!horiz) {
                for(int j = 0; j < n; j++) {
                    int yPos = r.center().y()-(n*d)/2+d*j+1;
                    if(_scrollBarLines)
                        p->drawLine(r.x()+1, yPos, r.right()-1, yPos);
                    else
                    {
                        for(int k = 3; k <= 13; k+=4) {
                            renderDot(p, TQPoint(k, yPos), surface, false, true );
                        }
                    }
                }
            } else {
                for(int j = 0; j < n; j++) {
                    int xPos = r.center().x()-(n*d)/2+d*j+1;
                    if(_scrollBarLines)
                        p->drawLine(xPos, r.y()+1, xPos, r.bottom()-1);
                    else
                    {
                        for(int k = 3; k <= 13; k+=4) {
                            renderDot(p, TQPoint(xPos, k), surface, false, true );
                        }
                    }
                }
            }*/

            break;
        }

        case PE_ScrollBarAddPage:
        case PE_ScrollBarSubPage: {
            // draw double buffered to avoid flicker...
            TQPixmap buffer;
            if(flags & Style_Horizontal) {
                buffer.resize(2, r.height() );
            } else {
                buffer.resize(r.width(), 2 );
            }
            TQRect br(buffer.rect() );
            TQPainter bp(&buffer);

            if (on || down) {
                bp.fillRect(br, TQBrush(cg.mid().dark()));
            } else {
                if(flags & Style_Horizontal) {
                    bp.setPen(cg.background().dark(106));
                    bp.drawLine(br.left(), br.top(), br.right(), br.top());
                    bp.setPen(cg.background().light(106));
                    bp.drawLine(br.left(), br.bottom(), br.right(), br.bottom());
                    bp.fillRect(br.left(), br.top()+1, br.width(), br.height()-2,cg.background());
                } else {
                    bp.setPen(cg.background().dark(106));
                    bp.drawLine(br.left(), br.top(), br.left(), br.bottom());
                    bp.setPen(cg.background().light(106));
                    bp.drawLine(br.right(), br.top(), br.right(), br.bottom());
                    bp.fillRect(br.left()+1, br.top(), br.width()-2, br.height(),cg.background());
                }
            }

            bp.fillRect(br, TQBrush(cg.background().light(), Dense4Pattern));

            bp.end();

            p->drawTiledPixmap(r, buffer, TQPoint(0, r.top()%2) );
            break;
        }

    // SCROLLBAR BUTTONS
    // -----------------
        case PE_ScrollBarSubLine: {
            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            if(down) surfaceFlags|=Is_Sunken;
            if(!enabled) {
                contourFlags|=Is_Disabled;
                surfaceFlags|=Is_Disabled;
            }
            if(horiz) {
                contourFlags |= Round_UpperLeft|Round_BottomLeft|Is_Horizontal;
                surfaceFlags |= Round_UpperLeft|Round_BottomLeft|Is_Horizontal;
            } else {
                contourFlags |= Round_UpperLeft|Round_UpperRight;
                surfaceFlags |= Round_UpperLeft|Round_UpperRight;
            }
            /*
            renderContour(p, r, cg.background(), getColor(cg, ButtonContour),
                    contourFlags);
            renderSurface(p, TQRect(r.left()+1, r.top()+1, r.width()-2, r.height()-2),
                    cg.background(), cg.button(), getColor(cg,MouseOverHighlight), _contrast+3,
                    surfaceFlags);
            */

            p->setPen(cg.foreground());
            drawPrimitive((horiz ? PE_ArrowLeft : PE_ArrowUp), p, ceData, elementFlags, r, cg, flags);
            break;
        }

        case PE_ScrollBarAddLine: {
            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            if(down) surfaceFlags|=Is_Sunken;
            if(!enabled) {
                contourFlags|=Is_Disabled;
                surfaceFlags|=Is_Disabled;
            }
            if(horiz) {
                contourFlags |= Round_UpperRight|Round_BottomRight|Is_Horizontal;
                surfaceFlags |= Round_UpperRight|Round_BottomRight|Is_Horizontal;
            } else {
                contourFlags |= Round_BottomLeft|Round_BottomRight;
                surfaceFlags |= Round_BottomLeft|Round_BottomRight;
            }
            /*
            renderContour(p, r, cg.background(), getColor(cg, ButtonContour),
                    contourFlags);
            renderSurface(p, TQRect(r.left()+1, r.top()+1, r.width()-2, r.height()-2),
                    cg.background(), cg.button(), getColor(cg,MouseOverHighlight), _contrast+3,
                    surfaceFlags);
            */

            p->setPen(cg.foreground());
            drawPrimitive((horiz ? PE_ArrowRight : PE_ArrowDown), p, ceData, elementFlags, r, cg, flags);
            break;
        }

    // CHECKBOXES
    // ----------
        case PE_Indicator: {
            TQColor contentColor = enabled?cg.base():cg.background();

            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom;
            if(!enabled) {
                contourFlags |= Is_Disabled;
            }
            renderContour(p, r, cg.background(), getColor(cg, ButtonContour),
                    contourFlags);

            // surface
            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
            if(!enabled) {
                surfaceFlags |= Is_Disabled;
            } else if(mouseOver) {
                contentColor = alphaBlendColors(contentColor, getColor(cg,MouseOverHighlight), 240);
                surfaceFlags |= Is_Highlight;
                surfaceFlags |= Highlight_Left|Highlight_Right|
                        Highlight_Top|Highlight_Bottom;
            }
            renderSurface(p, TQRect(r.x()+1, r.y()+1, r.width()-2, r.height()-2),
                    cg.background(), contentColor, getColor(cg,MouseOverHighlight), enabled?_contrast+3:(_contrast/2), surfaceFlags);

            drawPrimitive(PE_CheckMark, p, ceData, elementFlags, r, cg, flags);

            break;
        }

        case PE_IndicatorMask: {
            p->fillRect (r, color1);
            break;
        }

    // RADIOBUTTONS
    // ------------
        case PE_ExclusiveIndicator: {
            const TQColor contourColor = getColor(cg, ButtonContour, enabled);
            TQColor contentColor = enabled?cg.base():cg.background();

            TQBitmap bmp;
            bmp = TQBitmap(13, 13, radiobutton_mask_bits, true);
            // first the surface...
            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
            if(!enabled) {
                surfaceFlags |= Is_Disabled;
            } else if (mouseOver) {
                contentColor = alphaBlendColors(contentColor, getColor(cg,MouseOverHighlight), 240);
            }
            p->setClipRegion(bmp);
            renderSurface(p, r,
                    cg.background(), contentColor, getColor(cg,MouseOverHighlight), enabled?_contrast+3:(_contrast/2), surfaceFlags);
            p->setClipping(false);

            // ...then contour, anti-alias, mouseOver...
            // contour
            bmp = TQBitmap(13, 13, radiobutton_contour_bits, true);
            bmp.setMask(bmp);
            p->setPen(alphaBlendColors(cg.background(), contourColor, 50) );
            p->drawPixmap(x, y, bmp);
            // anti-alias outside
            bmp = TQBitmap(13, 13, radiobutton_aa_outside_bits, true);
            bmp.setMask(bmp);
            p->setPen(alphaBlendColors(cg.background(), contourColor, 150) );
            p->drawPixmap(x, y, bmp);
            // highlighting...
            if(mouseOver) {
                bmp = TQBitmap(13, 13, radiobutton_highlight1_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, getColor(cg,MouseOverHighlight), 80) );
                p->drawPixmap(x, y, bmp);
                bmp = TQBitmap(13, 13, radiobutton_highlight2_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, getColor(cg,MouseOverHighlight), 150) );
                p->drawPixmap(x, y, bmp);
            }
            // anti-alias inside, "above" the higlighting!
            bmp = TQBitmap(13, 13, radiobutton_aa_inside_bits, true);
            bmp.setMask(bmp);
            if(mouseOver) {
                p->setPen(alphaBlendColors(getColor(cg,MouseOverHighlight), contourColor, 180) );
            } else {
                p->setPen(alphaBlendColors(contentColor, contourColor, 180) );
            }
            p->drawPixmap(x, y, bmp);


            TQColor checkmarkColor = enabled?getColor(cg,CheckMark):cg.background();
            if(flags & Style_Down) {
                checkmarkColor = alphaBlendColors(contentColor, checkmarkColor, 150);
            }

            // draw the radio mark
            if (flags & Style_On || flags & Style_Down) {
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, radiomark_dark_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(150), 50) );
                p->drawPixmap(x+2, y+2, bmp);
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, radiomark_light_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(125), 50) );
                p->drawPixmap(x+2, y+2, bmp);
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, radiomark_aa_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(150), 150) );
                p->drawPixmap(x+2, y+2, bmp);
            }

            break;
        }

        case PE_ExclusiveIndicatorMask: {
            p->fillRect(r, color0);

            TQBitmap bmp;
            bmp = TQBitmap(13, 13, radiobutton_mask_bits, true);
            bmp.setMask(bmp);
            p->setPen(color1);
            p->drawPixmap(x, y, bmp);

            break;
        }

    // GENERAL PANELS
    // --------------
        case PE_Splitter: {
            // highlight on mouse over
            TQColor color = (mouseOver)?TQColor(cg.background().light(100+_contrast)):cg.background();
            p->fillRect(r, color);
            if (w > h) {
                if (h > 4) {
                    int ycenter = r.height()/2;
                    for(int k = 2*r.width()/10; k < 8*r.width()/10; k+=5) {
                        renderDot(p, TQPoint(k, ycenter-1), color, false, true);
                    }
                }
            } else {
                if (w > 4) {
                    int xcenter = r.width()/2;
                    for(int k = 2*r.height()/10; k < 8*r.height()/10; k+=5) {
                        renderDot(p, TQPoint(xcenter-1, k), color, false, true);
                    }
                }
            }

                break;
        }

        case PE_PanelGroupBox:
        case PE_GroupBoxFrame: {
            if ( opt.isDefault() || opt.lineWidth() <= 0 )
                break;
            renderPanel(p, r, cg, false);

            break;
        }

        case PE_WindowFrame:
        case PE_Panel: {
            if ( opt.isDefault() || opt.lineWidth() <= 0 )
                break;
            renderPanel(p, r, cg, true, sunken);
            break;
        }

        case PE_PanelLineEdit: {
            bool isReadOnly = false;
            bool isEnabled = true;
            // panel is highlighted by default if it has focus, but if we have access to the
            // widget itself we can try to avoid highlighting in case it's readOnly or disabled.
            if (p->device() && dynamic_cast<TQLineEdit*>(p->device()))
            {
                TQLineEdit* lineEdit = dynamic_cast<TQLineEdit*>(p->device());
                isReadOnly = lineEdit->isReadOnly();
                isEnabled = lineEdit->isEnabled();
            }

            uint contourFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                    Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight;

            // HACK!!
            //
            // In order to draw nice edges in tdehtml, we need to paint alpha-blended.
            // On the other hand, we can't paint alpha-blended in normal widgets.
            //
            // In this place there is no reliable way to detect if we are in tdehtml; the
            // only thing we know is that tdehtml buffers its widgets into a pixmap. So
            // when the paint device is a TQPixmap, chances are high that we are in tdehtml.
            // It's possible that this breaks other things, so let's see how it works...
            if (p->device() && dynamic_cast<TQPixmap*>(p->device() ) ) {
                contourFlags += Draw_AlphaBlend;
            }

            if ( _inputFocusHighlight && hasFocus && !isReadOnly && isEnabled)
            {
                renderContour(p, r, cg.background(),
                              getColor(cg,FocusHighlight,enabled), contourFlags );
            }
            else
            {
                renderContour(p, r, cg.background(),
                              getColor(cg, ButtonContour, enabled), contourFlags );
            }
            const TQColor contentColor = enabled?cg.base():cg.background();
            if (_inputFocusHighlight && hasFocus && !isReadOnly && isEnabled)
            {
                p->setPen( getColor(cg,FocusHighlight).dark(130) );
            }
            else
            {
                p->setPen(contentColor.dark(130) );
            }
            p->drawLine(r.left()+1, r.top()+2, r.left()+1, r.bottom()-2 );
            p->drawLine(r.left()+2, r.top()+1, r.right()-2, r.top()+1 );
            if (_inputFocusHighlight && hasFocus && !isReadOnly && isEnabled)
            {
                p->setPen( getColor(cg,FocusHighlight).light(130) );
            }
            else
            {
              p->setPen(contentColor.light(130) );
            }
            p->drawLine(r.left()+2, r.bottom()-1, r.right()-2, r.bottom()-1 );
            p->drawLine(r.right()-1, r.top()+2, r.right()-1, r.bottom()-2 );
            break;
        }

        case PE_StatusBarSection: {
            /*
            renderContour(p, r, cg.background(), cg.background().dark(160),
                          Draw_Left|Draw_Right|Draw_Top|Draw_Bottom);
            */
            
            TQWidget* w = dynamic_cast<TQWidget*>(p->device());
            if( CHECKBGPIXMAP(w->parentWidget()) )
            {
                // FIXME!!
                renderWindowBackground( p, w->parentWidget()->rect(), cg );
            }
            
            break;
        }

        case PE_TabBarBase: // Still not sure what this one does
        case PE_PanelTabWidget: {
            renderPanel(p, r, cg, true, sunken);
            break;
        }

        case PE_PanelPopup: {
            renderContour(p, r, cg.background(), cg.background().dark(200),
                          Draw_Left|Draw_Right|Draw_Top|Draw_Bottom);
            break;
        }

    // MENU / TOOLBAR PANEL
    // --------------------
        case PE_PanelMenuBar:
        case PE_PanelDockWindow: {
            // fix for toolbar lag (from Mosfet Liquid) 
            TQWidget* w = dynamic_cast<TQWidget*>(p->device());
            if(w && w->backgroundMode() == PaletteButton) 
                w->setBackgroundMode(PaletteBackground);
            p->fillRect(r, cg.brush(TQColorGroup::Background));
            //renderWindowBackground( p, w->parentWidget()->rect(), cg );
            // FIXME
            //if( CHECKBGPIXMAP(w->parentWidget()) )
            {
                renderWindowBackground( p, w->parentWidget()->rect(), cg );
                // FIXME!
                //copyBlt( p, bgWidgets[w->parentWidget()] );
                //p->drawPixmap( 0, 0, *bgWidgets[w->parentWidget()], r.bottom(), r.right() );
            }

            if ( _drawToolBarSeparator ) {
                if ( r.width() > r.height() ) {
                    p->setPen( getColor(cg, PanelLight) );
                    p->drawLine( r.left(), r.top(), r.right(), r.top() );
                    p->setPen( getColor(cg, PanelDark) );
                    p->drawLine( r.left(), r.bottom(), r.right(), r.bottom() );
                }
                else {
                    p->setPen( getColor(cg, PanelLight) );
                    p->drawLine( r.left(), r.top(), r.left(), r.bottom() );
                    p->setPen( getColor(cg, PanelDark) );
                    p->drawLine( r.right(), r.top(), r.right(), r.bottom() );
                }
            }

            break;
        }

    // TOOLBAR/DOCK WINDOW HANDLE
    // --------------------------
        case PE_DockWindowResizeHandle: {
            renderButton(p, r, cg);
            break;
        }

        case PE_DockWindowHandle: {

            int counter = 1;

            if(horiz) {
                int center = r.left()+r.width()/2;
                for(int j = r.top()+2; j <= r.bottom()-3; j+=3) {
                    if(counter%2 == 0) {
                        renderDot(p, TQPoint(center+1, j), cg.background(), true, true);
                    } else {
                        renderDot(p, TQPoint(center-2, j), cg.background(), true, true);
                    }
                    counter++;
                }
            } else {
                int center = r.top()+r.height()/2;
                for(int j = r.left()+2; j <= r.right()-3; j+=3) {
                    if(counter%2 == 0) {
                        renderDot(p, TQPoint(j, center+1), cg.background(), true, true);
                    } else {
                        renderDot(p, TQPoint(j, center-2), cg.background(), true, true);
                    }
                    counter++;
                }
            }

            break;
        }

    // TOOLBAR SEPARATOR
    // -----------------
        case PE_DockWindowSeparator: {
            p->fillRect(r, cg.background());
            TQWidget* w = dynamic_cast<TQWidget*>(p->device());
            if( CHECKBGPIXMAP(w->parentWidget()) )
            {
                // FIXME!!
                //renderWindowBackground( p, w->parentWidget()->rect(), cg );
            }

            if(_drawToolBarItemSeparator) {
                if(horiz) {
                    int center = r.left()+r.width()/2;
                    p->setPen( getColor(cg, PanelDark) );
                    p->drawLine( center-1, r.top()+3, center-1, r.bottom()-3 );
                    p->setPen( getColor(cg, PanelLight) );
                    p->drawLine( center, r.top()+3, center, r.bottom()-3 );
                } else {
                    int center = r.top()+r.height()/2;
                    p->setPen( getColor(cg, PanelDark) );
                    p->drawLine( r.x()+3, center-1, r.right()-3, center-1 );
                    p->setPen( getColor(cg, PanelLight) );
                    p->drawLine( r.x()+3, center, r.right()-3, center );
                }
            }
            break;
        }

        case PE_CheckMark: {
            const TQColor contentColor = enabled?cg.base():cg.background();
            TQColor checkmarkColor = enabled?getColor(cg,CheckMark):cg.background();
            if(flags & Style_Down) {
                checkmarkColor = alphaBlendColors(contentColor, checkmarkColor, 150);
            }

            int x = r.center().x() - 4, y = r.center().y() - 4;
            TQBitmap bmp;
            if( flags & Style_On ) {
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, checkmark_dark_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(150), 50) );
                p->drawPixmap(x, y, bmp);
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, checkmark_light_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(125), 50) );
                p->drawPixmap(x, y, bmp);
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, checkmark_aa_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(150), 150) );
                p->drawPixmap(x, y, bmp);
            } else if ( flags & Style_Off ) {
                // empty
            } else { // tristate
                bmp = TQBitmap(CHECKMARKSIZE, CHECKMARKSIZE, checkmark_tristate_bits, true);
                bmp.setMask(bmp);
                p->setPen(alphaBlendColors(contentColor, checkmarkColor.dark(150), 50) );
                p->drawPixmap(x, y, bmp);
            }

            break;
        }

	case PE_MenuItemIndicatorFrame:
		{
			int x, y, w, h;
			r.rect( &x, &y, &w, &h );
			int checkcol = styleHint(SH_MenuIndicatorColumnWidth, ceData, elementFlags, opt, NULL, NULL);
			TQRect cr = visualRect( TQRect( x + 2, y + 2, checkcol - 1, h - 4 ), r );
			qDrawShadePanel( p, cr.x(), cr.y(), cr.width(), cr.height(), cg, true, 1, &cg.brush(TQColorGroup::Midlight) );
			break;
		}
	case PE_MenuItemIndicatorIconFrame:
		{
			int x, y, w, h;
			r.rect( &x, &y, &w, &h );
			int checkcol = styleHint(SH_MenuIndicatorColumnWidth, ceData, elementFlags, opt, NULL, NULL);
			TQRect cr = visualRect( TQRect( x + 2, y + 2, checkcol - 1, h - 4 ), r );
			qDrawShadePanel( p, cr.x(), cr.y(), cr.width(), cr.height(), cg, true, 1, &cg.brush(TQColorGroup::Midlight) );
			break;
		}
	case PE_MenuItemIndicatorCheck:
		{
			int x, y, w, h;
			r.rect( &x, &y, &w, &h );
			int checkcol = styleHint(SH_MenuIndicatorColumnWidth, ceData, elementFlags, opt, NULL, NULL);
			TQRect cr = visualRect( TQRect( x + 2, y + 2, checkcol - 1, h - 4 ), r );
			// Draw the checkmark
			SFlags cflags = Style_On;
			if (enabled)
				cflags |= Style_Enabled;
			drawPrimitive( PE_CheckMark, p, ceData, elementFlags, cr, cg, cflags );
			break;
		}

        case PE_SpinWidgetUp:
        case PE_SpinWidgetDown:
        case PE_HeaderArrow:
        case PE_ArrowUp:
        case PE_ArrowDown:
        case PE_ArrowLeft:
        case PE_ArrowRight: {
            TQPointArray a;

            switch (pe) {
                case PE_SpinWidgetUp:
                case PE_ArrowUp: {
                    a.setPoints(7, u_arrow);
                    break;
                }
                case PE_SpinWidgetDown:
                case PE_ArrowDown: {
                    a.setPoints(7, d_arrow);
                    break;
                }
                case PE_ArrowLeft: {
                    a.setPoints(7, l_arrow);
                    break;
                }
                case PE_ArrowRight: {
                    a.setPoints(7, r_arrow);
                    break;
                }
                default: {
                    if (flags & Style_Up) {
                        a.setPoints(7, u_arrow);
                    } else {
                        a.setPoints(7, d_arrow);
                    }
                }
            }

            const TQWMatrix oldMatrix( p->worldMatrix() );

            if (flags & Style_Down) {
                p->translate(pixelMetric(PM_ButtonShiftHorizontal, ceData, elementFlags),
                                pixelMetric(PM_ButtonShiftVertical, ceData, elementFlags));
            }

            a.translate((r.x()+r.width()/2), (r.y()+r.height()/2));
            // extra-pixel-shift, correcting some visual tics...
            switch(pe) {
                case PE_ArrowLeft:
                case PE_ArrowRight:
                    a.translate(0, -1);
                    break;
                case PE_SpinWidgetUp:
                case PE_SpinWidgetDown:
                    a.translate(+1, 0);
                    break;
                default:
                    a.translate(0, 0);
            }

            if (p->pen() == TQt::NoPen) {
                if (flags & Style_Enabled) {
                    p->setPen(cg.buttonText());
                } else {
                    p->setPen(cg.highlightedText());
                }
            }
            p->drawLineSegments(a, 0, 3);
            p->drawPoint(a[6]);

            p->setWorldMatrix( oldMatrix );

            break;
        }

        default: {
            return TDEStyle::drawPrimitive(pe, p, ceData, elementFlags, r, cg, flags, opt);
        }
    }
}


void OxygenStyle::drawControl(ControlElement element,
                              TQPainter *p,
                              const TQStyleControlElementData &ceData,
                              ControlElementFlags elementFlags,
                              const TQRect &r,
                              const TQColorGroup &cg,
                              SFlags flags,
                              const TQStyleOption& opt,
                              const TQWidget *widget) const
{
    const bool reverseLayout = TQApplication::reverseLayout();

    const bool enabled = (flags & Style_Enabled);

    switch (element) {

    // PROGRESSBAR
    // -----------
        case CE_ProgressBarGroove: {
            const TQColor content = enabled?cg.base():cg.background();
            renderContour(p, r, cg.background(), getColor(cg, ButtonContour, enabled) );
            p->setPen(content.dark(105) );
            p->drawLine(r.left()+2, r.top()+1, r.right()-2, r.top()+1 );
            p->drawLine(r.left()+1, r.top()+2, r.left()+1, r.bottom()-2);
            p->setPen(content.light(105) );
            p->drawLine(r.left()+2, r.bottom()-1, r.right()-2, r.bottom()-1 );
            p->drawLine(r.right()-1, r.top()+2, r.right()-1, r.bottom()-2);
            break;
        }

        case CE_ProgressBarContents: {
            int steps = ceData.totalSteps;

            const TQColor bg = enabled?cg.background().dark(130):cg.background(); // background
            const TQColor fg = enabled?cg.highlight():TQColor(cg.background().dark(110)); // foreground

            if( steps == 0 ) { // Busy indicator
                static const int barWidth = 10;
                int progress = ceData.currentStep % (2*(r.width()-barWidth));
                if( progress < 0)
                    progress = 0;
                if( progress > r.width()-barWidth )
                    progress = (r.width()-barWidth)-(progress-(r.width()-barWidth));
                p->fillRect( TQRect( r.x(), r.y(), r.width(), r.height() ), bg );
                renderContour( p, TQRect( r.x()+progress, r.y(), barWidth, r.height() ),
                                bg, fg.dark(160),
                                Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|
                                    Round_BottomRight|Round_UpperLeft|Round_BottomLeft );
                renderSurface(p, TQRect( r.x()+progress+1, r.y()+1, barWidth-2, r.height()-2 ),
                                    bg, fg, cg.highlight(),
                                    2*(_contrast/3),
                                    Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperRight|Round_BottomRight|
                                        Round_UpperLeft|Round_BottomLeft|Is_Horizontal);
            } else {
                double percent = static_cast<double>(ceData.currentStep) / static_cast<double>(steps);

                if( _animateProgressBar )
                {
                    TQWidget* nonConstWidget = const_cast<TQWidget*>(widget);
                    TQMapConstIterator<TQWidget*, int> iter = progAnimWidgets.find(nonConstWidget);
                    if (iter != progAnimWidgets.end())
                        percent = iter.data() / 100.0;
                }

                int w = static_cast<int>(r.width() * percent);
                // renderContour/renderSurface handle small sizes not very well, so set a minimal
                // progressbar width...
                if(w<4) w = 4;
                int w2 = r.width()-(r.width()-w);

                TQRect Rempty(reverseLayout?r.left():r.left()+w-1, r.top(), r.width()-w+1, r.height() );
                TQRect Rcontour(reverseLayout?r.right()-w2+1:r.left(), r.top(), w2, r.height() );
                TQRect Rsurface(Rcontour.left()+1, Rcontour.top()+1, w2-2, Rcontour.height()-2);

                p->fillRect(Rempty, bg);

                renderContour(p, Rcontour,
                              bg, fg.dark(160),
                              reverseLayout ? Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_BottomLeft
                              : Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|Round_BottomRight);

                TQRegion mask(Rsurface);
                if(reverseLayout) {
                    mask -= TQRegion(Rsurface.left(), Rsurface.top(), 1, 1);
                    mask -= TQRegion(Rsurface.left(), Rsurface.bottom(), 1, 1);
                } else {
                    mask -= TQRegion(Rsurface.right(), Rsurface.top(), 1, 1);
                    mask -= TQRegion(Rsurface.right(), Rsurface.bottom(), 1, 1);
                }
                p->setClipRegion(mask);
                int counter = 0;
                TQPixmap surfaceTile(21, r.height()-2);
                TQPainter surfacePainter(&surfaceTile);
                // - 21 pixel -
                //  __________
                // |    `    `| <- 3
                // | 1   | 2  |
                // |____,____,| <- 3
                // 1 = light, 11 pixel, 1 pixel overlapping with 2
                // 2 = dark, 11 pixel, 1 pixel overlapping with 3
                // 3 = light edges
                const int tileHeight = surfaceTile.height();
                // 3
                /*renderSurface(&surfacePainter,
                                TQRect(0, 0, 20, tileHeight),
                                fg.light(105), fg, cg.highlight(), 2*(_contrast/3),
                                reverseLayout ? Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperLeft|Round_BottomLeft|Is_Horizontal
                                : Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperRight|Round_BottomRight|Is_Horizontal);
                // 2
                /*renderSurface(&surfacePainter,
                                TQRect(10, 0, 11, tileHeight),
                                fg, fg.light(105), cg.highlight(), 2*(_contrast/3),
                                reverseLayout ? Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperLeft|Round_BottomLeft|Is_Horizontal
                                : Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperRight|Round_BottomRight|Is_Horizontal);
                // 1
                renderSurface(&surfacePainter,
                                TQRect(0, 0, 11, tileHeight),
                                fg.light(105), fg, cg.highlight(), 2*(_contrast/3),
                                reverseLayout ? Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperLeft|Round_BottomLeft|Is_Horizontal
                                : Draw_Right|Draw_Left|Draw_Top|Draw_Bottom|
                                        Round_UpperRight|Round_BottomRight|Is_Horizontal);

                surfacePainter.end();*/
                int staticShift = 0;
                int animShift = 0;
                //if (!_animateProgressBar) {
                    staticShift = (reverseLayout ? Rsurface.left() : Rsurface.right()) % 40 - 40;
                /*} else {
                    // find the animation Offset for the current Widget
                    TQWidget* nonConstWidget = const_cast<TQWidget*>(widget);
                    TQMapConstIterator<TQWidget*, int> iter = progAnimWidgets.find(nonConstWidget);
                    if (iter != progAnimWidgets.end())
                        animShift = iter.data();
                }*/
                /*while((counter*10) < (Rsurface.width()+20)) {
                    counter++;
                    if (reverseLayout) {    
                        // from right to left, overlap 1 pixel with the previously drawn tile
                        p->drawPixmap(Rsurface.right()-counter*20-animShift+40+staticShift, r.top()+1,
                                    surfaceTile);
                    } else {
                        // from left to right, overlap 1 pixel with the previously drawn tile
                        p->drawPixmap(Rsurface.left()+counter*20+animShift-40+staticShift, r.top()+1,
                                    surfaceTile);
                    }
                }*/
                //int hw = Rsurface.width()/2;
                //renderGradient(p, Rsurface, fg, fg.light(150));
                
                //if(false)
                {
                    int r_w = Rsurface.width();
                    int r_h = Rsurface.height();
                    int r_x, r_y, r_x2, r_y2;
                    Rsurface.coords(&r_x, &r_y, &r_x2, &r_y2);

                    int rDiff, gDiff, bDiff;
                    int rc, gc, bc;

                    int x, y;
                    
                    TQColor c1(fg);
                    TQColor c2(fg.light(250));
                    
                    bool horizontal = true;

                    rDiff = ( c2.red())   - (rc = c1.red());
                    gDiff = ( c2.green()) - (gc = c1.green());
                    bDiff = ( c2.blue())  - (bc = c1.blue());

                    int rl = rc << 16;
                    int gl = gc << 16;
                    int bl = bc << 16;

                    int rdelta = ((1<<16) / (horizontal ? r_h : r_w)) * rDiff;
                    int gdelta = ((1<<16) / (horizontal ? r_h : r_w)) * gDiff;
                    int bdelta = ((1<<16) / (horizontal ? r_h : r_w)) * bDiff;

                    int hw = r_w/2;
                    int hh = r_h/2;
                    
                    float dd = 0;
                    float ddelta = 1.f / hw;
                    for( x = 0; x < hw; x++)
                    {
                        dd += ddelta;

                        for ( y = hh; y < r_h; y++ )
                        {
                            rl += rdelta*dd;
                            gl += gdelta*dd;
                            bl += bdelta*dd;
                            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
                            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
                        }
                        
                        for ( y = 0; y < hh; y++ )
                        {
                            rl -= rdelta*dd;
                            gl -= gdelta*dd;
                            bl -= bdelta*dd;
                            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
                            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
                        }
                    }
                    
                    for( ; x < r_w; x++)
                    {
                        dd -= ddelta;

                        for ( y = hh; y < r_h; y++ )
                        {
                            rl += rdelta*dd;
                            gl += gdelta*dd;
                            bl += bdelta*dd;
                            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
                            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
                        }
                        
                        for ( y = 0; y < hh; y++ )
                        {
                            rl -= rdelta*dd;
                            gl -= gdelta*dd;
                            bl -= bdelta*dd;
                            p->setPen(TQColor((rl>>16), (gl>>16), (bl>>16)));
                            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
                        }
                    }
                    
                    /*for ( y = 0; y < r_h; y++ ) {
                        rl += rdelta;
                        gl += gdelta;
                        bl += bdelta;

                        p->setPen(TQColor(rl>>16, gl>>16, bl>>16));
                        p->drawLine(r_x, r_y+y, r_x2, r_y+y);
                    }*/
                    
                    /*for( x = 0; x < hw; x++)
                    {
                        rl -= rdelta;
                        gl -= gdelta;
                        bl -= bdelta;

                        for ( y = 0; y < r_h; y++ )
                        {
                            p->setPen(TQColor(rl>>16, gl>>16, bl>>16));
                            p->drawLine(r_x+x, r_y+y, r_x+x, r_y+y);
                        }
                    }*/
                    
                    if(horizontal) {
                        
                    } else {
                        
                    }

                    //p.end();

                    // draw the result...
                    //painter->drawTiledPixmap(rect, *result);
                }

                p->setClipping(false);
            }

            break;
        }

    // TABS
    // ----
        case CE_TabBarTab: {
            bool cornerWidget = false;
            if (!ceData.tabBarData.cornerWidgets[TQStyleControlElementTabBarData::CWL_TopLeft].widgetObjectTypes.isEmpty()) {
                cornerWidget = true;
            }
            TQTabBar::Shape tbs = ceData.tabBarData.shape;
            bool selected = false;
            if (flags & Style_Selected) selected = true;
            TabPosition pos;
            if (ceData.tabBarData.tabCount == 1) {
                pos = Single;
            } else if (ceData.tabBarData.identIndexMap[opt.tab()->identifier()] == 0) {
                pos = First;
            } else if (ceData.tabBarData.identIndexMap[opt.tab()->identifier()] == (ceData.tabBarData.tabCount - 1)) {
                pos = Last;
            } else {
                pos = Middle;
            }

            bool mouseOver = false;
            if (opt.tab() == opt.hoverTab()) {
                mouseOver = true;
                flags |= Style_MouseOver;
            }

            switch (tbs) {
                case TQTabBar::TriangularAbove:
//                     renderTriangularTab(p, r, cg, (flags & Style_MouseOver), selected, false, pos);
                    renderTab(p, r, cg, mouseOver, selected, false, pos, true, cornerWidget);
                    break;
                case TQTabBar::RoundedAbove:
                    renderTab(p, r, cg, mouseOver, selected, false, pos, false, cornerWidget);
                    break;
                case TQTabBar::TriangularBelow:
//                     renderTriangularTab(p, r, cg, (flags & Style_MouseOver), selected, true, pos);
                    renderTab(p, r, cg, mouseOver, selected, true, pos, true, cornerWidget);
                    break;
                case TQTabBar::RoundedBelow:
                    renderTab(p, r, cg, mouseOver, selected, true, pos, false, cornerWidget);
                    break;
                    default:
                            TDEStyle::drawControl(element, p, ceData, elementFlags, r, cg, flags, opt, widget);
            }

            break;
        }

        case CE_PushButton: {
            TQPushButton *button = (TQPushButton *)widget;

            const bool isDefault = enabled && (elementFlags & CEF_IsDefault);

            if ( elementFlags & CEF_IsFlat )
                flatMode = true;

            TQColorGroup g2 = cg;
            if (isDefault)
                g2.setColor(TQColorGroup::Background, cg.background().dark(120) );
            drawPrimitive(PE_ButtonBevel, p, ceData, elementFlags,
                    isDefault?TQRect(r.x()+1,r.y()+1,r.width()-2,r.height()-2):r,
                    g2, flags, TQStyleOption(button) );

            if (isDefault ) {
                drawPrimitive(PE_ButtonDefault, p, ceData, elementFlags, r, cg, flags);
            }

            break;
        }

        case CE_PushButtonLabel:
        {
            int x, y, w, h;
            r.rect( &x, &y, &w, &h );

            bool active = (elementFlags & CEF_IsOn) || (elementFlags & CEF_IsDown);
            bool cornArrow = false;

            // Shift button contents if pushed.
            if ( active )
            {
                x += pixelMetric(PM_ButtonShiftHorizontal, ceData, elementFlags, widget);
                y += pixelMetric(PM_ButtonShiftVertical, ceData, elementFlags, widget);
                flags |= Style_Sunken;
            }

            // Does the button have a popup menu?
            if ( elementFlags & CEF_IsMenuWidget )
            {
                int dx = pixelMetric( PM_MenuButtonIndicator, ceData, elementFlags, widget );
                if ( !ceData.iconSet.isNull() &&
                    (dx + ceData.iconSet.pixmap (TQIconSet::Small, TQIconSet::Normal, TQIconSet::Off ).width()) >= w )
                {
                    cornArrow = true; //To little room. Draw the arrow in the corner, don't adjust the widget
                }
                else
                {
		    p->setPen(cg.buttonText());
                    drawPrimitive( PE_ArrowDown, p, ceData, elementFlags, visualRect( TQRect(x + w - dx - 8, y + 2, dx, h - 4), r ),
                                cg, flags, opt );
                    w -= dx;
                }
            }

            // Draw the icon if there is one
            if ( !ceData.iconSet.isNull() )
            {
                TQIconSet::Mode  mode  = TQIconSet::Disabled;
                TQIconSet::State state = TQIconSet::Off;

                if (elementFlags & CEF_IsEnabled)
                    mode = (elementFlags & CEF_HasFocus) ? TQIconSet::Active : TQIconSet::Normal;
                if ((elementFlags & CEF_BiState) && (elementFlags & CEF_IsOn))
                    state = TQIconSet::On;

                TQPixmap pixmap = ceData.iconSet.pixmap( TQIconSet::Small, mode, state );

                if (ceData.text.isEmpty() && ceData.fgPixmap.isNull())
                    p->drawPixmap( x + w/2 - pixmap.width()/2, y + h / 2 - pixmap.height() / 2,
                                    pixmap );
                else
                    p->drawPixmap( x + 4, y + h / 2 - pixmap.height() / 2, pixmap );

                if (cornArrow) //Draw over the icon
                    drawPrimitive( PE_ArrowDown, p, ceData, elementFlags, visualRect( TQRect(x + w - 6, x + h - 6, 7, 7), r ),
                                cg, flags, opt );


                int  pw = pixmap.width();
                x += pw + 4;
                w -= pw + 4;
            }

            // Make the label indicate if the button is a default button or not
            drawItem( p, TQRect(x, y, w, h), AlignCenter|ShowPrefix, ceData.colorGroup,
                        (elementFlags & CEF_IsEnabled), (ceData.fgPixmap.isNull())?NULL:&ceData.fgPixmap, ceData.text, -1,
                        &ceData.colorGroup.buttonText() );


            if ( flags & Style_HasFocus )
                drawPrimitive( PE_FocusRect, p, ceData, elementFlags,
                                visualRect( subRect( SR_PushButtonFocusRect, ceData, elementFlags, widget ), ceData, elementFlags ),
                                cg, flags );
            break;
        }

    // MENUBAR ITEM (sunken panel on mouse over)
    // -----------------------------------------
        case CE_MenuBarItem: {
            TQMenuItem *mi = opt.menuItem();
            bool active  = flags & Style_Active;
            bool focused = flags & Style_HasFocus;
            bool down = flags & Style_Down;
            const int text_flags =
                AlignVCenter | AlignHCenter | ShowPrefix | DontClip | SingleLine;

            p->fillRect(r, cg.background());

            if (active && focused) {
                if (down) {
                    drawPrimitive(PE_ButtonTool, p, ceData, elementFlags, r, cg, flags|Style_Down, opt);
                } else {
                    drawPrimitive(PE_ButtonTool, p, ceData, elementFlags, r, cg, flags, opt);
                }
            }
            else
            {
                //renderWindowBackground( p, widget->parentWidget()->rect(), cg );
                renderWindowBackground( p, r, widget->parentWidget()->rect(), cg, r.top(), r.left() );
            }

            p->setPen(cg.foreground() );
            p->drawText(r, text_flags, mi->text());
            break;
        }

    // POPUPMENU ITEM (highlighted on mouseover)
    // ------------------------------------------
        case CE_PopupMenuItem: {
            TQMenuItem *mi = opt.menuItem();

            if ( !mi )
            {
                // Don't leave blank holes if we set NoBackground for the TQPopupMenu.
                // This only happens when the popupMenu spans more than one column.
                if ( ceData.bgPixmap.isNull() )
                    p->fillRect( r, cg.background().light( 105 ) );

                break;
            }
            int  tab        = opt.tabWidth();
            int  checkcol   = opt.maxIconWidth();
            bool enabled    = mi->isEnabled();
            bool checkable  = (elementFlags & CEF_IsCheckable);
            bool active     = flags & Style_Active;
            bool etchtext   = styleHint( SH_EtchDisabledText, ceData, elementFlags );
            bool reverse    = TQApplication::reverseLayout();
            if ( checkable )
                checkcol = TQMAX( checkcol, 20 );

            // Draw the menu item background
            if (active) {
                if (enabled) {
                    renderSurface(p, r, cg.background(), cg.highlight(), cg.highlight(),
                            _contrast+3, Draw_Top|Draw_Bottom|Is_Horizontal);
                }
                else {
                    if ( ceData.bgPixmap.isNull() )
                        p->drawPixmap( r.topLeft(), ceData.bgPixmap, r );
                    else p->fillRect( r, cg.background().light(105) );
                    if(_drawFocusRect)
                        p->drawWinFocusRect( r );
                }
            }
            // Draw the transparency pixmap
            else if ( !ceData.bgPixmap.isNull() )
                p->drawPixmap( r.topLeft(), ceData.bgPixmap, r );
            // Draw a solid background
            else
                p->fillRect( r, cg.background().light( 105 ) );
            // Are we a menu item separator?
            if ( mi->isSeparator() )
            {
                p->setPen( cg.mid() );
                p->drawLine( r.x()+5, r.y() + 1, r.right()-5, r.y() + 1 );
                p->setPen( cg.light() );
                p->drawLine( r.x()+5, r.y() + 2, r.right()-5 , r.y() + 2 );
                break;
            }

            TQRect cr = visualRect( TQRect( r.x() + 2, r.y() + 2, checkcol - 1, r.height() - 4 ), r );
            // Do we have an icon?
            if ( mi->iconSet() )
            {
                TQIconSet::Mode mode;

                // Select the correct icon from the iconset
                if (active)
                    mode = enabled?TQIconSet::Active:TQIconSet::Disabled;
                else
                    mode = enabled?TQIconSet::Normal:TQIconSet::Disabled;

                // Do we have an icon and are checked at the same time?
                // Then draw a "pressed" background behind the icon
                if ( checkable && /*!active &&*/ mi->isChecked() )
                    drawPrimitive(PE_MenuItemIndicatorIconFrame, p, ceData, elementFlags, r, cg, flags, opt);
                // Draw the icon
                TQPixmap pixmap = mi->iconSet()->pixmap(TQIconSet::Small, mode);
                TQRect pmr( 0, 0, pixmap.width(), pixmap.height() );
                pmr.moveCenter( cr.center() );
                p->drawPixmap( pmr.topLeft(), pixmap );
            }

            // Are we checked? (This time without an icon)
            else if ( checkable && mi->isChecked() )
            {
                // We only have to draw the background if the menu item is inactive -
                // if it's active the "pressed" background is already drawn
            // if ( ! active )
                    drawPrimitive(PE_MenuItemIndicatorFrame, p, ceData, elementFlags, r, cg, flags, opt);

                drawPrimitive(PE_MenuItemIndicatorCheck, p, ceData, elementFlags, r, cg, flags, opt);
            }

            // Time to draw the menu item label...
            int xm = 2 + checkcol + 2; // X position margin

            int xp = reverse ? // X position
                    r.x() + tab + rightBorder + itemHMargin + itemFrame - 1 :
                    r.x() + xm;

            int offset = reverse ? -1 : 1; // Shadow offset for etched text

            // Label width (minus the width of the accelerator portion)
            int tw = r.width() - xm - tab - arrowHMargin - itemHMargin * 3 - itemFrame + 1;

            // Set the color for enabled and disabled text
            // (used for both active and inactive menu items)
            p->setPen( enabled ? cg.buttonText() : cg.mid() );

            // This color will be used instead of the above if the menu item
            // is active and disabled at the same time. (etched text)
            TQColor discol = cg.mid();

            // Does the menu item draw it's own label?
            if ( mi->custom() ) {
                int m = 2;
                // Save the painter state in case the custom
                // paint method changes it in some way
                p->save();

                // Draw etched text if we're inactive and the menu item is disabled
                if ( etchtext && !enabled && !active ) {
                    p->setPen( cg.light() );
                    mi->custom()->paint( p, cg, active, enabled, xp+offset, r.y()+m+1, tw, r.height()-2*m );
                    p->setPen( discol );
                }
                mi->custom()->paint( p, cg, active, enabled, xp, r.y()+m, tw, r.height()-2*m );
                p->restore();
            }
            else {
                // The menu item doesn't draw it's own label
                TQString s = mi->text();
                // Does the menu item have a text label?
                if ( !s.isNull() ) {
                    int t = s.find( '\t' );
                    int m = 2;
                    int text_flags = AlignVCenter | ShowPrefix | DontClip | SingleLine;
                    text_flags |= reverse ? AlignRight : AlignLeft;

                    //TQColor draw = cg.text();
                    TQColor draw = (active && enabled) ? cg.highlightedText () : cg.foreground();
                    p->setPen(draw);


                    // Does the menu item have a tabstop? (for the accelerator text)
                    if ( t >= 0 ) {
                        int tabx = reverse ? r.x() + rightBorder + itemHMargin + itemFrame :
                            r.x() + r.width() - tab - rightBorder - itemHMargin - itemFrame;

                        // Draw the right part of the label (accelerator text)
                        if ( etchtext && !enabled ) {
                            // Draw etched text if we're inactive and the menu item is disabled
                            p->setPen( cg.light() );
                            p->drawText( tabx+offset, r.y()+m+1, tab, r.height()-2*m, text_flags, s.mid( t+1 ) );
                            p->setPen( discol );
                        }
                        p->drawText( tabx, r.y()+m, tab, r.height()-2*m, text_flags, s.mid( t+1 ) );
                        s = s.left( t );
                    }

                    // Draw the left part of the label (or the whole label
                    // if there's no accelerator)
                    if ( etchtext && !enabled ) {
                        // Etched text again for inactive disabled menu items...
                        p->setPen( cg.light() );
                        p->drawText( xp+offset, r.y()+m+1, tw, r.height()-2*m, text_flags, s, t );
                        p->setPen( discol );
                    }


                    p->drawText( xp, r.y()+m, tw, r.height()-2*m, text_flags, s, t );

                    p->setPen(cg.text());

                }

                // The menu item doesn't have a text label
                // Check if it has a pixmap instead
                else if ( mi->pixmap() ) {
                    TQPixmap *pixmap = mi->pixmap();

                    // Draw the pixmap
                    if ( pixmap->depth() == 1 )
                        p->setBackgroundMode( TQt::OpaqueMode );

                    int diffw = ( ( r.width() - pixmap->width() ) / 2 )
                                    + ( ( r.width() - pixmap->width() ) % 2 );
                    p->drawPixmap( r.x()+diffw, r.y()+1, *pixmap );

                    if ( pixmap->depth() == 1 )
                        p->setBackgroundMode( TQt::TransparentMode );
                }
            }

            // Does the menu item have a submenu?
            if ( mi->popup() ) {
                PrimitiveElement arrow = reverse ? PE_ArrowLeft : PE_ArrowRight;
                int dim = pixelMetric(PM_MenuButtonIndicator, ceData, elementFlags) - 1;
                TQRect vr = visualRect( TQRect( r.x() + r.width() - 5 - 1 - dim,
                            r.y() + r.height() / 2 - dim / 2, dim, dim), r );

                // Draw an arrow at the far end of the menu item
                if ( active ) {
                    if ( enabled )
                        discol = cg.buttonText();

                    TQColorGroup g2( discol, cg.highlight(), white, white,
                                    enabled ? white : discol, discol, white );

                    drawPrimitive( arrow, p, ceData, elementFlags, vr, g2, Style_Enabled );
                } else
                    drawPrimitive( arrow, p, ceData, elementFlags, vr, cg,
                            enabled ? Style_Enabled : Style_Default );
            }
            break;
        }

    // Menu and dockwindow empty space
    //
        case CE_DockWindowEmptyArea:
            p->fillRect(r, cg.background());
            //renderWindowBackground( p, widget->topLevelWidget()->rect(), cg );
            if( CHECKBGPIXMAP(widget->parentWidget()) )
            {
                // FIXME!!
                //renderWindowBackground( p, widget->parentWidget()->rect(), cg );
            }
            break;

        case CE_MenuBarEmptyArea:
            //p->fillRect(r, cg.background());

            /*{
                TQColor c1(cg.background());
                TQColor c2(cg.background().light(250));

                renderGradient(p, r, c1, c2, true);
            }*/
                renderWindowBackground( p, widget->topLevelWidget()->rect(), cg );

//             if ( _drawToolBarSeparator ) {
//                 p->setPen( getColor(cg, PanelDark) );
//                 p->drawLine( r.left(), r.bottom(), r.right(), r.bottom() );
//             }

            break;

    // more empty space
        /*case CE_CheckBoxLabel:
        case CE_RadioButtonLabel:*/
        /*case CE_HeaderLabel:
        {
            if( CHECKBGPIXMAP(widget->parentWidget()) )
            {
                // FIXME!!
                renderWindowBackground( p, widget->parentWidget()->rect(), cg );
            }
            
            TDEStyle::drawControl(element, p, ceData, elementFlags, r, cg, flags, opt, widget);
            
            break;
        }*/

        default:
        
            /*if(widget->isTopLevel())
            {
                renderWindowBackground( p, widget->parentWidget()->rect(), cg );
                break;
            }*/
        
          TDEStyle::drawControl(element, p, ceData, elementFlags, r, cg, flags, opt, widget);
    }
}

void OxygenStyle::drawControlMask(ControlElement element,
                                  TQPainter *p,
                                  const TQStyleControlElementData &ceData,
                                  ControlElementFlags elementFlags,
                                  const TQRect &r,
                                  const TQStyleOption &opt,
                                  const TQWidget *w) const
{
    switch (element) {
        case CE_PushButton: {
                    p->fillRect (r, color0);
                    renderMask(p, r, color1,
                            Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight);
                    break;
        }

        default: {
            TDEStyle::drawControlMask (element, p, ceData, elementFlags, r, opt, w);
        }
    }
}

void OxygenStyle::drawComplexControlMask(ComplexControl c,
                                         TQPainter *p,
                                         const TQStyleControlElementData &ceData,
                                         const ControlElementFlags elementFlags,
                                         const TQRect &r,
                                         const TQStyleOption &o,
                                         const TQWidget *w) const
{
    switch (c) {
        case CC_SpinWidget:
        case CC_ListView:
        case CC_ComboBox: {
                p->fillRect (r, color0);
                renderMask(p, r, color1,
                        Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight);
            break;
        }
        default: {
            TDEStyle::drawComplexControlMask (c, p, ceData, elementFlags, r, o, w);
        }
    }
}

void OxygenStyle::drawComplexControl(ComplexControl control,
                                     TQPainter *p,
                                     const TQStyleControlElementData &ceData,
                                     ControlElementFlags elementFlags,
                                     const TQRect &r,
                                     const TQColorGroup &cg,
                                     SFlags flags,
                                     SCFlags controls,
                                     SCFlags active,
                                     const TQStyleOption& opt,
                                     const TQWidget *widget) const
{
    const bool reverseLayout = TQApplication::reverseLayout();

    const bool enabled = (flags & Style_Enabled);

    switch(control) {
    // COMBOBOX
    // --------
        case CC_ComboBox: {
            static const unsigned int handleWidth = 15;

            const TQComboBox *cb = dynamic_cast<const TQComboBox *>(widget);
            bool editable = false;
            bool hasFocus = false;
            editable = (elementFlags & CEF_IsEditable);
            hasFocus = (elementFlags & CEF_HasFocus);

            const TQColor buttonColor = enabled?cg.button():cg.background();
            const TQColor inputColor = enabled?(editable?cg.base():cg.button())
                                              :cg.background();

            uint contourFlags = 0;
            if( tdehtmlWidgets.contains(cb) )
                contourFlags |= Draw_AlphaBlend;
            
            if (_inputFocusHighlight && hasFocus && editable && enabled)
            {
                TQRect editField = querySubControlMetrics(control, ceData, elementFlags, SC_ComboBoxEditField, TQStyleOption::Default, widget);
                TQRect editFrame = r;
                TQRect buttonFrame = r;
                
                uint editFlags = contourFlags;
                uint buttonFlags = contourFlags;
                
                // Hightlight only the part of the contour next to the control button
                if (reverseLayout)
                {
                    // querySubControlMetrics doesn't work right for reverse Layout
                    int dx = r.right() - editField.right();
                    editFrame.setLeft(editFrame.left() + dx);
                    buttonFrame.setRight(editFrame.left() - 1);
                    editFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|Round_BottomRight;
                    buttonFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_BottomLeft;
                }
                else
                {
                    editFrame.setRight(editField.right());
                    buttonFrame.setLeft(editField.right() + 1);
                    
                    editFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_BottomLeft;
                    buttonFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|Round_BottomRight;
                }
                renderContour(p, editFrame, cg.background(),  getColor(cg,FocusHighlight,enabled), editFlags);
                renderContour(p, buttonFrame, cg.background(), 
                              getColor(cg, ButtonContour, enabled), buttonFlags); 
            }
            else
            {
                contourFlags |= Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                    Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight;
                renderContour(p, r, cg.background(), getColor(cg, ButtonContour, enabled), contourFlags);
            }
            //extend the contour: between input and handler...
            p->setPen(alphaBlendColors(cg.background(), getColor(cg, ButtonContour, enabled), 50) );
            if(reverseLayout) {
                p->drawLine(r.left()+1+handleWidth, r.top()+1, r.left()+1+handleWidth, r.bottom()-1);
            } else {
                p->drawLine(r.right()-handleWidth-1, r.top()+1, r.right()-handleWidth-1, r.bottom()-1);
            }

            const TQRect RbuttonSurface(reverseLayout?r.left()+1:r.right()-handleWidth, r.top()+1,
                                        handleWidth, r.height()-2);
            const TQRect RcontentSurface(reverseLayout?r.left()+1+handleWidth+1:r.left()+1, r.top()+1,
                                         r.width()-handleWidth-3, r.height()-2);

            // handler

            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
            if(reverseLayout) {
                surfaceFlags |= Round_UpperLeft|Round_BottomLeft;
            } else {
                surfaceFlags |= Round_UpperRight|Round_BottomRight;
            }

            if (flags & Style_MouseOver) {
                surfaceFlags |= Is_Highlight;
                if(editable) surfaceFlags |= Highlight_Left|Highlight_Right;
                surfaceFlags |= Highlight_Top|Highlight_Bottom;
            }
            renderSurface(p, RbuttonSurface,
                           cg.background(), buttonColor, getColor(cg,MouseOverHighlight), enabled?_contrast+3:(_contrast/2),
                           surfaceFlags);

            if(!editable) {
                surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
                if(reverseLayout) {
                    surfaceFlags |= Round_UpperRight|Round_BottomRight;
                } else {
                    surfaceFlags |= Round_UpperLeft|Round_BottomLeft;
                }

                if (flags & Style_MouseOver) {
                    surfaceFlags |= Is_Highlight;
                    surfaceFlags |= Highlight_Top|Highlight_Bottom;
                }
                renderSurface(p, RcontentSurface,
                               cg.background(), buttonColor, getColor(cg,MouseOverHighlight), enabled?_contrast+3:(_contrast/2),
                               surfaceFlags);
                if (hasFocus) {
                    drawPrimitive(PE_FocusRect, p, ceData, elementFlags,
                        TQRect(RcontentSurface.x() + 2,
                            RcontentSurface.y() + 2,
                            RcontentSurface.width() - 4,
                            RcontentSurface.height() - 4), cg);
                }
            } else {
                // thin frame around the input area
                if (_inputFocusHighlight && hasFocus && editable && enabled)
                {
                  p->setPen( getColor(cg,FocusHighlight).dark(130) );
                }
                else
                {
                  p->setPen(inputColor.dark(130) );
                }
                p->drawLine(RcontentSurface.x(), reverseLayout?RcontentSurface.y():RcontentSurface.y()+1,
                        RcontentSurface.x(), reverseLayout?RcontentSurface.bottom():RcontentSurface.bottom()-1);
                p->drawLine(RcontentSurface.x()+1, RcontentSurface.y(),
                        reverseLayout?RcontentSurface.right()-1:RcontentSurface.right(), RcontentSurface.y() );
                if (_inputFocusHighlight && hasFocus && editable && enabled)
                {
                  p->setPen( getColor(cg,FocusHighlight).light(130) );
                }
                else
                {
                  p->setPen(inputColor.light(130) );
                }
                p->drawLine(reverseLayout?RcontentSurface.x():RcontentSurface.x()+1, RcontentSurface.bottom(),
                        reverseLayout?RcontentSurface.right()-1:RcontentSurface.right(), RcontentSurface.bottom() );
                p->drawLine(RcontentSurface.right(), RcontentSurface.top()+1,
                        RcontentSurface.right(), RcontentSurface.bottom()-1 );

                // input area
                p->fillRect(RcontentSurface.x()+1, RcontentSurface.y()+1,
                        RcontentSurface.width()-2, RcontentSurface.height()-2, inputColor );
            }

            p->setPen(cg.foreground());
            drawPrimitive(PE_SpinWidgetDown, p, ceData, elementFlags, RbuttonSurface, cg, Style_Default|Style_Enabled|Style_Raised);

            // TQComboBox draws the text using cg.text(), we can override this
            // from here
            p->setPen( cg.buttonText() );
            p->setBackgroundColor( cg.button() );
            break;
        }

    // TOOLBUTTON
    // ----------
        case CC_ToolButton: {
            TQRect button, menuarea;
            button   = querySubControlMetrics(control, ceData, elementFlags, SC_ToolButton, opt, widget);
            menuarea = querySubControlMetrics(control, ceData, elementFlags, SC_ToolButtonMenu, opt, widget);

            SFlags bflags = flags,
                    mflags = flags;

            if (kornMode) {
                drawPrimitive(PE_ButtonTool, p, ceData, elementFlags, button, cg, bflags, opt);
                break;
            } else {
                // don't want to have the buttoncolor as the background...
                p->fillRect(r, cg.background());
                bflags &= ~Style_MouseOver;
            }

            if (active & SC_ToolButton)
                bflags |= Style_Down;

            if (active & SC_ToolButtonMenu)
                mflags |= Style_Down;

            if (controls & SC_ToolButton) {
            // If we're pressed, on, or raised...
                if (bflags & (Style_Down | Style_On | Style_Raised) || (flags & Style_MouseOver) ) {
                    drawPrimitive(PE_ButtonTool, p, ceData, elementFlags, button, cg, bflags, opt);
                } else if ((elementFlags & CEF_HasParentWidget) &&
                            !ceData.parentWidgetData.bgPixmap.isNull()) {
                    TQPixmap pixmap = ceData.parentWidgetData.bgPixmap;
                    p->drawTiledPixmap( r, pixmap, ceData.pos );
                }
            }

            // Draw a toolbutton menu indicator if required
            if (controls & SC_ToolButtonMenu) {
                if (mflags & (Style_Down | Style_On | Style_Raised)) {
                    drawPrimitive(PE_ButtonDropDown, p, ceData, elementFlags, menuarea, cg, mflags, opt);
                }
                drawPrimitive(PE_ArrowDown, p, ceData, elementFlags, menuarea, cg, mflags, opt);
            }

            if ((elementFlags & CEF_HasFocus) && !(elementFlags & CEF_HasFocusProxy)) {
                TQRect fr = ceData.rect;
                fr.addCoords(2, 2, -2, -2);
                drawPrimitive(PE_FocusRect, p, ceData, elementFlags, fr, cg);
            }

            // Set the color for the ToolButton menu indicator
            p->setPen(cg.buttonText() );

            break;
        }

    // SPINWIDGETS
    // -----------
        case CC_SpinWidget: {
            static const unsigned int handleWidth = 15;

            const TQSpinWidget *sw = dynamic_cast<const TQSpinWidget *>(widget);
            SFlags sflags = flags;
            PrimitiveElement pe;

            bool hasFocus = false;
            if (sw)
                hasFocus = sw->hasFocus();

            const TQColor buttonColor = enabled?cg.button():cg.background();
            const TQColor inputColor = enabled?cg.base():cg.background();

            // contour
            const bool heightDividable = ((r.height()%2) == 0);
            if (_inputFocusHighlight && hasFocus && enabled)
            {
                TQRect editField = querySubControlMetrics(control, ceData, elementFlags, SC_SpinWidgetEditField, TQStyleOption::Default, widget);
                TQRect editFrame = r;
                TQRect buttonFrame = r;
                
                uint editFlags = 0;
                uint buttonFlags = 0;
                
                // Hightlight only the part of the contour next to the control buttons
                if (reverseLayout)
                {
                    // querySubControlMetrics doesn't work right for reverse Layout
                    int dx = r.right() - editField.right();
                    editFrame.setLeft(editFrame.left() + dx);
                    buttonFrame.setRight(editFrame.left() - 1);
                    editFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|Round_BottomRight;
                    buttonFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_BottomLeft;
                }
                else
                {
                    editFrame.setRight(editField.right());
                    buttonFrame.setLeft(editField.right() + 1);
                    
                    editFlags |= Draw_Left|Draw_Top|Draw_Bottom|Round_UpperLeft|Round_BottomLeft;
                    buttonFlags |= Draw_Right|Draw_Top|Draw_Bottom|Round_UpperRight|Round_BottomRight;
                }
                renderContour(p, editFrame, cg.background(), cg.highlight(), editFlags);
                renderContour(p, buttonFrame, cg.background(), 
                              getColor(cg, ButtonContour, enabled), buttonFlags);
            }
            else
            {
                renderContour(p, querySubControlMetrics(control, ceData, elementFlags, SC_SpinWidgetFrame, TQStyleOption::Default, widget),
                              cg.background(), getColor(cg, ButtonContour, enabled) );
            }
            p->setPen(alphaBlendColors(cg.background(), getColor(cg, ButtonContour, enabled), 50) );
            p->drawLine(reverseLayout?r.left()+1+handleWidth:r.right()-handleWidth-1, r.top()+1,
                    reverseLayout?r.left()+1+handleWidth:r.right()-handleWidth-1, r.bottom()-1);
            p->drawLine(reverseLayout?r.left()+1:r.right()-handleWidth, r.top()+1+(r.height()-2)/2,
                    reverseLayout?r.left()+handleWidth:r.right()-1, r.top()+1+(r.height()-2)/2);
            if(heightDividable)
                p->drawLine(reverseLayout?r.left()+1:r.right()-handleWidth, r.top()+1+(r.height()-2)/2-1,
                        reverseLayout?r.left()+handleWidth:r.right()-1, r.top()+1+(r.height()-2)/2-1);

            // surface
            TQRect upRect = TQRect(reverseLayout?r.left()+1:r.right()-handleWidth, r.top()+1,
                    handleWidth, (r.height()-2)/2);
            TQRect downRect = TQRect(reverseLayout?r.left()+1:r.right()-handleWidth,
                    heightDividable?r.top()+1+((r.height()-2)/2):r.top()+1+((r.height()-2)/2)+1,
                    handleWidth, ((r.height()-2)/2) );
            if(heightDividable) {
                upRect = TQRect(upRect.left(), upRect.top(), upRect.width(), upRect.height()-1 );
                downRect = TQRect(downRect.left(), downRect.top()+1, downRect.width(), downRect.height()-1 );
            }

            uint surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
            if(reverseLayout) {
                surfaceFlags |= Round_UpperLeft;
            } else {
                surfaceFlags |= Round_UpperRight;
            }
            if (sflags & Style_MouseOver) {
                surfaceFlags |= Is_Highlight;
                surfaceFlags |= Highlight_Top|Highlight_Left|Highlight_Right;
            }
            if (active==SC_SpinWidgetUp) surfaceFlags|=Is_Sunken;
            if(!enabled) surfaceFlags|=Is_Disabled;
            renderSurface(p, upRect, cg.background(), buttonColor, getColor(cg,MouseOverHighlight),
                           _contrast, surfaceFlags);
            surfaceFlags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|Is_Horizontal;
            if(reverseLayout) {
                surfaceFlags |= Round_BottomLeft;
            } else {
                surfaceFlags |= Round_BottomRight;
            }
            if (sflags & Style_MouseOver) {
                surfaceFlags |= Is_Highlight;
                surfaceFlags |= Highlight_Bottom|Highlight_Left|Highlight_Right;
            }
            if (active==SC_SpinWidgetDown) surfaceFlags|=Is_Sunken;
            if(!enabled) surfaceFlags|=Is_Disabled;
            renderSurface(p, downRect, cg.background(), buttonColor, getColor(cg,MouseOverHighlight),
                           _contrast, surfaceFlags);

            // icons...
            sflags = Style_Default | Style_Enabled;
            if (active == SC_SpinWidgetUp) {
                sflags |= Style_On;
                sflags |= Style_Sunken;
            } else
                sflags |= Style_Raised;
            if (sw->buttonSymbols() == TQSpinWidget::PlusMinus)
                pe = PE_SpinWidgetPlus;
            else
                pe = PE_SpinWidgetUp;
            p->setPen(cg.foreground());
            drawPrimitive(pe, p, ceData, elementFlags, upRect, cg, sflags);

            sflags = Style_Default | Style_Enabled;
            if (active == SC_SpinWidgetDown) {
                sflags |= Style_On;
                sflags |= Style_Sunken;
            } else
                sflags |= Style_Raised;
            if (sw->buttonSymbols() == TQSpinWidget::PlusMinus)
                pe = PE_SpinWidgetMinus;
            else
                pe = PE_SpinWidgetDown;
            p->setPen(cg.foreground());
            drawPrimitive(pe, p, ceData, elementFlags, downRect, cg, sflags);

            // thin frame around the input area
            const TQRect Rcontent = TQRect(reverseLayout?r.left()+1+handleWidth+1:r.left()+1, r.top()+1,
                    r.width()-1-2-handleWidth, r.height()-2);
            if (_inputFocusHighlight && hasFocus && enabled)
            {
              p->setPen( getColor(cg,FocusHighlight).dark(130) );
            }
            else
            {
              p->setPen(inputColor.dark(130) );
            }
            p->drawLine(Rcontent.left(), reverseLayout?Rcontent.top():Rcontent.top()+1,
                    Rcontent.left(), reverseLayout?Rcontent.bottom():Rcontent.bottom()-1 );
            p->drawLine(Rcontent.left()+1, Rcontent.top(),
                    reverseLayout?Rcontent.right()-1:Rcontent.right(), Rcontent.top() );
            if (_inputFocusHighlight && hasFocus && enabled)
            {
              p->setPen( getColor(cg,FocusHighlight).light(130) );
            }
            else
            {
              p->setPen(inputColor.light(130) );
            }
            p->drawLine(Rcontent.left()+1, Rcontent.bottom(), Rcontent.right()-1, Rcontent.bottom() );
            p->drawLine(Rcontent.right(), Rcontent.top()+1,
                    Rcontent.right(), reverseLayout?Rcontent.bottom()-1:Rcontent.bottom() );

            break;
        }
        
        /*case CC_ListView:
        //REMOVE!!!
            //p->fillRect (r, color0);
            renderGradient(p, r, color0, color1);
            break;*/
        
        default:
            TDEStyle::drawComplexControl(control, p, ceData, elementFlags,
                                        r, cg, flags, controls,
                                        active, opt, widget);
            break;
    }
}


TQRect OxygenStyle::subRect(SubRect r, const TQStyleControlElementData &ceData, const ControlElementFlags elementFlags, const TQWidget *widget) const
{
    switch (r) {
        case SR_ComboBoxFocusRect: {
            return querySubControlMetrics( CC_ComboBox, ceData, elementFlags, SC_ComboBoxEditField, TQStyleOption::Default, widget );
        }

        // Don't use TDEStyles progressbar subrect
        // TODO:
        case SR_ProgressBarGroove: {
            return TQRect(ceData.rect);
        }
        case SR_ProgressBarContents:
        case SR_ProgressBarLabel: {
            TQRect rw = ceData.rect;
            return TQRect(rw.left()+2, rw.top()+2, rw.width()-4, rw.height()-4 );
        }

        default: {
            return TDEStyle::subRect(r, ceData, elementFlags, widget);
        }
    }
}

TQRect OxygenStyle::querySubControlMetrics(ComplexControl control,
                                          const TQStyleControlElementData &ceData,
                                          ControlElementFlags elementFlags,
                                          SubControl subcontrol,
                                          const TQStyleOption &opt,
                                          const TQWidget *widget) const
{
    TQRect r(ceData.rect);
    switch (control) {
        case CC_ComboBox: {
            switch (subcontrol) {
                case SC_ComboBoxEditField: {
                    // TODO: is the handler width in pixelmetric?
                    return TQRect(r.left()+2, r.top()+2, r.width()-4-15-1, r.height()-4);
                }
                default: {
                    return TDEStyle::querySubControlMetrics(control, ceData, elementFlags, subcontrol, opt, widget);
                }
            }
            break;
        }
        case CC_SpinWidget: {
            const int fw = 2; // Frame width...

            const bool heightDividable = ((r.height()%2) == 0);

            TQSize bs;
            if(heightDividable) {
                bs.setHeight(TQMAX(8, (r.height()-2)/2));
            } else {
                bs.setHeight(TQMAX(8, (r.height()-2-1)/2));
            }
            bs.setWidth(15);

            const int buttonsLeft = /*reverseLayout?r.left()+1:*/r.right()-bs.width();

            switch (subcontrol) {
                case SC_SpinWidgetUp: {
                    return TQRect(buttonsLeft, r.top()+1, bs.width(), bs.height() );
                }
                case SC_SpinWidgetDown: {
                    if(heightDividable) {
                        return TQRect(buttonsLeft, r.top()+1+bs.height(),
                                bs.width(), r.height()-(bs.height()+2) );
                    } else {
                        return TQRect(buttonsLeft, r.top()+1+bs.height()+1,
                                bs.width(), r.height()-(bs.height()+2+1) );
                    }
                }
                case SC_SpinWidgetFrame: {
                    return TQRect(r.left(), r.top(), r.width(), r.height() );
                }
                case SC_SpinWidgetEditField: {
                    return TQRect(r.left()+fw, r.top()+fw,
                            r.width()-(bs.width()+1+2*fw), r.height()-2*fw);
                }
                case SC_SpinWidgetButtonField: {
                    return TQRect(buttonsLeft, r.top()+1, bs.width(), r.height()-2);
                }
                default: {
                    return TDEStyle::querySubControlMetrics(control, ceData, elementFlags, subcontrol, opt, widget);
                }
            }
            break;
        }
        default: {
            return TDEStyle::querySubControlMetrics(control, ceData, elementFlags, subcontrol, opt, widget);
        }
    }
}

int OxygenStyle::pixelMetric(PixelMetric m, const TQStyleControlElementData &ceData, ControlElementFlags elementFlags, const TQWidget *widget) const
{
    switch(m) {
    // TABS
    // ----
        case PM_TabBarTabVSpace: {
            if (ceData.tabBarData.shape == TQTabBar::RoundedAbove ||
                ceData.tabBarData.shape == TQTabBar::RoundedBelow)
                return 12;
            else
                return 4;
        }

        case PM_TabBarTabOverlap: {
            return 1;
        }

    // extra space between menubar items
        case PM_MenuBarItemSpacing: {
            return 6;
        }

//     // extra space between toolbar items
//         case PM_ToolBarItemSpacing: {
//             return 4;
//         }

    // SCROLL BAR
        case PM_ScrollBarSliderMin: {
            return 21;
        }
        case PM_ScrollBarExtent: {
            return 14;
        }

        case PM_DockWindowSeparatorExtent:
            return 6;

    // SPLITTERS
    // ---------
        case PM_SplitterWidth: {
            return 6;
        }

    // PROGRESSBARS
    // ------------
        case PM_ProgressBarChunkWidth:
            return 10;

    // SLIDER
    // ------
        case PM_SliderLength:
            return 11;

    // MENU INDICATOR
    // --------------
        case PM_MenuButtonIndicator:
            return 8;

    // CHECKBOXES / RADIO BUTTONS
    // --------------------------
        case PM_ExclusiveIndicatorWidth:    // Radiobutton size
        case PM_ExclusiveIndicatorHeight:   // 13x13
        case PM_IndicatorWidth:             // Checkbox size
        case PM_IndicatorHeight:            // 13x13
            return 13;

    // FRAMES
    // ------
        case PM_SpinBoxFrameWidth:
            return 1;

        case PM_MenuBarFrameWidth:
            return 1;

        case PM_DefaultFrameWidth: {
            if(widget && ::tqt_cast<TQPopupMenu*>(widget))
                return 1;
            else
                return 2;
        }

        case PM_ButtonDefaultIndicator: {
            return 0;
        }

        case PM_ButtonMargin: {
            return 2;
        }

        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal: {
            return 1;
        }

        case PM_MenuIndicatorFrameHBorder:
        case PM_MenuIndicatorFrameVBorder:
        case PM_MenuIconIndicatorFrameHBorder:
        case PM_MenuIconIndicatorFrameVBorder:
            return 2;

        default:
            return TDEStyle::pixelMetric(m, ceData, elementFlags, widget);
    }
}


TQSize OxygenStyle::sizeFromContents(ContentsType t,
                                    const TQStyleControlElementData &ceData,
                                    ControlElementFlags elementFlags,
                                    const TQSize &s,
                                    const TQStyleOption &opt,
                                    const TQWidget *widget) const
{
    switch (t) {
        case CT_PopupMenuItem: {
            if (!widget || opt.isDefault())
                return s;

            const TQPopupMenu *popup = dynamic_cast<const TQPopupMenu *>(widget);
            TQMenuItem *mi = opt.menuItem();
            int maxpmw = opt.maxIconWidth();
            int w = s.width(), h = s.height();
            bool checkable = popup->isCheckable();

            if (mi->custom()) {
                w = mi->custom()->sizeHint().width();
                h = mi->custom()->sizeHint().height();
                if (!mi->custom()->fullSpan() )
                    h += 4;
            } else if (mi->widget()) {
            // don't change the size in this case.
            } else if (mi->isSeparator()) {
                w = 20;
                h = 2;
            } else {
                if (mi->pixmap()) {
                    h = TQMAX(h, mi->pixmap()->height() + 2);
                } else {
                    h = TQMAX(h, 16 + 2 );
                    h = TQMAX(h, popup->fontMetrics().height() + 4 );
                }

                if (mi->iconSet()) {
                    h = TQMAX(h, mi->iconSet()->pixmap(TQIconSet::Small, TQIconSet::Normal).height() + 2);
                }
            }

            if (!mi->text().isNull() && (mi->text().find('\t') >= 0)) {
                w += itemHMargin + itemFrame*2 + 7;
            } else if (mi->popup()) {
                w += 2 * arrowHMargin;
            }

            if (maxpmw) {
                w += maxpmw + 6;
            }
            if (checkable && maxpmw < 20) {
                w += 20 - maxpmw;
            }
            if (checkable || maxpmw > 0) {
                w += 12;
            }

            w += rightBorder;

            return TQSize(w, h);
        }

        case CT_PushButton:
        {
            const TQPushButton* btn = static_cast<const TQPushButton*>(widget);

            int w = s.width() + 2 * pixelMetric(PM_ButtonMargin, ceData, elementFlags, widget);
            int h = s.height() + 2 * pixelMetric(PM_ButtonMargin, ceData, elementFlags, widget);
            if ( btn->text().isEmpty() && s.width() < 32 ) return TQSize(w, h);

            return TQSize( w+25, h+5 );
        }

        case CT_ToolButton:
        {
            if(widget->parent() && ::tqt_cast<TQToolBar*>(widget->parent()) )
                return TQSize( s.width()+2*4, s.height()+2*4 );
            else
                return TDEStyle::sizeFromContents (t, ceData, elementFlags, s, opt, widget);
        }

        default:
            return TDEStyle::sizeFromContents (t, ceData, elementFlags, s, opt, widget);
    }

    return TDEStyle::sizeFromContents (t, ceData, elementFlags, s, opt, widget);
}

int OxygenStyle::styleHint( StyleHint stylehint,
                                   const TQStyleControlElementData &ceData,
                                   ControlElementFlags elementFlags,
                                   const TQStyleOption &option,
                                   TQStyleHintReturn* returnData,
                                   const TQWidget *widget ) const
{
    switch (stylehint) {
        case SH_PopupMenu_SubMenuPopupDelay:
            return 96; // Motif-like delay...

        case SH_ItemView_ChangeHighlightOnFocus:
            return 1;

	case SH_MenuIndicatorColumnWidth:
	{
		int  checkcol   = option.maxIconWidth();
		bool checkable = (elementFlags & CEF_IsCheckable);
	
		if ( checkable )
                	checkcol = TQMAX( checkcol, 20 );
	
		return checkcol;
	}

        default:
            return TDEStyle::styleHint(stylehint, ceData, elementFlags, option, returnData, widget);
    }
}

bool OxygenStyle::objectEventHandler( const TQStyleControlElementData &ceData, ControlElementFlags elementFlags, void* source, TQEvent *ev )
{
    if (TDEStyle::objectEventHandler(ceData, elementFlags, source, ev) )
        return true;

    if (ceData.widgetObjectTypes.contains("TQObject")) {
	TQObject* obj = reinterpret_cast<TQObject*>(source);

	if (!obj->isWidgetType() ) return false;

    TQWidget * widget = ::tqt_cast<TQWidget*>(obj);
    if( widget && widget->isTopLevel() && ev->type() == TQEvent::Resize )
    {
        if( CHECKBGPIXMAP(widget) )
        {
            TQPixmap * result = bgWidgets[widget];
            
            result->resize(widget->rect().width(),widget->rect().height());
            
            TQPainter p(result);
            p.fillRect(widget->rect(), widget->colorGroup().background());
            renderWindowBackground( &p, widget->rect(), widget->colorGroup() );
            
            // may not be necessary here
            widget->setBackgroundPixmap(*result);
            
            widget->repaint();
        }
    }
    
    if( ::tqt_cast<TQListView*>(widget) && ev->type() == TQEvent::Paint )
    //if( widget && paint )
    {
        TQPaintEvent * paint = dynamic_cast<TQPaintEvent*>(ev);
        //if( !paint->erased() )
        //    widget->erase(paint->region());
        //printf("TESTTT\n");
    }

	// Track show events for progress bars
	if ( _animateProgressBar && ::tqt_cast<TQProgressBar*>(obj) )
	{
		if ((ev->type() == TQEvent::Show) && !animationTimer->isActive())
		{
		animationTimer->start( 50, false );
		}
	}
	if ( !qstrcmp(obj->name(), "tde toolbar widget") )
	{
		TQWidget* lb = static_cast<TQWidget*>(obj);
		if (lb->backgroundMode() == TQt::PaletteButton)
		lb->setBackgroundMode(TQt::PaletteBackground);
		removeObjectEventHandler(ceData, elementFlags, source, this);
	}
    }

    return false;
}

TQColor OxygenStyle::getColor(const TQColorGroup &cg, const ColorType t, const bool enabled)const
{
    return getColor(cg, t, enabled?IsEnabled:IsDisabled);
}

TQColor OxygenStyle::getColor(const TQColorGroup &cg, const ColorType t, const WidgetState s)const
{
    const bool enabled = (s != IsDisabled) &&
            ((s == IsEnabled) || (s == IsPressed) || (s == IsHighlighted));
    const bool pressed = (s == IsPressed);
    const bool highlighted = (s == IsHighlighted);
    switch(t) {
        case ButtonContour:
            return enabled ? cg.button().dark(130+_contrast*8)
                           : cg.background().dark(120+_contrast*8);
        case DragButtonContour: {
            if(enabled) {
                if(pressed)
                    return cg.button().dark(130+_contrast*6); // bright
                else if(highlighted)
                    return cg.button().dark(130+_contrast*9); // dark
                else
                    return cg.button().dark(130+_contrast*8); // normal
            } else {
                return cg.background().dark(120+_contrast*8);
            }
        }
        case DragButtonSurface: {
            if(enabled) {
                if(pressed)
                    return cg.button().dark(100-_contrast);  // bright
                else if(highlighted)
                    return cg.button().light(100+_contrast); // dark
                else
                    return cg.button();                      // normal
            } else {
                return cg.background();
            }
        }
        case PanelContour:
            return cg.background().dark(160+_contrast*8);
        case PanelDark:
            return alphaBlendColors(cg.background(), cg.background().dark(120+_contrast*5), 110);
        case PanelDark2:
            return alphaBlendColors(cg.background(), cg.background().dark(110+_contrast*5), 110);
        case PanelLight:
            return alphaBlendColors(cg.background(), cg.background().light(120+_contrast*5), 110);
        case PanelLight2:
            return alphaBlendColors(cg.background(), cg.background().light(110+_contrast*5), 110);
        case MouseOverHighlight:
            if( _customOverHighlightColor )
                return _overHighlightColor;
            else
                return cg.highlight();
        case FocusHighlight:
            if( _customFocusHighlightColor )
                return _focusHighlightColor;
            else
                return cg.highlight();
        case CheckMark:
            if( _customCheckMarkColor )
                return _checkMarkColor;
            else
                return cg.foreground();
        default:
            return cg.background();
    }
}
