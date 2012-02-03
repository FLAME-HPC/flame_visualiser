/*!
 * \file timedialog.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of time dialog
 */
#include "./timedialog.h"
#include "./ui_timedialog.h"

TimeDialog::TimeDialog(TimeScale * ts, QWidget *parent)
    : QDialog(parent), ui(new Ui::TimeDialog) {
    ui->setupUi(this);
    timeScale = ts;
    ui->spinBox_millisecond->setValue(ts->millisecond);
    ui->spinBox_second->setValue(ts->second);
    ui->spinBox_minute->setValue(ts->minute);
    ui->spinBox_hour->setValue(ts->hour);
    ui->spinBox_day->setValue(ts->day);
    ui->checkBox_displayTimeInVisual->setChecked(ts->displayInVisual);

    connect(ui->spinBox_millisecond, SIGNAL(valueChanged(int)),
            this, SLOT(millisecondsChanged(int)));
    connect(ui->spinBox_second, SIGNAL(valueChanged(int)),
            this, SLOT(secondsChanged(int)));
    connect(ui->spinBox_minute, SIGNAL(valueChanged(int)),
            this, SLOT(minutesChanged(int)));
    connect(ui->spinBox_hour, SIGNAL(valueChanged(int)),
            this, SLOT(hoursChanged(int)));
    connect(ui->spinBox_day, SIGNAL(valueChanged(int)),
            this, SLOT(daysChanged(int)));
    connect(ui->checkBox_displayTimeInVisual, SIGNAL(clicked(bool)),
            this, SLOT(timeInVisualChanged(bool)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

TimeDialog::~TimeDialog() {
    delete ui;
}

void TimeDialog::closeEvent(QCloseEvent *event) {
    emit(time_dialog_closed());
    QDialog::closeEvent(event);
}

void TimeDialog::timeInVisualChanged(bool b) {
    timeScale->displayInVisual = b;
}

void TimeDialog::millisecondsChanged(int i) {
    timeScale->millisecond = i;
    timeScale->calcTotalSeconds();
}

void TimeDialog::secondsChanged(int i) {
    timeScale->second = i;
    timeScale->calcTotalSeconds();
}

void TimeDialog::minutesChanged(int i) {
    timeScale->minute = i;
    timeScale->calcTotalSeconds();
}

void TimeDialog::hoursChanged(int i) {
    timeScale->hour = i;
    timeScale->calcTotalSeconds();
}

void TimeDialog::daysChanged(int i) {
    timeScale->day = i;
    timeScale->calcTotalSeconds();
}
