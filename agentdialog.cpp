#include "agentdialog.h"
#include "ui_agentdialog.h"

AgentDialog::AgentDialog(Agent * a, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentDialog)
{
    ui->setupUi(this);
    agent = a;

    setWindowTitle("Agent Memory");

    ui->tableWidget_Variables->setColumnCount(2);
    ui->tableWidget_Variables->setRowCount(a->tags.size());
    QStringList headers;
    headers.append("name");
    headers.append("value");
    ui->tableWidget_Variables->setHorizontalHeaderLabels(headers);
    ui->tableWidget_Variables->verticalHeader()->hide();
    QHeaderView *headerView = ui->tableWidget_Variables->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget_Variables->setSelectionBehavior(QAbstractItemView::SelectRows);

    for(int i = 0; i < a->tags.size(); i++)
    {
        int row = i;
        int column = 0;
        QTableWidgetItem *newItem = new QTableWidgetItem(a->tags[i]);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget_Variables->setItem(row, column, newItem);
        column = 1;
        newItem = new QTableWidgetItem(a->values[i]);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget_Variables->setItem(row, column, newItem);
    }

    //resize(ui->tableWidget_Variables->size());

    connect(ui->pushButton_Okay, SIGNAL(clicked()), this, SLOT(close()));
}

AgentDialog::~AgentDialog()
{
    delete ui;
}
