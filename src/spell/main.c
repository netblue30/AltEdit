/*
 *    Copyright (C) 2017 ned Authors
 *
 *    This file is part of ned project
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define MAXBUF 4096

static void alarm_handler(int sig) {
	(void) sig;
	exit(1);
}

FILE *sopen(const char *program) {
	int fds[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
		return NULL;

	pid_t pid = fork();
	if (pid == -1) {
		close(fds[0]);
		close(fds[1]);
		return NULL;
	}

	if (pid == 0 ) { // child
		close(fds[0]);
		dup2(fds[1], 0);
		dup2(fds[1], 1);
		close(fds[1]);
		execl("/bin/sh", "sh", "-c", program, NULL);
		exit(1);
	}

	// parent
	close(fds[1]);
	return fdopen(fds[0], "r+");
}


int main(int argc, char **argv) {
	if (access("/usr/bin/hunspell", X_OK)) {
		printf("Error: /usr/bin/hunspell not found\n");
		return 1;
	}

	// build text string
	int i;
	int size = 0;
	for (i = 1; i < argc; i++)
		size += strlen(argv[i]) + 1;
	size++;
	char *str = malloc(size);
	if (!str) {
		perror("malloc");
		exit(1);
	}
	memset(str, 0, size);
	for (i = 1; i < argc; i++) {
		strcat(str, argv[i]);
		strcat(str, " ");
	}

	// reap zombie processes automatically
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		free(str);
		perror("signal");
		return 1;
	}

	FILE *fp = sopen("hunspell -d en_US");
	if (!fp) {
		free(str);
		printf("Error: cannot run hunspell\n");
		return 1;
	}

	char buf[MAXBUF];

	signal(SIGALRM, alarm_handler);
	alarm(5);	// wait not more than 5 seconds for the answer

	// wait for hunspell intro
	if (fgets(buf, MAXBUF, fp)) {
		fflush(fp);
		printf("spelling: \"%s\"\n", str);

		// send the string
		fprintf(fp, "%s\n", str);
		fflush(fp);
		i = 0;
		for (i = 0; i < (argc - 1); i++) {
			char *rv = fgets(buf, MAXBUF, fp);
			if (rv == NULL)
				break;
			if (*buf =='&') {
				// clean output
				char *ptr = buf;
				while (*ptr != '\0') {
					if (*ptr == '&' || *ptr == '.' || isdigit(*ptr))
						*ptr = ' ';
					ptr++;
				}
				printf("%s", buf);
			}
		}
	}

	fclose(fp);
}



