/*!
 * \file iterationinfodialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for iteration info dialog
 */
#ifndef ITERATIONINFODIALOG_H_
#define ITERATIONINFODIALOG_H_

#include <QDialog>
#include "./dimension.h"

namespace Ui {
class IterationInfoDialog;
}

class IterationInfoDialog : public QDialog {
    Q_OBJECT

  public:
    explicit IterationInfoDialog(QHash<QString, int> * atc, Dimension * ad,
                                 QWidget *parent = 0);
    ~IterationInfoDialog();

  signals:
    void iterationInfoDialog_closed();

  public slots:
    void update_info();

  protected:
    void closeEvent(QCloseEvent *event);

  private slots:
    void on_buttonBox_accepted();

  private:
    void addRow(QString type, QString value);
    Ui::IterationInfoDialog *ui;
    QHash<QString, int> * agentTypeCounts;
    Dimension * agentDimension;
};

#endif  // ITERATIONINFODIALOG_H_
