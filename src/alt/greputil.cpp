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
#include "grep.h"
#include <QtWidgets>
#include <ftw.h>
#include <assert.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include "global.h"
#include <QRegExp>

QString word;
Qt::CaseSensitivity cs; // Qt::CaseInsensitive, Qt::CaseSensitive
QString starting_dir;
QString result;
QString file_pattern;

static int fn(const char *path, const struct stat *ptr, int flag) {
	if (flag == FTW_F) {
		assert(ptr);

		// process only files smaller then 1MB
		if (ptr->st_size > (1024 * 1024))
			return 0;
		QString qpath(path);
		if (qpath.endsWith(".o") || qpath.endsWith(".png"))
			return 0;

		QRegExp rx(file_pattern);
		rx.setPatternSyntax(QRegExp::Wildcard);
		if (!rx.exactMatch(path))
			return 0;

		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return 0;

		QTextStream in(&file);
		int lineno = 0;
		while (!in.atEnd()) {
			lineno++;
			QString line = in.readLine();
			if (line.indexOf(word, 0, cs) != -1) {
				result += QString("%1:%2\n").arg(path).arg(lineno);
				result += QString("\t") + line + "\n";
			}
		}

		file.close();
	}

	return 0;
}

QString greputil(GrepDialog *dialog) {
	FLOG();
	assert(dialog);
	result = "";

	if (dialog->getIgnoreCase())
		cs = Qt::CaseInsensitive;
	else
		cs = Qt::CaseSensitive;

	file_pattern = dialog->getFpattern();

	word = dialog->getWord();
	fdebug("look for #%s#\n", qPrintable(word));

	starting_dir = dialog->getSdirectory();
	if (starting_dir == "./")
		starting_dir = get_current_dir_name();

	if (ftw(qPrintable(starting_dir), fn, FOPEN_MAX) == -1) {
		perror("ftw");
	}

	return result;
}

