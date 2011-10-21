#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QtGui/QMouseEvent>
#include <QTextEdit>
#include <QDir>
#include <math.h>
#include "zeroxmlreader.h"
#include "visualsettingsmodel.h"
#include "visualsettingsitem.h"
#include "configxmlreader.h"
#include "agenttypedelegate.h"
#include "shapedelegate.h"
#include "colourdelegate.h"
#include "positiondelegate.h"
#include "conditiondelegate.h"
#include "graphsettingsitem.h"
#include "graphsettingsmodel.h"
#include "graphwidget.h"
#include "enableddelegate.h"
#include "graphdelegate.h"

/** \fn MainWindow::MainWindow(QWidget *parent)
 *  \brief Setup the main window.
 *  \param *parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* Initialise variables */
    itLocked = false;
    fileOpen = false;
    images_dialog_open = false;
    time_dialog_open = false;
    timeString = "";
    iteration = 0;
    configPath = "";
    configName = "";
    timeScale = new TimeScale();
    visual_settings_model = new VisualSettingsModel();
    connect(visual_settings_model, SIGNAL(ruleUpdated(int)), this, SLOT(ruleUpdated(int)));
    graph_settings_model = new GraphSettingsModel(&agents);
    connect(graph_settings_model, SIGNAL(plotGraphChanged(GraphSettingsItem*,QString,QString)), this, SLOT(plotGraphChanged(GraphSettingsItem*,QString,QString)));

    /* Setup User Interface */
    ui->setupUi(this);
    this->setWindowTitle("FLAME Visualiser - ");
    enableInterface(false); /* Disable UI to start */

    /* Setup 3D OpenGL visual window */
    //ui->pushButton_OpenCloseVisual->setText("Open visual");
    opengl_window_open  = false;

    /* Setup tables in UI */
    /* Set tableViewVisual to stretch columns to table size */
    QHeaderView *headerView = ui->tableViewVisual->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewVisual->verticalHeader()->hide();
    ui->tableViewVisual->setModel(visual_settings_model);
    ui->tableViewVisual->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Set tableViewVisual delegates for each column below */
    ui->tableViewVisual->setItemDelegateForColumn(0, new AgentTypeDelegate(&agentTypes));
    ui->tableViewVisual->setItemDelegateForColumn(1, new ConditionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(2, new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(3, new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(4, new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(5, new ShapeDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(6, new ColourDelegate); /* Only to draw the cell */
    /* Connect signals that affect tableViewVisual */
    connect(ui->tableViewVisual, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getColourVisual(QModelIndex))); /* Handles the color choosing */
    connect(ui->pushButton_AddAgentType, SIGNAL(clicked()), this, SLOT(addRule()));
    connect(ui->pushButton_DeleteAgentType, SIGNAL(clicked()), this, SLOT(deleteRule()));
    /* Set tableViewGraph to stretch columns to table size */
    headerView = ui->tableViewGraph->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewGraph->verticalHeader()->hide();
    ui->tableViewGraph->setModel(graph_settings_model);
    ui->tableViewGraph->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Set tableViewGraph delegates for each column below */
    ui->tableViewGraph->setItemDelegateForColumn(0, new GraphDelegate(graph_settings_model));
    ui->tableViewGraph->setItemDelegateForColumn(2, new AgentTypeDelegate(&agentTypes));
    ui->tableViewGraph->setItemDelegateForColumn(3, new ConditionDelegate(&agentTypes, graph_settings_model));
    ui->tableViewGraph->setItemDelegateForColumn(4, new ColourDelegate); /* Only to draw the cell */
    /* Connect signals that affect tableViewGraph */
    connect(ui->tableViewGraph, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getColourGraph(QModelIndex))); /* Handles the color choosing */
    connect(ui->pushButton_AddPlot, SIGNAL(clicked()), this, SLOT(addPlot()));
    connect(ui->pushButton_DeletePlot, SIGNAL(clicked()), this, SLOT(deletePlot()));
    connect(ui->tableViewGraph, SIGNAL(clicked(QModelIndex)), this, SLOT(enabledGraph(QModelIndex))); /* Handles the enabling of graphs */
    /* Connect signals of time scale */
    connect(ui->checkBox_timeScale, SIGNAL(clicked(bool)), this, SLOT(enableTimeScale(bool)));
    /* Connect signals of iteration buttons */
    connect(ui->pushButton_ForwardIteration, SIGNAL(clicked()), this, SLOT(increment_iteration()));
    connect(ui->pushButton_BackIteration, SIGNAL(clicked()), this, SLOT(decrement_iteration()));
    /* Connect signals of the menu items */
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(save_as_config_file()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open_config_file()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save_config_file()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(save_as_config_file()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close_config_file()));

    /* Try and find .flamevisualisersettings and load last known model and iteration number */
    findLoadSettings();
}

/** \fn MainWindow::~MainWindow()
 *  \brief Destroy the main window.
 */
MainWindow::~MainWindow()
{
    delete ui;

    /* Output settings */
    QFile file(".flamevisualisersettings");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

    if(fileOpen)
    {
        QString configFile;
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);

        QTextStream out(&file);
        out << "true|" << configFile << "|" << iteration << "\n";
    }
    else
    {
        QTextStream out(&file);
        out << "false|\n";
    }

    file.close();
}

/** \fn MainWindow::findLoadSettings()
 *  \brief Try and find .flamevisualisersettings and load last known model and iteration number.
 */
void MainWindow::findLoadSettings()
{
    /* Try and open file, return if fail */
    QFile file(".flamevisualisersettings");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return;

    /* Read in each line */
    QTextStream in(&file);
    if(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split("|"); /* Split using | into a string list */
        /* If the first value is true */
        if(QString::compare(list.at(0),"true") == 0)
        {
            /* Try and read in the config file */
            readConfigFile(list.at(1), list.at(2).toInt());
        }
    }
}

/** \fn MainWindow::visual_window_closed()
 *  \brief When the visual window is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::visual_window_closed()
{
    disconnect(this, SIGNAL(updateVisual()), visual_window, SLOT(updateGL()));
    disconnect(visual_window, SIGNAL(increase_iteration()), this, SLOT(increment_iteration()));
    disconnect(visual_window, SIGNAL(decrease_iteration()), this, SLOT(decrement_iteration()));
    disconnect(visual_window, SIGNAL(visual_window_closed()), this, SLOT(visual_window_closed()));
    disconnect(this, SIGNAL(iterationLoaded()), visual_window, SLOT(iterationLoaded()));
    disconnect(this, SIGNAL(animate()), visual_window, SLOT(animate()));
    disconnect(this, SIGNAL(takeSnapshotSignal()), visual_window, SLOT(takeSnapshot()));
    disconnect(visual_window, SIGNAL(imageStatus(QString)), this, SLOT(imageStatusSlot(QString)));
    disconnect(this, SIGNAL(takeAnimationSignal(bool)), visual_window, SIGNAL(takeAnimation(bool)));
    disconnect(this, SIGNAL(updateImagesLocationSignal(QString)), visual_window, SLOT(updateImagesLocation(QString)));
    disconnect(this, SIGNAL(stopAnimation()), visual_window, SLOT(stopAnimation()));

    opengl_window_open = false;
    ui->pushButton_OpenCloseVisual->setText("Open Visual Window");
}

/** \fn MainWindow::image_dialog_closed()
 *  \brief When the image dialog is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::image_dialog_closed()
{
    disconnect(images_dialog, SIGNAL(image_dialog_closed()), this, SLOT(image_dialog_closed()));
    disconnect(images_dialog, SIGNAL(take_snapshot()), this, SLOT(takeSnapshotSlot()));
    disconnect(this, SIGNAL(imageStatusSignal(QString)), images_dialog, SLOT(imageStatus(QString)));
    disconnect(images_dialog, SIGNAL(takeAnimationSignal(bool)), this, SLOT(takeAnimationSlot(bool)));
    disconnect(images_dialog, SIGNAL(updateImagesLocation(QString)), this, SLOT(updateImagesLocationSlot(QString)));
    images_dialog_open = false;
    ui->pushButton_ImageSettings->setText("Open Image Settings");
}

/** \fn MainWindow::time_dialog_closed()
 *  \brief When the time dialog is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::time_dialog_closed()
{
    disconnect(time_dialog, SIGNAL(time_dialog_closed()), this, SLOT(time_dialog_closed()));
    time_dialog_open = false;
    ui->pushButton_timeScale->setText("Open Time Settings");
    calcTimeScale();
}

/** \fn MainWindow::graph_window_closed(QString graphName)
 *  \brief When a graph window is closed, close all windows with the same graph name.
 *  \param graphName The graph name.
 */
void MainWindow::graph_window_closed(QString graphName)
{
    closeGraphWindows(graphName);
}

/** \fn MainWindow::enableInterface(bool enable)
 *  \brief Enable and disable the user interface when a file is open or closed.
 *  \param enable True for enabled, false for disabled.
 */
void MainWindow::enableInterface(bool enable)
{
    /* Enable/Disable UI */
    emit( ui->groupBox->setEnabled(enable) );
    emit( ui->groupBox_2->setEnabled(enable) );
    emit( ui->groupBox_3->setEnabled(enable) );
    emit( ui->groupBox_4->setEnabled(enable) );
    /* Enable/Disable menu items */
    emit( ui->actionClose->setEnabled(enable) );
    emit( ui->actionSave->setEnabled(enable) );
    emit( ui->actionSave_As->setEnabled(enable) );
}

/** \fn MainWindow::addPlot()
 *  \brief Add a new plot to the graph model.
 */
void MainWindow::addPlot()
{
    graph_settings_model->addPlot();
}

/** \fn MainWindow::createGraphWindow(GraphWidget *graph_window)
 *  \brief Create a new graph window, setup.
 */
void MainWindow::createGraphWindow(GraphWidget *graph_window)
{
    graphs.append(graph_window);
    graph_window->updateData(0);
    graph_window->resize(720,380);
    graph_window->show();
    connect(graph_window, SIGNAL(increase_iteration()), this, SLOT(increment_iteration()));
    connect(graph_window, SIGNAL(decrease_iteration()), this, SLOT(decrement_iteration()));
    connect(graph_window, SIGNAL(graph_window_closed(QString)), this, SLOT(graph_window_closed(QString)));
}
/** \fn MainWindow::closeGraphWindows(QString graphName)
 *  \brief Set all rules with the graph name as disabled and close and remove all associated graph windows.
 *  \param graphName The graph name
 */
void MainWindow::closeGraphWindows(QString graphName)
{
    graph_settings_model->setDisabled(graphName);

    /* Check open graphs */
    for(int i = 0; i < graphs.count(); i++)
    {
        if(QString::compare(graphs[i]->getGraph(),graphName) == 0)
        {
            graphs[i]->close();
            graphs.removeAt(i);
            i--;
        }
    }
}

/** \fn MainWindow::enabledGraph(QModelIndex index)
 *  \brief Show or hide a graph window when the enabled cell of the graph table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::enabledGraph(QModelIndex index)
{
    bool enabled;

    if(index.column() == 5)
    {
        enabled = !(graph_settings_model->getPlot(index.row())->getEnable());
        graph_settings_model->switchEnabled(index);
        if(enabled)
        {
            GraphWidget * gw = new GraphWidget(&agents);
            gw->setGraph(graph_settings_model->getPlot(index.row())->getGraph());
            QList<GraphSettingsItem *> subplots = graph_settings_model->getPlotsInSameGraph(index);
            for(int i=0; i < subplots.count(); i++) gw->addPlot(subplots.at(i));
            createGraphWindow(gw);
        }
        else
        {
            closeGraphWindows(graph_settings_model->getPlot(index.row())->getGraph());
        }
    }
}

/** \fn MainWindow::plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph, QString newGraph)
 *  \brief Handle a graph name change of a plot rule.
 *  \param gsi The GraphSettingsItem.
 *  \param oldGraph The old graph name
 *  \param newGraph The new graph name
 */
void MainWindow::plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph, QString newGraph)
{
    for(int i = 0; i < graphs.count(); i++)
    {
        qDebug() << i << graphs[i]->getGraph() << newGraph;
        if(QString::compare(graphs[i]->getGraph(),oldGraph) == 0)
        {
            int rc = graphs[i]->removePlot(gsi);
            if(rc == 0)
            {
                closeGraphWindows(oldGraph);
            }
            else graphs[i]->repaint();
        }
        if(QString::compare(graphs[i]->getGraph(),newGraph) == 0)
        {
            graphs[i]->addPlot(gsi);
            graphs[i]->updateData(iteration);
            graphs[i]->repaint();
        }
    }
}

