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
#include "find.h"
#include "global.h"

FindDialog::FindDialog(): QDialog() {
	FLOG();
	QGridLayout *grid = new QGridLayout();

	// target word
	label_ = new QLabel(tr("Target: "));
	text_ = new QLineEdit();
	combotext_ = new QComboBox();
	combotext_->setLineEdit(text_);
	grid->addWidget(label_, 0, 0);
	grid->addWidget(combotext_, 0, 1, 1, 10);

	// checkboxes
	ignore_case_ = new QCheckBox(tr("Ignore case"));
	whole_ = new QCheckBox(tr("Whole words"));
	backwards_ = new QCheckBox(tr("Search Backwards"));
	grid->addWidget(ignore_case_, 1, 1, 1, 2);
	grid->addWidget(whole_, 2, 1, 1, 2);
	grid->addWidget(backwards_, 3, 1, 1, 2);

	// accept/reject
	grid->setRowMinimumHeight(4, 15);
	button_ok_ = new QPushButton(tr("OK"));
	button_cancel_ = new QPushButton(tr("Cancel"));
	grid->addWidget(button_ok_, 5, 9);
	grid->addWidget(button_cancel_, 5, 10);
	setLayout(grid);
	grid->setContentsMargins(30, 30, 30, 30);

	connect(button_ok_, SIGNAL(clicked()), SLOT(clicked_ok()));
	connect(button_cancel_, SIGNAL(clicked()), SLOT(reject()));
	setWindowTitle("Find");
}

void FindDialog::setWord(QString word) {
	text_->setText(word);
}

QString FindDialog::getWord() {
	return text_->text();
}

QTextDocument::FindFlags FindDialog::getFlags() {
	// process flags
	unsigned flags = 0;
	if (ignore_case_->isChecked())
		;
	else
		flags += QTextDocument::FindCaseSensitively;

	if (whole_->isChecked())
		flags += QTextDocument::FindWholeWords;

	if (backwards_->isChecked())
		flags += QTextDocument::FindBackward;

	return (QTextDocument::FindFlags) flags;
}

void FindDialog::clicked_ok() {
	FLOG();
	QString word = text_->text();
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
