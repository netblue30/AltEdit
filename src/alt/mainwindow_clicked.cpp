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
#include "mainwindow.h"
#include "basicedit.h"
#include "bufmgr.h"
#include "highlighter.h"
#include "find.h"
#include "replace.h"
#include "grep.h"
#include "jump.h"
#include "global.h"

void MainWindow::clicked_filebar(bool right) {
	FLOG();
	(void) right;

	// get full path for the file
	QString fname = filebar_->getLineText();
	fdebug("clicked filebar %s\n", qPrintable(fname));
	openFile(fname);
	functionList();
}

// line is 1 based
void MainWindow::clicked_info(bool right) {
	FLOG();
	(void) right;

	active().text_->setFocus();

	assert(infotext_);
	QString txt = infotext_->getLineText();
	char *str = strdup(qPrintable(txt));
	if (!str) {
		perror("strdup");
		return;
	}
	fdebug("clicked_info: %s\n", str);

	// parse text line
	if (*str == ' ' || *str == '\t') {
		free(str);
		return;
	}

	char *ptr = strchr(str, '\n');
	if (ptr)
		*ptr = '\0';
	ptr = strchr(str, '\t');
	if (ptr)
		*ptr = '\0';

	// the line number
	char *nstr = strchr(str, ':');
	if (!nstr) {
		free(str);
		return;
	}
	*nstr = '\0';
	if (*(++nstr) == '\0') {
		free(str);
		return;
	}

	// after ptr
	//  - make output
	//         :11	void MainWindow::clicked_filebar(int line, int col) {
	// - search (F5)
	//         :99		int line = atoi(nstr);
	// grep
	//         /home/netblue/work/alt/src/alt/mainwindow_clicked.cpp:83
	ptr = nstr;
	while (*ptr != '\0') {
		if (!isdigit(*ptr)) {
			*ptr = '\0';
			break;
		}
		ptr++;
	}

	// check file and line number
	int line = atoi(nstr);
	if (line < 0) {
		free(str);
		return;
	}

	if (right) {
		QString out = open_tagged_file(str);
		infotext_->setPlainText(out);
		infotext_->setLine(line);
	}
	else {
		QString file = QString(str);
		if (!file.isEmpty()) {
			// try to access the file
			if (access(str, R_OK)) {
				free(str);
				return;
			}
		}
		else {
			// the first line could be the file name
			file = infotext_->getFirstLineText();
			file.chop(1); // remove '\n"

			if (access(qPrintable(file), R_OK)) {
				free(str);
				return;
			}
		}

		fdebug("clicked_info: processing #%s# %d\n", qPrintable(file), line);
		openFile(file);
		active().text_->setLine(line);
	}

	free(str);
}

void MainWindow::clicked0(bool right) {
	FLOG();
	(void) right;
	bool dostatus = (active_ != 0);
	active_ = 0;
	if (dostatus) {
		setTitle();
		updateBufmgrActions();
	}
	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::clicked1(bool right) {
	FLOG();
	(void) right;
	bool dostatus = (active_ != 1);
	active_ = 1;
	if (dostatus) {
		setTitle();
		updateBufmgrActions();
	}
	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::clicked2(bool right) {
	FLOG();
	(void) right;
	bool dostatus = (active_ != 2);
	active_ = 2;
	if (dostatus) {
		setTitle();
		updateBufmgrActions();
	}
	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::clicked3(bool right) {
	FLOG();
	(void) right;

	bool dostatus = (active_ != 3);
	active_ = 3;
	if (dostatus) {
		setTitle();
		updateBufmgrActions();
	}
	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}


void MainWindow::clicked_bufmgr(QAction *action) {
	FLOG();
	fdebug("clicked_bufmgr: %s\n", qPrintable(action->text()));
	saveBufmgr();

	// goto next buffer
	if (action == nextbufferAct) {
		active().bufmgr_->next();
		goto restore;
	}

	// switch buffer
	if (active().bufmgr_->set(action->text()))
		goto restore;

	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);

	// do nothing
	return;

restore:
	// restore editor window
	restoreBufmgr();

	if (!active().bufmgr_->active()->file_.isEmpty())
		filebar_->setDirForFile(active().bufmgr_->active()->file_);

	// set window title
	setTitle();
}

void MainWindow::textModified0() {
	FLOG();
	active_ = 0;
	panel[0].text_->setFocus();
	BufMgrData *data = panel[0].bufmgr_->active();
	if (!data->modified_) {
		data->modified_ = true;
		updateBufmgrActions();
		setTitle();
	}
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::textModified1() {
	FLOG();
	active_ = 1;
	panel[1].text_->setFocus();
	BufMgrData *data = panel[1].bufmgr_->active();
	if (!data->modified_) {
		data->modified_ = true;
		updateBufmgrActions();
		setTitle();
	}
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::textModified2() {
	FLOG();
	active_ = 1;
	panel[2].text_->setFocus();
	BufMgrData *data = panel[2].bufmgr_->active();
	if (!data->modified_) {
		data->modified_ = true;
		updateBufmgrActions();
		setTitle();
	}
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::textModified3() {
	FLOG();
	active_ = 1;
	panel[3].text_->setFocus();
	BufMgrData *data = panel[3].bufmgr_->active();
	if (!data->modified_) {
		data->modified_ = true;
		updateBufmgrActions();
		setTitle();
	}
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::cursorMoved() {
	FLOG();
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::statusDelay() {
	int line = active().text_->getLineNumber();
	int col = active().text_->getColumnNumber();
	status_position_->setText(QString("%1:%2").arg(line).arg(col));
}

void MainWindow::selectionChanged() {
	FLOG();
	int start = active().text_->selectionStart();
	int end = active().text_->selectionEnd();
	int delta = end - start;
	if (delta)
		status_selection_->setText(QString("%1").arg(end - start));
	else
		status_selection_->setText(QString(""));
}

void MainWindow::recording(bool status) {
	FLOG();

	if (status)
		status_recording_->setText("Recording");
	else
		status_recording_->setText("");
}


void MainWindow::accumulating(bool status) {
	FLOG();

	if (status)
		status_accumulating_->setText("Accumulating");
	else
		status_accumulating_->setText("");
}

#if 0
void MainWindow::setStatusModified(bool status) {
	FLOG();

	if (status)
		status_modified_->setText("Modified");
	else
		status_modified_->setText("");
}
#endif