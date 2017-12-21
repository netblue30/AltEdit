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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
class BasicEdit;
class Highlighter;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QString program, QString file);

private:
	void readSettings();
	void writeSettings();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void setExtraSelection();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void next();
	void prev();

private:
	void run_diff(const char *program);

	// text window
	BasicEdit *text_;
	Highlighter *highlighter_;

	// info window
	BasicEdit *info_;
	Highlighter *info_highlighter_;

	// menus and actions
	QMenu *fileMenu;
	QMenu *editMenu;
	QAction *exitAct;
	QAction *nextAct;
	QAction *prevAct;
	QToolBar *toolsToolBar;
};
#endif






