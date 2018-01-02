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

	// target word1
	QLabel *label = new QLabel(tr("Target: "));
	text1_ = new QLineEdit();
	combotext1_ = new QComboBox();
	combotext1_->setLineEdit(text1_);
	grid->addWidget(label, 0, 0);
	grid->addWidget(combotext1_, 0, 1, 1, 10);

	// target word2
	target_and_ = new QCheckBox(tr("and"));
	target_and_->setChecked(false);
	text2_ = new QLineEdit();
	combotext2_ = new QComboBox();
	combotext2_->setLineEdit(text2_);
	grid->addWidget(target_and_, 1, 0);
	grid->addWidget(combotext2_, 1, 1, 1, 10);
	combotext2_->setEnabled(false);

	QLabel *label2 = new QLabel(tr("Starting directory: "));
	sdirectory_ = new QLineEdit();
	button_dir_ = new QPushButton("...");
	sdirectory_->setText("./");
	grid->addWidget(label2, 2, 0);
	grid->addWidget(sdirectory_, 2, 1, 1, 9);
	grid->addWidget(button_dir_, 2, 10);

	QLabel *label3 = new QLabel(tr("File pattern: "));
	fpattern_ = new QLineEdit();
	fpattern_->setText("*.*");
	grid->addWidget(label3, 3, 0);
	grid->addWidget(fpattern_, 3, 1, 1, 10);

	ignore_case_ = new QCheckBox(tr("Ignore case"));
	grid->addWidget(ignore_case_, 4, 1);

	// accept/reject
	grid->setRowMinimumHeight(8, 15);
	button_ok_ = new QPushButton(tr("OK"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_ok_, 9, 9);
	grid->addWidget(button_cancel_, 9, 10);

	grid->setContentsMargins(20, 20, 20, 20);
	setLayout(grid);

	connect(button_ok_, SIGNAL(clicked()), SLOT(clicked_ok()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));
	connect(button_dir_, SIGNAL(clicked()), this, SLOT(clicked_dir()));
	connect(target_and_, SIGNAL(toggled(bool)), this, SLOT(target_and_toggled(bool)));
	setWindowTitle("grep");
}

bool GrepDialog::getIgnoreCase() {
	return ignore_case_->isChecked();
}

bool GrepDialog::getTargetAnd() {
	return target_and_->isChecked();
}

QString GrepDialog::getSdirectory() {
	return sdirectory_->text();
}

QString GrepDialog::getFpattern() {
	return fpattern_->text();
}

QString GrepDialog::getWord1() {
	return text1_->text();
}

QString GrepDialog::getWord2() {
	return text2_->text();
}

void GrepDialog::setWord(QString word) {
	target_and_->setChecked(false);
	text1_->setText(word);
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
	QString word = text1_->text();;
	// check if the word is not already in the list
	bool found = false;
	for (int i = 0; i < combotext1_->count(); i++) {
		if (combotext1_->itemText(i) == word) {
			found = true;
			break;
		}
	}
	if (!found)
		combotext1_->insertItem(0, word);

	found = false;
	word = text2_->text();;
	for (int i = 0; i < combotext2_->count(); i++) {
		if (combotext2_->itemText(i) == word) {
			found = true;
			break;
		}
	}
	if (!found)
		combotext2_->insertItem(0, word);

	accept();
}

void GrepDialog::target_and_toggled(bool state) {
	(void) state;
	combotext2_->setEnabled(target_and_->checkState());
}

