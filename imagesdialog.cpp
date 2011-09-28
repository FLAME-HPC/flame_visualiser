#include <QDebug>
#include <QFileDialog>
#include "imagesdialog.h"
#include "ui_imagesdialog.h"

ImagesDialog::ImagesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImagesDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->appendPlainText("Images saved with be listed here...");

    connect(ui->checkBox, SIGNAL(clicked(bool)), this, SLOT(takeAnimationSlot(bool)));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(updateImagesLocation(QString)));
}

ImagesDialog::~ImagesDialog()
{
    delete ui;
}

void ImagesDialog::closeEvent(QCloseEvent *event)
{
    emit( takeAnimationSignal(false) );
    emit( image_dialog_closed() );
    QDialog::closeEvent(event);
}

void ImagesDialog::on_pushButton_Close_clicked()
{
    close();
}

void ImagesDialog::sendImageAniUpdate()
{
    emit( takeAnimationSignal(ui->checkBox->isChecked()) );
}

void ImagesDialog::setLocation(QString l)
{
    ui->lineEdit->setText(l);
}

void ImagesDialog::on_pushButton_Snapshot_clicked()
{
    emit( take_snapshot() );
}

void ImagesDialog::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        default:
            event->ignore();
            break;
    }
}

void ImagesDialog::imageStatus(QString s)
{
    ui->plainTextEdit->appendPlainText(s);
}

void ImagesDialog::takeAnimationSlot(bool b)
{
    emit( takeAnimationSignal(b) );
}

void ImagesDialog::on_pushButton_Find_clicked()
{
    QString s;
    s.append(*configpath);
    s.append("/");
    s.append(ui->lineEdit->text());

    /* Provide dialog to select folder */
    QString filepath = QFileDialog::getExistingDirectory(this,
                                                    tr("Open images location"), s, QFileDialog::ShowDirsOnly);
    /* Return relative path from currentPath to location */
    QDir dir(*configpath);
    s = dir.canonicalPath();
    QDir dir2(s);
    s = dir2.relativeFilePath(filepath);
    ui->lineEdit->setText(s);
}
