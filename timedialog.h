#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>
#include "timescale.h"

namespace Ui {
    class TimeDialog;
}

class TimeDialog : public QDialog
{
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

#endif // TIMEDIALOG_H
