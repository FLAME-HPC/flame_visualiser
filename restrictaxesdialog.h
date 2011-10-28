#ifndef RESTRICTAXESDIALOG_H
#define RESTRICTAXESDIALOG_H

#include <QDialog>
#include "dimension.h"

namespace Ui {
    class RestrictAxesDialog;
}

class RestrictAxesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestrictAxesDialog(Dimension * rd, Dimension * ad, Dimension * rad, double * r, QWidget *parent = 0);
    ~RestrictAxesDialog();

signals:
    void updateRestrictDimensions(Dimension);
    void closed();

public slots:
    void updatedAgentDimensions();

private slots:
    void updateXMin(double);
    void updateXMax(double);
    void updateYMin(double);
    void updateYMax(double);
    void updateZMin(double);
    void updateZMax(double);
    void updateXMinS(int);
    void updateXMaxS(int);
    void updateYMinS(int);
    void updateYMaxS(int);
    void updateZMinS(int);
    void updateZMaxS(int);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void enableXMin(bool);
    void enableXMax(bool);
    void enableYMin(bool);
    void enableYMax(bool);
    void enableZMin(bool);
    void enableZMax(bool);
    Ui::RestrictAxesDialog *ui;
    Dimension * restrictDimension;
    Dimension * restrictAgentDimension;
    Dimension * agentDimension;
    double * ratio;
    double xStep, xStart, xEnd;
    double yStep, yStart, yEnd;
    double zStep, zStart, zEnd;
    bool xminBlock, xmaxBlock;
    bool yminBlock, ymaxBlock;
    bool zminBlock, zmaxBlock;
    double xStartK, xEndK;
    double yStartK, yEndK;
    double zStartK, zEndK;
    bool firstDim;
};

#endif // RESTRICTAXESDIALOG_H
