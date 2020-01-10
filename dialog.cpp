#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnSearch_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory("C:\\");
    this->path = dialog.getExistingDirectory();
    this->ui->tbxPath->setText(this->path);
}

void Dialog::on_buttonBox_accepted()
{
    this->path = this->ui->tbxPath->text();
}

QString Dialog::GetPath()
{
    return this->path;
}

void Dialog::on_buttonBox_rejected()
{
    exit(0);
}