/** \fn MainWindow::getColourVisual(QModelIndex index)
 *  \brief Show the colour dialog when the colour cell of the visual table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::getColourVisual(QModelIndex index)
{
    if(index.column() == 6)
    {
        colourIndex = index;
        colour = qVariantValue<QColor>(index.data());
        QColorDialog *colourDialog = new QColorDialog(this);
        colourDialog->setOption(QColorDialog::ShowAlphaChannel);
        colourDialog->setCurrentColor(colour);
        connect(colourDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(colourChanged(QColor)));

        int rc = colourDialog->exec();
        if(rc == QDialog::Rejected)
        {
            visual_settings_model->setData(index, qVariantFromValue(colour));
        }
        delete colourDialog;
    }
}

/** \fn MainWindow::colourChanged(QColor c)
 *  \brief Update the visual settings model with any change in colour.
 *  \param c The new colour.
 */
void MainWindow::colourChanged(QColor c)
{
    visual_settings_model->setData(colourIndex, qVariantFromValue(c));
}

/** \fn MainWindow::getColourGraph(QModelIndex index)
 *  \brief Show the colour dialog when the colour cell of the graph table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::getColourGraph(QModelIndex index)
{
    if(index.column() == 4)
    {
        QColor colour = QColorDialog::getColor( qVariantValue<QColor>(index.data()) );

        if(colour.isValid())
        {
            graph_settings_model->setData(index, qVariantFromValue(colour));
        }
    }
}

/** \fn MainWindow::addRule()
 *  \brief Connect the add rule button with the data model.
 */
