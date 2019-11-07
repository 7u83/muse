//=========================================================
//  MusE
//  Linux Music Editor
//    $Id: piano.cpp,v 1.3 2004/05/31 11:48:55 lunar_shuttle Exp $
//  (C) Copyright 1999 Werner Schweer (ws@seh.de)
//  (C) Copyright 2012 Tim E. Real (terminator356 on users dot sourceforge dot net)
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

#include <QMouseEvent>
#include <QPainter>

#include <stdio.h>

#include <map>

#include "piano.h"
#include "globals.h"
#include "song.h"
#include "track.h"
#include "midieditor.h"
#include "midictrl.h"
#include "icons.h"

namespace MusEGui {
  
static const char *oct_xpm[] = {
    // w h colors
    "40 91 2 1",
    ". c #c0c0c0",
    "# c #000000",
    //           x
    "########################################",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#", // 10
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#", //------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",
    "########################################",     // 7
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",     // 6
    ".......................................#",
    ".......................................#",
    ".......................................#", //------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",     // 7
    "########################################",
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",    // 6
    ".......................................#",
    ".......................................#",
    ".......................................#", //------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",    // 7
    "########################################",
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",    // 10
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    "########################################", //----------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",    // 9
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#", //------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",
    "########################################",   // 7
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",     // 6
    ".......................................#",
    ".......................................#",
    ".......................................#", //--------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",     // 7
    "########################################",
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",     // 9
    ".......................................#",
    ".......................................#",
    ".......................................#",
};


static const char *mk1_xpm[] = {
    "40 13 2 1",
    ". c #ff0000",
    "# c none",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    "########################...............#",
    "########################...............#",
    "########################...............#",
    "########################################",
};

static const char *mk2_xpm[] = {
    "40 13 2 1",
    ". c #ff0000",
    "# c none",
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",     // 6
    ".......................................#",
    ".......................................#",
    ".......................................#", //--------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",     // 7
    "########################################",
};

static const char *mk3_xpm[] = {
    "40 13 2 1",
    ". c #ff0000",
    "# c none",
    "########################...............#",
    "########################...............#",
    "########################...............#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    "########################################",
};

static const char *mk4_xpm[] = {
    "40 13 2 1",
    "# c #ff0000",
    ". c none",
    "........................................",
    "........................................",
    "........................................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "........................................",
    "........................................",
    "........................................",
};

static const char *mk5_xpm[] = {
    "40 13 2 1",
    ". c #ffff00",
    "# c none",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    "########################...............#",
    "########################...............#",
    "########################...............#",
    "########################################",
};

static const char *mk6_xpm[] = {
    "40 13 2 1",
    ". c #ffff00",
    "# c none",
    "########################...............#",
    "########################...............#",
    "########################...............#", //------------------------
    ".......................................#",
    ".......................................#",
    ".......................................#",     // 6
    ".......................................#",
    ".......................................#",
    ".......................................#", //--------------------------
    "########################...............#",
    "########################...............#",
    "########################...............#",     // 7
    "########################################",
};

static const char *mk7_xpm[] = {
    "40 13 2 1",
    ". c #ffff00",
    "# c none",
    "########################...............#",
    "########################...............#",
    "########################...............#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    ".......................................#",
    "########################################",
};

static const char *mk8_xpm[] = {
    "40 13 2 1",
    "# c #ffff00",
    ". c none",
    "........................................",
    "........................................",
    "........................................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "########################................",
    "........................................",
    "........................................",
    "........................................",
};
      
/*
      0   1   2  3  4  5  6  7  8  9  10
      c-2 c-1 C0 C1 C2 C3 C4 C5 C6 C7 C8 - G8

      Grid ьber Oktave:

           +------------+ ------------------------------
       11  |            |
           |         h  |         7
           +------+     |
       10  |  a#  +-----+ ..............................
           +------+  a  |
        9  |            |         6
           +------+     |
        8  |  g#  +-----+ ..............................
           +------+  g  |
        7  |            |         5
           +------+     |
        6  |  f#  +-----+ ..............................
           +------+  f  |
        5  |            |         4
           |            |
           +------------+ ------------------------------
        4  |            |
           |         e  |         3
           +------+     |
        3  |  d#  +-----+ ..............................
           +------+  d  |
        2  |            |         2
           +------+     |
        1  |  c#  +-----+ ..............................
           +------+  c  |
           |            |         1
        0  |            |
           +------------+ ------------------------------
 */

//---------------------------------------------------------
//   Piano
//---------------------------------------------------------

Piano::Piano(QWidget* parent, int ymag, MidiEditor* editor)
   : View(parent, 1, ymag)
      {
      setMouseTracking(true);
      _midiEditor = editor;
      curPitch = -1;
      _curSelectedPitch = 60;  // Start with 'C3"
      octave = new QPixmap(oct_xpm);

      mk1 = new QPixmap(mk1_xpm);
      mk2 = new QPixmap(mk2_xpm);
      mk3 = new QPixmap(mk3_xpm);
      mk4 = new QPixmap(mk4_xpm);
      
      mk5 = new QPixmap(mk5_xpm);
      mk6 = new QPixmap(mk6_xpm);
      mk7 = new QPixmap(mk7_xpm);
      mk8 = new QPixmap(mk8_xpm);
      
      keyDown = -1;
      button = Qt::NoButton;
      }

//---------------------------------------------------------
//   draw
//---------------------------------------------------------

void Piano::draw(QPainter& p, const QRect& mr, const QRegion&)
      {
      // FIXME: For some reason need the expansion otherwise drawing
      //        artifacts (incomplete drawing). Can't figure out why.
      const QRect ur = mapDev(mr).adjusted(0, -4, 0, 4);
      
      QPoint offset(0, KH*2);
      p.drawTiledPixmap(ur, *octave, ur.topLeft()+offset);

      if (_curSelectedPitch != -1 && _curSelectedPitch != curPitch)
      {
        int y = pitch2y(_curSelectedPitch);
        QPixmap* pm;
        switch(_curSelectedPitch % 12) {
              case 0:
              case 5:
                    pm = mk7;
                    break;
              case 2:
              case 7:
              case 9:
                    pm = mk6;
                    break;
              case 4:
              case 11:
                    pm = mk5;
                    break;
              default:
                    pm = mk8;
                    break;
              }
        p.drawPixmap(0, y, *pm);
      }
      
      if (curPitch != -1)
      {
        int y = pitch2y(curPitch);
        QPixmap* pm;
        switch(curPitch % 12) {
              case 0:
              case 5:
                    pm = mk3;
                    break;
              case 2:
              case 7:
              case 9:
                    pm = mk2;
                    break;
              case 4:
              case 11:
                    pm = mk1;
                    break;
              default:
                    pm = mk4;
                    break;
              }
        p.drawPixmap(0, y, *pm);
      }
      
      // draw C notes    
      p.setRenderHint(QPainter::Antialiasing);
      p.setPen(Qt::black);
      p.setFont(QFont("Arial", 7));
      int octaveSize=91;

      for (int drawKey = 0; drawKey < 11; drawKey++) {
          int drawY = octaveSize * drawKey + 82 - KH*2;
          p.drawText(23, drawY + 8, "C" + QString::number(8 - drawKey));
      }



      if(!_midiEditor)
        return;
      MusECore::PartList* part_list = _midiEditor->parts();
      MusECore::Part* cur_part = _midiEditor->curCanvasPart();
      if(!part_list || !cur_part || !cur_part->track()->isMidiTrack())
        return;
      
      MusECore::MidiTrack* track = (MusECore::MidiTrack*)(cur_part->track());
      int channel      = track->outChannel();
      MusECore::MidiPort* port   = &MusEGlobal::midiPorts[track->outPort()];
      MusECore::MidiCtrlValListList* cll = port->controller();
      const int min = channel << 24;
      const int max = min + 0x1000000;

      for(MusECore::ciMidiCtrlValList it = cll->lower_bound(min); it != cll->lower_bound(max); ++it)
      {
        MusECore::MidiCtrlValList* cl = it->second;
        MusECore::MidiController* c   = port->midiController(cl->num());
        if(!c->isPerNoteController())
          continue;
        int cnum = c->num();
        int num = cl->num();
        int pitch = num & 0x7f;
        bool used = false;
        for (MusECore::ciEvent ie = cur_part->events().begin(); ie != cur_part->events().end(); ++ie)
        {
          MusECore::Event e = ie->second;
          if(e.type() != MusECore::Controller)
            continue;
          int ctl_num = e.dataA();
          if((ctl_num | 0xff) == cnum && (ctl_num & 0x7f) == pitch)
          {
            used = true;
            break;
          }
        }

        bool off = cl->hwVal() == MusECore::CTRL_VAL_UNKNOWN;  // Does it have a value or is it 'off'?

        int y = pitch2y(pitch) + 3;
        if(used)
        {
          if(off)
            p.drawPixmap(0, y, 6, 6, *greendot12x12Icon);
          else
            p.drawPixmap(0, y, 6, 6, *orangedot12x12Icon);
        }
        else
        {
          if(off)
            p.drawPixmap(0, y, 6, 6, *graydot12x12Icon);
          else
            p.drawPixmap(0, y, 6, 6, *bluedot12x12Icon);
        }
      }
      
      }
      
//---------------------------------------------------------
//   pitch2y
//---------------------------------------------------------

int Piano::pitch2y(int pitch) const
      {
      int tt[] = {
            12, 19, 25, 32, 38, 51, 58, 64, 71, 77, 84, 90
            };
      int y = (75 * KH) - (tt[pitch%12] + (7 * KH) * (pitch/12));
      if (y < 0)
            y = 0;
      return y;
      }

//---------------------------------------------------------
//   y2pitch
//---------------------------------------------------------

int Piano::y2pitch(int y) const
      {
      const int total = (10 * 7 + 5) * KH;       // 75 full tone steps
      y = total - y;
      int oct = (y / (7 * KH)) * 12;
      char kt[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1,
            2, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3, 3,
            4, 4, 4, 4, 4, 4, 4, 4, 4,
            5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
            6, 6, 6, 6, 6, 6, 6,
            7, 7, 7, 7, 7, 7,
            8, 8, 8, 8, 8, 8, 8,
            9, 9, 9, 9, 9, 9,
            10, 10, 10, 10, 10, 10, 10,
            11, 11, 11, 11, 11, 11, 11, 11, 11, 11
            };
      return kt[y % 91] + oct;
      }

//---------------------------------------------------------
//   leaveEvent
//---------------------------------------------------------

void Piano::leaveEvent(QEvent*)
      {
      if (keyDown != -1) {
            emit keyReleased(keyDown, shift);
            keyDown = -1;
            }
      emit pitchChanged(-1);
      setPitch(-1);
      }

//---------------------------------------------------------
//   setPitch
//---------------------------------------------------------

void Piano::setPitch(int pitch)
      {
      if (curPitch == pitch)
            return;
      curPitch = pitch;
      redraw();
      }

//---------------------------------------------------------
//   viewMouseMoveEvent
//---------------------------------------------------------

void Piano::viewMouseMoveEvent(QMouseEvent* event)
      {
      int pitch = y2pitch(event->y());
      emit pitchChanged(pitch);
      setPitch(pitch);

      if (button != Qt::NoButton) {
            int nk = y2pitch(event->y());
            if (nk < 0 || nk > 127)
                  nk = -1;
            if (nk != keyDown) {
                  if (keyDown != -1) {
                        emit keyReleased(keyDown, shift);
                        }
                  keyDown = nk;
                  if (keyDown != -1) {
                        int velocity = event->x()*127/40;
                        emit keyPressed(keyDown, velocity>127 ? 127 : velocity, shift);
                        }
                  }
            }
      }

//---------------------------------------------------------
//   viewMousePressEvent
//---------------------------------------------------------

void Piano::viewMousePressEvent(QMouseEvent* event)
      {
      button = event->button();
      shift  = event->modifiers() & Qt::ShiftModifier;
      if (keyDown != -1) {
            emit keyReleased(keyDown, shift);
            keyDown = -1;
            }
      keyDown = y2pitch(event->y());
      if (keyDown < 0 || keyDown > 127) {
            keyDown = -1;
            }
      else {
            int velocity = event->x()*127/40;
            // REMOVE Tim. Noteoff. Changed. Zero note on vel is not allowed now.
//             emit keyPressed(keyDown, velocity>127 ? 127 : velocity, shift); //emit keyPressed(keyDown, shift);
            if(velocity > 127)
              velocity = 127;
            else if(velocity <= 0)
              velocity = 1;  // Zero note on vel is not allowed.
            emit keyPressed(keyDown, velocity, shift); //emit keyPressed(keyDown, shift);
            }
            
      if (keyDown != -1 && keyDown != _curSelectedPitch) {
            _curSelectedPitch = keyDown;
            emit curSelectedPitchChanged(_curSelectedPitch);
            redraw(); 
            MusEGlobal::song->update(SC_DRUMMAP);
            }
      }

//---------------------------------------------------------
//   viewMouseReleaseEvent
//---------------------------------------------------------

void Piano::viewMouseReleaseEvent(QMouseEvent* event)
      {
      button = Qt::NoButton;
      shift = event->modifiers() & Qt::ShiftModifier;
      if (keyDown != -1) {
            emit keyReleased(keyDown, shift);
            keyDown = -1;
            }
      }

//---------------------------------------------------------
//   setCurSelectedPitch
//---------------------------------------------------------

void Piano::setCurSelectedPitch(int pitch)
      {
      if (pitch < 0 || pitch >= 128)
        return; 
      if (pitch != _curSelectedPitch) {
            _curSelectedPitch = pitch;
            emit curSelectedPitchChanged(_curSelectedPitch);
            redraw(); 
            }
      }

//---------------------------------------------------------
//   wheelEvent
//---------------------------------------------------------

void Piano::wheelEvent(QWheelEvent* ev)
      {
            emit redirectWheelEvent(ev);
      }
      
} // namespace MusEGui
