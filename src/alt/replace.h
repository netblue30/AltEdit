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
#ifndef REPLACE_H
#define REPLACE_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;

class ReplaceDialog: public QDialog {
	Q_OBJECT

public:
	typedef enum {
		ACTION_NONE = 0,
		ACTION_FINDNEXT,
		ACTION_REPLACE,
		ACTION_REPLACEALL,
		ACTION_CANCEL
	} ReplaceAction;

	ReplaceDialog(QString tofind, QString replacement);

	// action functions
	ReplaceAction getAction() {
		return action_;
	}
	void reset() {
		action_ = ACTION_NONE;
	}

	// retrieve dialog data
	QString getWordToFind();
	QString getReplacement();
	void getOut();

signals:
	void replaceClicked();

private slots:
	void clicked_findnext();
	void clicked_replace();
	void clicked_replaceall();
	void clicked_cancel();

private:
	QLabel *label1_;
	QLabel *label2_;
	QLineEdit *text1_;
	QLineEdit *text2_;
	QComboBox *combotext1_;
	QComboBox *combotext2_;

	QPushButton *button_findnext_;
	QPushButton *button_replace_;
	QPushButton *button_replaceall_;
	QPushButton *button_cancel_;
	volatile ReplaceAction action_;
};

#endif
