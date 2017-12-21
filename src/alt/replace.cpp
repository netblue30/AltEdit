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
#include "replace.h"
#include <QtWidgets>
#include "global.h"

ReplaceDialog::ReplaceDialog(QString tofind, QString replacement): QDialog() {
	FLOG();
	QGridLayout *grid = new QGridLayout();

	// target word
	label1_ = new QLabel(tr("Target: "));
	text1_ = new QLineEdit();
	text1_->setText(tofind);
	combotext1_ = new QComboBox();
	combotext1_->setLineEdit(text1_);
	grid->addWidget(label1_, 0, 0);
	grid->addWidget(combotext1_, 0, 1, 1, 3);

	label2_ = new QLabel(tr("Replacement: "));
	text2_ = new QLineEdit();
	text2_->setText(replacement);
	combotext2_ = new QComboBox();
	combotext2_->setLineEdit(text2_);
	grid->addWidget(label2_, 1, 0);
	grid->addWidget(combotext2_, 1, 1, 1, 3);

	// accept/reject
	grid->setRowMinimumHeight(2, 15);
	button_findnext_ = new QPushButton(tr("Find Next"));
	button_replace_ = new QPushButton(tr("Replace"));
	button_replaceall_ = new QPushButton(tr("Replace All"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_findnext_, 3, 0);
	grid->addWidget(button_replace_, 3, 1);
	grid->addWidget(button_replaceall_, 3, 2);
	grid->addWidget(button_cancel_, 3, 3);
	grid->setContentsMargins(20, 20, 20, 20);
	setLayout(grid);

	connect(button_findnext_, SIGNAL(clicked()), this, SLOT(clicked_findnext()));
	connect(button_replace_, SIGNAL(clicked()), this, SLOT(clicked_replace()));
	connect(button_replaceall_, SIGNAL(clicked()), this, SLOT(clicked_replaceall()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(clicked_cancel()));
//	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));

	action_ = ACTION_NONE;
	setModal(false);
	setWindowTitle("Search and replace");
}


void ReplaceDialog::clicked_findnext() {
	FLOG();
	action_ = ACTION_FINDNEXT;
	emit replaceClicked();
}

void ReplaceDialog::clicked_replace() {
	FLOG();
	action_ = ACTION_REPLACE;
	emit replaceClicked();
}

void ReplaceDialog::clicked_replaceall() {
	FLOG();
	action_ = ACTION_REPLACEALL;
	emit replaceClicked();
}

void ReplaceDialog::clicked_cancel() {
	FLOG();
	action_ = ACTION_CANCEL;
	emit replaceClicked();
}

void ReplaceDialog::getOut() {
	FLOG();
	QString str = getWordToFind();
	// check if the word is not already in the list
	bool found = false;
	for (int i = 0; i < combotext1_->count(); i++) {
		if (combotext1_->itemText(i) == str) {
			found = true;
			break;
		}
	}
	if (!found)
		combotext1_->insertItem(0, str);

	str = getReplacement();
	found = false;
	for (int i = 0; i < combotext2_->count(); i++) {
		if (combotext2_->itemText(i) == str) {
			found = true;
			break;
		}
	}
	if (!found)
		combotext2_->insertItem(0, str);

	action_ = ACTION_NONE;
	accept();
}

QString ReplaceDialog::getWordToFind() {
	FLOG();
	if (text1_ == 0)
		return "";

	QString retval;

	if (text1_->text().isEmpty() == true)
		retval = "";
	else {
		retval = text1_->text();
	}

	return retval;
}

QString ReplaceDialog::getReplacement() {
	FLOG();
	if (text2_ == 0)
		return "";

	QString retval;

	if (text2_->text().isEmpty() == true)
		retval = "";
	else {
		retval = text2_->text();
	}

	return retval;
}

