//=========================================================
//  MusE
//  Linux Music Editor
//  $Id: ./muse/widgets/slider.cpp $
//
//  Copyright (C) 1999-2011 by Werner Schweer and others
//  (C) Copyright 2011 Orcan Ogetbil (ogetbilo at sf.net)
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; version 2 of
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//=========================================================
#include "muse_math.h"
#include "mmath.h"

#include <QPainter>
#include <QResizeEvent>

#include "utils.h"
#include "thinslider.h"

namespace MusEGui {

//-------------------------------------------------------------
//  ThinSlider - The ThinSlider Widget
//
//  Slider is a slider widget which operates on an interval
//  of type double. Slider supports different layouts as
//  well as a scale.
//------------------------------------------------------------

//------------------------------------------------------------
//.F  ThinSlider::Slider
//
//    Constructor
//
//.u  Syntax:
//.f  ThinSlider::Slider(QWidget *parent, const char *name, Orientation orient = Horizontal, ScalePos scalePos = None, int bgStyle = BgTrough)
//
//.u  Parameters
//.p
//  QWidget *parent --  parent widget
//  const char *name -- The Widget's name. Default = 0.
//  Orientation Orient -- Orientation of the slider. Can be ThinSlider::Horizontal
//        or ThinSlider::Vertical.
//                    Defaults to Horizontal.
//  ScalePos scalePos --  Position of the scale.  Can be ThinSlider::None,
//        ThinSlider::Left, ThinSlider::Right, ThinSlider::Top,
//        or ThinSlider::Bottom. Defaults to ThinSlider::None.  !!! CURRENTLY only ThinSlider::None supported - oget 20110913
//  QColor fillcolor -- the color used to fill in the full side
//        of the Slider
//------------------------------------------------------------

ThinSlider::ThinSlider(QWidget *parent, const char *name,
	       Qt::Orientation orient, ScalePos scalePos, QColor fillColor)
      : SliderBase(parent,name)
      {
      d_thumbLength = 16;
      d_thumbHalf = 8;
      d_thumbWidth = 16;
        

      d_scaleDist   = 4;
      d_scaleStep   = 0.0;
      d_scalePos    = scalePos;
      d_xMargin     = 0;
      d_yMargin     = 0;
      d_mMargin    = 1;

      d_fillColor = fillColor;

      d_sliderRect.setRect(0, 0, 8, 8);
      setOrientation(orient);
      }

//------------------------------------------------------------
//.F  ThinSlider::setSizeHint
//------------------------------------------------------------

void ThinSlider::setSizeHint(uint w, uint h)
      {
      horizontal_hint = w;
      vertical_hint = h;
      }

//------------------------------------------------------------
//.F  ThinSlider::~Slider
//    Destructor
//.u  Syntax
//.f  ThinSlider::~Slider()
//------------------------------------------------------------

ThinSlider::~ThinSlider()
      {
      }

//----------------------------------------------------
//
//.F  ThinSlider::setThumbLength
//
//    Set the slider's thumb length
//
//.u  Syntax
//  void ThinSlider::setThumbLength(int l)
//
//.u  Parameters
//.p  int l   --    new length
//
//-----------------------------------------------------
void ThinSlider::setThumbLength(int l)
{
    d_thumbLength = MusECore::qwtMax(l,8);
    d_thumbHalf = d_thumbLength / 2;
    resize(size());
}

//------------------------------------------------------------
//
//.F  ThinSlider::setThumbWidth
//  Change the width of the thumb
//
//.u  Syntax
//.p  void ThinSlider::setThumbWidth(int w)
//
//.u  Parameters
//.p  int w -- new width
//
//------------------------------------------------------------
void ThinSlider::setThumbWidth(int w)
{
    d_thumbWidth = MusECore::qwtMax(w,4);
    resize(size());
}


//------------------------------------------------------------
//.-  
//.F  ThinSlider::scaleChange
//  Notify changed scale
//
//.u  Syntax
//.f  void ThinSlider::scaleChange()
//
//.u  Description
//  Called by QwtScaledWidget
//
//------------------------------------------------------------
void ThinSlider::scaleChange()
{
    if (!hasUserScale())
       d_scale.setScale(minValue(), maxValue(), d_maxMajor, d_maxMinor);
    update();
}


//------------------------------------------------------------
//.-
//.F  ThinSlider::fontChange
//  Notify change in font
//  
//.u  Syntax
//.f   ThinSlider::fontChange(const QFont &oldFont)
//
//------------------------------------------------------------
void ThinSlider::fontChange(const QFont & /*oldFont*/)
{
    repaint();
}

//------------------------------------------------------------
//    drawSlider
//     Draw the slider into the specified rectangle.  
//------------------------------------------------------------

void ThinSlider::drawSlider(QPainter *p, const QRect &r)
{
    p->setRenderHint(QPainter::Antialiasing);

    const QPalette& pal = palette();
    QBrush brBack(pal.window());
    QBrush brMid(pal.mid());
    QBrush brDark(pal.dark());

    QRect cr;
    
    int ipos,dist1;
    double rpos;

    int xrad = 4;
    int yrad = 4;

    // for the empty side
    QColor e_mask_edge = pal.mid().color();
    QColor e_mask_center = pal.midlight().color();
    int e_alpha = 215;
    e_mask_edge.setAlpha(e_alpha);
    e_mask_center.setAlpha(e_alpha);
    
    QLinearGradient e_mask;
    e_mask.setColorAt(0, e_mask_edge);
    e_mask.setColorAt(0.5, e_mask_center);
    e_mask.setColorAt(1, e_mask_edge);
    
    // for the full side
    rpos = (value()  - minValue()) / (maxValue() - minValue());
    
    int f_brightness = 155 * rpos + 100;
    int f_alpha;
    int f_edge;
    if (pal.currentColorGroup() == QPalette::Disabled)
        {
        f_alpha = 185;
        f_edge = 100;
        }
    else
        {
        f_alpha = 127;
        f_edge = 0;
        }
	   
    QColor f_mask_center = QColor(f_brightness, f_brightness, f_brightness, f_alpha);
    QColor f_mask_edge = QColor(f_edge, f_edge, f_edge, f_alpha);
    QLinearGradient f_mask;
	   
    f_mask.setColorAt(0, f_mask_edge);
    f_mask.setColorAt(0.5, f_mask_center);
    f_mask.setColorAt(1, f_mask_edge);
    
    // for the thumb
    QLinearGradient thumbGrad;
    QColor thumb_edge = pal.dark().color();
    QColor thumb_center = pal.midlight().color();
    
    thumbGrad.setColorAt(0, thumb_edge);
    thumbGrad.setColorAt(0.5, thumb_center);
    thumbGrad.setColorAt(1, thumb_edge);
    
    
    if (d_orient == Qt::Horizontal)
        {

        cr.setRect(r.x(),
                   r.y() + d_mMargin,
                   r.width(),
                   r.height() - 2*d_mMargin);


        //
        // Draw background
        //
        QPainterPath bg_rect = MusECore::roundedPath(cr, 
                                           xrad, yrad, 
                                           (MusECore::Corner) (MusECore::UpperLeft | MusECore::UpperRight | MusECore::LowerLeft | MusECore::LowerRight) );
	   
        p->fillPath(bg_rect, d_fillColor);
	   
        dist1 = int(double(cr.width() - d_thumbLength) * rpos);
        ipos =  cr.x() + dist1;
        markerPos = ipos + d_thumbHalf;
	   

        //
        // Draw empty right side
        // 
	   
        e_mask.setStart(QPointF(0, cr.y()));
        e_mask.setFinalStop(QPointF(0, cr.y() + cr.height()));

        QPainterPath e_rect = MusECore::roundedPath(ipos + d_thumbLength, cr.y(), 
                                          cr.width() - d_thumbLength - dist1, cr.height(), 
                                          xrad, yrad, (MusECore::Corner) (MusECore::UpperRight | MusECore::LowerRight) );
   
        p->fillPath(e_rect, QBrush(e_mask));
   
   
        //
        // Draw full left side
        //
           
        f_mask.setStart(QPointF(0, cr.y()));
        f_mask.setFinalStop(QPointF(0, cr.y() + cr.height()));
          
        QPainterPath f_rect = MusECore::roundedPath(cr.x(), cr.y(), 
                                          ipos + 1, cr.height(),
                                          xrad, yrad, 
                                          (MusECore::Corner) (MusECore::LowerLeft | MusECore::UpperLeft) );

        p->fillPath(f_rect, QBrush(f_mask));
          
           
        //
        //  Draw thumb
        //
	   
        QPainterPath thumb_rect = MusECore::roundedPath(ipos, r.y(), 
                                              d_thumbLength, r.height(), 
                                              2, 2, 
                                              (MusECore::Corner) (MusECore::UpperLeft | MusECore::UpperRight | MusECore::LowerLeft | MusECore::LowerRight) );
   
        thumbGrad.setStart(QPointF(0, cr.y()));
        thumbGrad.setFinalStop(QPointF(0, cr.y() + cr.height()));
           
           
        p->fillPath(thumb_rect, QBrush(thumbGrad));
           
        // center line
        p->fillRect(ipos + d_thumbHalf, cr.y(), 1, cr.height(), pal.dark().color());
           

        }
    else // (d_orient == Qt::Vertical)
        {
	      
        cr.setRect(r.x() + d_mMargin,
                   r.y(),
                   r.width() - 2*d_mMargin,
                   r.height());
	    

        //
        // Draw background
        //
        QPainterPath bg_rect = MusECore::roundedPath(cr,
                                           xrad, yrad, 
                                           (MusECore::Corner) (MusECore::UpperLeft | MusECore::UpperRight | MusECore::LowerLeft | MusECore::LowerRight) );
	    
        p->fillPath(bg_rect, d_fillColor);

        dist1 = int(double(cr.height() - d_thumbLength) * (1.0 - rpos));
        ipos = cr.y() + dist1;
        markerPos = ipos + d_thumbHalf;

  
        //
        // Draw empty upper filling
        // 

        e_mask.setStart(QPointF(cr.x(), 0));
        e_mask.setFinalStop(QPointF(cr.x() + cr.width(), 0));
	    
        QPainterPath e_rect = MusECore::roundedPath(cr.x(), cr.y(), 
                                          cr.width(), ipos + 1,
                                          xrad, yrad, 
                                          (MusECore::Corner) (MusECore::UpperLeft | MusECore::UpperRight) );
	    
        p->fillPath(e_rect, QBrush(e_mask));
            
            
        //
        // Draw lower filling mask
        //

        f_mask.setStart(QPointF(cr.x(), 0));
        f_mask.setFinalStop(QPointF(cr.x() + cr.width(), 0));
            
        QPainterPath f_rect = MusECore::roundedPath(cr.x(), ipos + d_thumbLength, 
                                          cr.width(), cr.height() - d_thumbLength - dist1,
                                          xrad, yrad, (MusECore::Corner) (MusECore::LowerLeft | MusECore::LowerRight) );
	    
        p->fillPath(f_rect, QBrush(f_mask));
            
            
        //
        //  Draw thumb
        //
            
        QPainterPath thumb_rect = MusECore::roundedPath(r.x(), ipos, 
                                              r.width(), d_thumbLength,
                                              2, 2, 
                                              (MusECore::Corner) (MusECore::UpperLeft | MusECore::UpperRight | MusECore::LowerLeft | MusECore::LowerRight) );
	    
        thumbGrad.setStart(QPointF(cr.x(), 0));
        thumbGrad.setFinalStop(QPointF(cr.x() + cr.width(), 0));
            
            
        p->fillPath(thumb_rect, QBrush(thumbGrad));
            
        // center line
        p->fillRect(cr.x(), ipos + d_thumbHalf, cr.width(), 1, pal.dark().color());
            
        }

}

//------------------------------------------------------------
//.-
//.F  ThinSlider::getValue
//  Determine the value corresponding to a specified
//  mouse location.
//
//.u  Syntax
//.f     double ThinSlider::getValue(const QPoint &p)
//
//.u  Parameters
//.p  const QPoint &p --
//
//.u  Description
//  Called by SliderBase
//------------------------------------------------------------
double ThinSlider::getValue( const QPoint &p)
{
    double rv;
    int pos;
    QRect r = d_sliderRect;

    if (d_orient == Qt::Horizontal)
    {
  
  if (r.width() <= d_thumbLength)
  {
      rv = 0.5 * (minValue() + maxValue());
  }
  else
  {
      pos = p.x() - r.x() - d_thumbHalf;
      rv  =  minValue() +
         rint( (maxValue() - minValue()) * double(pos)
        / double(r.width() - d_thumbLength)
        / step() ) * step();
  }
  
    }
    else
    {
  if (r.height() <= d_thumbLength)
  {
      rv = 0.5 * (minValue() + maxValue());
  }
  else
  {
      pos = p.y() - r.y() - d_thumbHalf;
      rv =  minValue() +
         rint( (maxValue() - minValue()) *
        (1.0 - double(pos)
         / double(r.height() - d_thumbLength))
        / step() ) * step();
  }
  
    }

    return(rv);
}


//------------------------------------------------------------
//
//.F  ThinSlider::moveValue
//  Determine the value corresponding to a specified mouse movement.
//
//.u  Syntax
//.f  void Slider::moveValue(const QPoint &deltaP, bool fineMode)
//
//.u  Parameters
//.p  const QPoint &deltaP -- Change in position
//.p  bool fineMode -- Fine mode if true, coarse mode if false.
//
//.u  Description
//    Called by SliderBase
//    Coarse mode (the normal mode) maps pixels to values depending on range and width,
//     such that the slider follows the mouse cursor. Fine mode maps one step() value per pixel.
//------------------------------------------------------------
double ThinSlider::moveValue(const QPoint &deltaP, bool fineMode)
{
  double rv = d_valAccum;
  const QRect r = d_sliderRect;

  const double val = value(ConvertNone);

  if((fineMode || borderlessMouse()) && d_scrollMode != ScrDirect)
  {
    double newval;
    if(d_orient == Qt::Horizontal)
      newval = val + deltaP.x() * step();
    else
      newval = val - deltaP.y() * step();
    d_valAccum = newval; // Reset.
    return newval;
  }
  
  const double min = minValue(ConvertNone);
  const double max = maxValue(ConvertNone);
  const double drange = max - min;

  if(d_orient == Qt::Horizontal)
  {
    if(r.width() <= d_thumbLength)
      rv = 0.5 * (min + max);
    else
    {
      // Only if there is a change! Otherwise it adjusts the value on mouse release simply from
      //  touching the control, we don't want that.
      if(deltaP.x() != 0)
      {
        const double dpos = double(deltaP.x());
        const double dwidth = double(r.width() - d_thumbLength);
        const double dval_diff = (drange * dpos) / dwidth;
        d_valAccum += dval_diff;
        rv = rint(d_valAccum / step()) * step();
      }
    }
  }
  else
  {
    if(r.height() <= d_thumbLength)
      rv = 0.5 * (min + max);
    else
    {
      // Only if there is a change! Otherwise it adjusts the value on mouse release simply from
      //  touching the control, we don't want that.
      if(deltaP.y() != 0)
      {
        const double dpos = double(-deltaP.y());
        const double dheight = double(r.height() - d_thumbLength);
        const double dval_diff = (drange * dpos) / dheight;
        d_valAccum += dval_diff;
        rv = rint(d_valAccum / step()) * step();
      }
    }
  }
  return(rv);
}

//------------------------------------------------------------
//.-
//.F  ThinSlider::getScrollMode
//  Determine scrolling mode and direction
//
//.u  Syntax
//.f   void ThinSlider::getScrollMode( const QPoint &p, int &scrollMode, int &direction )
//
//.u  Parameters
//.p  const QPoint &p -- point
//
//.u  Description
//  Called by SliderBase
//
//------------------------------------------------------------
void ThinSlider::getScrollMode( QPoint &p, const Qt::MouseButton &button, const Qt::KeyboardModifiers& /*modifiers*/, int &scrollMode, int &direction )
{
    if(cursorHoming() && button == Qt::LeftButton)
    {
      if(d_sliderRect.contains(p))
      {
        scrollMode = ScrMouse;
        direction = 0;

        int mp = 0;
        QRect cr;
        QPoint cp;
        int ipos,dist1;
        double rpos;

        cr = d_sliderRect;
  
        rpos = (value()  - minValue()) / (maxValue() - minValue());
  
        if(d_orient == Qt::Horizontal)
        {
          dist1 = int(double(cr.width() - d_thumbLength) * rpos);
          ipos =  cr.x() + dist1;
          mp = ipos + d_thumbHalf;
        
          p.setX(mp);
          cp = mapToGlobal( QPoint(mp, p.y()) );
        }  
        else
        {
          dist1 = int(double(cr.height() - d_thumbLength) * (1.0 - rpos));
          ipos = cr.y() + dist1;
          mp = ipos + d_thumbHalf;
          p.setY(mp);
          cp = mapToGlobal( QPoint(p.x(), mp) );
        }  
        cursor().setPos(cp.x(), cp.y());
      }
    }
    else
    {
      int currentPos;
      if (d_orient == Qt::Horizontal)
       currentPos = p.x();
      else
       currentPos = p.y();
      
      if (d_sliderRect.contains(p))
      {
        if ((currentPos > markerPos - d_thumbHalf)  
            && (currentPos < markerPos + d_thumbHalf))
        {
          scrollMode = ScrMouse;
          direction = 0;
        }
        else
        {
          scrollMode = ScrPage;
          if (((currentPos > markerPos) && (d_orient == Qt::Horizontal))
              || ((currentPos <= markerPos) && (d_orient != Qt::Horizontal)))
            direction = 1;
          else
            direction = -1;
        }
      }
      else
      {
        scrollMode = ScrNone;
        direction = 0;
      }
    
    }
}

//------------------------------------------------------------
//.F  ThinSlider::paintEvent
//  Qt paint event
//
//.u  Syntax
//.f  void ThinSlider::paintEvent(QPaintEvent *e)
//------------------------------------------------------------

void ThinSlider::paintEvent(QPaintEvent* ev)
      {
      QPainter p(this);

      /* Scale is not supported
      if (p.begin(this)) {
            if (d_scalePos != None) {
                  p.fillRect(rect(), palette().window());
                  d_scale.draw(&p);
                  }
            drawSlider(&p, d_sliderRect);
            }
      p.end();
      */
      drawSlider(&p, d_sliderRect);
      p.setPen(Qt::red);
      p.drawRect(ev->rect());
      }

//------------------------------------------------------------
//.F  ThinSlider::resizeEvent
//  Qt resize event
//
//.u  Parameters
//.p  QResizeEvent *e
//
//.u  Syntax
//.f  void ThinSlider::resizeEvent(QResizeEvent *e)
//------------------------------------------------------------

void ThinSlider::resizeEvent(QResizeEvent *e)
{
    SliderBase::resizeEvent(e);
    d_resized = true;
    QSize s = e->size();
    /* Scale is not supported
    int sliderWidth = d_thumbWidth;

    // reposition slider
    if(d_orient == Qt::Horizontal)
    {
  switch(d_scalePos)
  {
  case Top:
  
      d_sliderRect.setRect(this->rect().x() + d_xMargin,
         this->rect().y() + s.height() - 1
         - d_yMargin - sliderWidth,
         s.width() - 2 * d_xMargin,
         sliderWidth);
      d_scale.setGeometry(d_sliderRect.x() + d_thumbHalf,
        d_sliderRect.y() - d_scaleDist,
        d_sliderRect.width() - d_thumbLength,
        ScaleDraw::Top);
  
      break;
  
  case Bottom:
  
      d_sliderRect.setRect(this->rect().x() + d_xMargin,
         this->rect().y() + d_yMargin,
         s.width() - 2*d_xMargin,
         sliderWidth);
      d_scale.setGeometry(d_sliderRect.x() + d_thumbHalf,
        d_sliderRect.y() + d_sliderRect.height() +  d_scaleDist,
        d_sliderRect.width() - d_thumbLength,
        ScaleDraw::Bottom);
  
      break;
  
  default:
      d_sliderRect.setRect(this->rect().x(), this->rect().x(),
         s.width(), s.height());
      break;
  }
    }
    else // d_orient == Qt::Vertical
    {
  switch(d_scalePos)
  {
  case Left:
      d_sliderRect.setRect(this->rect().x() + s.width()
         - sliderWidth - 1 - d_xMargin,
         this->rect().y() + d_yMargin,
         sliderWidth,
         s.height() - 2 * d_yMargin);
      d_scale.setGeometry(d_sliderRect.x() - d_scaleDist,
        d_sliderRect.y() + d_thumbHalf,
        s.height() - d_thumbLength,
        ScaleDraw::Left);
  
      break;
  case Right:
      d_sliderRect.setRect(this->rect().x() + d_xMargin,
         this->rect().y() + d_yMargin,
         sliderWidth,
         s.height() - 2* d_yMargin);
      d_scale.setGeometry(this->rect().x() + d_sliderRect.width()
        + d_scaleDist,
        d_sliderRect.y() + d_thumbHalf,
        s.height() - d_thumbLength,
        ScaleDraw::Right);
      break;
  default:
      d_sliderRect.setRect(this->rect().x(), this->rect().x(),
         s.width(), s.height());
      break;
  }
    }
    */
  d_sliderRect.setRect(this->rect().x(), this->rect().y(),
                       s.width(), s.height());
}

//------------------------------------------------------------
//.-
//.F  ThinSlider::valueChange
//  Notify change of value
//
//.u  Syntax
//.f  void ThinSlider::valueChange()
//
//------------------------------------------------------------

void ThinSlider::valueChange()
      {
      update();
      SliderBase::valueChange();
      }

//------------------------------------------------------------
//.-  
//.F  ThinSlider::rangeChange
//  Notify change of range
//
//.u  Description
//
//.u  Syntax
//.f  void ThinSlider::rangeChange()
//
//------------------------------------------------------------
void ThinSlider::rangeChange()
{
    if (!hasUserScale())
       d_scale.setScale(minValue(), maxValue(), d_maxMajor, d_maxMinor);
    SliderBase::rangeChange();
    repaint();
}

//------------------------------------------------------------
//
//.F  ThinSlider::setMargins
//  Set distances between the widget's border and
//  internals.
//
//.u  Syntax
//.f  void ThinSlider::setMargins(int hor, int vert)
//
//.u  Parameters
//.p  int hor, int vert -- Margins
//
//------------------------------------------------------------
void ThinSlider::setMargins(int hor, int vert)
{
    d_xMargin = MusECore::qwtMax(0, hor);
    d_yMargin = MusECore::qwtMax(0, vert);
    resize(this->size());
}

//------------------------------------------------------------
//
//.F  ThinSlider::sizeHint
//  Return a recommended size
//
//.u  Syntax
//.f  QSize ThinSlider::sizeHint() const
//
//.u  Note
//  The return value of sizeHint() depends on the font and the
//  scale.
//------------------------------------------------------------

QSize ThinSlider::sizeHint() const
      {
      /* Scale is not supported
      int w = 40;
      int h = 40;
      QPainter p;
      int msWidth = 0, msHeight = 0;

      if (d_scalePos != None) {
            if (p.begin(this)) {
                  msWidth = d_scale.maxWidth(&p, FALSE);
                  msHeight = d_scale.maxHeight(&p);
                  }
            p.end();

            switch(d_orient) {
                  case Qt::Vertical:
                        w = 2*d_xMargin + d_thumbWidth + msWidth + d_scaleDist + 2;
                        break;
                  case Qt::Horizontal:
                        h = 2*d_yMargin + d_thumbWidth + msHeight + d_scaleDist;
                        break;
                  }
            }
      else {      // no scale
            switch(d_orient) {
                  case Qt::Vertical:
                        w = 16;
                        break;
                  case Qt::Horizontal:
                        h = 16;
                        break;
                  }
            }
      */
      return QSize(horizontal_hint, vertical_hint);
      }

//---------------------------------------------------------
//   setOrientation
//---------------------------------------------------------

void ThinSlider::setOrientation(Qt::Orientation o)
      {
      d_orient = o;
      /* Scale is not supported
      ScaleDraw::OrientationX so = ScaleDraw::Bottom;
      switch(d_orient) {
            case Qt::Vertical:
                  if (d_scalePos == Right)
                        so = ScaleDraw::Right;
                  else
                        so = ScaleDraw::Left;
                  break;
            case Qt::Horizontal:
                  if (d_scalePos == Bottom)
                        so = ScaleDraw::Bottom;
                  else
                        so = ScaleDraw::Top;
                  break;
            }

      d_scale.setGeometry(0, 0, 40, so);
      if (d_orient == Qt::Vertical)
            setMinimumSize(10,20);
      else
            setMinimumSize(20,10);
      QRect r = geometry();
      setGeometry(r.x(), r.y(), r.height(), r.width());
      update();
      */

      switch(d_orient) {
            case Qt::Vertical:
                  horizontal_hint = 32;
                  vertical_hint = 64;
                  break;
            case Qt::Horizontal:
                  horizontal_hint = 64;
                  vertical_hint = 32;
                  break;
            }
      }

Qt::Orientation ThinSlider::orientation() const
      {
      return d_orient;
      }

double ThinSlider::lineStep() const
      {
      return 1.0;
      }

double ThinSlider::pageStep() const
      {
      return 1.0;
      }

void ThinSlider::setLineStep(double)
      {
      }

void ThinSlider::setPageStep(double)
      {
      }

} // namespace MusEGui
