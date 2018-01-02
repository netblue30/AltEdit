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
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
Q_OBJECT

	public:
	Highlighter(QTextDocument *parent = 0);
	void setFile(QString fname);

protected:
	void highlightBlock(const QString &text) override;


private:
	void configureCpp();
	void highlightCpp(const QString &text);
	void configureJava();
	void highlightJava(const QString &text);
	void configureXml();
	void highlightXml(const QString &text);

	typedef enum {
		CPP,
		JAVA,
		XML
	} HighlightType;
	HighlightType htype_;


private:
	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};

	// CPP
	QVector<HighlightingRule> cppHighlightingRules;
	QRegularExpression cppCommentStartExpression;
	QRegularExpression cppCommentEndExpression;
	QTextCharFormat cppMultiLineCommentFormat;

	// JAVA
	QVector<HighlightingRule> javaHighlightingRules;
	QRegularExpression javaCommentStartExpression;
	QRegularExpression javaCommentEndExpression;
	QTextCharFormat javaMultiLineCommentFormat;

	// XML
	QVector<HighlightingRule> xmlHighlightingRules;

};
#endif						  // HIGHLIGHTER_H
