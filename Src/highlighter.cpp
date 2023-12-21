#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *document) :QSyntaxHighlighter(document)
{
    // 添加普通文本高亮
    addNormalTextFormat();

    // 添加数字高亮
    addNumberFormat();

    // 添加字符串高亮
    addStringFormat();

    addCommentFormat();
}

// 按行传来的
void Highlighter::highlightBlock(const QString &text)
{
    foreach( const auto &rule, m_rules)
    {
        QRegExp regExp(rule.pattern);
        int index = regExp.indexIn(text);
        while (index >= 0) {
            int length = regExp.matchedLength();
            setFormat(index, length, rule.format);
            index = regExp.indexIn(text, index + length);
        }
    }
}

void Highlighter::addNormalTextFormat()
{
    HighlightRule rule;
    rule.pattern = QRegExp("[a-z0-9A-Z]+");

    QTextCharFormat format;
    format.setFont(QFont(m_FontFamily, m_fontSize));
    format.setForeground(QColor(0, 0, 0));

    rule.format = format;

    m_rules.append(rule);
}

void Highlighter::addNumberFormat()
{
    HighlightRule rule;
    rule.pattern = QRegExp("\\b\\d+|\\d+\\.\\d+\\b");

    QTextCharFormat format;
    format.setFont(QFont(m_FontFamily, m_fontSize));
    format.setForeground(QColor(250,  80, 50));

    rule.format = format;

    m_rules.append(rule);
}

void Highlighter::addStringFormat()
{
    HighlightRule rule;
    rule.pattern = QRegExp("'[^']*'|`[^`].*`|\"[^\"]*\"");

    QTextCharFormat format;
    format.setFont(QFont(m_FontFamily, m_fontSize));
    format.setForeground(QColor(146, 189, 108));

    rule.format = format;

    m_rules.append(rule);
}

void Highlighter::addCommentFormat()
{
    HighlightRule rule;

    QTextCharFormat format;
    format.setFont(QFont(m_FontFamily, m_fontSize));
    format.setForeground(Qt::darkGreen);

    rule.format = format;

    rule.pattern = QRegExp("//.*$");
    m_rules.append(rule);

    rule.pattern = QRegExp("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/");
    m_rules.append(rule);
}
