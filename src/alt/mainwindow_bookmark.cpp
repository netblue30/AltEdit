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


void MainWindow::gotoBookmark() {
	FLOG();

	BookmarkDialog bd;
	if (QDialog::Accepted == bd.exec()) {
		int index = bd.getBookmark();
		if (index >= MAXBOOKMARKS)
			return;

		saveBufmgr();
		if (active().bufmgr_->active()->file_ == active().bookmarks_[index].file_) {
			active().text_->setPosition(active().bookmarks_[index].pos_);
		}
		else if (active().bufmgr_->set(active().bookmarks_[index].file_)) {
			restoreBufmgr();
			active().text_->setPosition(active().bookmarks_[index].pos_);
		}
		updateBufmgrActions();
		QTimer::singleShot(50, this, SLOT(statusDelay()));
		active().text_->setFocus();
	}
}

void MainWindow::saveBufmgr() {
	FLOG();

	// save all data in bufmgr
	int i;
	for (i = 0; i < MAX_PANELS; i++) {
		BufMgrData *data = panel[i].bufmgr_->active();
		data->text_= panel[i].text_->toPlainText();
		data->pos_ = panel[i].text_->getPosition();
	}

	QString file = active().bufmgr_->active()->file_;
	// update bookmarks

	QList<QTextEdit::ExtraSelection> es =active().text_->extraSelections(); ;
	fdebug("extra selections list size %d\n", es.size());
	for (int i = 0; i < es.size(); i++) {
		QTextEdit::ExtraSelection bm = es[i];
		int index = bm.format.toolTip().toInt();
		if (index < MAXBOOKMARKS) {
			if (file == active().bookmarks_[index].file_) {
				int pos = bm.cursor.position();
				active().bookmarks_[index].pos_ = pos;
				fdebug("update bookmark %d, %s, %d\n", index, file.toUtf8().data(), pos);
			}
		}
	}
}

void MainWindow::restoreBufmgr() {
	FLOG();

	// resore the current text from bufmgr
	BufMgrData *data = active().bufmgr_->active();
	active().text_->setPlainText(data->text_);

	// display bookmarks
	int i;
	QString file = data->file_;
	QList<QTextEdit::ExtraSelection> es;
	if (!file.isEmpty() && !file.isNull()) {
		for (i = 0; i < MAXBOOKMARKS; i++) {
			if (active().bookmarks_[i].file_ == file) {
				fdebug("found %s at pos %d\n", file.toUtf8().data(), active().bookmarks_[i].pos_);
				active().text_->setPosition(active().bookmarks_[i].pos_);
				QTextEdit::ExtraSelection selection;

				QColor lineColor = QColor(Qt::red).darker(250);
				selection.format.setBackground(lineColor);
				selection.format.setProperty(QTextFormat::FullWidthSelection, true);
				selection.format.setToolTip(QString("%1").arg(i));

				QTextCursor qtc(active().text_->textCursor());
				selection.cursor = qtc;
				selection.cursor.clearSelection();
				es.append(selection);
	//			active().text_->setExtraSelections(es);
			}
		}
		fdebug("size %d\n", es.size());
	}
	active().text_->setExtraSelections(es);

	// restore cursor position
	active().text_->setPosition(data->pos_);
	filebar_->setDirForFile(active().bufmgr_->active()->file_);
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::setBookmark0() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[0].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[0].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark1() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[1].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[1].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark2() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[2].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[2].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark3() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[3].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[3].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark4() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[4].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[4].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark5() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[5].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[5].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark6() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[6].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[6].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark7() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[7].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[7].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark8() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[8].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[8].pos_ = active().text_->getPosition();
	restoreBufmgr();
}

void MainWindow::setBookmark9() {
	FLOG();
	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
			tr("This is an untitled file, you would need to save it firest."));
		return;
	}

	saveBufmgr();
	active().bookmarks_[9].file_ = active().bufmgr_->active()->file_;
	active().bookmarks_[1].pos_ = active().text_->getPosition();
	restoreBufmgr();
}