void MainWindow::addRule()
{
    visual_settings_model->addRule();
}

/** \fn MainWindow::deleteRule()
 *  \brief Connect the delete rule button with the data model and keep deleting all selected rows.
 */
void MainWindow::deleteRule()
{
     QModelIndexList indexList = ui->tableViewVisual->selectionModel()->selectedRows();

     while(indexList.count() > 0)
     {
         visual_settings_model->deleteRule(indexList.at(0));
         indexList = ui->tableViewVisual->selectionModel()->selectedRows();
     }
}

/** \fn MainWindow::deletePlot()
 *  \brief Connect the delete plot button with the data model and keep deleting all selected rows.
 */
void MainWindow::deletePlot()
{
     QModelIndexList indexList = ui->tableViewGraph->selectionModel()->selectedRows();

     while(indexList.count() > 0)
     {
         graph_settings_model->deletePlot(indexList.at(0));
         indexList = ui->tableViewGraph->selectionModel()->selectedRows();
     }
}

/** \fn MainWindow::on_pushButton_LocationFind_clicked()
 *  \brief Provide a dialog to select the folder location of the 0.xml and make the path relative to the config xml location.
 */
void MainWindow::on_pushButton_LocationFind_clicked()
{
    QString s = configPath;
    s.append("/");
    s.append(ui->lineEdit_ResultsLocation->text());
    /* Provide dialog to select folder */
    QString filepath =
            QFileDialog::getExistingDirectory(this, tr("Open output location"), s, QFileDialog::ShowDirsOnly);
    /* Return relative path from currentPath to location */
    QDir dir(configPath);
    QString s1 = dir.canonicalPath();
    QDir dir2(s1);
    s = dir2.relativeFilePath(filepath);
    ui->lineEdit_ResultsLocation->setText(s);

    readZeroXML(1); /* Read in new agent data */
    readZeroXML(0); /* Create a list of the different agent types */
}

