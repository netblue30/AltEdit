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
#include <QString>
#include "global.h"

class QAction;
class QMenu;
class MyTextEdit;
class MyFileBar;
class MyInfoText;
class BufMgr;
class Highlighter;
class FindDialog;
class ReplaceDialog;
class GrepDialog;
class DiffDialog;
class QLabel;

struct Bookmark {
	QString file_;
	int pos_;
	Bookmark(): pos_(0) {}
};

struct Panel {
	MyTextEdit *text_;
	BufMgr *bufmgr_;
	Highlighter *highlighter_;
	Bookmark bookmarks_[MAXBOOKMARKS];
};

class MainWindow : public QMainWindow {
	Q_OBJECT
	static const int MAX_PANELS = 4;

public:
	MainWindow();
	void openFile(QString fileName,  bool print_function_list = false);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	bool saveAll();
	void closeBuffer();
	void about();
	void undo();
	void cut();
	void copy();
	void paste();
	void del();
	void find();
	void findNext();
	void replace();
	void replaceSlot();
	void grep();
	void diff();
	void astyle();
	void gotoLine();
	void gotoStart();
	void gotoEnd();
	void gotoBookmark();
	bool maybeSaveAll();
	void functionList();
	void make();
	void spell();
	void block();
	void metaPressed();

	void clicked0(bool);
	void clicked1(bool);
	void clicked2(bool);
	void clicked3(bool);

	void textModified0();
	void textModified1();
	void textModified2();
	void textModified3();

	void accumulating(bool);

	void clicked_filebar(bool);
	void clicked_info(bool);
	void clicked_bufmgr(QAction *action);
	void cursorMoved();
	void setTitleDelay();
	void statusDelay();
	void selectionChanged();
	void quickSearch(QString word);
	void quickGrep(QString word);
	void recording(bool);
	void setBookmark0();
	void setBookmark1();
	void setBookmark2();
	void setBookmark3();
	void setBookmark4();
	void setBookmark5();
	void setBookmark6();
	void setBookmark7();
	void setBookmark8();
	void setBookmark9();

private:
	void setStatusModified(bool status);
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName, bool warning = true);
//	QString strippedName(const QString &fullFileName);
	void setTitle();
	void updateBufmgrActions(void);
	void saveBufmgr();
	void restoreBufmgr();
	void saveTitledFiles();

	// text windows
	Panel panel[MAX_PANELS];
	int active_;	// active panel
	Panel &active() {
		return panel[active_];
	}

	// file and information windows
	MyFileBar *filebar_;
	MyInfoText *infotext_;
	Highlighter *infohighlighter_;

	// status bar
	QLabel *status_position_;
	QLabel *status_selection_;
	QLabel *status_recording_;
	QLabel *status_accumulating_;
	QLabel *status_modified_;

	// menu
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *buffersMenu;
	QMenu *toolsMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *toolsToolBar;
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *saveAllAct;
	QAction *closeAct;
	QAction *exitAct;
	QAction *undoAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *deleteAct;
	QAction *findAct;
	QAction *findNextAct;
	QAction *replaceAct;
	QAction *grepAct;
	QAction *astyleAct;
	QAction *diffAct;
	QAction *blockAct;
	QAction *gotoLineAct;
	QAction *gotoStartAct;
	QAction *gotoEndAct;
	QAction *functionListAct;
	QAction *makeAct;
	QAction *spellAct;
	QAction *gotoBookmarkAct;
	QAction *nextbufferAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
	QAction* setBookmark0Act;
	QAction* setBookmark1Act;
	QAction* setBookmark2Act;
	QAction* setBookmark3Act;
	QAction* setBookmark4Act;
	QAction* setBookmark5Act;
	QAction* setBookmark6Act;
	QAction* setBookmark7Act;
	QAction* setBookmark8Act;
	QAction* setBookmark9Act;
	FindDialog *find_;
	ReplaceDialog *replace_;
	GrepDialog *grep_;
	DiffDialog *diff_;

	bool no_ctags_;
	bool no_make_;
	bool no_git_;
	bool no_hunspell_;
	bool no_grep_;
	bool no_astyle_;
};
#endif
