//////////////////////////////////////////////////////////////////////////////
// exampleclient.cc
// -------------------
// Oxygen window decoration for TDE
// Copyright (c) 2024 Glen Harpring
//
// Based on example KDE window decoration
// Copyright (c) 2003, 2004 David Johnson
// 
// Using button pixmap loading code from deKorator
// Copyright (C) 2005  moty rahamim <moty.rahamim@gmail.com>
//
// (which was maybe based on) KCMicons for KDE
// Copyright (c) 2000 Antonio Larrosa <larrosa@kde.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//////////////////////////////////////////////////////////////////////////////

#include <tdeconfig.h>
#include <tdeversion.h>
#include <tdeglobal.h>
#include <tdeglobalsettings.h>
#include <tdelocale.h>

#include <tqbitmap.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqpainter.h>
#include <tqtooltip.h>

#include <math.h>

#include "oxygenclient.h"

using namespace Oxygen;

// global constants

static const int BUTTONSIZE      = 21;
static const int DECOSIZE        = 8;
static const int TITLESIZE       = 18;
static const int FRAMESIZE       = 4;

// window button decorations

static const unsigned char close_bits[] = {
    0x00, 0x66, 0x7e, 0x3c, 0x3c, 0x7e, 0x66, 0x00};

static const unsigned char help_bits[] = {
    0x7e, 0x7e, 0x60, 0x78, 0x78, 0x00, 0x18, 0x18};

static const unsigned char max_bits[] = {
    0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00};

static const unsigned char min_bits[] = {
    0x00, 0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00};

static const unsigned char minmax_bits[] = {
    0x00, 0x02, 0x06, 0x0e, 0x1e, 0x3e, 0x7e, 0x00};

static const unsigned char stickydown_bits[] = {
    0x00, 0x18, 0x18, 0x7e, 0x7e, 0x18, 0x18, 0x00};

static const unsigned char sticky_bits[] = {
    0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00};

static const unsigned char above_on_bits[] = {
   0x7e, 0x18, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00 };

static const unsigned char above_off_bits[] = {
   0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x00, 0x00, 0x00 };

static const unsigned char below_on_bits[] = {
   0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x7e };

static const unsigned char below_off_bits[] = {
   0x00, 0x00, 0x00, 0x7e, 0x18, 0x7e, 0x3c, 0x18 };

static const unsigned char shade_on_bits[] = {
   0xff, 0xff, 0x81, 0xa5, 0x81, 0xa5, 0x81, 0xff };