/** \fn MainWindow::on_pushButton_OpenCloseVisual_clicked()
 *  \brief Switch between the visual window being shown or hidden.
 */
void MainWindow::on_pushButton_OpenCloseVisual_clicked()
{
    if(opengl_window_open == false)
    {
        // Set ratio to be 1
        ratio = 1.0;
        // Read in agents with model dimensions
        readZeroXML(1);
        // Calculate model to opengl dimension ratio
        calcPositionRatio();
        // Reread agents with opengl dimension using new ratio
        readZeroXML(1);

        visual_window = new GLWidget();
        visual_window->setAttribute(Qt::WA_DeleteOnClose); // Make the window destory on close rather than hide
        visual_window->resize(800,600);
        visual_window->update_agents(&agents);
        visual_window->set_rules(visual_settings_model);
        visual_window->setIteration(&iteration);
        visual_window->setConfigPath(&configPath);
        visual_window->setTimeDisplayed(&(timeScale->displayInVisual));
        visual_window->setTimeString(&timeString);

        /* Connect signals between MainWindow and visual_window */
        connect(this, SIGNAL(updateVisual()), visual_window, SLOT(updateGL()));
        connect(visual_window, SIGNAL(increase_iteration()), this, SLOT(increment_iteration()));
        connect(visual_window, SIGNAL(decrease_iteration()), this, SLOT(decrement_iteration()));
        connect(visual_window, SIGNAL(visual_window_closed()), this, SLOT(visual_window_closed()));
        connect(this, SIGNAL(iterationLoaded()), visual_window, SLOT(iterationLoaded()));
        connect(this, SIGNAL(animate()), visual_window, SLOT(animate()));
        connect(this, SIGNAL(takeSnapshotSignal()), visual_window, SLOT(takeSnapshot()));
        connect(visual_window, SIGNAL(imageStatus(QString)), this, SLOT(imageStatusSlot(QString)));
        connect(this, SIGNAL(takeAnimationSignal(bool)), visual_window, SLOT(takeAnimation(bool)));
        connect(this, SIGNAL(updateImagesLocationSignal(QString)), visual_window, SLOT(updateImagesLocation(QString)));
        connect(this, SIGNAL(stopAnimation()), visual_window, SLOT(stopAnimation()));

        visual_window->show();
        visual_window->setFocus();
        ui->pushButton_OpenCloseVisual->setText("Close Visual Window");
        opengl_window_open = true;
    }
    else
    {
        visual_window->close();
    }
}

/** \fn MainWindow::readZeroXML(int flag)
 *  \brief Read the 0 xml defined by the current iteration.
 *  \param flag 1 means clear the agent data and read in new agent data, 0 means just read the different agent types into a list.
 *  \return False if the file could not be found.
 */
bool MainWindow::readZeroXML(int flag)
{
    if(itLocked) return false;

    itLocked = true;

    QString fileName;
    fileName.append(configPath);
    fileName.append("/");
    fileName.append(ui->lineEdit_ResultsLocation->text());
    fileName.append("/");
    fileName.append(QString().number(iteration));
    fileName.append(".xml");

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->label_5->setText("Not found");
        itLocked = false;
        return false;
    }

    if(flag == 1)
    {
        // used by graphs
        agents.clear();

        // used by visual
        for(int i = 0; i < visual_settings_model->rowCount(); i++)
        {
            visual_settings_model->getRule(i)->agents.clear();
        }
    }

    ZeroXMLReader reader(&agents, &agentTypes, visual_settings_model, &ratio);
    if (!reader.read(&file, flag))
    {
         ui->label_5->setText("Not found");
         emit( animate() );
         itLocked = false;
         return false;
    }
    else
    {
        itLocked = false;
         if(opengl_window_open) emit( iterationLoaded() );
         ui->label_5->setText("Found");
    }

    return true;
}

/** \fn MainWindow::on_spinBox_valueChanged(int arg1)
 *  \brief Change the iteration number to the number of the spin box.
 *  \param arg1 The value of the spin box
 */
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    iteration = arg1;
    readZeroXML(1); /* Read in new agent data */
    if(ui->checkBox_timeScale->isChecked()) calcTimeScale();
}

/** \fn MainWindow::increment_iteration()
 *  \brief Increment the iteration number, read in new agent data, set the spin box value, update all graphs.
 */
void MainWindow::increment_iteration()
{
    iteration++;
    if(readZeroXML(1) == false)
    {
        if(checkDirectoryForNextIteration(iteration, 0))
        {
            readZeroXML(1);
        }
        else
        {
            iteration--;
            emit( stopAnimation() );
        }
    }
    ui->spinBox->setValue(iteration);
    if(ui->checkBox_timeScale->isChecked()) calcTimeScale();

    for(int i = 0; i < graphs.count(); i++)
    {
        graphs.at(i)->updateData(iteration);
    }
}

/** \fn MainWindow::decrement_iteration()
 *  \brief Decrement the iteration number, set the spin box value, read in new agent data.
 */
void MainWindow::decrement_iteration()
{
    if(iteration > 0) iteration--;
    if(readZeroXML(1) == false)
    {
        if(checkDirectoryForNextIteration(iteration, 1))
        {
            readZeroXML(1);
        }
        else
        {
            iteration++;
        }
    }
    ui->spinBox->setValue(iteration);
    if(ui->checkBox_timeScale->isChecked()) calcTimeScale();
}

