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
#include "grep.h"
#include <QtWidgets>
#include "global.h"

GrepDialog::GrepDialog(): QDialog() {
	FLOG();
	QGridLayout *grid = new QGridLayout();

	// target word
	label_ = new QLabel(tr("Target: "));
	text_ = new QLineEdit();
	combotext_ = new QComboBox();
	combotext_->setLineEdit(text_);
	grid->addWidget(label_, 0, 0);
	grid->addWidget(combotext_, 0, 1, 1, 10);


	label2_ = new QLabel(tr("Starting directory: "));
	sdirectory_ = new QLineEdit();
	button_dir_ = new QPushButton("...");
	sdirectory_->setText("./");
	grid->addWidget(label2_, 1, 0);
	grid->addWidget(sdirectory_, 1, 1, 1, 9);
	grid->addWidget(button_dir_, 1, 10);

	label3_ = new QLabel(tr("File pattern: "));
	fpattern_ = new QLineEdit();
	fpattern_->setText("*.*");
	grid->addWidget(label3_, 2, 0);
	grid->addWidget(fpattern_, 2, 1, 1, 10);

	ignore_case_ = new QCheckBox(tr("Ignore case"));
	grid->addWidget(ignore_case_, 3, 1);

	// accept/reject
	grid->setRowMinimumHeight(7, 15);
	button_ok_ = new QPushButton(tr("OK"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_ok_, 8, 9);
	grid->addWidget(button_cancel_, 8, 10);

	grid->setContentsMargins(20, 20, 20, 20);
	setLayout(grid);

//	connect(button_ok_, SIGNAL(clicked()), SLOT(accept()));
	connect(button_ok_, SIGNAL(clicked()), SLOT(clicked_ok()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));
	connect(button_dir_, SIGNAL(clicked()), this, SLOT(clicked_dir()));
	setWindowTitle("grep");
}

bool GrepDialog::getIgnoreCase() {
	return ignore_case_->isChecked();
}

QString GrepDialog::getSdirectory() {
	return sdirectory_->text();
}

QString GrepDialog::getFpattern() {
	return fpattern_->text();
}

QString GrepDialog::getWord() {
	return text_->text();
}

void GrepDialog::setWord(QString word) {
	return text_->setText(word);
}

void GrepDialog::clicked_dir() {
	FLOG();
	QString s = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (s.isNull() || s.isEmpty())
		return;

	sdirectory_->setText(s);
}

void GrepDialog::clicked_ok() {
	FLOG();
	QString word = text_->text();;
	// check if the word is not already in the list
	bool found = false;
	for (int i = 0; i < combotext_->count(); i++) {
		if (combotext_->itemText(i) == word) {
			found = true;
			break;
		}
	}
	if (!found)
		combotext_->insertItem(0, word);

	accept();
}
