/*!
 * \file positiondialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for position dialog
 */
#ifndef POSITIONDIALOG_H_
#define POSITIONDIALOG_H_

#include <QDialog>
#include "./agenttype.h"
#include "./position.h"
#include "./visualsettingsmodel.h"
#include "./visualsettingsitem.h"
#include "./ui_positiondialog.h"

class PositionDialog : public QDialog, public Ui::PositionDialog {
    Q_OBJECT

  public:
    PositionDialog(QList<AgentType> * ats = 0,
            VisualSettingsModel * model = 0, int r = 0, QWidget *parent = 0);
    void setPosition(Position p);
    Position getPosition();

  private:
    QList<AgentType> * agentTypes;
    VisualSettingsModel * vsm;
    Position position;
    int row;
};

#endif  // POSITIONDIALOG_H_
