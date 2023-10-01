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
#include "basicedit.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include "global.h"

BasicEdit::BasicEdit(): QPlainTextEdit() {
	FLOG();

	// colorize
	QPalette qp = palette();
	qp.setColor(QPalette::Active, QPalette::Base, Qt::black);
	qp.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
	qp.setColor(QPalette::Active, QPalette::Text, Qt::white);
	qp.setColor(QPalette::Inactive, QPalette::Text, Qt::white);
	qp.setColor(QPalette::Active, QPalette::Highlight, Qt::red);
	qp.setColor(QPalette::Inactive, QPalette::Highlight, Qt::red);
	setPalette(qp);

	// no word wrap
	setWordWrapMode(QTextOption::NoWrap);

//	QFont font("Neue Haas Grotesk Text Pro Medium");
//	QFont font("Courier New");
//QString f = font.family();
//printf("%s\n", f.toStdString().c_str());
//	font.setPointSize(12);
//	font.setStyleHint(QFont::Monospace);
//	this->setFont(font);
}

void BasicEdit::mouseReleaseEvent(QMouseEvent *event) {
	FLOG();

	if (event->button() == Qt::MidButton) {}
	else
		emit clicked(event->button() == Qt::RightButton);

	QPlainTextEdit::mouseReleaseEvent(event);
}

bool MyTextEdit::accumulating_ = false;
QString MyTextEdit::acc_buffer_ = QString("");

void MyTextEdit::mouseReleaseEvent(QMouseEvent *event) {
	FLOG();

	BasicEdit::mouseReleaseEvent(event);

	if (event->button() == Qt::MidButton)
		emit textModified();
}

void MyTextEdit::keyPressEvent(QKeyEvent *e) {
	int key = e->key();
	bool numpad_mod = int(e->modifiers()) & Qt::KeypadModifier;
	bool alt_mod = int(e->modifiers()) & Qt::AltModifier;
	bool ctrl_mod = int(e->modifiers()) & Qt::ControlModifier;

	if (recording_keys_ && key != Qt::Key_F1) {
		fdebug("MyTextEdit - recording key %d\n", list_.size());
		list_.push_back(*e);
	}

	if (!numpad_mod && !alt_mod && !ctrl_mod) {
		switch (key) {
		case Qt::Key_Return: {
			QString line = getLineText();
			int col = getColumnNumber();
			char *start = strdup(qPrintable(line));
			if (!start) {
				perror("strdup");
				return;
			}
			char *ptr = start;
			int col_cnt = 0;
			while ((*ptr == ' ' || *ptr == '\t') && col_cnt < col) {
				ptr++;
				col_cnt++;
			}
			*ptr = '\0';
			insertPlainText(QString("\n") + QString(start));
			free(start);
			emit textModified();
			return;
		}

		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
		case Qt::Key_Home:
		case Qt::Key_End:
			emit cursorMoved();
			break;

		case Qt::Key_Insert:
//				gotoStartLine();
			if (accumulating_) {
				accumulating_ = false;
				insertPlainText(acc_buffer_);
				emit accumulating(false);
			}
			else
				paste();
			emit textModified();
			return;

		case Qt::Key_F1:
			if (recording_keys_) {
				fdebug("MyTextEdit - stop recording\n");
				recording_keys_ = false;
				emit recording(false);
			}
			else {
				fdebug("MyTextEdit - playback %d keys\n", list_.size());
				QList<QKeyEvent>::iterator it;
				for (it = list_.begin(); it != list_.end(); ++it) {
					QKeyEvent ev = *it;
					keyPressEvent(&ev);
				}
			}
			break;

		case Qt::Key_Shift:
		case Qt::Key_Escape:
		case Qt::Key_NumLock:
		case Qt::Key_CapsLock:
			break;

		case Qt::Key_Meta:
			emit metaPressed();
			break;

		default:
			emit textModified();
			break;
		}
	}


	// ctrl
	if (ctrl_mod) {
		switch (key) {
		case Qt::Key_PageUp:
			gotoStart();
			return;
		case Qt::Key_PageDown:
			gotoEnd();
			return;
		default:
			break;
		}
	}

	// alt
	if (alt_mod) {
		switch (key) {
		case Qt::Key_R:
			recording_keys_ = true;
			//clear the list
			while (!list_.empty())
				list_.erase(list_.begin());
			fdebug("MyTextEdit - start recording\n");
			emit recording(true);
			return;
		default:
			break;
		}
	}

	// numeric pad + and - keys
	if (numpad_mod) {
		emit textModified();
		if (key == Qt::Key_Plus) {
			if (hasSelection()) {
				if (accumulating_)
					acc_buffer_ += getSelection();
				copy();
			}
			else {
				int pos = getPosition();
				selectLine();
				if (accumulating_)
					acc_buffer_ += getSelection();
				copy();
				setPosition(pos);
			}

			clearSelection();
			return;
		}
		else if (key == Qt::Key_Minus) {
			if (hasSelection())
				cut();
			else {
				// go to the start of the line
				gotoStartLine();
				int pos = getPosition();
				selectLine();
				cut();
				setPosition(pos);
			}

			return;
		}
	}

	BasicEdit::keyPressEvent(e);
}

