#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "texteditor.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->tabWidget);
    ui->tabWidget->tabBar()->setTabsClosable(true);

    QPushButton *btnMenu = new QPushButton();
    //    btnMenu->setMinimumWidth(60);
    //    btnMenu->setMaximumWidth(60);
    ui->tabWidget->setCornerWidget(btnMenu, Qt::Corner::TopLeftCorner);

    m_searchWidget = new SearchWidget(this);
    m_searchWidget->hide();

    initConnect();
    //    m_searchWidget->raise();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotTabCloseClicked(int index)
{
    // 判断是否修改，修改则提示用户进行保存
    TextEditor *editor = dynamic_cast<TextEditor *>(ui->tabWidget->widget(index));
    if ( editor == nullptr )
    {
        return;
    }

    if ( editor->document()->isModified() )
    {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "提示", "修改尚未保存，是否仍要关闭？");
        if ( btn != QMessageBox::Yes )
        {
            return;
        }
    }

    qInfo() << "close File:" << ui->tabWidget->tabBar()->tabText(index);
    ui->tabWidget->removeTab(index);
    delete editor;
}

void MainWindow::slotSearchChanged(const QString &text, bool direction, bool reset)
{
    QString search_text = text;
    if ( search_text.trimmed().isEmpty() )
    {
        //        QMessageBox::information(this, tr("Empty search field"), tr("The search field is empty."));
        return;
    }

    TextEditor *editor = dynamic_cast<TextEditor *>(ui->tabWidget->currentWidget());
    if ( editor == nullptr )
    {
        return;
    }

    QTextDocument *document = editor->document();
    QTextCursor    cur      = editor->textCursor();

    static QList<QTextCursor> highlight_cursors;
    static int                index = 0;
    if ( reset )
    {
        // 遍历搜索所有的
        cur = editor->textCursor();
        cur.clearSelection();
        cur.movePosition(QTextCursor::Start);

        highlight_cursors.clear();
        QTextCursor highlight_cursor = document->find(search_text);
        while ( !highlight_cursor.isNull() )
        {
            highlight_cursors.append(highlight_cursor);
            highlight_cursor = document->find(search_text, highlight_cursor);
        }
    }
    else
    {
        if ( direction )
        {
            index += 1;
        }
        else
        {
            index -= 1;
        }
        index = qMax(0, index);

        index = index % highlight_cursors.size();
    }

    m_searchWidget->setSearchResult(index, highlight_cursors.size());

    QList<QTextEdit::ExtraSelection> list; /* = editor->extraSelections();*/

    if ( highlight_cursors.size() > 0 && index < highlight_cursors.size() )
    {
        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(Qt::yellow);
        highlightFormat.setForeground(Qt::blue);
        QTextEdit::ExtraSelection selection;
        selection.cursor = highlight_cursors[index];
        selection.format = highlightFormat;

        list.append(selection);

        editor->setTextCursor(highlight_cursors[index]);

        editor->setExtraSelections(list);
    }
}

void MainWindow::on_actionNew_triggered()
{
    addNewTab();
}

void MainWindow::initConnect()
{
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabCloseClicked);
    connect(m_searchWidget, &SearchWidget::searchText, this, &MainWindow::slotSearchChanged);
}

void MainWindow::addNewTab(QString fileName)
{
    QString file = "";
    if ( fileName.isEmpty() )
    {
        file = "新建文本.txt";
    }
    else
    {
        QFileInfo info(fileName);
        if ( info.exists() )
        {
            file = info.fileName();
        }
    }

    TextEditor *editor = new TextEditor(fileName);
    ui->tabWidget->addTab(editor, file);
    int index = ui->tabWidget->count() - 1;
    ui->tabWidget->setCurrentIndex(index);
    ui->tabWidget->tabBar()->setTabToolTip(index, editor->getCompleteFileName());

    connect(editor, &TextEditor::modificationChanged, this, [=](bool isModify) {
        QString tabTitle = isModify ? ("*" + file) : file;
        ui->tabWidget->tabBar()->setTabText(index, tabTitle);
    });
    //    connect(eidtor->document(), &QTextDocument::modificationChanged, this, [=](bool isModify) {
    //        QString tabTitle = isModify ? ("*" + file) : file;
    //        ui->tabWidget->tabBar()->setTabText(index, tabTitle);
    //    });
    connect(editor, &TextEditor::saveFinished, this, [=]() {
        QString tabTitle = editor->getFileName();
        ui->tabWidget->tabBar()->setTabText(index, tabTitle);
        ui->tabWidget->tabBar()->setTabToolTip(index, editor->getCompleteFileName());
    });
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", QApplication::applicationDirPath());
    if ( fileName.isEmpty() )
    {
        return;
    }

    addNewTab(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    TextEditor *editor = dynamic_cast<TextEditor *>(ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    if ( editor == nullptr )
    {
        return;
    }
    editor->saveFile();
}

void MainWindow::on_actionSaveAs_triggered()
{
    TextEditor *editor = dynamic_cast<TextEditor *>(ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    if ( editor == nullptr )
    {
        return;
    }
    editor->saveAsFile();
}

void MainWindow::on_actionSearch_triggered()
{
    // 计算搜索框的位置
    QPoint topRight = mapToGlobal(this->rect().topRight());
    qDebug() << topRight;
    m_searchWidget->move(width() - m_searchWidget->width() - 50, 10);
    m_searchWidget->raise();
    m_searchWidget->show();
}