static const unsigned char shade_off_bits[] = {
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


static TQPixmap * BUTTONSARR[ ButtonTypeCount ][ buttonStateCount ][ pixTypeCount ];


//////////////////////////////////////////////////////////////////////////////
// OxygenFactory Class                                                     //
//////////////////////////////////////////////////////////////////////////////

bool OxygenFactory::initialized_              = false;
TQt::AlignmentFlags OxygenFactory::titlealign_ = TQt::AlignHCenter;

extern "C" KDecorationFactory* create_factory()
{
    return new Oxygen::OxygenFactory();
}

//////////////////////////////////////////////////////////////////////////////
// OxygenFactory()
// ----------------
// Constructor

OxygenFactory::OxygenFactory()
{
    readConfig();
    initialized_ = true;
    
    int i,j,k;
    for ( i = 0; i < ButtonTypeCount; i++ )
    {
        for ( j = 0; j < buttonStateCount; j++ )
        {
            for ( k = 0; k < pixTypeCount; k++ )
            {
                BUTTONSARR[ i ][ j ][ k ] = new TQPixmap();
            }
        }
    }
    
    loadPixmaps();
}

void OxygenFactory::loadPixmaps()
{
    // FIXME: change this directory
    TQString btnPixDir("/home/nelg/dev/trinity/tde-style-oxygen/twin/buttons");

    BUTTONSARR[ ButtonHelp ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonHelp.png" );
    BUTTONSARR[ ButtonHelp ][ press ][ normal ] ->load( btnPixDir + "/press/buttonHelpPress.png" );
    BUTTONSARR[ ButtonHelp ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonHelpHover.png" );

    BUTTONSARR[ ButtonMax ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonMax.png" );
    BUTTONSARR[ ButtonMax ][ press ][ normal ] ->load( btnPixDir + "/press/buttonMaxPress.png" );
    BUTTONSARR[ ButtonMax ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonMaxHover.png" );
    
    BUTTONSARR[ ButtonMax ][ regular ][ down ] ->load( btnPixDir + "/normal/buttonRestore.png" );
    BUTTONSARR[ ButtonMax ][ press ][ down ] ->load( btnPixDir + "/press/buttonRestorePress.png" );
    BUTTONSARR[ ButtonMax ][ hover ][ down ] ->load( btnPixDir + "/hover/buttonRestoreHover.png" );

    BUTTONSARR[ ButtonMin ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonMin.png" );
    BUTTONSARR[ ButtonMin ][ press ][ normal ] ->load( btnPixDir + "/press/buttonMinPress.png" );
    BUTTONSARR[ ButtonMin ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonMinHover.png" );

    BUTTONSARR[ ButtonClose ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonClose.png" );
    BUTTONSARR[ ButtonClose ][ press ][ normal ] ->load( btnPixDir + "/press/buttonClosePress.png" );
    BUTTONSARR[ ButtonClose ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonCloseHover.png" );

    BUTTONSARR[ ButtonSticky ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonSticky.png" );
    BUTTONSARR[ ButtonSticky ][ press ][ normal ] ->load( btnPixDir + "/press/buttonStickyDownPress.png" );
    BUTTONSARR[ ButtonSticky ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonStickyHover.png" );

    BUTTONSARR[ ButtonSticky ][ regular ][ down ] ->load( btnPixDir + "/normal/buttonStickyDown.png" );
    BUTTONSARR[ ButtonSticky ][ press ][ down ] ->load( btnPixDir + "/press/buttonStickyDownPress.png" );
    BUTTONSARR[ ButtonSticky ][ hover ][ down ] ->load( btnPixDir + "/hover/buttonStickyDownHover.png" );

    BUTTONSARR[ ButtonAbove ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonAbove.png" );
    BUTTONSARR[ ButtonAbove ][ press ][ normal ] ->load( btnPixDir + "/press/buttonAbovePress.png" );
    BUTTONSARR[ ButtonAbove ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonAboveHover.png" );

    BUTTONSARR[ ButtonAbove ][ regular ][ down ] ->load( btnPixDir + "/normal/buttonAboveDown.png" );
    BUTTONSARR[ ButtonAbove ][ press ][ down ] ->load( btnPixDir + "/press/buttonAboveDownPress.png" );
    BUTTONSARR[ ButtonAbove ][ hover ][ down ] ->load( btnPixDir + "/hover/buttonAboveDownHover.png" );

    BUTTONSARR[ ButtonBelow ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonBelow.png" );
    BUTTONSARR[ ButtonBelow ][ press ][ normal ] ->load( btnPixDir + "/press/buttonBelowPress.png" );
    BUTTONSARR[ ButtonBelow ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonBelowHover.png" );

    BUTTONSARR[ ButtonBelow ][ regular ][ down ] ->load( btnPixDir + "/normal/buttonBelowDown.png" );
    BUTTONSARR[ ButtonBelow ][ press ][ down ] ->load( btnPixDir + "/press/buttonBelowDownPress.png" );
    BUTTONSARR[ ButtonBelow ][ hover ][ down ] ->load( btnPixDir + "/hover/buttonBelowDownHover.png" );

    BUTTONSARR[ ButtonShade ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonShade.png" );
    BUTTONSARR[ ButtonShade ][ press ][ normal ] ->load( btnPixDir + "/press/buttonShadePress.png" );
    BUTTONSARR[ ButtonShade ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonShadeHover.png" );

    BUTTONSARR[ ButtonShade ][ regular ][ down ] ->load( btnPixDir + "/normal/buttonShadeDown.png" );
    BUTTONSARR[ ButtonShade ][ press ][ down ] ->load( btnPixDir + "/press/buttonShadeDownPress.png" );
    BUTTONSARR[ ButtonShade ][ hover ][ down ] ->load( btnPixDir + "/hover/buttonShadeDownHover.png" );

    BUTTONSARR[ ButtonMenu ][ regular ][ normal ] ->load( btnPixDir + "/normal/buttonMenu.png" );
    BUTTONSARR[ ButtonMenu ][ press ][ normal ] ->load( btnPixDir + "/press/buttonMenuPress.png" );
    BUTTONSARR[ ButtonMenu ][ hover ][ normal ] ->load( btnPixDir + "/hover/buttonMenuHover.png" );
}

//////////////////////////////////////////////////////////////////////////////
// ~OxygenFactory()
// -----------------
// Destructor

OxygenFactory::~OxygenFactory() { initialized_ = false; }

//////////////////////////////////////////////////////////////////////////////
// createDecoration()
// -----------------
// Create the decoration

KDecoration* OxygenFactory::createDecoration(KDecorationBridge* b)
{
    return new OxygenClient(b, this);
}

//////////////////////////////////////////////////////////////////////////////
// reset()
// -------
// Reset the handler. Returns true if decorations need to be remade, false if
// only a repaint is necessary

bool OxygenFactory::reset(unsigned long changed)
{
    // read in the configuration
    initialized_ = false;
    bool confchange = readConfig();
    initialized_ = true;

    if (confchange ||
        (changed & (SettingDecoration | SettingButtons | SettingBorder))) {
        return true;
    } else {
        resetDecorations(changed);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////
// readConfig()
// ------------
// Read in the configuration file

bool OxygenFactory::readConfig()
{
    // create a config object
    TDEConfig config("twinoxygenrc");
    config.setGroup("General");

    // grab settings
    TQt::AlignmentFlags oldalign = titlealign_;
    TQString value = config.readEntry("TitleAlignment", "AlignHCenter");
    if (value == "AlignLeft") titlealign_ = TQt::AlignLeft;
    else if (value == "AlignHCenter") titlealign_ = TQt::AlignHCenter;
    else if (value == "AlignRight") titlealign_ = TQt::AlignRight;

    if (oldalign == titlealign_)
        return false;
    else
        return true;
}

//////////////////////////////////////////////////////////////////////////////
// OxygenButton Class                                                      //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// OxygenButton()
// ---------------
// Constructor

OxygenButton::OxygenButton(OxygenClient *parent, const char *name,
                             const TQString& tip, ButtonType type,
                             const unsigned char *bitmap)
    : TQButton(parent->widget(), name), client_(parent), type_(type),
      deco_(0), lastmouse_(NoButton)
{
    setBackgroundMode(NoBackground);
    setFixedSize(BUTTONSIZE, 18);
    setCursor(arrowCursor);
    if (bitmap) setBitmap(bitmap);
    TQToolTip::add(this, tip);
    
    isActive_ = false;
    hover_ = false;
    
    bg = 0L;
}

OxygenButton::~OxygenButton()
{
    if (deco_) delete deco_;
}

//////////////////////////////////////////////////////////////////////////////
// setBitmap()
// -----------
// Set the button decoration

void OxygenButton::setBitmap(const unsigned char *bitmap)
{
    if (!bitmap) return; // no bitmap, probably the menu button

    if (deco_) delete deco_;
    deco_ = new TQBitmap(DECOSIZE, DECOSIZE, bitmap, true);
    deco_->setMask(*deco_);
    repaint(false);
}

//////////////////////////////////////////////////////////////////////////////
// sizeHint()
// ----------
// Return size hint

TQSize OxygenButton::sizeHint() const
{
    return TQSize(BUTTONSIZE, 18);
}

//////////////////////////////////////////////////////////////////////////////
// enterEvent()
// ------------
// Mouse has entered the button

void OxygenButton::enterEvent(TQEvent *e)
{
    // if we wanted to do mouseovers, we would keep track of it here
    TQButton::enterEvent(e);
    hover_ = TRUE;
    repaint( FALSE );
}

//////////////////////////////////////////////////////////////////////////////
// leaveEvent()
// ------------
// Mouse has left the button

void OxygenButton::leaveEvent(TQEvent *e)
{
    // if we wanted to do mouseovers, we would keep track of it here
    TQButton::leaveEvent(e);
    hover_ = FALSE;
    repaint( FALSE );
}

//////////////////////////////////////////////////////////////////////////////
// mousePressEvent()
// -----------------
// Button has been pressed

void OxygenButton::mousePressEvent(TQMouseEvent* e)
{
    lastmouse_ = e->button();

    // translate and pass on mouse event
    int button = LeftButton;
    if ((type_ != ButtonMax) && (e->button() != LeftButton)) {
        button = NoButton; // middle & right buttons inappropriate
    }
    TQMouseEvent me(e->type(), e->pos(), e->globalPos(),
                   button, e->state());
    TQButton::mousePressEvent(&me);
}

//////////////////////////////////////////////////////////////////////////////
// mouseReleaseEvent()
// -----------------
// Button has been released

void OxygenButton::mouseReleaseEvent(TQMouseEvent* e)
{
    lastmouse_ = e->button();

    // translate and pass on mouse event
    int button = LeftButton;
    if ((type_ != ButtonMax) && (e->button() != LeftButton)) {
        button = NoButton; // middle & right buttons inappropriate
    }
    TQMouseEvent me(e->type(), e->pos(), e->globalPos(), button, e->state());
    TQButton::mouseReleaseEvent(&me);
}

//////////////////////////////////////////////////////////////////////////////
// drawButton()
// ------------
// Draw the button

void OxygenButton::drawButton(TQPainter *painter)
{
    if (!OxygenFactory::initialized()) return;

    TQColorGroup group;
    int dx, dy;

    // paint a plain box with border
    group = KDecoration::options()->
        colorGroup(KDecoration::ColorButtonBg, client_->isActive());
    //painter->fillRect(rect(), group.button());
    painter->setPen(group.dark());
    //painter->drawRect(rect());

    if( bg )
    {
        // why -2? no idea...
        painter->drawPixmap( -this->x(), -2, *bg );
    }

    if (type_ == ButtonMenu) {
        // we paint the mini icon (which is 16 pixels high)
        dx = (width() - 16) / 2;
        dy = (height() - 16) / 2 - 1;
        if (isDown()) { dx++; dy++; }
        painter->drawPixmap(dx, dy, client_->icon().pixmap(TQIconSet::Small,
                                                           TQIconSet::Normal));
    }
    else if( !BUTTONSARR[ type_ ][ regular ][ normal ]->isNull() )
    {
        dx = -1;
        dy = -2;
        
        if( isDown() )
            painter->drawPixmap(dx, dy, *BUTTONSARR[ type_ ][ press ][ isActive_?down:normal ]);
        else if( hover_ )
            painter->drawPixmap(dx, dy, *BUTTONSARR[ type_ ][ hover ][ isActive_?down:normal ]);
        else
            painter->drawPixmap(dx, dy, *BUTTONSARR[ type_ ][ regular ][ isActive_?down:normal ]);
        
    }
    else if (deco_) {
        // otherwise we paint the deco
        dx = (width() - DECOSIZE) / 2;
        dy = (height() - DECOSIZE) / 2;
        if (isDown()) { dx++; dy++; }
        painter->setPen(group.dark());
        painter->drawPixmap(dx, dy, *deco_);
    }
}

//////////////////////////////////////////////////////////////////////////////
// OxygenClient Class                                                      //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// OxygenClient()
// ---------------
// Constructor

OxygenClient::OxygenClient(KDecorationBridge *b, KDecorationFactory *f)
    : KDecoration(b, f)
{
}

OxygenClient::~OxygenClient()
{
    for (int n=0; n<ButtonTypeCount; n++) {
        if (button[n]) delete button[n];
    }
}

//////////////////////////////////////////////////////////////////////////////
// init()
// ------
// Actual initializer for class

void OxygenClient::init()
{
    createMainWidget(WResizeNoErase | WRepaintNoErase);
    widget()->installEventFilter(this);

    // for flicker-free redraws
    widget()->setBackgroundMode(NoBackground);

    // setup layout
    TQGridLayout *mainlayout = new TQGridLayout(widget(), 4, 3); // 4x3 grid
    TQHBoxLayout *titlelayout = new TQHBoxLayout();
    titlebar_ = new TQSpacerItem(1, TITLESIZE, TQSizePolicy::Expanding,
                                TQSizePolicy::Fixed);

    mainlayout->setResizeMode(TQLayout::FreeResize);
    mainlayout->addRowSpacing(0, FRAMESIZE);
    mainlayout->addRowSpacing(3, FRAMESIZE*2);
    mainlayout->addColSpacing(0, FRAMESIZE);
    mainlayout->addColSpacing(2, FRAMESIZE);

    mainlayout->addLayout(titlelayout, 1, 1);
    if (isPreview()) {
        // TODO: render the full gradient here
        mainlayout->addWidget(
        new TQLabel(i18n("<b><center>Oxygen preview</center></b>"),
        widget()), 2, 1);
    } else {
        mainlayout->addItem(new TQSpacerItem(0, 0), 2, 1);
    }

    // the window should stretch
    mainlayout->setRowStretch(2, 10);
    mainlayout->setColStretch(1, 10);

    // setup titlebar buttons
    for (int n=0; n<ButtonTypeCount; n++) button[n] = 0;
    addButtons(titlelayout, options()->titleButtonsLeft());
    titlelayout->addItem(titlebar_);
    addButtons(titlelayout, options()->titleButtonsRight());

    // connections
    connect(this, TQ_SIGNAL(keepAboveChanged(bool)), TQ_SLOT(keepAboveChange(bool)));
    connect(this, TQ_SIGNAL(keepBelowChanged(bool)), TQ_SLOT(keepBelowChange(bool)));
}

//////////////////////////////////////////////////////////////////////////////
// addButtons()
// ------------
// Add buttons to title layout

void OxygenClient::addButtons(TQBoxLayout *layout, const TQString& s)
{
    const unsigned char *bitmap;
    TQString tip;

    OxygenButton * btn = 0L;

    if (s.length() > 0) {
        for (unsigned n=0; n < s.length(); n++) {
            switch (s[n]) {
              case 'M': // Menu button
                  if (!button[ButtonMenu]) {
                      button[ButtonMenu] = btn =
                          new OxygenButton(this, "menu", i18n("Menu"),
                                            ButtonMenu, 0);
                      connect(button[ButtonMenu], TQ_SIGNAL(pressed()),
                              this, TQ_SLOT(menuButtonPressed()));
                      layout->addWidget(button[ButtonMenu]);
                  }
                  break;

              case 'S': // Sticky button
                  if (!button[ButtonSticky]) {
              if (isOnAllDesktops()) {
              bitmap = stickydown_bits;
              tip = i18n("Un-Sticky");
              } else {
              bitmap = sticky_bits;
              tip = i18n("Sticky");
              }
                      button[ButtonSticky] = btn =
                          new OxygenButton(this, "sticky", tip,
                                            ButtonSticky, bitmap);
                      connect(button[ButtonSticky], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(toggleOnAllDesktops()));
                      layout->addWidget(button[ButtonSticky]);
                  }
                  break;

              case 'H': // Help button
                  if ((!button[ButtonHelp]) && providesContextHelp()) {
                      button[ButtonHelp] = btn =
                          new OxygenButton(this, "help", i18n("Help"),
                                            ButtonHelp, help_bits);
                      connect(button[ButtonHelp], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(showContextHelp()));
                      layout->addWidget(button[ButtonHelp]);
                  }
                  break;

              case 'I': // Minimize button
                  if ((!button[ButtonMin]) && isMinimizable())  {
                      button[ButtonMin] = btn =
                          new OxygenButton(this, "iconify", i18n("Minimize"),
                                            ButtonMin, min_bits);
                      connect(button[ButtonMin], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(minimize()));
                      layout->addWidget(button[ButtonMin]);
                  }
                  break;

              case 'A': // Maximize button
                  if ((!button[ButtonMax]) && isMaximizable()) {
                      if (maximizeMode() == MaximizeFull) {
                          bitmap = minmax_bits;
                          tip = i18n("Restore");
                      } else {
                          bitmap = max_bits;
                          tip = i18n("Maximize");
                      }
                      button[ButtonMax]  = btn =
                          new OxygenButton(this, "maximize", tip,
                                            ButtonMax, bitmap);
                    if( maximizeMode() == MaximizeFull ) button[ButtonMax]->isActive_ = true;
                    
                      connect(button[ButtonMax], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(maxButtonPressed()));
                      layout->addWidget(button[ButtonMax]);
                  }
                  break;

              case 'X': // Close button
                  if ((!button[ButtonClose]) && isCloseable()) {
                      button[ButtonClose] = btn =
                          new OxygenButton(this, "close", i18n("Close"),
                                            ButtonClose, close_bits);
                      connect(button[ButtonClose], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(closeWindow()));
                      layout->addWidget(button[ButtonClose]);
                  }
                  break;

              case 'F': // Above button
                  if ((!button[ButtonAbove])) {
                      button[ButtonAbove] = btn =
                          new OxygenButton(this, "above",
                              i18n("Keep Above Others"), ButtonAbove,
                              keepAbove() ? above_on_bits : above_off_bits);
                      connect(button[ButtonAbove], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(aboveButtonPressed()));
                      layout->addWidget(button[ButtonAbove]);
                  }
                  break;

              case 'B': // Below button
                  if ((!button[ButtonBelow])) {
                      button[ButtonBelow] = btn =
                          new OxygenButton(this, "below",
                              i18n("Keep Below Others"), ButtonBelow,
                              keepBelow() ? below_on_bits : below_off_bits);
                      connect(button[ButtonBelow], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(belowButtonPressed()));
                      layout->addWidget(button[ButtonBelow]);
                  }
                  break;

              case 'L': // Shade button
                  if ((!button[ButtonShade && isShadeable()])) {
                      if ( isSetShade()) {
                          bitmap = shade_on_bits;
                          tip = i18n("Unshade");
                      } else {
                          bitmap = shade_off_bits;
                          tip = i18n("Shade");
                      }
                      button[ButtonShade] = btn =
                          new OxygenButton(this, "shade", tip,
                                            ButtonShade, bitmap);
                      connect(button[ButtonShade], TQ_SIGNAL(clicked()),
                              this, TQ_SLOT(shadeButtonPressed()));
                      layout->addWidget(button[ButtonShade]);
                  }
                  break;

              case '_': // Spacer item
                  layout->addSpacing(FRAMESIZE);
            }
            
            if( btn )
            {
                btn->bg = &pixbar;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// activeChange()
// --------------
// window active state has changed

void OxygenClient::activeChange()
{
    for (int n=0; n<ButtonTypeCount; n++)
        if (button[n]) button[n]->reset();
    widget()->repaint(false);
}

//////////////////////////////////////////////////////////////////////////////
// captionChange()
// ---------------
// The title has changed

void OxygenClient::captionChange()
{
    widget()->repaint(titlebar_->geometry(), false);
}

//////////////////////////////////////////////////////////////////////////////
// desktopChange()
// ---------------
// Called when desktop/sticky changes

void OxygenClient::desktopChange()
{
    bool d = isOnAllDesktops();
    if (button[ButtonSticky]) {
        button[ButtonSticky]->setBitmap(d ? stickydown_bits : sticky_bits);
    TQToolTip::remove(button[ButtonSticky]);
    TQToolTip::add(button[ButtonSticky], d ? i18n("Un-Sticky") : i18n("Sticky"));
    }
}

//////////////////////////////////////////////////////////////////////////////
// iconChange()
// ------------
// The title has changed

void OxygenClient::iconChange()
{
    if (button[ButtonMenu]) {
        button[ButtonMenu]->setBitmap(0);
        button[ButtonMenu]->repaint(false);
    }
}

//////////////////////////////////////////////////////////////////////////////
// maximizeChange()
// ----------------
// Maximized state has changed

void OxygenClient::maximizeChange()
{
    bool m = (maximizeMode() == MaximizeFull);
    if (button[ButtonMax]) {
        button[ButtonMax]->setBitmap(m ? minmax_bits : max_bits);
    TQToolTip::remove(button[ButtonMax]);
    TQToolTip::add(button[ButtonMax], m ? i18n("Restore") : i18n("Maximize"));
    }
}

//////////////////////////////////////////////////////////////////////////////
// shadeChange()
// -------------
// Called when window shading changes

void OxygenClient::shadeChange()
{
    bool s = isSetShade();
    if (button[ButtonShade]) {
        button[ButtonShade]->setBitmap(s ? shade_on_bits : shade_off_bits);
        TQToolTip::remove(button[ButtonShade]);
        TQToolTip::add(button[ButtonShade], s ? i18n("Unshade") : i18n("Shade"));
    }
}

//////////////////////////////////////////////////////////////////////////////
// keepAboveChange()
// ------------
// The above state has changed

void OxygenClient::keepAboveChange(bool a)
{
    if (button[ButtonAbove]) {
        button[ButtonAbove]->setBitmap(a ? above_on_bits : above_off_bits);
    }
}

//////////////////////////////////////////////////////////////////////////////
// keepBelowChange()
// ------------
// The below state has changed

void OxygenClient::keepBelowChange(bool b)
{
    if (button[ButtonBelow]) {
        button[ButtonBelow]->setBitmap(b ? below_on_bits : below_off_bits);
    }
}

//////////////////////////////////////////////////////////////////////////////
// borders()
// ----------
// Get the size of the borders

void OxygenClient::borders(int &l, int &r, int &t, int &b) const
{
    l = r = FRAMESIZE;
    t = TITLESIZE + FRAMESIZE;
    b = FRAMESIZE + 1;
}

//////////////////////////////////////////////////////////////////////////////
// resize()
// --------
// Called to resize the window

void OxygenClient::resize(const TQSize &size)
{
    widget()->resize(size);
}

//////////////////////////////////////////////////////////////////////////////
// minimumSize()
// -------------
// Return the minimum allowable size for this window

TQSize OxygenClient::minimumSize() const
{
    return widget()->minimumSize();
}

//////////////////////////////////////////////////////////////////////////////
// mousePosition()
// ---------------
// Return logical mouse position

KDecoration::Position OxygenClient::mousePosition(const TQPoint &point) const
{
    const int corner = 24;
    Position pos;

    if (point.y() <= FRAMESIZE) {
        // inside top frame
        if (point.x() <= corner)                 pos = PositionTopLeft;
        else if (point.x() >= (width()-corner))  pos = PositionTopRight;
        else                                     pos = PositionTop;
    } else if (point.y() >= (height()-FRAMESIZE*2)) {
        // inside handle
        if (point.x() <= corner)                 pos = PositionBottomLeft;
        else if (point.x() >= (width()-corner))  pos = PositionBottomRight;
        else                                     pos = PositionBottom;
    } else if (point.x() <= FRAMESIZE) {
        // on left frame
        if (point.y() <= corner)                 pos = PositionTopLeft;
        else if (point.y() >= (height()-corner)) pos = PositionBottomLeft;
        else                                     pos = PositionLeft;
    } else if (point.x() >= width()-FRAMESIZE) {
        // on right frame
        if (point.y() <= corner)                 pos = PositionTopRight;
        else if (point.y() >= (height()-corner)) pos = PositionBottomRight;
        else                                     pos = PositionRight;
    } else {
        // inside the frame
        pos = PositionCenter;
    }
    return pos;
}

//////////////////////////////////////////////////////////////////////////////
// eventFilter()
// -------------
// Event filter

bool OxygenClient::eventFilter(TQObject *obj, TQEvent *e)
{
    if (obj != widget()) return false;

    switch (e->type()) {
      case TQEvent::MouseButtonDblClick: {
          mouseDoubleClickEvent(static_cast<TQMouseEvent *>(e));
          return true;
      }
      case TQEvent::MouseButtonPress: {
          processMousePressEvent(static_cast<TQMouseEvent *>(e));
          return true;
      }
      case TQEvent::Paint: {
          paintEvent(static_cast<TQPaintEvent *>(e));
          return true;
      }
      case TQEvent::Resize: {
          resizeEvent(static_cast<TQResizeEvent *>(e));
          return true;
      }
      case TQEvent::Show: {
          showEvent(static_cast<TQShowEvent *>(e));
          return true;
      }
      default: {
          return false;
      }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////
// mouseDoubleClickEvent()
// -----------------------
// Doubleclick on title

void OxygenClient::mouseDoubleClickEvent(TQMouseEvent *e)
{
    if (titlebar_->geometry().contains(e->pos())) titlebarDblClickOperation();
}

//////////////////////////////////////////////////////////////////////////////
// paintEvent()
// ------------
// Repaint the window

void OxygenClient::paintEvent(TQPaintEvent*)
{
    if (!OxygenFactory::initialized()) return;

    TQColorGroup group;
    TQPainter p(widget());

    // draw the titlebar
    TQRect title(titlebar_->geometry());
    group = options()->colorGroup(KDecoration::ColorTitleBar, isActive());
    
    // TODO: FIXME: proper / configurable blacklist later
    bool doesGradient = ( caption().find( "Pale Moon" ) == -1 );
    
    
    if( doesGradient )
    {
        int r_w = width();
        int r_h = title.height() + 4;
        
        //p.drawTiledPixmap( pixbar.rect(), pixbar );
        
        if( pixbar.width() != r_w || pixbar.height() != r_h )
        {
            pixbar.resize(r_w, r_h);
            
            TQPainter p(&pixbar);
            
            // origin of the radial gradient
            int cx = r_w/2, cy = -72;
            
            int x, y;
            
            // FIXME: these colors are probably wrong
            //TQColor c1(group.background().light(106));
            TQColor c1(group.background().dark(94));
            //TQColor c1(224,221,220);
            TQColor c2(255,255,255);
            
            p.fillRect(pixbar.rect(), c1);
            
            int rDiff, gDiff, bDiff;
            
            rDiff = ( c2.red())   - c1.red();
            gDiff = ( c2.green()) - c1.green();
            bDiff = ( c2.blue())  - c1.blue();
            
            int rc, gc, bc;
            
            // it was not so hard to write radial gradient code
            // after all!!!
            // I was previously imagining some ridiculous sin/cos loops
            // but the solution was much simpler and more deterministic
            
            for( y = r_h; y > 0; --y )
            {
                for( x = r_w; x > 0; --x)
                {
                    // with vertical scaling
                    // to try to match real Oxygen
                    int xs = x - cx, ys = (y*3.5) - cy;
                    float s = sqrtf( xs*xs + ys*ys );
                    
                    // closest value I've found
                    #define SIZE 308
                    if( s < SIZE )
                    {
                        //TQColor c3(c1);
                        
                        rc = c2.red()   - roundf( rDiff * s / ((float)SIZE) );
                        gc = c2.green() - roundf( gDiff * s / ((float)SIZE) );
                        bc = c2.blue()  - roundf( bDiff * s / ((float)SIZE) );
                        
                        p.setPen(TQColor(rc, gc, bc));
                        p.drawPoint(x, y);
                    }
                }
            }
            
            p.end();
        }
        
        // and of course we use a pixmap to avoid flickering on title changes
        p.drawPixmap( pixbar.rect(), pixbar );
    }
    else
    {
        p.fillRect(title, group.background());
    }
    
    
    //p.setPen(group.dark());
    //p.drawRect(title);

    // draw title text
    p.setFont(options()->font(isActive(), false));
    p.setPen(options()->color(KDecoration::ColorFont, isActive()));
    p.drawText(title.x() + FRAMESIZE, title.y() - 1,
                     title.width() - FRAMESIZE * 2, title.height(),
                     OxygenFactory::titleAlign() | AlignVCenter,
                     caption());

    // draw frame
    //group = options()->colorGroup(KDecoration::ColorFrame, isActive());

    TQColor c1(group.background().dark(94));
    TQColor c2(193,187,184); // FIXME: calculate color properly

    TQRect frame(0, 0, width(), FRAMESIZE);
    if( !doesGradient ) p.fillRect(frame, group.background());
    frame.setRect(0, 0, FRAMESIZE, height());
    p.fillRect(frame, group.background());
    frame.setRect(width()-FRAMESIZE, 0, FRAMESIZE, height());
    p.fillRect(frame, group.background());
    frame.setRect(0, height() - FRAMESIZE*2, width(), FRAMESIZE*2);
    p.fillRect(frame, doesGradient?c2:group.background());

    // outline the frame
    p.setPen(group.dark());
    frame = widget()->rect();
    p.drawRect(frame);
    /*frame.setRect(frame.x() + FRAMESIZE-1, frame.y() + FRAMESIZE-1,
                  frame.width() - FRAMESIZE*2 +2,
                  frame.height() - FRAMESIZE*3 +2);
    p.drawRect(frame);*/
}

//////////////////////////////////////////////////////////////////////////////
// resizeEvent()
// -------------
// Window is being resized

void OxygenClient::resizeEvent(TQResizeEvent *)
{
    if (widget()->isShown()) {
        TQRegion region = widget()->rect();
        region = region.subtract(titlebar_->geometry());
    widget()->erase(region);
    }
}

//////////////////////////////////////////////////////////////////////////////
// showEvent()
// -----------
// Window is being shown

void OxygenClient::showEvent(TQShowEvent *)
{
    widget()->repaint();
}

//////////////////////////////////////////////////////////////////////////////
// maxButtonPressed()
// -----------------
// Max button was pressed

void OxygenClient::maxButtonPressed()
{
    if (button[ButtonMax]) {
#if KDE_IS_VERSION(3, 3, 0)
        maximize(button[ButtonMax]->lastMousePress());
#else
        switch (button[ButtonMax]->lastMousePress()) {
          case MidButton:
              maximize(maximizeMode() ^ MaximizeVertical);
              break;
          case RightButton:
              maximize(maximizeMode() ^ MaximizeHorizontal);
              break;
          default:
              (maximizeMode() == MaximizeFull) ? maximize(MaximizeRestore)
                  : maximize(MaximizeFull);
        }
#endif
    }
}

//////////////////////////////////////////////////////////////////////////////
// shadeButtonPressed()
// -----------------
// Shade button was pressed

void OxygenClient::shadeButtonPressed()
{
    if (button[ButtonShade]) {
        setShade( !isSetShade());
    }
}

//////////////////////////////////////////////////////////////////////////////
// aboveButtonPressed()
// -----------------
// Above button was pressed

void OxygenClient::aboveButtonPressed()
{
    if (button[ButtonAbove]) {
        setKeepAbove( !keepAbove());
    }
}

//////////////////////////////////////////////////////////////////////////////
// belowButtonPressed()
// -----------------
// Below button was pressed

void OxygenClient::belowButtonPressed()
{
    if (button[ButtonBelow]) {
        setKeepBelow( !keepBelow());
    }
}

//////////////////////////////////////////////////////////////////////////////
// menuButtonPressed()
// -------------------
// Menu button was pressed (popup the menu)

void OxygenClient::menuButtonPressed()
{
    if (button[ButtonMenu]) {
        TQPoint p(button[ButtonMenu]->rect().bottomLeft().x(),
                 button[ButtonMenu]->rect().bottomLeft().y());
        KDecorationFactory* f = factory();
        showWindowMenu(button[ButtonMenu]->mapToGlobal(p));
        if (!f->exists(this)) return; // decoration was destroyed
        button[ButtonMenu]->setDown(false);
    }
}

#include "oxygenclient.moc"