/** \fn MainWindow::open_config_file()
 *  \brief Provide a dialog to select a config file to open.
 */
void MainWindow::open_config_file()
{
    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open model"), "", tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     readConfigFile(fileName, 0);
}

bool MainWindow::checkDirectoryForNextIteration(int it, int flag)
{
    QString fileName;
    fileName.append(configPath);
    fileName.append("/");
    fileName.append(ui->lineEdit_ResultsLocation->text());
    fileName.append("/");

    QDir dir(fileName);
    QStringList filters;
    filters << "*.xml";
    dir.setNameFilters(filters);
    QStringList list = dir.entryList();
    QList<int> ilist;
    for(int i=0; i < list.size(); i++)
    {
        QString f = list.at(i);
        f.chop(4);
        bool s;
        int i = f.toInt(&s);
        if(s) ilist.append(i);
    }
    qSort(ilist);
    for(int i = 0; i < ilist.size(); i++)
    {
        if((ilist.at(i) > it && flag == 0) || (ilist.at(ilist.size()-1-i) < it && flag == 1))
        {
            if(flag == 0) iteration = ilist.at(i);
            if(flag == 1) iteration = ilist.at(ilist.size()-1-i);
            return true;
        }
    }

    return false;
}

/** \fn MainWindow::readConfigFile(QString fileName, int it)
 *  \brief Read in a config file.
 *  \param fileName The file name
 *  \param it The iteration number to be set
 */
void MainWindow::readConfigFile(QString fileName, int it)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("FLAME Visualiser"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    emit visual_settings_model->deleteRules();
    emit graph_settings_model->deletePlots();
    agents.clear();
    agentTypes.clear();
    graphs.clear();
    enableTimeScale(false);
    timeScale->reset();

    ConfigXMLReader reader(visual_settings_model, graph_settings_model, &resultsData, timeScale, &ratio);
    if (!reader.read(&file))
    {
        QMessageBox::warning(this, tr("flame visualiser"),
         tr("Parse error in file %1 at line %2, column %3:\n%4").arg(fileName).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.errorString()));
    }
    else
    {
        if(opengl_window_open) visual_window->reset_camera();

        /* Setup time scale */
        timeScale->calcTotalSeconds();
        enableTimeScale(timeScale->enabled);

        QFileInfo fileInfo(file.fileName());
        configPath = fileInfo.absolutePath();
        configName = fileInfo.fileName();

        QString wtitle;
        wtitle.append("FLAME Visualiser - ");
        wtitle.append(file.fileName());
        this->setWindowTitle(wtitle);

        iteration = it;
        ui->spinBox->setValue(iteration);

        ui->lineEdit_ResultsLocation->setText(resultsData);
        readZeroXML(1);
        readZeroXML(0);

        /* For each new plot create a graph window */
        for(int i=0; i < graph_settings_model->rowCount(); i++)
        {

        }

        enableInterface(true);
        fileOpen = true;
    }
    file.close();
}

/** \fn MainWindow::enableTimeScale(bool b)
 *  \brief Enable or disable the time scale UI.
 *  \param b The enable or disable flag
 */
void MainWindow::enableTimeScale(bool b)
{
    ui->checkBox_timeScale->setChecked(b);
    ui->lineEdit_timeScale->setEnabled(b);
    ui->pushButton_timeScale->setEnabled(b);

    QPalette palet;
    if(b) palet.setColor(ui->checkBox_timeScale->foregroundRole(),QColor(0,0,0));
    else palet.setColor(ui->checkBox_timeScale->foregroundRole(),QColor(120,120,120));
    ui->checkBox_timeScale->setPalette(palet);

    if(b) calcTimeScale();
    else emit( ui->lineEdit_timeScale->setText("") );
}

/** \fn MainWindow::new_config_file()
 *  \brief Open a new config file.
 */
void MainWindow::new_config_file()
{
    enableTimeScale(false);
    save_as_config_file();
}

/** \fn MainWindow::save_as_config_file()
 *  \brief Provide a dialog to save a config file.
 */
void MainWindow::save_as_config_file()
{
    QString configFile = "";
    if(fileOpen)
    {
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);
    }

    QString fileName =
             QFileDialog::getSaveFileName(this, tr("Save config File"),
                                          configFile,
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text))
     {
         QMessageBox::warning(this, tr("FLAME Visualiser"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     enableInterface(true);

     writeConfigXML(&file);
}

/** \fn MainWindow::save_config_file()
 *  \brief Save a config file.
 */
void MainWindow::save_config_file()
{
    if(fileOpen == false) save_as_config_file();
    else
    {
        QString configFile;
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);

        QFile file(configFile);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("FLAME Visualiser"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(configFile)
                                 .arg(file.errorString()));
            return;
        }

        writeConfigXML(&file);
    }
}

/** \fn MainWindow::close_config_file()
 *  \brief Close the config file and disable the user interface.
 */
void MainWindow::close_config_file()
{
    this->setWindowTitle("FLAME Visualiser - ");
    visual_settings_model->deleteRules();
    graph_settings_model->deletePlots();
    ui->lineEdit_ResultsLocation->setText("");
    agents.clear();
    agentTypes.clear();
    enableInterface(false);
    fileOpen = false;
    timeScale->reset();
    enableTimeScale(false);
    iteration = 0;
    configPath = "";
    configName = "";
}

