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
#ifndef GLOBAL_H
#define GLOBAL_H

#include <assert.h>
#define FLOG() fdebug("%s\n", __PRETTY_FUNCTION__);
#define MAXBOOKMARKS 10
#define MAXBUFMGR 32

class BasicEdit;
class QString;

// main.cpp
extern int arg_debug;

// ctags.cpp
QString get_tags(QString word, int *line);
QString function_list(QString fname);
QString open_tagged_file(QString file);

// utils.cpp
// returns allocated memory, use delete[]
char *run_program(const char *program);
// output goes in wdg
void run_program_window(const char *program, BasicEdit *wdg);
// fork a program; output goes in the terminal
void run_program_fork(const char *program);
// mark the first {} or () blocik
// return true if the block was found, and out_start and out_end are poitions in the string
bool find_block(const char *str, unsigned *out_start, unsigned *out_end);
// debug messages
void fdebug(const char* fmt, ...);
// check programs
bool check_program(QString program);

#endif