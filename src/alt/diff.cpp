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
#include "diff.h"
#include <QtWidgets>

DiffDialog::DiffDialog(): QDialog(), mode_(GIT_MODE) {
	QGridLayout *grid = new QGridLayout(this);

	// git diff
	git_box_ = new QCheckBox(tr("Git diff"));
	git_box_->setChecked(true);
	connect(git_box_, SIGNAL(toggled(bool)), this, SLOT(setGitMode(bool)));

	// file diff
	files_box_ = new QGroupBox(tr("Independent files"));
	files_box_->setCheckable(true);
	files_box_->setChecked(false);
//	files_box->setStyleSheet("QGroupBox { color : black; }");
	connect(files_box_, SIGNAL(toggled(bool)), this, SLOT(setFilesMode(bool)));

	// file1
	QLabel *label1 = new QLabel(tr("File 1: "));
	file1_ = new QLineEdit();
	QPushButton *button_browse1 = new QPushButton(tr("browse"));
//	grid->addWidget(label1, 0, 0);
//	grid->addWidget(file1_, 0, 1, 1, 2);
//	grid->addWidget(button_browse1, 0, 3);

	// file2
	QLabel *label2 = new QLabel(tr("File 2: "));
	file2_ = new QLineEdit();
	QPushButton *button_browse2 = new QPushButton(tr("browse"));
//	grid->addWidget(label2, 1, 0);
//	grid->addWidget(file2_, 1, 1, 1, 2);
//	grid->addWidget(button_browse2, 1, 3);

	// files layout
	QGridLayout *files_layout = new QGridLayout();
	files_layout->addWidget(label1, 0, 0);
	files_layout->addWidget(file1_, 0, 1, 1, 4);
	files_layout->addWidget(button_browse1, 0, 5);
	files_layout->addWidget(label2, 1, 0);
	files_layout->addWidget(file2_, 1, 1, 1, 4);
	files_layout->addWidget(button_browse2, 1, 5);
	files_box_->setLayout(files_layout);

	// accept/reject
	grid->addWidget(git_box_, 0, 0, 1, 7);
	grid->setRowMinimumHeight(1, 1);
	grid->addWidget(files_box_, 2, 0, 1, 7);
	grid->setRowMinimumHeight(3, 15);
	QPushButton *button_ok = new QPushButton(tr("OK"));
	QPushButton *button_cancel = new QPushButton(tr("Cancel"));
	QLabel *w1 = new QLabel(" ");
	grid->addWidget(w1, 4, 0);
	grid->addWidget(button_ok, 5, 5);
	grid->addWidget(button_cancel, 5, 6);

	connect(button_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(button_cancel, SIGNAL(clicked()), SLOT(reject()));
	connect(button_browse1, SIGNAL(clicked()), this, SLOT(clicked_browse1()));
	connect(button_browse2, SIGNAL(clicked()), this, SLOT(clicked_browse2()));
	grid->setContentsMargins(30, 30, 30, 30);
	setLayout(grid);
}

DiffDialog::mode_t DiffDialog::getMode() {
	return mode_;
}

void DiffDialog::setFilesMode(bool on) {
	mode_ = (on)? FILES_MODE: GIT_MODE;
	files_box_->setChecked(on);
	git_box_->setChecked(!on);
}

void DiffDialog::setGitMode(bool on) {
	mode_ = (on)? GIT_MODE: FILES_MODE;
	files_box_->setChecked(!on);
	git_box_->setChecked(on);
}

QString DiffDialog::getFile1() {
	if (file1_ == 0)
		return "";

	QString retval;

	if (file1_->text().isEmpty() == true)
		retval = "";
	else {
		retval = file1_->text();
	}
	return retval;
}


QString DiffDialog::getFile2() {
	if (file2_ == 0)
		return "";

	QString retval;

	if (file2_->text().isEmpty() == true)
		retval = "";
	else {
		retval = file2_->text();
	}
	return retval;
}


void DiffDialog::clicked_browse1() {
	QString file = QFileDialog::getOpenFileName(this );
	if (!file.isNull()) {
		file1_->setText(file);
	}
}


void DiffDialog::clicked_browse2() {
	QString file = QFileDialog::getOpenFileName(this );
	if (!file.isNull()) {
		file2_->setText(file);
	}
}


