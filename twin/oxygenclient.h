//////////////////////////////////////////////////////////////////////////////
// exampleclient.h
// -------------------
// Oxygen window decoration for KDE
// Copyright (c) 2024 Glen Harpring
// 
// Based on example KDE window decoration
// Copyright (c) 2003, 2004 David Johnson <david@usermode.org>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#ifndef EXAMPLECLIENT_H
#define EXAMPLECLIENT_H

#include <tqbutton.h>
#include <kdecoration.h>
#include <kdecorationfactory.h>

class TQSpacerItem;
class TQPoint;

namespace Oxygen {

class OxygenClient;

enum ButtonType {
    ButtonHelp=0,
    ButtonMax,
    ButtonMin,
    ButtonClose, 
    ButtonMenu,
    ButtonSticky,
    ButtonAbove,
    ButtonBelow,
    ButtonShade,
    ButtonTypeCount
};

enum pixType {
    normal = 0,
    down,
    pixTypeCount
};

enum buttonState {
    regular = 0,
    hover,
    press,
    buttonStateCount
};

// OxygenFactory /////////////////////////////////////////////////////////////

class OxygenFactory: public KDecorationFactory
{
public:
    OxygenFactory();
    virtual ~OxygenFactory();
    virtual KDecoration *createDecoration(KDecorationBridge *b);
    virtual bool reset(unsigned long changed);

    static bool initialized();
    static TQt::AlignmentFlags titleAlign();

private:
    bool readConfig();
    void loadPixmaps();

private:
    static bool initialized_;
    static TQt::AlignmentFlags titlealign_;
};

inline bool OxygenFactory::initialized()
    { return initialized_; }

inline TQt::AlignmentFlags OxygenFactory::titleAlign()
    { return titlealign_; }

// OxygenButton //////////////////////////////////////////////////////////////

class OxygenButton : public TQButton
{
public:
    OxygenButton(OxygenClient *parent=0, const char *name=0,
                  const TQString &tip=NULL,
                  ButtonType type=ButtonHelp,
                  const unsigned char *bitmap=0);
    ~OxygenButton();

    void setBitmap(const unsigned char *bitmap);
    TQSize sizeHint() const;
    ButtonState lastMousePress() const;
    void reset();
    bool isActive_;
    
    TQPixmap * bg;

private:
    void enterEvent(TQEvent *e);
    void leaveEvent(TQEvent *e);
    void mousePressEvent(TQMouseEvent *e);
    void mouseReleaseEvent(TQMouseEvent *e);
    void drawButton(TQPainter *painter);

private:
    OxygenClient *client_;
    ButtonType type_;
    bool hover_;
    TQBitmap *deco_;
    ButtonState lastmouse_;
};

inline TQt::ButtonState OxygenButton::lastMousePress() const
    { return lastmouse_; }

inline void OxygenButton::reset()
    { repaint(false); }

// OxygenClient //////////////////////////////////////////////////////////////

class OxygenClient : public KDecoration
{
    TQ_OBJECT
public:
    OxygenClient(KDecorationBridge *b, KDecorationFactory *f);
    virtual ~OxygenClient();

    virtual void init();

    virtual void activeChange();
    virtual void desktopChange();
    virtual void captionChange();
    virtual void iconChange();
    virtual void maximizeChange();
    virtual void shadeChange();

    virtual void borders(int &l, int &r, int &t, int &b) const;
    virtual void resize(const TQSize &size);
    virtual TQSize minimumSize() const;
    virtual Position mousePosition(const TQPoint &point) const;

private:
    void addButtons(TQBoxLayout* layout, const TQString& buttons);

    bool eventFilter(TQObject *obj, TQEvent *e);
    void mouseDoubleClickEvent(TQMouseEvent *e);
    void paintEvent(TQPaintEvent *e);
    void resizeEvent(TQResizeEvent *);
    void showEvent(TQShowEvent *);

private slots:
    void maxButtonPressed();
    void menuButtonPressed();
    void aboveButtonPressed();
    void belowButtonPressed();
    void shadeButtonPressed();
    void keepAboveChange(bool);
    void keepBelowChange(bool);

private:
    OxygenButton * button[ButtonTypeCount];
    TQSpacerItem * titlebar_;
    
    TQPixmap pixbar;
};

} // namespace Oxygen

#endif // EXAMPLECLIENT_H
