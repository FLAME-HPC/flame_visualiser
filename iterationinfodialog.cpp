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
}

IterationInfoDialog::~IterationInfoDialog() {
    delete ui;
}
