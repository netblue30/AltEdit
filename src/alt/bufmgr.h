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
#ifndef BUFMGR_H
#define BUFMGR_H
#include "global.h"

struct BufMgrData {
	QString file_;
	QString text_;
	int pos_;
	bool active_;
	bool modified_;

	BufMgrData(QString file): file_(file), pos_(0), active_(true), modified_(false){}
	BufMgrData(): file_(""), pos_(0), active_(false), modified_(false) {}
	~BufMgrData() {}
	void clean() {
		file_ = "";
		text_ = "";
		pos_ = 0;
		active_ = false;
		modified_ = false;
	}
};


class BufMgr {
public:
	BufMgr(): aindex_(0) {}

	void debug() {
		printf("current buffer %d\n", aindex_);
		int i;
		for (i = 0; i < MAXBUFMGR; i++)  {
			if (data_[i].active_)
				printf("%d: %s %s %s\n",  i, (data_[i].active_)? "active": "inactive",
					data_[i].file_.toUtf8().data(), (data_[i].modified_)? "modified": "not modified");
		}

	}

	// add a new buffer and make it active
	// returns false if all buffers are in use
	bool add(QString fname) {
		FLOG();
		// find an empty buffer
		int i;
		int index = -1;
		for (i = 0; i < MAXBUFMGR; i++) {
			// inactive?
			if (!data_[i].active_) {
				index = i;
				break;
			}
			// try to reuse an active empty buffer
			if (data_[i].active_ && data_[i].file_.isEmpty() && !data_[i].modified_) {
				index = i;
				break;
			}
		}
		fdebug("BufMgr: new index %d\n", index);

		if (index == -1)
			return false;

		data_[index].clean();
		data_[index].active_ = true;
		data_[index].file_ = fname;
		aindex_ = index;
		return true;
	}

	// returns true is we already have the file
	bool set(QString fname) {
		FLOG();
		int i;
		for (i = 0; i < MAXBUFMGR; i++) {
			if (data_[i].active_ && fname == data_[i].file_) {
				aindex_ = i;
				return true;
			}
		}

		return false;
	}

	void remove() {
		FLOG();
		data_[aindex_].clean();
		next();
		if (data_[aindex_].active_ == false)
			data_[aindex_].active_ = true;
	}

	// goto next buffer
	void next() {
		FLOG();
		int start = aindex_;
		int i = aindex_ + 1;
		if (i >= MAXBUFMGR)
			i = 0;
		for (;i < MAXBUFMGR; i++) {
			if (data_[i].active_) {
				aindex_ = i;
				fdebug("BufMgr: new bufmgr index %d\n", aindex_);
				return;
			}
		}
		for (i = 0; i < start; i++) {
			if (data_[i].active_) {
				aindex_ = i;
				fdebug("BufMgr: new bufmgr index %d\n", aindex_);
				return;
			}
		}
		fdebug("BufMgr: unchanged bufmgr index %d\n", aindex_);
	}

	// get active buffer, or the first one if none is active
	BufMgrData *active() {
		return &data_[aindex_];
	}

	int size() {
		return MAXBUFMGR;
	}

	BufMgrData &operator[](int index) {
		if (index >= MAXBUFMGR)
			index = MAXBUFMGR - 1;
		return data_[index];
	}

private:
	BufMgrData data_[MAXBUFMGR];
	// active index - 0 based
	int aindex_;
};
#endif
