/*
 *    Copyright (C) 2017 AltEdit Authors
 *
 *    This file is part of AltEdit project
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BUSYCURSOR_H
#define BUSYCURSOR_H
#include "global.h"
#include <QtWidgets>

// busy cursor
class BCursor {
public:
	BCursor() {
		QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
		qApp->processEvents();
	}
	~BCursor() {
		QApplication::restoreOverrideCursor();
	}
};

#endif
