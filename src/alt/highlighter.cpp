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

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent), htype_(NONE) {
	FLOG();
	configureNone();
	configureCpp();
	configureJava();
	configureXml();
	configureSh();
}

void Highlighter::setFile(QString fname) {
	FLOG();

	if (fname.endsWith(".h") || fname.endsWith(".c") || fname.endsWith(".cpp") || fname.endsWith(".hpp")) {
		htype_ = CPP;
	}
	else if (fname.endsWith(".java")) {
		htype_ = JAVA;
	}
	else if (fname.endsWith(".xml") || fname.endsWith(".html") || fname.endsWith(".qrc") || fname.endsWith(".svg")) {
		htype_ = XML;
	}
	else if (fname.endsWith(".sh")) {
		htype_ = SH;
	}
	else { // use NONE as default
		htype_ = NONE;
	}
	fdebug("New highlighter type %d for %s\n", htype_, qPrintable(fname));
}

void Highlighter::configureNone() {
}

void Highlighter::configureSh() {
	HighlightingRule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(DEFAULT_TAG4_COLOR);
//	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bif\\b" << "\\bthen\\b" << "\\belif\\b" << "\\belse\\b" <<
			"\\bfi\\b" << "\\btime\\b" << "\\bfor\\b" << "\\bin\\b" <<
			"\\buntil\\b" << "\\bwhile\\b" << "\\bdo\\b" << "\\bdone\\b" <<
			"\\bcase\\b" << "\\besac\\b" << "\\bcoproc\\b" << "\\bselect\\b" <<
			"\\bfunction\\b" << "\\becho\\b" << "\\bgrep\\b" << "\\bless\\b" <<
			"\\bcp\\b" << "\\bmv\\b"  << "\\bcd\\b" << "\\bexit\\b" << "\\beval\\b"
			 << "\\bexport\\b" << "\\bset\\b" << "\\bunset\\b";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		shHighlightingRules.append(rule);
	}

	QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setFontItalic(true);
	singleLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);
	QRegularExpression commentShExpression = QRegularExpression("#[^\n]*");
	rule.pattern = commentShExpression;
	rule.format = singleLineCommentFormat;
	shHighlightingRules.append(rule);
}

void Highlighter::configureXml() {
	HighlightingRule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(DEFAULT_TAG4_COLOR);
//	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
//	keywordPatterns << "/>" << ">" << "<" << "</";
	keywordPatterns << "/>" << ">" << "<[?A-Za-z0-9_-]*" << "</[A-Za-z0-9_-]*";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		xmlHighlightingRules.append(rule);
	}

#if 0
	QTextCharFormat elementFormat;
	elementFormat.setFontWeight(QFont::Bold);
//	functionFormat.setFontItalic(true);
	elementFormat.setForeground(DEFAULT_TAG5_COLOR);
	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_-]+(?=[\\s/>])");
//	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = elementFormat;
	xmlHighlightingRules.append(rule);
#endif
#if 0
	QTextCharFormat atributeFormat;
	atributeFormat.setForeground(DEFAULT_TAG1_COLOR);
	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\=)");
	rule.format = atributeFormat;
	xmlHighlightingRules.append(rule);
#endif
	QTextCharFormat quotationFormat;
	quotationFormat.setForeground(DEFAULT_TAG2_COLOR);
//	rule.pattern = QRegularExpression("\".*\"");
	rule.pattern = QRegularExpression("\".*?\"");
	rule.format = quotationFormat;
	xmlHighlightingRules.append(rule);

	QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setFontItalic(true);
	singleLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);
	QRegularExpression commentCppExpression = QRegularExpression("<!--[^\n]*-->");
	rule.pattern = commentCppExpression;
	rule.format = singleLineCommentFormat;
	xmlHighlightingRules.append(rule);

}

