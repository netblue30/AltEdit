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
#ifndef FIND_H
#define FIND_H

#include <QtWidgets>

class FindDialog: public QDialog {
	Q_OBJECT
public:
	FindDialog();
	QString getWord();
	QTextDocument::FindFlags getFlags();
	void setWord(QString word);

private slots:
	void clicked_ok();
private:
	bool hasFocus() {
		return true;
	}
private:
	QLabel *label_;
	QLineEdit *text_;
	QComboBox *combotext_;
	QPushButton *button_ok_;
	QPushButton *button_cancel_;
	QCheckBox *ignore_case_;
	QCheckBox *whole_;
	QCheckBox *backwards_;
};

#endif
