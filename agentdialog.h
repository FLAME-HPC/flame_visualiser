/*!
 * \file agentdialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for agent dialog
 */
#ifndef AGENTDIALOG_H_
#define AGENTDIALOG_H_

#include <QDialog>
#include "./agent.h"

namespace Ui {
    class AgentDialog;
}

class AgentDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AgentDialog(Agent * a, QWidget *parent = 0);
    ~AgentDialog();

  private:
    Ui::AgentDialog *ui;
    Agent * agent;
};

#endif  // AGENTDIALOG_H_
