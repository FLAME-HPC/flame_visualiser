#ifndef IMAGESDIALOG_H
#define IMAGESDIALOG_H

#include <QDialog>

namespace Ui {
    class ImagesDialog;
}

class ImagesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImagesDialog(QWidget *parent = 0);
    ~ImagesDialog();
    void setLocation(QString l);
    void sendImageAniUpdate();
    void setConfigPath(QString * cp) { configpath = cp; }

signals:
    void image_dialog_closed();
    void take_snapshot();
    void takeAnimationSignal(bool);
    void updateImagesLocation(QString);

public slots:
    void imageStatus(QString);
    void takeAnimationSlot(bool);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_Close_clicked();

    void on_pushButton_Snapshot_clicked();

    void on_pushButton_Find_clicked();

private:
    Ui::ImagesDialog *ui;
    QString * configpath;
};

#endif // IMAGESDIALOG_H
