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
#ifndef JUMP_H
#define JUMP_H

#include <QDialog>

class QLabel;
class QLineEdit;

class LineDialog: public QDialog {
	Q_OBJECT

public:
	LineDialog();
	int getLine();
private:
	QLabel *label_;
	QLineEdit *text_;
	QPushButton *button_ok_;
	QPushButton *button_cancel_;
};

class BookmarkDialog: public QDialog {
	Q_OBJECT

public:
	BookmarkDialog();
	int getBookmark();
private:
	QLabel *label_;
	QLineEdit *text_;
	QPushButton *button_ok_;
	QPushButton *button_cancel_;
};
#endif