/** \fn MainWindow::writeConfigXML(QFile * file)
 *  \brief Write a config to an xml file.
 *  \param file The QFile to write to.
 *  \return False if error, true if worked.
 */
bool MainWindow::writeConfigXML(QFile * file)
{
    fileOpen = true;

    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("flame_visualiser_config");
    stream.writeAttribute("version", "0.1");

    stream.writeStartElement("resultsData"); // resultsData
    stream.writeTextElement("directory", ui->lineEdit_ResultsLocation->text());
    stream.writeEndElement(); // resultsData

    stream.writeStartElement("timeScale"); // timeScale
    if(ui->checkBox_timeScale->isChecked()) stream.writeTextElement("enable", "true");
    else stream.writeTextElement("enable", "false");
    stream.writeTextElement("milliseconds", QString("%1").arg(timeScale->millisecond));
    stream.writeTextElement("seconds", QString("%1").arg(timeScale->second));
    stream.writeTextElement("minutes", QString("%1").arg(timeScale->minute));
    stream.writeTextElement("hours", QString("%1").arg(timeScale->hour));
    stream.writeTextElement("days", QString("%1").arg(timeScale->day));
    if(timeScale->displayInVisual) stream.writeTextElement("displayTimeInVisual", "true");
    else stream.writeTextElement("displayTimeInVisual", "false");
    stream.writeEndElement(); // timeScale

    stream.writeStartElement("visual");
    stream.writeTextElement("ratio", QString("%1").arg(ratio));
    stream.writeStartElement("rules");
    for(int i = 0; i < this->visual_settings_model->rowCount(); i++)
    {
        VisualSettingsItem *vsitem = visual_settings_model->getRule(i);

        stream.writeStartElement("rule"); // rule
        stream.writeTextElement("agentType", vsitem->agentType());
        stream.writeStartElement("condition"); // condition
        if(vsitem->condition().enable) stream.writeTextElement("enable", "true");
        else stream.writeTextElement("enable", "false");
        stream.writeStartElement("lhs"); // lhs
        stream.writeTextElement("type", "variable");
        stream.writeTextElement("variable", vsitem->condition().variable);
        stream.writeEndElement(); // lhs
        stream.writeTextElement("operator", vsitem->condition().op);
        stream.writeStartElement("rhs"); // rhs
        stream.writeTextElement("type", "value");
        stream.writeTextElement("value", QString("%1").arg(vsitem->condition().value));
        stream.writeEndElement(); // rhs
        stream.writeEndElement(); // condition
        stream.writeStartElement("x"); // x
        if(vsitem->x().useVariable) stream.writeTextElement("useVariable", "true");
        else stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->x().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(vsitem->x().opValue));
        stream.writeEndElement(); // x
        stream.writeStartElement("y"); // y
        if(vsitem->y().useVariable) stream.writeTextElement("useVariable", "true");
        else stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->y().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(vsitem->y().opValue));
        stream.writeEndElement(); // y
        stream.writeStartElement("z"); // z
        if(vsitem->z().useVariable) stream.writeTextElement("useVariable", "true");
        else stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->z().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(vsitem->z().opValue));
        stream.writeEndElement(); // z
        stream.writeStartElement("shape"); // shape
        stream.writeTextElement("object", vsitem->shape().getShape());
        if(QString::compare(vsitem->shape().getShape(),"sphere") == 0)
            stream.writeTextElement("quality", QString("%1").arg(vsitem->shape().getQuality()));
        stream.writeTextElement("dimension", QString("%1").arg(vsitem->shape().getDimension()));
        if(vsitem->shape().getUseVariable()) stream.writeTextElement("useVariable", "true");
        else stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("dimensionVariable", QString("%1").arg(vsitem->shape().getDimensionVariable()));
        stream.writeTextElement("dimensionY", QString("%1").arg(vsitem->shape().getDimensionY()));
        if(vsitem->shape().getUseVariableY()) stream.writeTextElement("useVariableY", "true");
        else stream.writeTextElement("useVariableY", "false");
        stream.writeTextElement("dimensionVariableY", QString("%1").arg(vsitem->shape().getDimensionVariableY()));
        stream.writeTextElement("dimensionZ", QString("%1").arg(vsitem->shape().getDimensionZ()));
        if(vsitem->shape().getUseVariableZ()) stream.writeTextElement("useVariableZ", "true");
        else stream.writeTextElement("useVariableZ", "false");
        stream.writeTextElement("dimensionVariableZ", QString("%1").arg(vsitem->shape().getDimensionVariableZ()));
        stream.writeEndElement(); // shape
        stream.writeStartElement("colour"); // colour
        stream.writeTextElement("r", QString("%1").arg(vsitem->colour().red()));
        stream.writeTextElement("g", QString("%1").arg(vsitem->colour().green()));
        stream.writeTextElement("b", QString("%1").arg(vsitem->colour().blue()));
        stream.writeTextElement("a", QString("%1").arg(vsitem->colour().alpha()));
        stream.writeEndElement(); // colour
        stream.writeEndElement(); // rule

    }
    stream.writeEndElement(); // rules
    stream.writeEndElement(); // visual

    stream.writeStartElement("graph");
    for(int i = 0; i < this->graph_settings_model->rowCount(); i++)
    {
        GraphSettingsItem * gsitem = graph_settings_model->getPlot(i);

        stream.writeStartElement("plot"); // plot
        stream.writeTextElement("graphNumber", gsitem->getGraph());
        stream.writeStartElement("xAxis"); // xAxis
        stream.writeTextElement("type", gsitem->getXaxis());
        stream.writeEndElement(); // xAxis
        stream.writeStartElement("yAxis"); // yAxis
        stream.writeTextElement("type", "agent");
        stream.writeTextElement("agentType", gsitem->getYaxis());
        stream.writeEndElement(); // yAxis
        stream.writeStartElement("condition"); // condition
        if(gsitem->condition().enable) stream.writeTextElement("enable", "true");
        else stream.writeTextElement("enable", "false");
        stream.writeStartElement("lhs"); // lhs
        stream.writeTextElement("type", "variable");
        stream.writeTextElement("variable", gsitem->condition().variable);
        stream.writeEndElement(); // lhs
        stream.writeTextElement("operator", gsitem->condition().op);
        stream.writeStartElement("rhs"); // rhs
        stream.writeTextElement("type", "value");
        stream.writeTextElement("value", QString("%1").arg(gsitem->condition().value));
        stream.writeEndElement(); // rhs
        stream.writeEndElement(); // condition
        stream.writeStartElement("colour"); // colour
        stream.writeTextElement("r", QString("%1").arg(gsitem->getColour().red()));
        stream.writeTextElement("g", QString("%1").arg(gsitem->getColour().green()));
        stream.writeTextElement("b", QString("%1").arg(gsitem->getColour().blue()));
        stream.writeEndElement(); // colour
        stream.writeEndElement(); // plot
    }
    stream.writeEndElement(); // graph

    stream.writeEndDocument();
    file->close();
    if(file->error())
    {
        return false;
    }

    /* Update config file path variables */
    QFileInfo fileInfo(file->fileName());
    configPath = fileInfo.absolutePath();
    configName = fileInfo.fileName();
    /* Update the main window title with the file path */
    QString wtitle;
    wtitle.append("FLAME Visualiser - ");
    wtitle.append(file->fileName());
    this->setWindowTitle(wtitle);

    return true;
}

