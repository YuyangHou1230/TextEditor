#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>

class LineNumberWidget;
class TextEditor : public QPlainTextEdit
{
public:
    TextEditor(QString fileName, QWidget *parent = nullptr);

    void lineNumberPaintEvent(QPaintEvent *e);

    void loadFile(QString fileName);
    bool saveFile();
    bool saveAsFile();

    QString getFileName() const;

public slots:
    void updateLineNumber(const QRect &rect, int dy);

protected:
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void highlightCurrentLine();
    void updateLineNumberMargin();

private:
    void initFont();
    void initConnect();
    void initHighlighter();

    int getLineNumberWidth();

private:
    LineNumberWidget *m_lineNumberWidget;

    QString m_fileName;   // 文件名
};

class LineNumberWidget : public QWidget
{
public:
    LineNumberWidget(TextEditor *editor);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    TextEditor *m_editor;
};

#endif   // TEXTEDITOR_H