void MyTextEdit::selectCodeBlock() {
	FLOG();

	int pos = getPosition();
	QString str = this->toPlainText();
	char *text = strdup(qPrintable(str) + pos);
	if (!text) {
		perror("strdup");
		return;
	}

	unsigned start;
	unsigned end;
	bool rv = find_block(text, &start, &end);
	if (rv) {	// select start to end
		setPosition(start + pos);
		start += pos;
		end += pos;
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end - start);
		setTextCursor(tc);
	}

	free(text);
}

void MyTextEdit::contextMenuEvent(QContextMenuEvent *event) {
	FLOG();

	QMenu *menu = createStandardContextMenu();
	menu->addAction("Accumulate clipboard");
	menu->addAction("Quick search");
	menu->addAction("Quick grep");

	QAction *sl = new QAction("Shift left", this);
	sl->setEnabled(false);
	menu->addAction(sl);

	QAction *sr = new QAction("Shift right", this);
	sr->setEnabled(false);
	menu->addAction(sr);

	if (hasSelection()) {
		sl->setEnabled(true);
		sr->setEnabled(true);
	}
	menu->addAction("{}");

	QAction *act = menu->exec(event->globalPos());
	if (act && act->text() == QString("Accumulate clipboard")) {
		accumulating_ = true;
		acc_buffer_ = QString("");
		emit accumulating(true);
	}
	else if (act && act->text() == QString("Quick search")) {
		QString word = wordAtCursor();
		fdebug("search %s\n", qPrintable(word));
		emit search(word);
	}
	else if (act && act->text() == QString("Quick grep")) {
		QString word = wordAtCursor();
		fdebug("grep %s\n", qPrintable(word));
		emit grep(word);
	}
	else if (act && act->text() == QString("{}")) {
		selectCodeBlock();
	}
	else if (act && act->text() == QString("Shift left")) {
		QString sel = getSelection();
		QStringList lines = sel.split('\n');
		QString result;

		for (int i = 0; i < lines.size(); i++) {
			if (lines[i].startsWith(" ") || lines[i].startsWith("\t"))
				lines[i] = lines[i].mid(1);

			result += lines[i];
			if (i != (lines.size() - 1))
				result += "\n";
		}

		replaceSelected(result);
		emit textModified();
	}
	else if (act && act->text() == QString("Shift right")) {
		QString sel = getSelection();
		QStringList lines = sel.split('\n');
		QString result;

		for (int i = 0; i < lines.size(); i++) {
			if (i != (lines.size() - 1))
				result += QString("\t") + lines[i] + "\n";
			else if (!lines[i].isEmpty())
				result += QString("\t") + lines[i];
		}

		replaceSelected(result);
		emit textModified();
	}
	delete menu;
}

