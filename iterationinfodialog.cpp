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

IterationInfoDialog::IterationInfoDialog(QHash<QString, int> * atc,
                                         Dimension * ad, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::IterationInfoDialog) {
    ui->setupUi(this);

    agentTypeCounts = atc;
    agentDimension = ad;

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

void IterationInfoDialog::closeEvent(QCloseEvent *event) {
    emit(iterationInfoDialog_closed());
    QDialog::closeEvent(event);
}

void IterationInfoDialog::addRow(QString type, QString value) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    QTableWidgetItem *newItem = new QTableWidgetItem(type);
    newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableWidget->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem(value);
    newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableWidget->setItem(row, 1, newItem);
}

void IterationInfoDialog::update_info() {
    /* Remove all rows */
    while (ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(0);
    /* Update info */
    QHash<QString, int>::iterator i;
    for (i = agentTypeCounts->begin(); i != agentTypeCounts->end(); ++i) {
        addRow(QString("Agent total: %1").arg(i.key()),
               QString::number(i.value()));
    }
    /* Add agent dimensions */
    addRow("X-axis Miniumum", QString::number(agentDimension->xmin));
    addRow("X-axis Maxiumum", QString::number(agentDimension->xmax));
    addRow("Y-axis Miniumum", QString::number(agentDimension->ymin));
    addRow("Y-axis Maxiumum", QString::number(agentDimension->ymax));
    addRow("Z-axis Miniumum", QString::number(agentDimension->zmin));
    addRow("Z-axis Maxiumum", QString::number(agentDimension->zmax));
}

void IterationInfoDialog::on_buttonBox_accepted() {
    close();
}
