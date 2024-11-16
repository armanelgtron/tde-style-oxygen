
Oxygen style for Trinity desktop environment
--------------------------------------------

**Very incomplete! Heavy work in progress.**

Most of the background gradients are there,
but most do not draw correctly behind other widgets.

TWin top gradient and TDEStyle gradients line up.

TWin top border gradient and gtk2 / Qt5 oxygen do not 100% line up,
but usually they are close enough that it is very hard for me to notice
without messing with the gamma. Well, except if the window size is 
horizontally small enough, then the gradient shrinks on Qt5/gtk
but currently not on TDE.

Drawing is also a bit inefficient right now,
although not totally unbearable on a 2.2 GHz Core 2 Duo.
But I also plan to fix that...


Widgets
-------

### Working
- Progressbar (animations still need work)

### In progress
- Sliders (copied from Domino, close but needs recoloring)

### Still look like Plastik
- Widget frames / separators
- Buttons
- Scrollbars
- Arrows
- CheckBoxes
- RadioButtons
- Spinboxes
- Dials


### Maybe impossible?
- ListViews (not sure I can draw the selection gradient)
