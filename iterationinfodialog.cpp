/*!
 * \file iterationinfodialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of iteration info dialog
 */
#include "./iterationinfodialog.h"
#include "./ui_iterationinfodialog.h"

IterationInfoDialog::IterationInfoDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::IterationInfoDialog) {
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers.append("type");
    headers.append("value");
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->verticalHeader()->hide();
    QHeaderView *headerView = ui->tableWidget->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
}

IterationInfoDialog::~IterationInfoDialog() {
    delete ui;
}

void IterationInfoDialog::update_info()
{
    /* Remove all rows */
    while(ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(0);


}
