#ifndef CONDITIONDIALOG_H
#define CONDITIONDIALOG_H

#include <QDialog>
#include "agenttype.h"
#include "condition.h"
#include "ui_conditiondialog.h"

class ConditionDialog : public QDialog, public Ui::ConditionDialog
{
    Q_OBJECT

public:
    ConditionDialog(QList<AgentType> * ats = 0, QString agentType = "", int r = 0, QWidget *parent = 0);
    void setCondition(Condition c);
    Condition getCondition();

signals:
    void setVariableComboBox(int i);
    void setOpComboBox(int i);
    void setValueSpinBox(double d);

private slots:
    void updateEnable(bool c);

private:
    QList<AgentType> * agentTypes;
    Condition condition;
    int row;
    QStringList operators;
};

#endif // CONDITIONDIALOG_H
