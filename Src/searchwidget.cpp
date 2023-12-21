#include "searchwidget.h"

#include <QHBoxLayout>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    init();

    setFixedSize(300, 40);
}

void SearchWidget::init()
{
    searchEidt = new QLineEdit();
    searchEidt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    resultLabel = new QLabel("无结果");
    resultLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    btnPrevious = new QPushButton("P");
    btnPrevious->setFixedSize(40, 40);
    btnPrevious->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    btnNext = new QPushButton("N");
    btnNext->setFixedSize(40, 40);
    btnNext->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    btnClose = new QPushButton("X");
    btnClose->setFixedSize(40, 40);
    btnClose->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);

    layout->addWidget(searchEidt, 1);
    layout->addWidget(resultLabel, 0);
    layout->addWidget(btnPrevious, 0);
    layout->addWidget(btnNext, 0);
    layout->addWidget(btnClose, 0);

    connect(searchEidt, &QLineEdit::textChanged, this, [=]() {
        searchText(searchEidt->text(), true, true);
    });
    connect(btnClose, &QPushButton::clicked, this, &SearchWidget::close);
    connect(btnPrevious, &QPushButton::clicked, this, [=]() {
        searchText(searchEidt->text(), false, false);
    });
    connect(btnNext, &QPushButton::clicked, this, [=]() {
        searchText(searchEidt->text(), true, false);
    });
}

void SearchWidget::setSearchResult(int curIndex, int sum)
{
    if ( sum == 0 )
    {
        resultLabel->setText("无结果");
        return;
    }
    resultLabel->setText(QString("当前%1, 总计%2").arg(curIndex + 1).arg(sum));
}
