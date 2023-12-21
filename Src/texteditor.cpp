#include "texteditor.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QTextBlock>

TextEditor::TextEditor(QString fileName, QWidget *parent)
    : QPlainTextEdit(parent)
    , m_lineNumberWidget(new LineNumberWidget(this))
    , m_fileName(fileName)
{
    setViewportMargins(25, 0, 0, 0);

    initFont();
    initConnect();
    initHighlighter();

    highlightCurrentLine();

    loadFile(fileName);
}

void TextEditor::lineNumberPaintEvent(QPaintEvent *e)
{
    QPainter painter(m_lineNumberWidget);
    painter.fillRect(e->rect(), QColor(200, 200, 200, 100));

    QTextBlock block = firstVisibleBlock();

    int lineHeight = blockBoundingGeometry(block).height();

    painter.setPen(QColor(80, 80, 80));
    int top    = blockBoundingGeometry(block).translated(contentOffset()).top() + 1;
    int bottom = top + blockBoundingGeometry(block).height();

    //    qDebug() << "start " << top;

    while ( block.isValid() && top <= e->rect().bottom() )
    {
        int lineNumber = block.blockNumber();
        lineHeight     = blockBoundingGeometry(block).height();
        if ( !block.next().isValid() )
        {
            lineHeight -= 4;
        }

        //        qDebug() << "lineNumbe" << lineNumber << "top " << top << "bottom " << bottom;
        QRect rect(0, top, getLineNumberWidth() - 2, lineHeight);
        QFont font = painter.font();
        font.setPointSize(12);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(lineNumber + 1));

        block  = block.next();
        top    = bottom;
        bottom = top + blockBoundingGeometry(block).height();
    }
}

void TextEditor::loadFile(QString fileName)
{
    if ( fileName.isEmpty() )
    {
        return;
    }

    QFile file(fileName);
    if ( !file.exists() )
    {
        return;
    }

    m_fileName = fileName;
    if ( !file.open(QFile::ReadOnly) )
    {
        return;
    }

    QString text = file.readAll();
    file.close();

    setPlainText(text);
}

bool TextEditor::saveFile()
{
    QString fileName = m_fileName;
    if ( fileName.isEmpty() )
    {
        fileName = QFileDialog::getSaveFileName(this, "保存文件", QApplication::applicationDirPath());
    }

    if ( fileName.isEmpty() )
    {
        return false;
    }

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Truncate) )
    {
        return false;
    }

    m_fileName = fileName;

    QTextStream stream(&file);
    stream << toPlainText();
    stream.flush();
    file.close();

    document()->setModified(false);

    return true;
}

bool TextEditor::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", QApplication::applicationDirPath(), "*.txt");
    if ( fileName.isEmpty() )
    {
        return false;
    }

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text) )
    {
        return false;
    }

    m_fileName = fileName;

    QTextStream stream(&file);
    stream << toPlainText();
    stream.flush();
    file.close();

    document()->setModified(false);

    return true;
}

void TextEditor::updateLineNumber(const QRect &rect, int dy)
{
    if ( dy > 0 )
    {
        m_lineNumberWidget->scroll(0, dy);
    }
    m_lineNumberWidget->update(0, rect.y(), getLineNumberWidth(), rect.height());
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    m_lineNumberWidget->setGeometry(0, 0, getLineNumberWidth(), contentsRect().height());
}

void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0, 100, 100, 20));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();

    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}

void TextEditor::updateLineNumberMargin()
{
    setViewportMargins(getLineNumberWidth(), 0, 0, 0);
}

void TextEditor::initFont()
{
    setFont(QFont("Consolas", 14));
}

void TextEditor::initConnect()
{
    connect(this, &QPlainTextEdit::updateRequest, this, &TextEditor::updateLineNumber);

    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &TextEditor::updateLineNumberMargin);
}

void TextEditor::initHighlighter()
{
}

int TextEditor::getLineNumberWidth()
{
    int defalut = 22;
    defalut     = 4 + QString::number(blockCount()).length() * fontMetrics().horizontalAdvance('0');
    defalut     = qMax(22, defalut);

    return defalut;
}

QString TextEditor::getFileName() const
{
    return m_fileName;
}

LineNumberWidget::LineNumberWidget(TextEditor *editor)
    : QWidget(editor)
{
    m_editor = editor;
}

void LineNumberWidget::paintEvent(QPaintEvent *event)
{
    m_editor->lineNumberPaintEvent(event);
}