/** \fn MainWindow::keyPressEvent(QKeyEvent* event)
 *  \brief Handle any key press events.
 *  \param event The key press event
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Z:
            decrement_iteration();
            break;
        case Qt::Key_X:
            increment_iteration();
            break;
        case Qt::Key_A:
            emit( animate() );
            break;
        default:
            event->ignore();
            break;
    }
}

/** \fn MainWindow::calcPositionRatio()
 *  \brief Automatically work out a good ratio to use to view agents visually.
 */
void MainWindow::calcPositionRatio()
{
    double smallest = 0.0;
    double largest = 0.0;

    for(int j = 0; j < visual_settings_model->rowCount(); j++)
    {
        for(int i= 0; i < visual_settings_model->getRule(j)->agents.count(); i++)
        {
            if(smallest > visual_settings_model->getRule(j)->agents.at(i).x) smallest = visual_settings_model->getRule(j)->agents.at(i).x;
            if(smallest > visual_settings_model->getRule(j)->agents.at(i).y) smallest = visual_settings_model->getRule(j)->agents.at(i).y;
            if(largest  < visual_settings_model->getRule(j)->agents.at(i).x) largest  = visual_settings_model->getRule(j)->agents.at(i).x;
            if(largest  < visual_settings_model->getRule(j)->agents.at(i).y) largest  = visual_settings_model->getRule(j)->agents.at(i).y;
        }
    }

    if(smallest < 0.0 && largest < -smallest) ratio = 1.0 / -smallest;
    else ratio = 1.0 / largest;
}

/** \fn MainWindow::on_pushButton_Animate_clicked()
 *  \brief Emit animate signal when the animate button is pressed.
 */
void MainWindow::on_pushButton_Animate_clicked()
{
    emit( animate() );
}

/** \fn MainWindow::on_pushButton_ImageSettings_clicked()
 *  \brief Open or close the image settings dialog.
 */
void MainWindow::on_pushButton_ImageSettings_clicked()
{
    if(images_dialog_open == false)
    {
        images_dialog_open = true;
        images_dialog = new ImagesDialog();

        connect(images_dialog, SIGNAL(image_dialog_closed()), this, SLOT(image_dialog_closed()));
        connect(images_dialog, SIGNAL(take_snapshot()), this, SLOT(takeSnapshotSlot()));
        connect(this, SIGNAL(imageStatusSignal(QString)), images_dialog, SLOT(imageStatus(QString)));
        connect(images_dialog, SIGNAL(takeAnimationSignal(bool)), this, SLOT(takeAnimationSlot(bool)));
        connect(images_dialog, SIGNAL(updateImagesLocation(QString)), this, SLOT(updateImagesLocationSlot(QString)));

        images_dialog->setConfigPath(&configPath);
        images_dialog->setLocation(ui->lineEdit_ResultsLocation->text());
        images_dialog->sendImageAniUpdate();
        images_dialog->show();
        ui->pushButton_ImageSettings->setText("Close Image Settings");
    }
    else
    {
        images_dialog->close();
    }
}

/** \fn MainWindow::takeSnapshotSlot()
 *  \brief Emit take snapshot signal if snapshot button is pressed.
 */
void MainWindow::takeSnapshotSlot()
{
    if(opengl_window_open) emit( takeSnapshotSignal() );
}

void MainWindow::imageStatusSlot(QString s)
{
    if(images_dialog_open) emit( imageStatusSignal(s) );
}