void Highlighter::configureCpp() {
	HighlightingRule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(DEFAULT_TAG4_COLOR);
//	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\balignas\\b" << "\\balignof\\b" << "\\band\\b" << "\\band_eq\\b" <<
			"\\basm\\b" << "\\batomic_cancel\\b" << "\\batomic_commit\\b" << "\\batomic_noexcept\\b" <<
			"\\bauto\\b" << "\\bbitand\\b" << "\\bbitor\\b"  << "\\bbool\\b" <<
			"\\bbreak\\b" << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" <<
			"\\bchar16_t\\b" << "\\bchar32_t\\b" << "\\bclass\\b" << "\\bcompl\\b" <<
			"\\bconcept\\b" << "\\bconst\\b" << "\\bconstexpr\\b" << "\\bconst_cast\\b" <<
			"\\bcontinue\\b" << "\\bco_await\\b" << "\\bco_return\\b" << "\\bco_yield\\b" <<
			"\\bdecltype\\b" << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdo\\b" <<
			"\\bdouble\\b" << "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b" <<
			"\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b" <<
			"\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b" << "\\bgoto\\b" <<
			"\\bif\\b" << "\\bimport\\b" << "\\binline\\b" << "\\bint\\b" <<
			"\\blong\\b" << "\\bmodule\\b" << "\\bmutable\\b" << "\\bnamespace\\b" <<
			"\\bnew\\b" << "\\bnoexcept\\b" << "\\bnot\\b" << "\\bnot_eq\\b" <<
			"\\bnullptr\\b" << "\\boperator\\b" << "\\bor\\b" << "\\bor_eq\\b" <<
			"\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\bregister\\b" <<
			"\\breinterpret_cast\\b" << "\\brequires\\b" << "\\breturn\\b" << "\\bshort\\b" <<
			"\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_assert\\b" <<
			"\\bstatic_cast\\b" << "\\bstruct\\b" << "\\bswitch\\b" << "\\bsynchronized\\b" <<
			"\\btemplate\\b" << "\\bthis\\b" << "\\bthread_local\\b" << "\\bthrow\\b" <<
			"\\btrue\\b" << "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b" <<
			"\\btypename\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\busing\\b" <<
			"\\bvirtual\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b" <<
			"\\bwhile\\b" << "\\bxor\\b" << "\\bxor_eq \\b" << "\\bsize_t\\b" << "\\bptrdiff_t\\b";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		cppHighlightingRules.append(rule);
	}
#if 0
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
#endif

	QTextCharFormat quotationFormat;
	quotationFormat.setForeground(DEFAULT_TAG2_COLOR);
	rule.pattern = QRegularExpression("\".*?\"");
	rule.format = quotationFormat;
	cppHighlightingRules.append(rule);

// function name
	QTextCharFormat functionFormat;
	functionFormat.setFontWeight(QFont::Bold);
//	functionFormat.setFontItalic(true);
	functionFormat.setForeground(DEFAULT_TAG5_COLOR);
	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+::[A-Za-z0-9_]+(?=\\()");
//	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	cppHighlightingRules.append(rule);

	QTextCharFormat preprocFormat;
	preprocFormat.setForeground(DEFAULT_TAG1_COLOR);
	QStringList preprocPatterns;
	preprocPatterns << "#ifdef\\b" << "#define\\b" << "#endif\\b"
			<< "#include\\b" << "#if\\b" << "#elif\\b" << "#error\\b" << "#ifndef\\b";
	foreach (const QString &pattern, preprocPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = preprocFormat;
		cppHighlightingRules.append(rule);
	}

	QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setFontItalic(true);
	singleLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);
	QRegularExpression commentCppExpression = QRegularExpression("//[^\n]*");
	rule.pattern = commentCppExpression;
	rule.format = singleLineCommentFormat;
	cppHighlightingRules.append(rule);

	cppMultiLineCommentFormat.setFontItalic(true);
	cppMultiLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);

	cppCommentStartExpression = QRegularExpression("/\\*");
	cppCommentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::configureJava() {
	HighlightingRule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(DEFAULT_TAG4_COLOR);
//	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b" << "\\bbreak\\b" <<
			"\\bbyte\\b" << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" <<
			"\\bclass\\b" << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b" <<
			"\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextends\\b" <<
			"\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b" <<
			"\\bif\\b" << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b" <<
			"\\bint\\b" << "\\binterface\\b" << "\\blong\\b" << "\\bnative\\b" <<
			"\\bnew\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b" <<
			"\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b" <<
			"\\bstrictfp\\b" << "\\bsuper\\b" << "\\bswitch\\b" << "\\bsynchronized\\b" <<
			"\\bthis\\b" << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b" <<
			"\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" <<
			"\\btrue\\b" << "\\bnull\\b" << "\\bfalse\\b" << "\\bconst\\b" << "\\bgoto\\b";

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = keywordFormat;
		javaHighlightingRules.append(rule);
	}