MyFileBar::MyFileBar(): BasicEdit(), fd_(-1), wd_(-1) {
	FLOG();
	setMaximumWidth(170);
	setReadOnly(true);
//	setContextMenuPolicy(Qt::PreventContextMenu);

	// set current directory
	char *dirname = get_current_dir_name();
	if (!dirname) {
		perror("dirname");
		exit(1);
	}
	dirname_ = QString(dirname);
	free(dirname);

	fd_ = inotify_init();
	if (fd_ == -1) {
		perror("inotify_init");
		exit(1);
	}
	int flags = fcntl(fd_, F_GETFL, 0);
	fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
	updateFiles(true);
}

MyFileBar::~MyFileBar() {
	if (wd_ != -1) {
		assert(fd_ != -1);
		inotify_rm_watch(fd_, wd_);
	}
	if (fd_ != -1)
		::close(fd_);
}

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define EVENT_BUF_LEN     (1024*(EVENT_SIZE + 16))

void MyFileBar::updateFiles(bool force) {
//	FLOG();
	if (force) {
		if (wd_ != -1)
			inotify_rm_watch(fd_, wd_);
		wd_ = inotify_add_watch(fd_, qPrintable(dirname_), IN_MOVE | IN_CREATE | IN_DELETE);
		fdebug("inotify fd_ %d, wd_ %d\n", fd_, wd_);
	}
	else {
		char buffer[EVENT_BUF_LEN];
		int len = read(fd_, buffer, EVENT_BUF_LEN);
		// usually we get a -1 if there is nothing to read
		if (len <= 0)	 {
			QTimer::singleShot(1000, this, SLOT(updateFiles()));
			return;
		}
	}

	fdebug("filebar_ update\n");
	DIR *dir = opendir(qPrintable(dirname_));
	if (!dir) {
		perror("opendir");
		return;
	}

	QStringList list;
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (QString(entry->d_name).endsWith(".o"))
			continue;
		list.append(entry->d_name);
	}
	list.sort();
	QString files;
	for (int i = 0; i < list.size(); ++i)
		files += list.at(i) + "\n";
	setPlainText(files);

	closedir(dir);
	QTimer::singleShot(1000, this, SLOT(updateFiles()));

}

void MyFileBar::setDirForFile(QString dir) {
	int pos = dir.lastIndexOf("/");
	dir.truncate(pos);
	if (dir.isEmpty())
		dir = "/";

	if (dir != dirname_) {
		dirname_ = dir;
		updateFiles(true);
	}
}

void MyFileBar::mouseReleaseEvent(QMouseEvent *event) {
	FLOG();
	QString fname = getLineText();

	struct stat s;
	if (stat(qPrintable(fname), &s) == -1) {
		perror("stat");
		goto normal_exit;
	}

	if (S_ISDIR(s.st_mode)) {
		// going up on directory tree?
		if (fname.endsWith("/..")) {
			int pos = fname.indexOf("/..");
			fname.truncate(pos);
			pos = fname.lastIndexOf("/");
			fname.truncate(pos);
			if (fname.isEmpty())
				fname = "/";
		}
		dirname_ = fname;
		updateFiles(true);
	}
	else {
		emit clicked(false);
	}

normal_exit:
	QPlainTextEdit::mouseReleaseEvent(event);
}



void MyInfoText::mousePressEvent(QMouseEvent *event) {
	FLOG();

	if (event->button() == Qt::RightButton) {
		QMouseEvent *evnew = new QMouseEvent(event->type(), event->localPos(), Qt::LeftButton,
						     event->buttons(), event->modifiers());
		QPlainTextEdit::mousePressEvent(evnew);
		delete evnew;
		event->accept();
	}
	else
		BasicEdit::mousePressEvent(event);
}
