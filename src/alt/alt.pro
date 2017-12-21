QMAKE_CXXFLAGS += $$(CXXFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_CFLAGS += $$(CFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_LFLAGS += $$(LDFLAGS) -Wl,-z,relro -Wl,-z,now
QT += widgets
HEADERS       = mainwindow.h basicedit.h bufmgr.h highlighter.h find.h replace.h grep.h jump.h global.h bcursor.h \
	diff.h
SOURCES       = main.cpp \
                ctags.cpp \
                mainwindow.cpp \
                mainwindow_clicked.cpp \
                basicedit.cpp \
                highlighter.cpp \
                find.cpp \
                replace.cpp \
                grep.cpp \
                jump.cpp \
                greputil.cpp \
                mainwindow_bookmark.cpp \
                utils.cpp \
                diff.cpp
RESOURCES     = alt.qrc



