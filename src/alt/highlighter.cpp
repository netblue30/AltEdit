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
#include "highlighter.h"
#include "global.h"

// C/C++ preproc
#define DEFAULT_TAG1_COLOR       QColor(255, 25, 25)
// C/C++ strings
#define DEFAULT_TAG2_COLOR       QColor(200, 255, 150)
// C/C++ comments
#define DEFAULT_TAG3_COLOR       QColor(150, 150, 255)
// C/C++ keywords
#define DEFAULT_TAG4_COLOR       QColor(75, 200, 75)
// function names
#define DEFAULT_TAG5_COLOR       QColor(255, 255, 160)

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
	FLOG();
	HighlightingRule rule;

	keywordFormat.setForeground(DEFAULT_TAG4_COLOR);
//	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
		<< "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
		<< "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
		<< "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
		<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
		<< "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
		<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
		<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
		<< "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
		<< "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
		<< "\\bif\\b" << "\\belse\\b" << "\\bdo\\b" << "\\while\\b" << "\\bswitch\\b"
		<< "\\bcase\\b" << "\\bdefault\\b" << "\\bfor\\b" << "\\breturn\\b"
		<< "\\b__PRETTY_FUNCTION__\\b" << "\\b__LINE__\\b"
		<< "\\b__FILE__\\b" << "__FUNCTION__\\b"
		<< "\\bbreak\\b" << "\\bcontinue\\b" << "\\bextern\\b" << "\\btrue\\b" << "\\bfalse\\b";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}
#if 0
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
#endif

	quotationFormat.setForeground(DEFAULT_TAG2_COLOR);
	rule.pattern = QRegularExpression("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

// function name
	functionFormat.setFontWeight(QFont::Bold);
//	functionFormat.setFontItalic(true);
	functionFormat.setForeground(DEFAULT_TAG5_COLOR);
	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+::[A-Za-z0-9_]+(?=\\()");
//	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	preprocFormat.setForeground(DEFAULT_TAG1_COLOR);
	QStringList preprocPatterns;
	preprocPatterns << "#ifdef\\b" << "#define\\b" << "#endif\\b"
		<< "#include\\b" << "#if\\b" << "#elif\\b" << "#error\\b" << "#ifndef\\b";
	foreach (const QString &pattern, preprocPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = preprocFormat;
		highlightingRules.append(rule);
	}

	singleLineCommentFormat.setFontItalic(true);
	singleLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);
	commentCppExpression = QRegularExpression("//[^\n]*");
	rule.pattern = commentCppExpression;
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setFontItalic(true);
	multiLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);

	commentStartExpression = QRegularExpression("/\\*");
	commentEndExpression = QRegularExpression("\\*/");
}


void Highlighter::highlightBlock(const QString &text) {
//printf("%s\n", text.toUtf8().data());
	foreach (const HighlightingRule &rule, highlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf(commentStartExpression);

		QRegularExpression cppcomment = QRegularExpression("//[^\n]*");
		int cppindex = text.indexOf(cppcomment);
		if (startIndex >= 0 && cppindex >= 0 && startIndex > cppindex)
			startIndex = -1;
	}

	while (startIndex >= 0) {
		QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
		int endIndex = match.capturedStart();
		int commentLength = 0;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else {
			commentLength = endIndex - startIndex
				+ match.capturedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
	}
}


