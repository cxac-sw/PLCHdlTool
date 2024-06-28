#include "dialogaboutqt.h"
#include "ui_dialogaboutqt.h"

#pragma execution_character_set("utf-8")

DialogAboutQt::DialogAboutQt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutQt)
{
    ui->setupUi(this);
    setWindowTitle("关于Qt");
}

DialogAboutQt::~DialogAboutQt()
{
    delete ui;
}

void DialogAboutQt::on_pushButton_Comfirm_clicked()
{
    this->close();
}
