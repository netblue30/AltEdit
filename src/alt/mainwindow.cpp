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
#include "bcursor.h"
#include "global.h"
#include "diff.h"
#include "../../alt_config.h"
QString greputil(GrepDialog *dialog);

MainWindow::MainWindow() {
	FLOG();

	// check external programs are installed
	no_ctags_ = false;
	if (!check_program("ctags")) {
		fprintf(stderr, "Warning: ctags not installed\n");
		no_ctags_ = true;
	}
	no_make_ = false;
	if (!check_program("make")) {
		fprintf(stderr, "Warning: make not installed\n");
		no_make_ = true;
	}
	no_bcpp_ = false;
	if (!check_program("bcpp")) {
		fprintf(stderr, "Warning: bcpp not installed\n");
		no_bcpp_ = true;
	}
	no_git_ = false;
	if (!check_program("git")) {
		fprintf(stderr, "Warning: git not installed\n");
		no_git_ = true;
	}
	no_hunspell_ = false;
	if (!check_program("hunspell")) {
		fprintf(stderr, "Warning: hunspell not installed\n");
		no_hunspell_ = true;
	}
	no_grep_ = false;
	if (!check_program("grep")) {
		fprintf(stderr, "Warning: grep not installed\n");
		no_grep_ = true;
	}
	no_astyle_ = false;
	if (!check_program("astyle")) {
		fprintf(stderr, "Warning: astyle not installed\n");
		no_astyle_ = true;
	}


	active_ = 0;
	panel[0].bufmgr_ = new BufMgr();
	panel[0].bufmgr_->add("");
	panel[1].bufmgr_ = new BufMgr();
	panel[1].bufmgr_->add("");
	QSplitter *split10 = new QSplitter();
	split10->setOrientation(Qt::Horizontal);
	panel[0].text_ = new MyTextEdit;
	panel[1].text_ = new MyTextEdit;
	panel[1].text_->setMinimumSize(0, 0);
	split10->addWidget(panel[0].text_);
	split10->addWidget(panel[1].text_);
	split10->setStyleSheet("QSplitter::handle{background: #a08080;}");

	panel[2].bufmgr_ = new BufMgr();
	panel[2].bufmgr_->add("");
	panel[3].bufmgr_ = new BufMgr();
	panel[3].bufmgr_->add("");
	QSplitter *split11 = new QSplitter();
	split11->setOrientation(Qt::Horizontal);
	panel[2].text_ = new MyTextEdit;
	panel[3].text_ = new MyTextEdit;
	panel[3].text_->setMinimumSize(0, 0);
	split11->addWidget(panel[2].text_);
	split11->addWidget(panel[3].text_);
	split11->setStyleSheet("QSplitter::handle{background: #a08080;}");

	QSplitter *split1 = new QSplitter();
	split1->setOrientation(Qt::Vertical);
	split1->addWidget(split10);
	split1->addWidget(split11);
	split1->setStyleSheet("QSplitter::handle{background: #a08080;}");


	panel[0].highlighter_ = new Highlighter(panel[0].text_->document());
	panel[1].highlighter_ = new Highlighter(panel[1].text_->document());
	panel[2].highlighter_ = new Highlighter(panel[2].text_->document());
	panel[3].highlighter_ = new Highlighter(panel[3].text_->document());

	QSplitter *split2 = new QSplitter();
	split2->setOrientation(Qt::Horizontal);
	filebar_ = new MyFileBar();
	split2->addWidget(filebar_);
	split2->addWidget(split1);

	QSplitter *split3 = new QSplitter();
	split3->setOrientation(Qt::Vertical);
	infotext_ = new MyInfoText();
	infohighlighter_ = new Highlighter(infotext_->document());
	split3->addWidget(split2);
	split3->addWidget(infotext_);
	setCentralWidget(split3);


	panel[0].text_->setFocus();

	connect(panel[0].text_, SIGNAL(clicked(bool)), this, SLOT(clicked0(bool)));
	connect(panel[0].text_, SIGNAL(textModified()), this, SLOT(textModified0()));
	connect(panel[0].text_, SIGNAL(cursorMoved()), this, SLOT(cursorMoved()));
	connect(panel[0].text_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(panel[0].text_, SIGNAL(search(QString)), this, SLOT(quickSearch(QString)));
	connect(panel[0].text_, SIGNAL(grep(QString)), this, SLOT(quickGrep(QString)));
	connect(panel[0].text_, SIGNAL(recording(bool)), this, SLOT(recording(bool)));
	connect(panel[0].text_, SIGNAL(accumulating(bool)), this, SLOT(accumulating(bool)));
	connect(panel[0].text_, SIGNAL(metaPressed()), this, SLOT(metaPressed()));

	connect(panel[1].text_, SIGNAL(clicked(bool)), this, SLOT(clicked1(bool)));
	connect(panel[1].text_, SIGNAL(textModified()), this, SLOT(textModified1()));
	connect(panel[1].text_, SIGNAL(cursorMoved()), this, SLOT(cursorMoved()));
	connect(panel[1].text_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(panel[1].text_, SIGNAL(search(QString)), this, SLOT(quickSearch(QString)));
	connect(panel[1].text_, SIGNAL(grep(QString)), this, SLOT(quickGrep(QString)));
	connect(panel[1].text_, SIGNAL(recording(bool)), this, SLOT(recording(bool)));
	connect(panel[1].text_, SIGNAL(accumulating(bool)), this, SLOT(accumulating(bool)));
	connect(panel[1].text_, SIGNAL(metaPressed()), this, SLOT(metaPressed()));

	connect(panel[2].text_, SIGNAL(clicked(bool)), this, SLOT(clicked2(bool)));
	connect(panel[2].text_, SIGNAL(textModified()), this, SLOT(textModified2()));
	connect(panel[2].text_, SIGNAL(cursorMoved()), this, SLOT(cursorMoved()));
	connect(panel[2].text_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(panel[2].text_, SIGNAL(search(QString)), this, SLOT(quickSearch(QString)));
	connect(panel[2].text_, SIGNAL(grep(QString)), this, SLOT(quickGrep(QString)));
	connect(panel[2].text_, SIGNAL(recording(bool)), this, SLOT(recording(bool)));
	connect(panel[2].text_, SIGNAL(accumulating(bool)), this, SLOT(accumulating(bool)));
	connect(panel[2].text_, SIGNAL(metaPressed()), this, SLOT(metaPressed()));

	connect(panel[3].text_, SIGNAL(clicked(bool)), this, SLOT(clicked3(bool)));
	connect(panel[3].text_, SIGNAL(textModified()), this, SLOT(textModified3()));
	connect(panel[3].text_, SIGNAL(cursorMoved()), this, SLOT(cursorMoved()));
	connect(panel[3].text_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(panel[3].text_, SIGNAL(search(QString)), this, SLOT(quickSearch(QString)));
	connect(panel[3].text_, SIGNAL(grep(QString)), this, SLOT(quickGrep(QString)));
	connect(panel[3].text_, SIGNAL(recording(bool)), this, SLOT(recording(bool)));
	connect(panel[3].text_, SIGNAL(accumulating(bool)), this, SLOT(accumulating(bool)));
	connect(panel[3].text_, SIGNAL(metaPressed()), this, SLOT(metaPressed()));

	connect(filebar_, SIGNAL(clicked(bool)), this, SLOT(clicked_filebar(bool)));
	connect(infotext_, SIGNAL(clicked(bool)), this, SLOT(clicked_info(bool)));

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	// read config info stored in ~/.config/alt/config file and adjust windows
	readSettings();
	QSize sz = size();
	QList<int> myList3;
	myList3.append(sz.height() * 9 / 10);
	myList3.append(sz.height() / 10);
	split3->setSizes(myList3);

	QList<int> myList1;
	myList1.append(sz.height());
	myList1.append(0);
	split1->setSizes(myList1);

	QList<int> myList10;
	myList10.append(sz.width());
	myList10.append(0);
	split10->setSizes(myList1);

	QList<int> myList11;
	myList11.append(sz.width());
	myList11.append(0);
	split11->setSizes(myList1);

	setUnifiedTitleAndToolBarOnMac(true);
	setTitle();

	find_ = new FindDialog();
	replace_ = new ReplaceDialog(QString(""), QString(""));
	connect(replace_, SIGNAL(replaceClicked()), this, SLOT(replaceSlot()));
	grep_ = new GrepDialog();
	diff_ = new DiffDialog();


}

void MainWindow::closeEvent(QCloseEvent *event) {
	FLOG();

	if (maybeSaveAll()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}

void MainWindow::newFile() {
	FLOG();

	// save the current text in bufmgr
	saveBufmgr();
	if (!active().bufmgr_->add(QString(""))) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("AltEdit"),
					   "Sorry, all buffers are in use.",
					   QMessageBox::Ok);
		(void) ret;
		return;
	}

	active().text_->clear();
	setTitle();
	updateBufmgrActions();
	active().text_->setFocus();
}

void MainWindow::openFile(QString fileName, bool print_function_list) {
	FLOG();

	// build full file path
	QFileInfo info(fileName);
	fileName = info.absoluteFilePath();
	fdebug("full path %s\n", fileName.toUtf8().data());

	// update current buffer
	saveBufmgr();
	if (!fileName.isEmpty()) {
		// resue the current buffer untitled, empty and unmodified
		BufMgrData *data = active().bufmgr_->active();

		// try to use an existing buffer
		int i;
		bool done = false;
		for (i = 0; i < MAX_PANELS; i++) {
			if (panel[i].bufmgr_->set(fileName)) {
				fdebug("active %s\n", fileName.toUtf8().data());
				active_ = i;
				restoreBufmgr();
				done = true;
				break;
			}
		}
		if (done) {;}

		// reuse active if untitled and unmodified,
		else if (!data->modified_ && data->file_.isEmpty() && data->text_.isEmpty()) {
			fdebug("reuse untitled %s\n", fileName.toUtf8().data());
			loadFile(fileName);
			data->file_ = fileName;
		}
		else {
			fdebug("new buffer %s\n", fileName.toUtf8().data());
			// create a new buffer
			if (!active().bufmgr_->add(fileName)) {
				QMessageBox::StandardButton ret;
				ret = QMessageBox::warning(this, tr("AltEdit"),
							   "Sorry, all buffers are in use.",
							   QMessageBox::Ok);
				(void) ret;
				return;
			}

			loadFile(fileName);

			// update bufmgr with new file information
			data = active().bufmgr_->active();
			data->text_ = active().text_->toPlainText();
			data->pos_ = active().text_->getPosition();
			data->modified_ = false;
		}

		setTitle();
		updateBufmgrActions();
		if (print_function_list)
			functionList();
		active().text_->setFocus();
		QTimer::singleShot(50, this, SLOT(statusDelay()));
	}

	filebar_->setDirForFile(active().bufmgr_->active()->file_);
	active().text_->setFocus();
}

void MainWindow::open() {
	FLOG();
	QString fileName = QFileDialog::getOpenFileName(this);
	openFile(fileName);
	functionList();
	active().text_->setFocus();
}

bool MainWindow::save() {
	FLOG();
	bool rv;

	BufMgrData *data = active().bufmgr_->active();
	if (data->file_.isEmpty()) {
		fdebug("save: no name for the buffer, running saveAs\n");
		rv = saveAs();
	}
	else {
		fdebug("save: saving %s\n", data->file_.toUtf8().data());
		rv = saveFile(data->file_);
	}

	if (rv) {
		fdebug("save: file %s saved\n", data->file_.toUtf8().data());
		// update bufmgr with new file information
		data->modified_ = false;
		updateBufmgrActions();
		setTitle();
		QTimer::singleShot(50, this, SLOT(statusDelay()));
	}

	active().text_->setFocus();
	return rv;
}

bool MainWindow::saveAs() {
	FLOG();

	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	BufMgrData *data = active().bufmgr_->active();
	data->file_ = fileName;
	updateBufmgrActions();
	setTitle();
	active().text_->setFocus();
	return saveFile(fileName);
}

void MainWindow::closeBuffer() {
	FLOG();

	// update modified flag
	BufMgrData *data = active().bufmgr_->active();

	// maybe we need to save the buffer
	if (data->modified_) {
		fdebug("closeBuffer: data modified\n");
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("AltEdit"),
					   tr("The document has been modified.\n"
					      "Do you want to save your changes?"),
					   QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
		if (ret == QMessageBox::Save)
			save();
		else if (ret == QMessageBox::Cancel)
			return;
	}

	fdebug("closeBuffer: removing buffer\n");
	active().bufmgr_->remove();

	// display the new current buffer
	data = active().bufmgr_->active();
	active().text_->setPlainText(data->text_);
	active().text_->setPosition(data->pos_);
	setTitle();
	updateBufmgrActions();
	active().text_->setFocus();
}

void MainWindow::cut() {
	FLOG();
	active().text_->cut();
	active().text_->setFocus();
}

void MainWindow::paste() {
	FLOG();
	active().text_->paste();
	active().text_->setFocus();
}

void MainWindow::del() {
	FLOG();
	active().text_->deleteSelection();
	active().text_->setFocus();
}

void MainWindow::gotoStart() {
	FLOG();
	active().text_->gotoStart();
	active().text_->setFocus();
}

void MainWindow::gotoEnd() {
	FLOG();
	active().text_->gotoEnd();
	active().text_->setFocus();
}

void MainWindow::find() {
	FLOG();
	assert(find_);

	if (QDialog::Accepted == find_->exec()) {
		QString word = find_->getWord();
		fdebug("find %s\n", word.toUtf8().data());
		bool rv = active().text_->find(word, find_->getFlags());
		if (rv)
			fdebug("found\n");

		// update info window
		if (rv) {
			BufMgrData *data = active().bufmgr_->active();
			QString result = data->file_ + "\n";
			result += QString(":%1").arg(active().text_->getLineNumber());
			result += QString("\t") + active().text_->getLineText();

			int actual_pos = active().text_->getPosition();
			while (active().text_->find(word, find_->getFlags())) {
				result += QString(":%1").arg(active().text_->getLineNumber());
				result += QString("\t") + active().text_->getLineText();
			}

			active().text_->setPosition(actual_pos);
			infotext_->setWordWrapMode(QTextOption::NoWrap);
			infotext_->setPlainText(result);
		}
	}
	active().text_->setFocus();
}

void MainWindow::quickSearch(QString word) {
	FLOG();
	assert(find_);

	find_->setWord(word);
	findNext();
	active().text_->setFocus();
}

void MainWindow::quickGrep(QString word) {
	FLOG();
	assert(grep_);
	fdebug("grep #%s#\n", word.toUtf8().data());
	if (word.isEmpty())
		return;

	grep_->setWord(word);
//	findNext();
	QString rv = greputil(grep_);
	infotext_->setWordWrapMode(QTextOption::NoWrap);
	infotext_->setPlainText(rv);
	active().text_->setFocus();
}

void MainWindow::findNext() {
	FLOG();
	assert(find_);
	QString word = find_->getWord();
	fdebug("find next %s\n", word.toUtf8().data());
	active().text_->find(word, find_->getFlags());
	active().text_->setFocus();
}

void MainWindow::replace() {
	FLOG();
	assert(replace_);
	replace_->show();
	replace_->raise();
	fdebug("accepted\n");
//qApp->processEvents();
	active().text_->setFocus();
}

void MainWindow::replaceSlot() {
	FLOG();

	ReplaceDialog::ReplaceAction action = replace_->getAction();
	fdebug("replace action %d\n", action);
	if (action == ReplaceDialog::ACTION_CANCEL)
		replace_->getOut();
	else if (action == ReplaceDialog::ACTION_REPLACEALL) {
		QString wfind = replace_->getWordToFind();
		QString wreplace = replace_->getReplacement();
		while (active().text_->find(wfind, QTextDocument::FindCaseSensitively))
			active().text_->replaceSelected(wreplace);
		replace_->getOut();
		fdebug("replace all %s %s\n", wfind.toUtf8().data(), wreplace.toUtf8().data());
	}
	else if (action == ReplaceDialog::ACTION_REPLACE) {
		QString wfind = replace_->getWordToFind();
		QString wreplace = replace_->getReplacement();
		// replace selected word and look for a new word
		active().text_->replaceSelected(wreplace);
		active().text_->find(wfind, QTextDocument::FindCaseSensitively);
		fdebug("replace %s %s\n", wfind.toUtf8().data(), wreplace.toUtf8().data());
	}
	else if (action == ReplaceDialog::ACTION_FINDNEXT) {
		QString wfind = replace_->getWordToFind();
		fdebug("replace find %s\n", wfind.toUtf8().data());
		active().text_->find(wfind, QTextDocument::FindCaseSensitively);
	}
	else
		assert(0);
}

void MainWindow::grep() {
	FLOG();
	assert(grep_);

	if (QDialog::Accepted == grep_->exec()) {
		QString rv = greputil(grep_);
		infotext_->setWordWrapMode(QTextOption::NoWrap);
		infotext_->setPlainText(rv);
	}
	active().text_->setFocus();
}

void MainWindow::gotoLine() {
	FLOG();

	LineDialog ld;
	if (QDialog::Accepted == ld.exec()) {
		int line = ld.getLine(); // 0 based
		fdebug("line %d\n", line);
		active().text_->setLine(line);
		cursorMoved();
	}
	active().text_->setFocus();
}

void MainWindow::copy() {
	FLOG();
	active().text_->copy();
	active().text_->setFocus();
}

void MainWindow::about() {
	FLOG();
	QMessageBox::about(this, tr("About AltEdit"),
			   tr("<b>AltEdit</b> is a tiling text editor."));
	active().text_->setFocus();
}

void MainWindow::createActions() {
	FLOG();

	newAct = new QAction(QIcon(":/images/document-new.svg"), tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	openAct = new QAction(QIcon(":/images/document-open.svg"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/document-save.svg"), tr("&Save"), this);
	QList<QKeySequence> shortcuts_save;
	shortcuts_save.append(QKeySequence(Qt::ALT + Qt::Key_W));
	saveAct->setShortcuts(shortcuts_save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(QIcon(":/images/document-save-as.svg"), tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	saveAllAct = new QAction(QIcon(":/images/document-save-all.svg"), tr("Sa&ve All"), this);
//	saveAllAct->setShortcuts(QKeySequence::SaveAll)lm;
	saveAllAct->setStatusTip(tr("Save all the documents"));
	connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

	closeAct = new QAction(QIcon(":/images/document-close.svg"), tr("&Close..."), this);
	QList<QKeySequence> shortcuts_close;
	shortcuts_close.append(QKeySequence(Qt::ALT + Qt::Key_D));
	closeAct->setShortcuts(shortcuts_close);
	closeAct->setStatusTip(tr("Close buffer"));
	connect(closeAct, SIGNAL(triggered()), this, SLOT(closeBuffer()));

	exitAct = new QAction(QIcon(":/images/application-exit.svg"), tr("E&xit"), this);
	QList<QKeySequence> shortcuts_exit;
	shortcuts_exit.append(QKeySequence(Qt::ALT + Qt::Key_X));
	exitAct->setShortcuts(shortcuts_exit);
	exitAct->setStatusTip(tr("Exit the text editor"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	undoAct = new QAction(QIcon(":/images/edit-undo.svg"), tr("&Undo"), this);
	undoAct->setStatusTip(tr("Undo"));
	QList<QKeySequence> shortcuts_undo;
	shortcuts_undo.append(QKeySequence(Qt::ALT + Qt::Key_U));
	undoAct->setShortcuts(shortcuts_undo);
	connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

	cutAct = new QAction(QIcon(":/images/edit-cut.svg"), tr("Cu&t"), this);
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
				"clipboard"));
	connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

	copyAct = new QAction(QIcon(":/images/edit-copy.svg"), tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
				 "clipboard"));
	connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

	pasteAct = new QAction(QIcon(":/images/edit-paste.svg"), tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
				  "selection"));
	connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

	deleteAct = new QAction(QIcon(":/images/edit-delete.svg"), tr("&Delete"), this);
	deleteAct->setShortcuts(QKeySequence::Delete);
	deleteAct->setStatusTip(tr("Delete the current selection"));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(del()));

	findAct = new QAction(QIcon(":/images/edit-find.svg"), tr("&Find"), this);
	QList<QKeySequence> shortcuts_find;
	shortcuts_find.append(QKeySequence(Qt::Key_F5));
	findAct->setShortcuts(shortcuts_find);
	findAct->setStatusTip(tr("Search current buffer"));
	connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

	findNextAct = new QAction(QIcon(":/images/edit-find.svg"), tr("Find &Next"), this);
	QList<QKeySequence> shortcuts_findnext;
	shortcuts_findnext.append(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	findNextAct->setShortcuts(shortcuts_findnext);
	findNextAct->setStatusTip(tr("Search the current buffer again"));
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));

	replaceAct = new QAction(QIcon(":/images/edit-find-replace.svg"), tr("&Replace"), this);
	QList<QKeySequence> shortcuts_replace;
	shortcuts_replace.append(QKeySequence(Qt::Key_F6));
	replaceAct->setShortcuts(shortcuts_replace);
	replaceAct->setStatusTip(tr("Replace text"));
	connect(replaceAct, SIGNAL(triggered()), this, SLOT(replace()));

	grepAct = new QAction(QIcon(":/images/grep.svg"), tr("&Grep"), this);
	QList<QKeySequence> shortcuts_grep;
	shortcuts_grep.append(QKeySequence(Qt::Key_F7));
	grepAct->setShortcuts(shortcuts_grep);
	grepAct->setStatusTip(tr("Grep"));
	connect(grepAct, SIGNAL(triggered()), this, SLOT(grep()));
	if (no_grep_)
		grepAct->setEnabled(false);

	gotoLineAct = new QAction(QIcon(":/images/go-jump.svg"), tr("Go to &Line"), this);
	QList<QKeySequence> shortcuts_gotoline;
	shortcuts_gotoline.append(QKeySequence(Qt::ALT + Qt::Key_G));
	gotoLineAct->setShortcuts(shortcuts_gotoline);
	gotoLineAct->setStatusTip(tr("Go to line"));
	connect(gotoLineAct, SIGNAL(triggered()), this, SLOT(gotoLine()));

	gotoStartAct = new QAction(QIcon(":/images/go-top.svg"), tr("Go to &Start"), this);
	QList<QKeySequence> shortcuts_gotostart;
	shortcuts_gotostart.append(QKeySequence(Qt::CTRL + Qt::Key_PageUp));
	gotoStartAct->setShortcuts(shortcuts_gotostart);
	gotoStartAct->setStatusTip(tr("Go to the start of the buffer"));
	connect(gotoStartAct, SIGNAL(triggered()), this, SLOT(gotoStart()));

	gotoEndAct = new QAction(QIcon(":/images/go-bottom.svg"), tr("Go to &End"), this);
	QList<QKeySequence> shortcuts_gotoend;
	shortcuts_gotoend.append(QKeySequence(Qt::CTRL + Qt::Key_PageDown));
	gotoEndAct->setShortcuts(shortcuts_gotoend);
	gotoEndAct->setStatusTip(tr("Go to end of the buffer"));
	connect(gotoEndAct, SIGNAL(triggered()), this, SLOT(gotoEnd()));

	gotoBookmarkAct = new QAction(QIcon(":/images/bookmark-jump.svg"), tr("&Jump to Bookmark"), this);
	QList<QKeySequence> shortcuts_gotobookmark;
	shortcuts_gotobookmark.append(QKeySequence(Qt::ALT + Qt::Key_J));
	gotoBookmarkAct->setShortcuts(shortcuts_gotobookmark);
	gotoBookmarkAct->setStatusTip(tr("Go to bookmark"));
	connect(gotoBookmarkAct, SIGNAL(triggered()), this, SLOT(gotoBookmark()));

	nextbufferAct = new QAction(QIcon(":/images/go-next.svg"), tr("&Next"), this);
	QList<QKeySequence> shortcuts_nextb;
	shortcuts_nextb.append(QKeySequence(Qt::ALT + Qt::Key_N));
	nextbufferAct->setShortcuts(shortcuts_nextb);
	nextbufferAct->setStatusTip(tr("Go to next buffer"));
	// bufmgr menu is connected

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
#if 0
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(text1_, SIGNAL(copyAvailable(bool)),
		cutAct, SLOT(setEnabled(bool)));
	connect(text1_, SIGNAL(copyAvailable(bool)),
		copyAct, SLOT(setEnabled(bool)));
#endif
	setBookmark0Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &0"), this);
	QList<QKeySequence> shortcuts_setBookmark0;
	shortcuts_setBookmark0.append(QKeySequence(Qt::ALT + Qt::Key_0));
	setBookmark0Act->setShortcuts(shortcuts_setBookmark0);
	setBookmark0Act->setStatusTip(tr("Set bookmark 0"));
	connect(setBookmark0Act, SIGNAL(triggered()), this, SLOT(setBookmark0()));

	setBookmark1Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &1"), this);
	QList<QKeySequence> shortcuts_setBookmark1;
	shortcuts_setBookmark1.append(QKeySequence(Qt::ALT + Qt::Key_1));
	setBookmark1Act->setShortcuts(shortcuts_setBookmark1);
	setBookmark1Act->setStatusTip(tr("Set bookmark 1"));
	connect(setBookmark1Act, SIGNAL(triggered()), this, SLOT(setBookmark1()));

	setBookmark2Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &2"), this);
	QList<QKeySequence> shortcuts_setBookmark2;
	shortcuts_setBookmark2.append(QKeySequence(Qt::ALT + Qt::Key_2));
	setBookmark2Act->setShortcuts(shortcuts_setBookmark2);
	setBookmark2Act->setStatusTip(tr("Set bookmark 2"));
	connect(setBookmark2Act, SIGNAL(triggered()), this, SLOT(setBookmark2()));

	setBookmark3Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &3"), this);
	QList<QKeySequence> shortcuts_setBookmark3;
	shortcuts_setBookmark3.append(QKeySequence(Qt::ALT + Qt::Key_3));
	setBookmark3Act->setShortcuts(shortcuts_setBookmark3);
	setBookmark3Act->setStatusTip(tr("Set bookmark 3"));
	connect(setBookmark3Act, SIGNAL(triggered()), this, SLOT(setBookmark3()));

	setBookmark4Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &4"), this);
	QList<QKeySequence> shortcuts_setBookmark4;
	shortcuts_setBookmark4.append(QKeySequence(Qt::ALT + Qt::Key_4));
	setBookmark4Act->setShortcuts(shortcuts_setBookmark4);
	setBookmark4Act->setStatusTip(tr("Set bookmark 4"));
	connect(setBookmark4Act, SIGNAL(triggered()), this, SLOT(setBookmark4()));

	setBookmark5Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &5"), this);
	QList<QKeySequence> shortcuts_setBookmark5;
	shortcuts_setBookmark5.append(QKeySequence(Qt::ALT + Qt::Key_5));
	setBookmark5Act->setShortcuts(shortcuts_setBookmark5);
	setBookmark5Act->setStatusTip(tr("Set bookmark 5"));
	connect(setBookmark5Act, SIGNAL(triggered()), this, SLOT(setBookmark5()));

	setBookmark6Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &6"), this);
	QList<QKeySequence> shortcuts_setBookmark6;
	shortcuts_setBookmark6.append(QKeySequence(Qt::ALT + Qt::Key_6));
	setBookmark6Act->setShortcuts(shortcuts_setBookmark6);
	setBookmark6Act->setStatusTip(tr("Set bookmark 6"));
	connect(setBookmark6Act, SIGNAL(triggered()), this, SLOT(setBookmark6()));

	setBookmark7Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &7"), this);
	QList<QKeySequence> shortcuts_setBookmark7;
	shortcuts_setBookmark7.append(QKeySequence(Qt::ALT + Qt::Key_7));
	setBookmark7Act->setShortcuts(shortcuts_setBookmark7);
	setBookmark7Act->setStatusTip(tr("Set bookmark 7"));
	connect(setBookmark7Act, SIGNAL(triggered()), this, SLOT(setBookmark7()));

	setBookmark8Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &8"), this);
	QList<QKeySequence> shortcuts_setBookmark8;
	shortcuts_setBookmark8.append(QKeySequence(Qt::ALT + Qt::Key_8));
	setBookmark8Act->setShortcuts(shortcuts_setBookmark8);
	setBookmark8Act->setStatusTip(tr("Set bookmark 8"));
	connect(setBookmark8Act, SIGNAL(triggered()), this, SLOT(setBookmark8()));

	setBookmark9Act = new QAction(QIcon(":/images/bookmark-new.svg"), tr("Set bookmark &9"), this);
	QList<QKeySequence> shortcuts_setBookmark9;
	shortcuts_setBookmark9.append(QKeySequence(Qt::ALT + Qt::Key_9));
	setBookmark9Act->setShortcuts(shortcuts_setBookmark9);
	setBookmark9Act->setStatusTip(tr("Set bookmark 9"));
	connect(setBookmark9Act, SIGNAL(triggered()), this, SLOT(setBookmark9()));

	functionListAct = new QAction(QIcon(":/images/functions.svg"), tr("&Function List"), this);
	QList<QKeySequence> shortcuts_function;
	shortcuts_function.append(QKeySequence(Qt::Key_F8));
	functionListAct->setShortcuts(shortcuts_function);
	functionListAct->setStatusTip(tr("Function list"));
	connect(functionListAct, SIGNAL(triggered()), this, SLOT(functionList()));
	if (no_ctags_)
		functionListAct->setEnabled(false);

	makeAct = new QAction(QIcon(":/images/make.svg"), tr("&make"), this);
	QList<QKeySequence> shortcuts_make;
	shortcuts_make.append(QKeySequence(Qt::Key_F9));
	makeAct->setShortcuts(shortcuts_make);
	makeAct->setStatusTip(tr("Run make"));
	connect(makeAct, SIGNAL(triggered()), this, SLOT(make()));
	if (no_make_)
		makeAct->setEnabled(false);

	spellAct = new QAction(QIcon(":/images/tools-check-spelling.svg"), tr("&Spell"), this);
	QList<QKeySequence> shortcuts_spell;
	shortcuts_spell.append(QKeySequence(Qt::Key_F10));
	spellAct->setShortcuts(shortcuts_spell);
	spellAct->setStatusTip(tr("Spell the current line starting from the cursor"));
	connect(spellAct, SIGNAL(triggered()), this, SLOT(spell()));
	if (no_hunspell_)
		spellAct->setEnabled(false);

	diffAct = new QAction(QIcon(":/images/diff.svg"), tr("&Diff"), this);
	QList<QKeySequence> shortcuts_diff;
	shortcuts_diff.append(QKeySequence(Qt::Key_F11));
	diffAct->setShortcuts(shortcuts_diff);
	diffAct->setStatusTip(tr("Diff"));
	connect(diffAct, SIGNAL(triggered()), this, SLOT(diff()));
	if (no_git_)
		diffAct->setEnabled(false);

	astyleAct = new QAction(QIcon(":/images/astyle.png"), tr("&Code styler"), this);
	QList<QKeySequence> shortcuts_astyle;
	shortcuts_astyle.append(QKeySequence(Qt::Key_F12));
	astyleAct->setShortcuts(shortcuts_astyle);
	astyleAct->setStatusTip(tr("Code styler"));
	connect(astyleAct, SIGNAL(triggered()), this, SLOT(astyle()));

	blockAct = new QAction(QIcon(":/images/blocks.svg"), tr("Next code block"), this);
	QList<QKeySequence> shortcuts_block;
	shortcuts_block.append(QKeySequence(Qt::CTRL + Qt::Key_K));
	blockAct->setShortcuts(shortcuts_block);
	blockAct->setStatusTip(tr("Go to the start of the buffer"));
	connect(blockAct, SIGNAL(triggered()), this, SLOT(block()));
}

void MainWindow::createMenus() {
	FLOG();

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addAction(saveAllAct);
	fileMenu->addAction(closeAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);
	editMenu->addAction(deleteAct);
	editMenu->addSeparator();
	editMenu->addAction(findAct);
	editMenu->addAction(findNextAct);
	editMenu->addAction(replaceAct);
	editMenu->addAction(grepAct);
	editMenu->addSeparator();
	editMenu->addAction(blockAct);
	editMenu->addAction(gotoLineAct);
	editMenu->addAction(gotoStartAct);
	editMenu->addAction(gotoEndAct);
	editMenu->addAction(gotoBookmarkAct);
	editMenu->addSeparator();
	editMenu->addAction(setBookmark0Act);
	editMenu->addAction(setBookmark1Act);
	editMenu->addAction(setBookmark2Act);
	editMenu->addAction(setBookmark3Act);
	editMenu->addAction(setBookmark4Act);
	editMenu->addAction(setBookmark5Act);
	editMenu->addAction(setBookmark6Act);
	editMenu->addAction(setBookmark7Act);
	editMenu->addAction(setBookmark8Act);
	editMenu->addAction(setBookmark9Act);

	buffersMenu = menuBar()->addMenu(tr("&Buffers"));
	buffersMenu->addAction(nextbufferAct);
	connect(buffersMenu, SIGNAL(triggered(QAction *)), this, SLOT(clicked_bufmgr(QAction *)));

	toolsMenu = menuBar()->addMenu("&Tools");
	toolsMenu->addAction(functionListAct);
	toolsMenu->addAction(makeAct);
	toolsMenu->addAction(spellAct);
	toolsMenu->addAction(diffAct);
	toolsMenu->addAction(astyleAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars() {
	FLOG();

	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);
	fileToolBar->addAction(saveAllAct);

	toolsToolBar = addToolBar(tr("Tools"));
	toolsToolBar->addAction(findAct);
	if (!no_grep_)
		toolsToolBar->addAction(grepAct);
	if (!no_ctags_)
		toolsToolBar->addAction(functionListAct);
	if (!no_make_)
		toolsToolBar->addAction(makeAct);
	if (!no_hunspell_)
		toolsToolBar->addAction(spellAct);
	if (!no_git_)
		toolsToolBar->addAction(diffAct);
	if (!no_astyle_)
		toolsToolBar->addAction(astyleAct);
}

void MainWindow::createStatusBar() {
	FLOG();

	status_modified_ = new QLabel("");
	statusBar()->addPermanentWidget(status_modified_);
	status_accumulating_ = new QLabel("");
	statusBar()->addPermanentWidget(status_accumulating_);
	status_recording_ = new QLabel("");
	statusBar()->addPermanentWidget(status_recording_);
	statusBar()->addPermanentWidget(new QLabel("   "));
	status_selection_ = new QLabel("");
	statusBar()->addPermanentWidget(status_selection_);
	statusBar()->addPermanentWidget(new QLabel("   "));
	status_position_ = new QLabel("1:1");
	statusBar()->addPermanentWidget(status_position_);
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
	FLOG();

	QSettings settings("alt", "alt");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::writeSettings() {
	FLOG();
	QSettings settings("alt", "alt");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

bool MainWindow::maybeSaveAll() {
	FLOG();

	saveBufmgr();

	// find untitled files
	bool untitled = false;
	for (int j = 0; j < MAX_PANELS; j++) {
		for (int i = 0; i < panel[j].bufmgr_->size(); i++) {
			BufMgrData &data = (*panel[j].bufmgr_)[i];
			if (data.active_ && data.modified_ && data.file_.isEmpty())
				untitled = true;
		}
	}

	if (untitled) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("AltEdit"),
					   "There are utitled files open. You need to save them first, one by one. ",
					   QMessageBox::Ok);
		if (ret == QMessageBox::Ok)
			return false;
	}

	// check all the remaining files
	bool unsaved = false;
	QString unsavedFiles;
	for (int j = 0; j < MAX_PANELS; j++) {
		for (int i = 0; i < panel[j].bufmgr_->size(); i++) {
			BufMgrData &data = (*panel[j].bufmgr_)[i];
			if (data.active_ && data.modified_) {
				unsaved = true;
				unsavedFiles += data.file_ + "\n";
			}
		}
	}

	if (unsaved) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("AltEdit"),
					   "There are unsaved files. You need to save them first, one by one:\n\n" + unsavedFiles,
					   QMessageBox::Ok);
		if (ret == QMessageBox::Ok)
			return false;
	}
	return true;
}

bool MainWindow::saveAll() {
	FLOG();
	saveBufmgr();

	// find untitled files
	bool untitled = false;
	for (int j = 0; j < MAX_PANELS; j++) {
		for (int i = 0; i < panel[j].bufmgr_->size(); i++) {
			BufMgrData &data = (*panel[j].bufmgr_)[i];
			if (data.active_ && data.modified_ && data.file_.isEmpty())
				untitled = true;
		}
	}

	if (untitled) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("AltEdit"),
					   "There are utitled files open. You need to save them first, one by one. ",
					   QMessageBox::Ok);
		if (ret == QMessageBox::Ok)
			return false;
	}

	saveTitledFiles();
	updateBufmgrActions();
	active().text_->setFocus();
	return true;
}

void MainWindow::saveTitledFiles() {
	// save all the remaining files

	// panel0
	for (int j = 0; j < MAX_PANELS; j++) {
		for (int i = 0; i < panel[j].bufmgr_->size(); i++) {
			BufMgrData &data = (*panel[j].bufmgr_)[i];
			if (data.file_.isEmpty())
				continue;
			if (data.active_ && data.modified_) {
				fdebug("saving file %s\n", data.file_.toUtf8().data());
				FILE *fp = fopen(data.file_.toUtf8().data(), "w");
				if (fp) {
					fprintf(fp, "%s", data.text_.toUtf8().data());
					fclose(fp);
					data.modified_ = false;
				}
			}
		}
	}
	active().text_->setFocus();
}

void MainWindow::loadFile(const QString &fileName) {
	FLOG();
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		active().text_->setPlainText("");
		statusBar()->showMessage(tr("Cannot load ") + fileName, 2000);
		return;
	}

	BCursor b;

	QTextStream in(&file);
	QString str = in.readAll();
	active().text_->setPlainText(str);
	statusBar()->showMessage(fileName + tr(" loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName, bool warning) {
	FLOG();
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		if (warning)
			QMessageBox::warning(this, tr("AltEdit"),
					     tr("Cannot write file %1:\n%2.")
					     .arg(fileName)
					     .arg(file.errorString()));
		return false;
	}
	QTextStream out(&file);
	BCursor b;
	bool endnl = active().text_->toPlainText().endsWith("\n");
	QStringList lines = active().text_->toPlainText().split('\n');
	int i;
	int size = lines.size();
	for (i = 0; i < size; i++) {
		// remove trailing space and tabs
		while (lines[i].endsWith(" ") || lines[i].endsWith("\t"))
			lines[i].chop(1);

		out << lines[i];
		if (i == (size - 1) && lines[i].length() == 0) {;}
		else if (i == (size - 1) && !endnl) {;}
		else
			out << "\n";
	}
//	out << active().text_->toPlainText();

	statusBar()->showMessage(tr("File saved"), 2000);
	fdebug("saveFile(): all done\n");
	return true;
}

void MainWindow::setTitle() {
	FLOG();
	// workaround for QTBUG-16507 - affecting 4.7.0, 4.7.2, 5.6.0
	QTimer::singleShot(50, this, SLOT(setTitleDelay()));
}

void MainWindow::setTitleDelay() {
	FLOG();

	BufMgrData *data = active().bufmgr_->active();
	QString shownName;
	if (data->file_.isEmpty())
		shownName += "untitled";
	else
		shownName += data->file_;

	setWindowFilePath(shownName);
}

//QString MainWindow::strippedName(const QString &fullFileName) {
//	return QFileInfo(fullFileName).fileName();
//}

void MainWindow::updateBufmgrActions(void) {
	FLOG();

	QList<QAction *> list = buffersMenu->actions();
	while (!list.empty()) {
		QAction *elem =  list.takeFirst();
		if (elem != nextbufferAct) {
			elem->disconnect();
			buffersMenu->removeAction(elem);
			delete elem;
		}
	}

	for (int i = 0; i < active().bufmgr_->size(); i++) {
		BufMgrData &data = (*active().bufmgr_)[i];
		if (data.active_) {
			// create a new action and add it to the menu
			QAction *newAct = new QAction((!data.modified_) ? QIcon(":/images/document-new.svg") : QIcon(":/images/document-edit.svg"),
						      data.file_, this);
			buffersMenu->addAction(newAct);
		}
	}

	active().text_->setFocus();
}

void MainWindow::undo() {
	FLOG();
	active().text_->undo();
	active().text_->setFocus();
}

void MainWindow::functionList() {
	FLOG();

	QString file = active().bufmgr_->active()->file_;
	if (file.isEmpty()) {
		QMessageBox::warning(this, tr("AltEdit"),
				     tr("This is an untitled file, you would need to save it first."));
		return;
	}
	// save the current buffer
	if (active().bufmgr_->active()->modified_)
		saveFile(file, false);

	// run ctags
	if (access(file.toUtf8().data(), R_OK) == 0) {
		BCursor b;
		fdebug("%s\n", file.toUtf8().data());
		QString text = function_list(file);
		infotext_->setWordWrapMode(QTextOption::NoWrap);
		infotext_->setPlainText(text);
	}
	active().text_->setFocus();
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::make() {
	FLOG();
	BCursor b;

	// save all active files
	saveBufmgr();
	saveTitledFiles();
	QString cmd = QString("make -j4 2>&1");
	infotext_->setWordWrapMode(QTextOption::WrapAnywhere);
	infotext_->setPlainText("");

	run_program_window(cmd.toUtf8().data(), infotext_);
	infotext_->insertPlainText("all done\n");
	updateBufmgrActions();
	active().text_->setFocus();
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}

void MainWindow::spell() {
	FLOG();

	BCursor b;
	QString linestr = active().text_->getLineText();
	int col = active().text_->getColumnNumber();
	linestr = linestr.mid(col);
	linestr = linestr.replace(QChar('('), QChar(' '));
	linestr = linestr.replace(QChar(')'), QChar(' '));
	linestr = linestr.replace(QChar('{'), QChar(' '));
	linestr = linestr.replace(QChar('}'), QChar(' '));
	linestr = linestr.replace(QChar('.'), QChar(' '));
	linestr = linestr.replace(QChar(';'), QChar(' '));
	linestr = linestr.replace(QChar(':'), QChar(' '));
	linestr = linestr.replace(QChar('>'), QChar(' '));
	linestr = linestr.replace(QChar('<'), QChar(' '));
	linestr = linestr.replace(QChar('\\'), QChar(' '));
	linestr = linestr.replace(QChar('*'), QChar(' '));
	linestr = linestr.replace(QChar('&'), QChar(' '));
	linestr = linestr.replace(QChar('|'), QChar(' '));
	linestr = linestr.replace(QChar('='), QChar(' '));
	fdebug("spelling %s\n", linestr.toUtf8().data());

	QString cmd = QString(PREFIX) + "/lib/alt/spell " + linestr;
	infotext_->setWordWrapMode(QTextOption::WrapAnywhere);
	infotext_->setPlainText("");
	qApp->processEvents();

	run_program_window(cmd.toUtf8().data(), infotext_);
	infotext_->insertPlainText("all done\n");
	updateBufmgrActions();
	active().text_->setFocus();
}

void MainWindow::diff() {
	FLOG();
	assert(diff_);

	if (QDialog::Accepted == diff_->exec()) {
		QString cmd = QString(PREFIX) + "/lib/alt/diff ";

		if (diff_->getMode() == DiffDialog::FILES_MODE) {
			cmd += diff_->getFile1() + " ";
			cmd += diff_->getFile2();
		}
		else { // git mode
			// don't allow unnamed files
			QString file = active().bufmgr_->active()->file_;
			if (file.isEmpty()) {
				QMessageBox::warning(this, tr("AltEdit"),
						     "You need to save the untitled file first.",
						     QMessageBox::Ok);
				return;
			}
			// save the currrent file
			saveFile(active().bufmgr_->active()->file_, false);

			// build ndif --git command
			cmd += QString("--git ");
			cmd += active().bufmgr_->active()->file_.toUtf8().data();
		}

		qApp->processEvents();
		fdebug("starting \"%s\"\n", cmd.toUtf8().data());
		run_program_fork(cmd.toUtf8().data());
		active().text_->setFocus();
	}

	QTimer::singleShot(50, this, SLOT(statusDelay()));
	active().text_->setFocus();
}

void MainWindow::block() {
	FLOG();

	active().text_->selectCodeBlock();
}

void MainWindow::astyle() {
	FLOG();

	// save the currrent file
	int rv = saveFile(active().bufmgr_->active()->file_, true);
	if (!rv)
		return;

	// build command and run it
	QString cmd = QString("bash -c \"") + QString(PREFIX) + "/lib/alt/style.sh " + active().bufmgr_->active()->file_.toUtf8().data() + QString("\"");
	char *out = run_program(cmd.toUtf8().data());
	if (!out) {
		QMessageBox::warning(this, tr("AltEdit"),
				     tr("Cannot run code styler."));
		return;
	}

	// replace current buffer
	active().bufmgr_->active()->text_ = QString(out);
	active().bufmgr_->active()->modified_ = true;
	delete[] out;
	restoreBufmgr();

	QTimer::singleShot(50, this, SLOT(statusDelay()));
	active().text_->setFocus();
}

void MainWindow::metaPressed() {
	FLOG();
	QString word = active().text_->wordAtCursor();
	fdebug("word #%s#\n", word.toUtf8().data());

	int line;
	QString text = get_tags(word, &line);
	infotext_->setWordWrapMode(QTextOption::NoWrap);
	infotext_->setPlainText(text);
	infotext_->setLine(line);
	active().text_->setFocus();
	QTimer::singleShot(50, this, SLOT(statusDelay()));
}


