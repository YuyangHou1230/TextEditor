#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "texteditor.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->tabWidget);
    ui->tabWidget->tabBar()->setTabsClosable(true);

    initConnect();
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
        if ( btn != QMessageBox::Ok )
        {
            return;
        }
    }

    qInfo() << "close File:" << ui->tabWidget->tabBar()->tabText(index);
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionNew_triggered()
{
    addNewTab();
}

void MainWindow::initConnect()
{
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabCloseClicked);
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
