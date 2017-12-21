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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "basicedit.h"

static const int PROG_BUF = 10000; // initial buffer size for run_prgram

struct MyList {
	char buf[PROG_BUF + 1];
	MyList *next;

	MyList(): next(0) {
		memset(buf, 0, sizeof(buf));
	}

	~MyList() {
		if (next != 0) {
			delete next;
		}
	}
};

// returns allocated memory, use delete[]
char *run_program(const char *program) {
	assert(program);
	FILE *input;
	MyList *ptr = new MyList();
	MyList *head = ptr;
	char *retval;
	int len;

	if (0 == (input = popen(program, "r"))) {
		fprintf(stderr, "Error: cannot run program %s\n", program);
		delete head;
		return 0;
	} else {
		while ((len = fread(ptr->buf, 1, PROG_BUF, input)) > 0) {
			ptr->next = new MyList();
			ptr = ptr->next;
		}

		pclose(input);
	}

	// calculate length of return string
	len = 0;
	ptr = head;
	while (ptr != 0) {
		len += strlen(ptr->buf);
		ptr = ptr->next;
	}

	if (len == 0) {
		delete head;
		return 0;
	}

	/* allocate retval +10 just in case... */
	if ((retval = new char[len + 10]) == 0) {
		delete head;
		return 0;
	}
	memset(retval, 0, len + 10);

	/* copy retval */
	ptr = head;
	while (ptr != 0) {
		strcat(retval, ptr->buf);
		ptr = ptr->next;
	}

	delete head;
	return retval;
}

// output goes in wdg
void run_program_window(const char *program, BasicEdit *wdg) {
	assert(program);
	FILE *input;
	char buf[PROG_BUF + 1];
	char *out;

	if (0 == (input = popen(program, "r")))
		return;

	else {
		while ((out = fgets(buf, PROG_BUF, input)) != 0) {
			printf("%s", buf);
			wdg->insertPlainText(QString::fromUtf8(buf));
			wdg->gotoEnd();
			wdg->update();
			qApp->processEvents();
		}

		pclose(input);
	}
}

// fork a program; output goes in the terminal
void run_program_fork(const char *program) {
	assert(program);

	// reap zombie processes automatically - we don't monitor child pid
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		fprintf(stderr, "Error: cannot set SIGCHILD\n");
		perror(0);
		return;
	}

	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Error: cannot fork\n");
		perror("fork");
		return;
	}

	// child
	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", program, NULL);
		exit(1);
	}
}

// mark the first {} or () blocik
// return true if the block was found, and out_start and out_end are poitions in the string
bool find_block(const char *str, unsigned *out_start, unsigned *out_end) {
	*out_start = -1;
	*out_end = -1;

	const char *ptr = str;
	while (*ptr != '(' && *ptr != '{' && *ptr != '\0')
		ptr++;
	if (*ptr == '\0')
		return false;

	*out_start = (unsigned) (ptr - str);

	char start_char = *ptr;
	char end_char = (start_char == '(')? ')': '}';
	int level = 1;
	ptr++;

	while (*ptr != '\0') {
		if (*ptr == start_char)
			level++;
		else if (*ptr == end_char) {
			level--;
			if (level == 0) {
				*out_end = (unsigned) (ptr - str + 1);
				return true;
			}
		}

		ptr++;
	}

	return false;
}

// debug messages
void fdebug(const char* fmt, ...) {
	if (!arg_debug)
		return;

	va_list args;
	va_start(args,fmt);
	fprintf(stderr, "debug - ");
	vfprintf(stderr, fmt, args);
	va_end(args);
}

// check programs
bool check_program(QString program) {
	// try /usr/bin
	QString path = QString("/usr/bin/") + program;
	if (access(path.toUtf8().data(), X_OK) == -1) {
		// try /bin
		path = QString("/bin/") + program;
		if (access(path.toUtf8().data(), X_OK) == -1)
			return false;
	}

	return true;
}
