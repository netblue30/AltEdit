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
#include "global.h"
#include <QString>
#include <QHash>
#include <QtWidgets>

static bool initialized = false;

struct Tag {
	QString text_;
	QString file_;
	QString line_;
	Tag() {}
	Tag(QString text, QString file, QString line):
		text_(text), file_(file), line_(line) {}
};

QHash<QString, Tag> hash;

static void initialize() {
	QString cmd = QString("ctags -e -f - -x -R ");
	char *output = run_program(qPrintable(cmd));
	QString text = QString("");

	if (output) {
		// parse the output file
		char *ptr = output;
		char *start = ptr;
		while ((ptr = strchr(ptr, '\n')) != 0) {
			*ptr = '\0';
			ptr++;

			char *line = strdup(start);
			if (!line) {
				perror("strdup");
				fprintf(stderr, "Error: cannot allocate memory\n");
				delete[] output;
				return;
			}

			// BasicEdit        function      7 /home/netblue/work/basicedit.cpp BasicEdit::BasicEdit(): QPlainTextEdit()
			char *ptr2 = strtok(line, " ");
			if (!ptr2) { // BasicEdit
				free(line);
				start = ptr;
				continue;
			}
			QString name(ptr2);

			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // function
				free(line);
				start = ptr;
				continue;
			}

			QString member_tab;
//			if (strcmp(ptr2, "macro") == 0) {
//				free(line);
//				start = ptr;
//				continue;
//			}
			if (strcmp(ptr2, "member") == 0)
				member_tab = QString("\t");

			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // 7
				free(line);
				start = ptr;
				continue;
			}
			QString line_number = QString(ptr2);

			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // /home/netblue/work/basicedit.cpp
				free(line);
				start = ptr;
				continue;
			}
			QString fname = QString(ptr2);

			ptr2 = strtok(NULL, "\n");
			if (!ptr2) { // BasicEdit::BasicEdit(): QPlainTextEdit()
				free(line);
				start = ptr;
				continue;
			}

			// add (name, value) pair to the hash table
			QString value = fname + ":" + line_number + "\n";
			value += QString("\t") + member_tab + ptr2 + "\n";
			Tag t(value, fname, line_number);
			hash.insertMulti(name, t);

			free(line);
			start = ptr;
		}

		delete[] output;
	}
}


QString open_tagged_file(QString fname) {
	QFile file(fname);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return QString("");

	QString str = fname + "\n";
	int cnt = 0;
	while (!file.atEnd()) {
		cnt++;
		QByteArray tmp = file.readLine();
		str += QString(":%1\t").arg(cnt) + QString(tmp);
	}

	return str;
}

QString get_tags(QString word, int *line) {
	*line = 0;
	if (!initialized) {
		initialize();
		initialized = true;
	}

	QString rv;
	QHash<QString, Tag>::iterator i = hash.find(word);
	int cnt = 0;
	Tag t;
	while (i != hash.end() && i.key() == word) {
		t = i.value();
		rv += t.text_;
		++i;
		cnt++;
	}

	if (cnt != 1)
		return rv;

	QString str = open_tagged_file(t.file_);
	if (str.isEmpty())
		return rv;

	*line = t.line_.toInt() + 1;
	return str;
}

QString function_list(QString file) {
	QString cmd = QString("ctags -e -f - -x ") + file;

	QString text = file + "\n";
	char *output = run_program(qPrintable(cmd));
	if (output) {
		// parse the output file
		char *ptr = output;
		char *start = ptr;
		while ((ptr = strchr(ptr, '\n')) != 0) {
			*ptr = '\0';
			ptr++;

			char *line = strdup(start);
			if (!line) {
				perror("strdup");
				fprintf(stderr, "Error: cannot allocate memory\n");
				delete[] output;
				return QString("");
			}

			// BasicEdit        function      7 /home/netblue/work/basicedit.cpp BasicEdit::BasicEdit(): QPlainTextEdit()
			char *ptr2 = strtok(line, " ");
			if (!ptr2) { // BasicEdit
				free(line);
				start = ptr;
				continue;
			}

			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // function
				free(line);
				start = ptr;
				continue;
			}

			QString tab;
//			if (strcmp(ptr2, "macro") == 0) {
//				free(line);
//				start = ptr;
//				continue;
//			}
			if (strcmp(ptr2, "member") == 0)
				tab = QString("\t");

			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // 7
				free(line);
				start = ptr;
				continue;
			}

			QString line_number = QString(":") + ptr2 + "\t";
			ptr2 = strtok(NULL, " ");
			if (!ptr2) { // /home/netblue/work/basicedit.cpp
				free(line);
				start = ptr;
				continue;
			}

			ptr2 = strtok(NULL, "\n");
			if (!ptr2) { // BasicEdit::BasicEdit(): QPlainTextEdit()
				free(line);
				start = ptr;
				continue;
			}

			text += line_number + tab + QString(ptr2) + "\n";
			free(line);
			start = ptr;
		}

		delete[] output;
	}

	return text;
}




