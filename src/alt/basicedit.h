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
#ifndef BASICEDIT_H
#define BASICEDIT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>

#include <QtWidgets>

class BasicEdit: public QPlainTextEdit {
	Q_OBJECT
public:
	BasicEdit();

	// line is 1 based
	int getLineNumber() {
		QTextCursor tc = textCursor();
		return tc.blockNumber() + 1;
	}

	int getColumnNumber() {
		QTextCursor tc = textCursor();
		return tc.columnNumber();
	}

	// get current line
	virtual QString getLineText() {
		QTextCursor tc = textCursor();
		return tc.block().text() + '\n';
	}

	// get first line
	QString getFirstLineText() {
		int pos = getPosition();
		gotoStart();
		QString rv = getLineText();
		setPosition(pos);
		return rv;
	}

	int getPosition() {
		QTextCursor tc = textCursor();
		return tc.position();
	}

	void setPosition(int pos) {
		QTextCursor tc = textCursor();
		tc.setPosition(pos);
		setTextCursor(tc);
	}

	void gotoStart() {
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::Start);
		setTextCursor(tc);
	}

	void gotoStartLine() {
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::StartOfLine);
		setTextCursor(tc);
	}

	void gotoEnd() {
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::End);
		setTextCursor(tc);
	}

	// line is 1 based
	void setLine(int line) {
		line--;
		QTextCursor tc = textCursor();
		int current_line = tc.blockNumber();

		if (line > current_line)
			tc.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line - current_line);
		else {
			tc.movePosition(QTextCursor::Start);
			tc.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line);
		}
		setTextCursor(tc);
	}

	bool hasSelection() {
		QTextCursor tc = textCursor();
		return tc.hasSelection();
	}

	QString getSelection() {
		QTextCursor tc = textCursor();
		QTextDocumentFragment sel = tc.selection();
		return sel.toPlainText();
	}

	// return the position of the start of selection
	int selectionStart() {
		QTextCursor tc = textCursor();
		return tc.selectionStart();
	}

	// return the position of the end of selection
	int selectionEnd() {
		QTextCursor tc = textCursor();
		return tc.selectionEnd();
	}

	void clearSelection() {
		QTextCursor tc = textCursor();
		tc.clearSelection();
		setTextCursor(tc);
	}

	void deleteSelection() {
		QTextCursor tc = textCursor();
		tc.removeSelectedText();
		setTextCursor(tc);
	}

	// select a line including the ending \n if available
	void selectLine() {
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		if (tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor))
			tc.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
		else
			tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		setTextCursor(tc);
	}

	QString wordAtCursor() {
		QTextCursor tc = textCursor();
		tc.select(QTextCursor::WordUnderCursor);
		return tc.selectedText();
	}

	void replaceSelected(QString word) {
		if (!hasSelection())
			return;
		QTextCursor tc = textCursor();
		tc.insertText(word);
	}

signals:
	// mouse click, right button
	void clicked(bool right);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
};


class MyTextEdit: public BasicEdit {
	Q_OBJECT
public:
	MyTextEdit(): BasicEdit(), recording_keys_(false) {}
	void selectCodeBlock();

signals:
	void textModified();
	void cursorMoved();
	void search(QString);
	void grep(QString);
	void recording(bool);
	void accumulating(bool);
	void metaPressed();

protected:
	void keyPressEvent(QKeyEvent *e);
	void mouseReleaseEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	// key recording
	QList<QKeyEvent> list_;
	bool recording_keys_;

	// clipboard accumulation
	static bool accumulating_;
	static QString acc_buffer_;

};

class MyFileBar: public BasicEdit {
	Q_OBJECT
public:
	MyFileBar();
	~MyFileBar();

	QString getLineText() {
		QTextCursor tc = textCursor();
		QString rv = dirname_ + "/" + tc.block().text();
		return rv;
	}

	void setDirForFile(QString file);

private slots:
	void updateFiles(bool force = false);

protected:
	void mouseReleaseEvent(QMouseEvent *event);

private:
	QString dirname_;
	int fd_;	// inotify descriptor
	int wd_;	// inotify watch descriptor
};

class MyInfoText: public BasicEdit {
	Q_OBJECT
public:
	MyInfoText(): BasicEdit() {
		setMaximumHeight(300);
		setReadOnly(true);
		setContextMenuPolicy(Qt::PreventContextMenu);
	}

protected:
	void mousePressEvent(QMouseEvent *event);
};

#endif




