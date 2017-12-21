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

int arg_debug = 0;

static void version() {
	printf("AltEdit - version %s\n", PACKAGE_VERSION);
}

void usage() {
	printf("AltEdit is a Qt5-based tiling programming text editor\n\n");
	printf("Usage: alt [options] [file(s)]\n\n");
	printf("Options:\n");
	printf("\t--debug - print debug messages\n");
	printf("\t-h, -?, --help - this help screen\n");
	printf("\t-v, --version - print program version\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	FLOG();

	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 ||
		    strcmp(argv[i], "--help") == 0 ||
		    strcmp(argv[i], "-?") == 0) {
		 	usage();
		 	return 0;
		}
		else if (strcmp(argv[i], "-v") == 0 ||
		    strcmp(argv[i], "--version") == 0) {
		    	version();
		    	return 0;
		}
		else if (strcmp(argv[i], "--debug") == 0)
			arg_debug = 1;
		else if (*argv[i] == '-') {
			fprintf(stderr, "Error: unrecognized option\n");
			usage();
			exit(1);
		}
		else
			break;
	}
	int file_index = i;

	Q_INIT_RESOURCE(alt);
	QApplication app(argc, argv);
	MainWindow mainWin;

	// open files
	if (file_index != argc) {
		for (i = file_index; i < argc; i++) {
			// build full path for the file
			mainWin.openFile(QString(argv[i]), true);
		}
	}

	mainWin.show();
	return app.exec();
}

