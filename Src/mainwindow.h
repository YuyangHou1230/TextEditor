#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "searchwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotTabCloseClicked(int index);

    void slotSearchChanged(const QString &text, bool direction, bool reset);

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionSearch_triggered();

private:
    void initConnect();

    void addNewTab(QString fileName = "");

private:
    Ui::MainWindow *ui;

    SearchWidget *m_searchWidget;
};
#endif   // MAINWINDOW_H
