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

namespace Ui {
class IterationInfoDialog;
}

class IterationInfoDialog : public QDialog {
    Q_OBJECT

  public:
    explicit IterationInfoDialog(QWidget *parent = 0);
    ~IterationInfoDialog();

  public slots:
    void update_info();

  private:
    Ui::IterationInfoDialog *ui;
};

#endif  // ITERATIONINFODIALOG_H_
