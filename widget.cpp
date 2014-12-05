#include "widget.h"
#include "ui_widget.h"
#include <QCheckBox>
#include "sphere.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qDebug("Widget init");
    ui->setupUi(this);

    qDebug("new Sphere");
    objectToDraw = new Sphere(0.8f, 10);
    ui->glWidget->setObject(objectToDraw);

    connect(ui->isWireframe, &QCheckBox::toggled, ui->glWidget, &GLWidget::setWireframe);
    connect(ui->isCullFace,  &QCheckBox::toggled, ui->glWidget, &GLWidget::setCullFace);
}

Widget::~Widget()
{
    delete ui;
}
