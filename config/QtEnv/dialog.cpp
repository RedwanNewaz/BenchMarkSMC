#include "dialog.h"
#include "ui_dialog.h"
//#include "displaybase.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    disp = new DisplayBase(ui->simulation, ui->performance, ui->progressBar);

    QString msg = "The robot is represented as blue dot. \n";
    msg += "Its mission is to sample red rocks and moves on the east \n";
    msg += "Once it samples a rock it becomes black \n";
    msg += "It does not get any reward for sampling black rocks \n";
    msg += " The blue line is discounted reward and red line is undiscounted reward.";
    ui->msgBox->setPlainText(msg);







}

Dialog::~Dialog()
{
    delete ui;
    delete disp;
}
