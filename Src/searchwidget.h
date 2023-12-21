#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

    void init();

    void setSearchResult(int curIndex, int sum);

signals:
    void searchText(const QString &text, bool direction, bool reset);

private:
    QLineEdit *  searchEidt;
    QLabel *     resultLabel;
    QPushButton *btnPrevious;
    QPushButton *btnNext;
    QPushButton *btnClose;
};

#endif   // SEARCHWIDGET_H
