#include "widget.h"
#include "ui_widget.h"

#include "sphere.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qDebug("Widget init");
    ui->setupUi(this);

    qDebug("new Sphere");
    objectToDraw = new Sphere(0.1f, 10);
    ui->glWidget->setObject(objectToDraw);
}

Widget::~Widget()
{
    delete ui;
}
