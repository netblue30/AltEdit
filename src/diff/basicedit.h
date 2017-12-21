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
	BasicEdit(): QPlainTextEdit() {
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
	}

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

	// select a line including the ending \n
	void selectLine() {
		QTextCursor tc = textCursor();
		tc.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
		tc.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
		setTextCursor(tc);
	}

	QString wordAtCursor() {
		QTextCursor tc = textCursor();
		tc.select(QTextCursor::WordUnderCursor);
		return tc.selectedText();
//		tc.clearSelection();
//		tc.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);

	}

	void replaceSelected(QString word) {
		if (!hasSelection())
			return;
		QTextCursor tc = textCursor();
		tc.insertText(word);
	}

#if 0
signals:
	void clicked(int line, int col);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
#endif
};

#endif