void MainWindow::takeAnimationSlot(bool b)
{
    if(opengl_window_open) emit( takeAnimationSignal(b) );
}

void MainWindow::updateImagesLocationSlot(QString s)
{
    if(opengl_window_open) emit( updateImagesLocationSignal(s) );
}

void MainWindow::ruleUpdated(int /*row*/)
{
    // Reread agents using updated rules
    if(opengl_window_open) readZeroXML(1);
}

void MainWindow::on_actionQuit_triggered()
{
    if(opengl_window_open) visual_window->close();
    close();
}

void MainWindow::on_actionAbout_triggered()
{
   QTextEdit *about=new QTextEdit(this);
   about->setWindowFlags(Qt::Dialog);
   about->setReadOnly(true);
   about->setAutoFormatting(QTextEdit::AutoNone);

   QString aboutText;
   aboutText.append("<h1>FLAME Visualiser</h1>");
   aboutText.append("<h3>Simon Coakley</h3>");
   aboutText.append("<h2>Version 2</h2>");
   aboutText.append("<h3>Changelog</h3>");
   /* Add new release notes here */
   aboutText.append("<h4>Version 2 (released tba)</h4><ul><li>Ability to change the quality of spheres being drawn</li>");
        aboutText.append("<li>Ability to add a time scale that can be displayed</li>");
        aboutText.append("<li>Ability to change the near clip plane</li>");
        aboutText.append("<li>Ability to pick an agent and its memory be displayed</li>");
        aboutText.append("<li>Ability to move the centre of the visual scene with the mouse</li>");
        aboutText.append("<li>Program searches for iterations if they are not sequential</li>");
        aboutText.append("<li>Program remembers the model last open and the iteration number</li>");
        aboutText.append("<li>Addition of a simple help dialog</li>");
        aboutText.append("<li>Bugfix: cubes and points were being drawn more than once</li></ul>");
   aboutText.append("<h4>Version 1 (released 2011-10-07)</h4><ul><li>Beta first release</li></ul>");

   about->setGeometry(50,50,600,400);
   about->insertHtml(aboutText);
   about->moveCursor(QTextCursor::Start);
   about->show();
}

void MainWindow::on_pushButton_timeScale_clicked()
{
    if(time_dialog_open == false)
    {
        time_dialog_open = true;
        time_dialog = new TimeDialog(timeScale);

        connect(time_dialog, SIGNAL(time_dialog_closed()), this, SLOT(time_dialog_closed()));

        time_dialog->show();

        ui->pushButton_timeScale->setText("Close Time Settings");
    }
    else
    {
        time_dialog->close();
    }
}

void MainWindow::calcTimeScale()
{
    double tsecs;
    double tmsecs = timeScale->millisecond*iteration/1000.0;
    tmsecs = modf(tmsecs, &tsecs);
    int totalseconds = (int)tsecs;
    totalseconds += timeScale->totalseconds*iteration;

    QString ts;
    int days = (int)(totalseconds/86400);
    int hours = (int)((totalseconds%86400)/3600);
    int minutes = (int)(((totalseconds%86400)%3600)/60);
    int seconds = (int)(((totalseconds%86400)%3600)%60);
    if(days > 0 || timeScale->lowestScale == 4)
    {
        ts.append(QString("%1 day").arg(days));
        if(days > 1) ts.append("s");
        ts.append(" ");
    }
    if(hours > 0 || timeScale->lowestScale < 4)
    {
        ts.append(QString("%1 hour").arg(hours));
        if(hours > 1) ts.append("s");
        ts.append(" ");
    }
    if(minutes > 0 || timeScale->lowestScale < 3)
    {
        ts.append(QString("%1 min").arg(minutes));
        if(minutes > 1) ts.append("s");
        ts.append(" ");
    }
    if(timeScale->lowestScale == 1) ts.append(QString("%1 second").arg(seconds));
    else if(timeScale->lowestScale == 0) ts.append(QString("%1 seconds").arg(seconds+tmsecs));

    timeString = ts;

    emit( ui->lineEdit_timeScale->setText(ts) );
}

void MainWindow::on_actionHelp_triggered()
{
    QTextEdit *help=new QTextEdit(this);
    help->setWindowFlags(Qt::Dialog);
    help->setReadOnly(true);
    help->setAutoFormatting(QTextEdit::AutoNone);
    QString helpText;
    helpText.append("<h1>FLAME Visualiser Help</h1>");
    helpText.append("<h4>Sphere Quality Setting</h4>");
    helpText.append("The quality of spheres being drawn affects the speed of the visualiser. To make the visualiser faster reduce the quality of the spheres. Also if any large spheres look blocky then increase their quality.");
    helpText.append("<ul><li>8  - low quality</li>");
    helpText.append("<li>16 - medium quality</li>");
    helpText.append("<li>32 - high quality</li></ul>");
    helpText.append("<h4>Change the Near Clip Plane</h4>The near clip plane can be changed by holding down the C key and the right mouse button and moving the mouse up and down.");
    helpText.append("<h4>Pick an Agent</h4>An agent can be picked and its memory displayed by holding down the P key and left mouse clicking on an agent.");
    helpText.append("<h4>Move the Centre of the Visual Scene</h4>The scene can be shifted up,down,left and right by holding down the Spacebar and the left mouse button and moving the mouse.");

    help->setGeometry(50,50,600,400);
    help->insertHtml(helpText);
    help->moveCursor(QTextCursor::Start);
    help->show();
}
