/*!
 * \file iterationinfodialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of iteration info dialog
 */
#include <QDebug>
#include "./iterationinfodialog.h"
#include "./ui_iterationinfodialog.h"

IterationInfoDialog::IterationInfoDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::IterationInfoDialog) {
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers.append("Agent type");
    headers.append("Total");
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->verticalHeader()->hide();
    QHeaderView *headerView = ui->tableWidget->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
}

IterationInfoDialog::~IterationInfoDialog() {
    delete ui;
}

void IterationInfoDialog::closeEvent(QCloseEvent *event) {
    emit(iterationInfoDialog_closed());
    QDialog::closeEvent(event);
}

void IterationInfoDialog::update_info(QHash<QString, int> * agentTypeCounts)
{
    /* Remove all rows */
    while(ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(0);
    /* Update info */
    int row = 0;
    int column;
    QHash<QString, int>::iterator i;
    for (i = agentTypeCounts->begin(); i != agentTypeCounts->end(); ++i) {
        // qDebug() << i.key() << i.value();
        ui->tableWidget->insertRow(row);
        column = 0;
        QTableWidgetItem *newItem = new QTableWidgetItem(i.key());
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, column, newItem);
        column = 1;
        newItem = new QTableWidgetItem(QString::number(i.value()));
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, column, newItem);
        row++;
    }


}

void IterationInfoDialog::on_buttonBox_accepted()
{
    close();
}
