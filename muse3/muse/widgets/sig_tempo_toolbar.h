//=========================================================
//  MusE
//  Linux Music Editor
//  sig_tempo_toolbar.h
//  (C) Copyright 2012 Florian Jung (flo93@users.sourceforge.net)
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

#ifndef __SIG_TEMPO_TOOLBAR_H__
#define __SIG_TEMPO_TOOLBAR_H__

#include <QWidget>
#include <QToolButton>
#include <QTimer>
#include <QDateTime>

#include "type_defs.h"

namespace Awl
{
	class SigEdit;
}

class QHBoxLayout;
class QLabel;

namespace MusEGui
{
	class TempoEdit;
	
	class SigToolbarWidget : public QWidget
	{
		Q_OBJECT
		
		private:
			QHBoxLayout* layout;
			QLabel* label;
			Awl::SigEdit* sig_edit;   
			
		public:
			SigToolbarWidget(QWidget* parent);
		
		signals:
			void returnPressed();
			void escapePressed();
		
		private slots:
			void pos_changed(int,unsigned,bool);
			void song_changed(MusECore::SongChangedFlags_t);
  };

	class TempoToolbarWidget : public QWidget
	{
		Q_OBJECT
		
		private:
			QHBoxLayout* layout;
			QLabel* label;
			MusEGui::TempoEdit* tempo_edit;
   QToolButton *tap_button;

   QTimer tap_timer;
   QDateTime last_tap_time;
   int n_taps;
   qint64 msecs_avg;
			
		public:
			TempoToolbarWidget(QWidget* parent);
		
		signals:
			void returnPressed();
			void escapePressed();
			
		private slots:
			void pos_changed(int,unsigned,bool);
			void song_changed(MusECore::SongChangedFlags_t);
   void tap_tempo();
   void tap_timer_signal();
  };
}

#endif
