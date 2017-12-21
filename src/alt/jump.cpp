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
#include "jump.h"
#include <QtWidgets>
#include "global.h"

LineDialog::LineDialog(): QDialog() {
	FLOG();
	QGridLayout *grid = new QGridLayout(this);

	// line number
	label_ = new QLabel(tr("Line: "));
	text_ = new QLineEdit();
	grid->addWidget(label_, 0, 0);
	grid->addWidget(text_, 0, 1, 1, 2);

	// accept/reject
	grid->setRowMinimumHeight(1, 15);
	button_ok_ = new QPushButton(tr("OK"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_ok_, 2, 1);
	grid->addWidget(button_cancel_, 2, 2);
	grid->setContentsMargins(20, 20, 20, 20);
	setLayout(grid);

	connect(button_ok_, SIGNAL(clicked()), SLOT(accept()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));
	setWindowTitle("Go to line number");
}

int LineDialog::getLine() {
	FLOG();
	int retval;

	if (text_->text().isEmpty() == true)
		retval = -1;
	else {
		QString str = text_->text();
		bool ok;
		retval = str.toInt(&ok);
		if (!ok || retval <= 0) {
			retval = -1;
			QMessageBox::warning(0,tr("Error"), tr("Not a valid line number"));
		}
	}
	return retval;
}

BookmarkDialog::BookmarkDialog(): QDialog() {
	FLOG();
	QGridLayout *grid = new QGridLayout(this);

	label_ = new QLabel(tr("Bookmark: "));
	text_ = new QLineEdit();
	grid->addWidget(label_, 0, 0);
	grid->addWidget(text_, 0, 1, 1, 2);

	// accept/reject
	grid->setRowMinimumHeight(1, 15);
	button_ok_ = new QPushButton(tr("OK"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_ok_, 2, 1);
	grid->addWidget(button_cancel_, 2, 2);
	connect(button_ok_, SIGNAL(clicked()), SLOT(accept()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));

	// layout
	grid->setContentsMargins(20, 20, 20, 20);
	setLayout(grid);
	setWindowTitle("Go to bookmark");
}

int BookmarkDialog::getBookmark() {
	FLOG();
	int retval;

	if (text_->text().isEmpty() == true)
		retval = -1;
	else {
		QString str = text_->text();
		bool ok;
		retval = str.toInt(&ok);
		if (!ok || retval < 0 || retval >= MAXBOOKMARKS) {
			retval = -1;
			QMessageBox::warning(0,tr("Error"), tr("Not a valid bookmark number"));
		}
	}
	return retval;
}
