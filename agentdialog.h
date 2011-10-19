#ifndef AGENTDIALOG_H
#define AGENTDIALOG_H

#include <QDialog>
#include "agent.h"

namespace Ui {
    class AgentDialog;
}

class AgentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgentDialog(Agent * a, QWidget *parent = 0);
    ~AgentDialog();

private:
    Ui::AgentDialog *ui;
    Agent * agent;
};

#endif // AGENTDIALOG_H
