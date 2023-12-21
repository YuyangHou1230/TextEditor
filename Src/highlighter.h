#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *document = nullptr);

    // QSyntaxHighlighter interface
protected:
    void highlightBlock(const QString &text) override;

private:

    struct HighlightRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    void addNormalTextFormat();

    void addNumberFormat();

    void addStringFormat();

    void addCommentFormat();

private:
    const QString m_FontFamily = "Consolas";
    const int m_fontSize = 14;

    QVector<HighlightRule> m_rules;

};

#endif // HIGHLIGHTER_H
