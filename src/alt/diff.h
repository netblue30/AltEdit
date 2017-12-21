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
#ifndef DIFF_H
#define DIFF_H

#include <QString>
#include <QDialog>
#include "global.h"
class QLineEdit;
class QCheckBox;
class QGroupBox;

class DiffDialog: public QDialog {
	Q_OBJECT

public:
	enum mode_t {
		GIT_MODE,
		FILES_MODE
	};

	DiffDialog();
	QString getFile1();
	QString getFile2();
	mode_t getMode();

private slots:
	void clicked_browse1();
	void clicked_browse2();
	void setFilesMode(bool);
	void setGitMode(bool);


private:
	QLineEdit *file1_;
	QLineEdit *file2_;
	QCheckBox *git_box_;
	QGroupBox *files_box_;
	mode_t mode_;
};

#endif