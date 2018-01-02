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
#ifndef GREP_H
#define GREP_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;

class GrepDialog: public QDialog {
	Q_OBJECT
public:
	GrepDialog();
	bool getIgnoreCase();
	bool getTargetAnd();
	void setWord(QString word);
	QString getWord1();
	QString getWord2();
	QString getSdirectory();
	QString getFpattern();

private slots:
	void clicked_dir();
	void clicked_ok();
	void target_and_toggled(bool state);

private:
	bool hasFocus() {
		return true;
	}
private:
	QPushButton *button_ok_;
	QPushButton *button_cancel_;
	QPushButton *button_dir_;
	QCheckBox *ignore_case_;
	QLineEdit *sdirectory_;
	QLineEdit *fpattern_;

	QLineEdit *text1_;
	QComboBox *combotext1_;
	QCheckBox *target_and_;
	QLineEdit *text2_;
	QComboBox *combotext2_;
};

#endif
