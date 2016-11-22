/*!
 * \file timedialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for time dialog
 */
#ifndef TIMEDIALOG_H_
#define TIMEDIALOG_H_

#include <QDialog>
#include "./timescale.h"

namespace Ui {
    class TimeDialog;
}

class TimeDialog : public QDialog {
    Q_OBJECT

  public:
    explicit TimeDialog(TimeScale * ts, QWidget *parent = 0);
    ~TimeDialog();

  signals:
    void time_dialog_closed();

  protected:
    void closeEvent(QCloseEvent *event);

  private slots:
    void millisecondsChanged(int);
    void secondsChanged(int);
    void minutesChanged(int);
    void hoursChanged(int);
    void daysChanged(int);
    void timeInVisualChanged(bool);

  private:
    Ui::TimeDialog *ui;
    TimeScale * timeScale;
};

#endif  // TIMEDIALOG_H_