#if 0
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
#endif

	QTextCharFormat quotationFormat;
	quotationFormat.setForeground(DEFAULT_TAG2_COLOR);
	rule.pattern = QRegularExpression("\".*?\"");
	rule.format = quotationFormat;
	javaHighlightingRules.append(rule);

// function name
	QTextCharFormat functionFormat;
	functionFormat.setFontWeight(QFont::Bold);
//	functionFormat.setFontItalic(true);
	functionFormat.setForeground(DEFAULT_TAG5_COLOR);
	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+::[A-Za-z0-9_]+(?=\\()");
//	rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	javaHighlightingRules.append(rule);

	QTextCharFormat preprocFormat;
	preprocFormat.setForeground(DEFAULT_TAG1_COLOR);
	QStringList preprocPatterns;
	preprocPatterns << "#ifdef\\b" << "#define\\b" << "#endif\\b"
			<< "#include\\b" << "#if\\b" << "#elif\\b" << "#error\\b" << "#ifndef\\b";
	foreach (const QString &pattern, preprocPatterns) {
		rule.pattern = QRegularExpression(pattern);
		rule.format = preprocFormat;
		javaHighlightingRules.append(rule);
	}

	QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setFontItalic(true);
	singleLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);
	QRegularExpression commentCppExpression = QRegularExpression("//[^\n]*");
	rule.pattern = commentCppExpression;
	rule.format = singleLineCommentFormat;
	javaHighlightingRules.append(rule);

	javaMultiLineCommentFormat.setFontItalic(true);
	javaMultiLineCommentFormat.setForeground(DEFAULT_TAG3_COLOR);

	javaCommentStartExpression = QRegularExpression("/\\*");
	javaCommentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text) {
	switch (htype_) {
	case NONE:
		highlightNone(text);
		break;
	case CPP:
		highlightCpp(text);
		break;
	case JAVA:
		highlightJava(text);
		break;
	case XML:
		highlightXml(text);
		break;
	case SH:
		highlightSh(text);
		break;
	default:
		fprintf(stderr, "Error: unknown highlighting type\n");
	}
}

void Highlighter::highlightNone(const QString &text) {
	(void) text;
}

void Highlighter::highlightSh(const QString &text) {
	foreach (const HighlightingRule &rule, shHighlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
}

void Highlighter::highlightXml(const QString &text) {
	foreach (const HighlightingRule &rule, xmlHighlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
}

void Highlighter::highlightCpp(const QString &text) {
	foreach (const HighlightingRule &rule, cppHighlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf(cppCommentStartExpression);

		QRegularExpression cppcomment = QRegularExpression("//[^\n]*");
		int cppindex = text.indexOf(cppcomment);
		if (startIndex >= 0 && cppindex >= 0 && startIndex > cppindex)
			startIndex = -1;
	}

	while (startIndex >= 0) {
		QRegularExpressionMatch match = cppCommentEndExpression.match(text, startIndex);
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
		setFormat(startIndex, commentLength, cppMultiLineCommentFormat);
		startIndex = text.indexOf(cppCommentStartExpression, startIndex + commentLength);
	}
}

void Highlighter::highlightJava(const QString &text) {
	foreach (const HighlightingRule &rule, javaHighlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = text.indexOf(javaCommentStartExpression);

		QRegularExpression javacomment = QRegularExpression("//[^\n]*");
		int javaindex = text.indexOf(javacomment);
		if (startIndex >= 0 && javaindex >= 0 && startIndex > javaindex)
			startIndex = -1;
	}

	while (startIndex >= 0) {
		QRegularExpressionMatch match = javaCommentEndExpression.match(text, startIndex);
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
		setFormat(startIndex, commentLength, javaMultiLineCommentFormat);
		startIndex = text.indexOf(javaCommentStartExpression, startIndex + commentLength);
	}
}
