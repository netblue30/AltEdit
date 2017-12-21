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
#include "highlighter.h"
#include "global.h"
#include "basicedit.h"
#include <QtWidgets>

MainWindow::MainWindow(QString program, QString file) {
	FLOG();

	// text window
	text_ = new BasicEdit();
	highlighter_ = new Highlighter(text_->document());

	// info window
	info_ = new BasicEdit();
	info_->setMaximumHeight(100);
	info_->setReadOnly(true);
	info_highlighter_ = new Highlighter(info_->document());

	QSplitter *split1 = new QSplitter();
	split1->setOrientation(Qt::Vertical);
	split1->addWidget(text_);
	split1->addWidget(info_);
//	split1->setStyleSheet("QSplitter::handle{background: #a08080;}");

	setCentralWidget(split1);
	run_diff(program.toUtf8().data());
	text_->setLine(1);
	next();

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	readSettings();
	QSize sz = size();
	QList<int> myList;
	myList.append(sz.height() * 9 / 10);
	myList.append(sz.height() / 10);
	split1->setSizes(myList);

	statusBar()->showMessage(file);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	FLOG();
	(void) event;
	writeSettings();
}

void MainWindow::readSettings() {
	FLOG();
	QSettings settings("alt", "ndiff");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::writeSettings() {
	FLOG();
	QSettings settings("alt", "ndiff");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void MainWindow::createActions() {
	FLOG();

	exitAct = new QAction(QIcon(":/images/application-exit.svg"), tr("E&xit"), this);
	QList<QKeySequence> shortcuts_exit;
	shortcuts_exit.append(QKeySequence(Qt::ALT+Qt::Key_X));
	exitAct->setShortcuts(shortcuts_exit);
	exitAct->setStatusTip(tr("Exit the text editor"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	nextAct = new QAction(QIcon(":/images/right.svg"), tr("&Next"), this);
	QList<QKeySequence> shortcuts_next;
	shortcuts_next.append(QKeySequence(Qt::ALT+Qt::Key_N));
	nextAct->setShortcuts(shortcuts_next);
	nextAct->setStatusTip(tr("Go to the next difference"));
	connect(nextAct, SIGNAL(triggered()), this, SLOT(next()));

	prevAct = new QAction(QIcon(":/images/left.svg"), tr("&Previous"), this);
	QList<QKeySequence> shortcuts_prev;
	shortcuts_prev.append(QKeySequence(Qt::ALT+Qt::Key_P));
	prevAct->setShortcuts(shortcuts_prev);
	prevAct->setStatusTip(tr("Go to the previous difference"));
	connect(prevAct, SIGNAL(triggered()), this, SLOT(prev()));
}

void MainWindow::createMenus() {
	FLOG();
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAct);
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(nextAct);
	editMenu->addAction(prevAct);
}

void MainWindow::createToolBars() {
	FLOG();
	toolsToolBar = addToolBar(tr("Tools"));
	toolsToolBar->addAction(prevAct);
	toolsToolBar->addAction(nextAct);
}

void MainWindow::createStatusBar() {
	FLOG();
}

void MainWindow::next() {
	FLOG();
	int line = text_->getLineNumber();
	bool firstline = (line == 1);

	// find the last line
	int last = text_->blockCount();

	while (line < last) {
		line++;
		text_->setLine(line);
		QString str = text_->getLineText();
		if (str.startsWith("+") || str.startsWith("-"))
			break;
	}

	if (line == last && !firstline)
		prev();

	setExtraSelection();
}

void MainWindow::prev() {
	FLOG();
	int line = text_->getLineNumber();

	while (line > 0) {
		line--;
		text_->setLine(line);
		QString str = text_->getLineText();
		if (str.startsWith("+") || str.startsWith("-"))
			break;
	}

	if (line == 0)
		next();
	setExtraSelection();
}

void MainWindow::setExtraSelection() {
	QList<QTextEdit::ExtraSelection> es;
	QTextEdit::ExtraSelection selection;
	QColor lineColor = QColor(Qt::white).darker(100);
	selection.format.setBackground(lineColor);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
//	selection.format.setToolTip(QString("%1").arg(i));
	QTextCursor qtc(text_->textCursor());
	selection.cursor = qtc;
	selection.cursor.clearSelection();
	es.append(selection);
	text_->setExtraSelections(es);
}

#define PROG_BUF 4096
void MainWindow::run_diff(const char *program) {
	char buf[PROG_BUF + 1];
	char *out;
	FILE *input = popen(program, "r");
	int line = 0;

	if (0 == input) {
		fprintf(stderr, "Error: cannot run %s\n", program);
		return;
	}
	else {
		while ((out = fgets(buf, PROG_BUF, input)) != 0) {
			line++;

			// skip the first two lines and line 5:
			// diff --git a/global.h b/basicedit.h
			// index aa6bb38..60f8d26 100644
			// --- a/global.h
			// +++ b/basicedit.h
			// @@ -1,29 +1,175 @@
			if (line == 1 || line == 2 || line == 5)
				continue;
			if (line == 3 || line == 4) {
				info_->insertPlainText(QString::fromUtf8(buf));
				info_->update();
				continue;
			}

			text_->insertPlainText(QString::fromUtf8(buf));
//			wdg->gotoEnd();
			text_->update();
			qApp->processEvents();
		}

		pclose(input);
	}

	if (!line)
		text_->insertPlainText("There are no differences!\n");

}





