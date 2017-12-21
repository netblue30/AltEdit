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
#include <QApplication>
#include <QFileInfo>
#include "global.h"
#include "../../alt_config.h"

static void version() {
	printf("diff - version %s\n", PACKAGE_VERSION);
}

void usage() {
	printf("This is the diff tool included in AltEdit programming text editor\n\n");
	printf("Usage: diff [options] file1 [file2]\n\n");
	printf("Options:\n");
	printf("\t-h, -?, --help - this help screen\n");
	printf("\t--git - git diff for file1\n");
	printf("\t-v, --version - print program version\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	FLOG();
	int arg_git = 0;
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 ||
		    strcmp(argv[i], "--help") == 0 ||
		    strcmp(argv[i], "-?") == 0) {
		 	usage();
		 	return 0;
		}
		else if (strcmp(argv[i], "--git") == 0)
			arg_git = 1;
		else if (strcmp(argv[i], "-v") == 0 ||
		 	strcmp(argv[i], "--version") == 0) {
		    	version();
		    	return 0;
		}
		else if (*argv[i] == '-') {
			fprintf(stderr, "Error: unrecognized option\n");
			usage();
			exit(1);
		}
		else
			break;
	}

	int file_index = i;
	if (arg_git) {
		if (argc != 3) {
			fprintf(stderr, "Error: invalid number of arguments\n");
			usage();
			exit(1);
		}
	}
	else {
		if (argc != 3) {
			fprintf(stderr, "Error: invalid number of arguments\n");
			usage();
			exit(1);
		}
	}

	Q_INIT_RESOURCE(diff);
	QApplication app(argc, argv);
	QString cmd = QString("git diff --unified=10000000 ");
	if (arg_git)
		cmd += QString(argv[file_index]);
	else
	 	cmd += QString(argv[file_index]) + " " + argv[file_index + 1];
	MainWindow mainWin(cmd, argv[file_index]);

	mainWin.show();
	return app.exec();
}


