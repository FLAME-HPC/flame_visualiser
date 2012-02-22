/*!
 * \file mainwindow.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of main window
 */
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QtGui/QMouseEvent>
#include <QTextEdit>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <math.h>
#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include "./zeroxmlreader.h"
#include "./visualsettingsmodel.h"
#include "./visualsettingsitem.h"
#include "./configxmlreader.h"
#include "./agenttypedelegate.h"
#include "./shapedelegate.h"
#include "./colourdelegate.h"
#include "./positiondelegate.h"
#include "./conditiondelegate.h"
#include "./graphsettingsitem.h"
#include "./graphsettingsmodel.h"
#include "./graphwidget.h"
#include "./enableddelegate.h"
#include "./graphdelegate.h"

/*! \brief Setup the main window.
 *  \param *parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    /* Setup User Interface */
    ui->setupUi(this);
    this->setWindowTitle("FLAME Visualiser - ");
    enableInterface(false); /* Disable UI to start */
    /* Set the application icon (for linux) as mac and win
     * have platform-dependent techniques (see .pro file).
     */
    #ifdef Q_WS_X11
    setWindowIcon(QIcon("flame-v.png"));
    #endif
    /* Initialise variables */
    itLocked = false;
    fileOpen = false;
    images_dialog_open = false;
    time_dialog_open = false;
    restrict_dimension_open = false;
    iterationInfo_dialog_open = false;
    timeString = "";
    iteration = 0;
    delayTime = 0;
    configPath = "";
    configName = "";
    timeScale = new TimeScale();
    visual_settings_model = new VisualSettingsModel();
    connect(visual_settings_model, SIGNAL(ruleUpdated(int)),
            this, SLOT(ruleUpdated(int)));
    graph_settings_model = new GraphSettingsModel(&agents);
    connect(graph_settings_model,
            SIGNAL(plotGraphChanged(GraphSettingsItem*, QString, QString)),
            this, SLOT(
                plotGraphChanged(GraphSettingsItem*, QString, QString)));
    /* Visual window camera variables */
    xrotate = 0.0;
    yrotate = 0.0;
    xmove = 0.0;
    ymove = 0.0;
    zmove = -3.0;
    xoffset = 0.0;
    yoffset = 0.0;
    zoffset = 0.0;
    orthoZoom = 1.0;
    restrictDimension = new Dimension();
    agentDimension = new Dimension();
    restrictAgentDimension = new Dimension();
    on_actionLines_triggered();

    /* Setup 3D OpenGL visual window */
    // ui->pushButton_OpenCloseVisual->setText("Open visual");
    opengl_window_open  = false;
    /* Set update viewpoint button to be false */
    ui->pushButton_updateViewpoint->setEnabled(false);
    on_actionPerspective_triggered();

    /* Setup tables in UI */
    /* Set tableViewVisual to stretch columns to table size */
    QHeaderView *headerView = ui->tableViewVisual->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewVisual->verticalHeader()->hide();
    ui->tableViewVisual->setModel(visual_settings_model);
    ui->tableViewVisual->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Set tableViewVisual delegates for each column below */
    ui->tableViewVisual->setItemDelegateForColumn(0,
            new AgentTypeDelegate(&agentTypes));
    ui->tableViewVisual->setItemDelegateForColumn(1,
            new ConditionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(2,
            new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(3,
            new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(4,
            new PositionDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(5,
            new ShapeDelegate(&agentTypes, visual_settings_model));
    ui->tableViewVisual->setItemDelegateForColumn(6,
            new ColourDelegate); /* Only to draw the cell */
    /* Connect signals that affect tableViewVisual */
    connect(ui->tableViewVisual, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(getColourVisual(QModelIndex)));
    /* Handles the enabling of visual rules */
    connect(ui->tableViewVisual, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enabledRule(QModelIndex)));
    connect(ui->pushButton_AddAgentType, SIGNAL(clicked()),
            this, SLOT(addRule()));
    connect(ui->pushButton_DeleteAgentType, SIGNAL(clicked()),
            this, SLOT(deleteRule()));
    /* Set tableViewGraph to stretch columns to table size */
    headerView = ui->tableViewGraph->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewGraph->verticalHeader()->hide();
    ui->tableViewGraph->setModel(graph_settings_model);
    ui->tableViewGraph->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* Set tableViewGraph delegates for each column below */
    ui->tableViewGraph->setItemDelegateForColumn(0,
            new GraphDelegate(graph_settings_model));
    ui->tableViewGraph->setItemDelegateForColumn(2,
            new AgentTypeDelegate(&agentTypes));
    ui->tableViewGraph->setItemDelegateForColumn(3,
            new ConditionDelegate(&agentTypes, graph_settings_model));
    ui->tableViewGraph->setItemDelegateForColumn(4,
            new ColourDelegate); /* Only to draw the cell */
    /* Connect signals that affect tableViewGraph */
    /* Handles the color choosing */
    connect(ui->tableViewGraph, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(getColourGraph(QModelIndex)));
    connect(ui->pushButton_AddPlot, SIGNAL(clicked()),
            this, SLOT(addPlot()));
    connect(ui->pushButton_DeletePlot, SIGNAL(clicked()),
            this, SLOT(deletePlot()));
    /* Handles the enabling of graphs */
    connect(ui->tableViewGraph, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enabledGraph(QModelIndex)));
    /* Connect signals of time scale */
    connect(ui->checkBox_timeScale, SIGNAL(clicked(bool)),
            this, SLOT(enableTimeScale(bool)));
    /* Connect signals of iteration buttons */
    connect(ui->pushButton_ForwardIteration, SIGNAL(clicked()),
            this, SLOT(increment_iteration()));
    connect(ui->pushButton_BackIteration, SIGNAL(clicked()),
            this, SLOT(decrement_iteration()));
    /* Connect signals of the menu items */
    connect(ui->actionNew, SIGNAL(triggered()),
            this, SLOT(new_config_file()));
    connect(ui->actionOpen, SIGNAL(triggered()),
            this, SLOT(open_config_file()));
    connect(ui->actionSave, SIGNAL(triggered()),
            this, SLOT(save_config_file()));
    connect(ui->actionSave_As, SIGNAL(triggered()),
            this, SLOT(save_as_config_file()));
    connect(ui->actionClose, SIGNAL(triggered()),
            this, SLOT(close_config_file()));

    /* Try and find .flamevisualisersettings and load
     * last known model and iteration number */
    findLoadSettings();
}

/*! \brief Destroy the main window.
 */
MainWindow::~MainWindow() {
    delete ui;

    /* Output settings */
    QFile file(".flamevisualisersettings");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
     return;

    if (fileOpen) {
        QString configFile;
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);

        QTextStream out(&file);
        out << "true|" << configFile << "|" << iteration << "\n";
    } else {
        QTextStream out(&file);
        out << "false|\n";
    }

    file.close();
}

void MainWindow::closeEvent(QCloseEvent */*event*/) {
    on_actionQuit_triggered();
}

/*! \brief Try and find .flamevisualisersettings and load last known model and iteration number.
 */
void MainWindow::findLoadSettings() {
    /* Try and open file, return if fail */
    QFile file(".flamevisualisersettings");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return;

    /* Read in each line */
    QTextStream in(&file);
    if (!in.atEnd()) {
        QString line = in.readLine();
        /* Split using | into a string list */
        QStringList list = line.split("|");
        /* If the first value is true */
        if (QString::compare(list.at(0), "true") == 0) {
            /* Try and read in the config file */
            readConfigFile(list.at(1), list.at(2).toInt());
        }
    }
}

/*! \brief When the visual window is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::visual_window_closed() {
    disconnect(this, SIGNAL(updateVisual()),
            visual_window, SLOT(updateGL()));
    disconnect(visual_window, SIGNAL(increase_iteration()),
            this, SLOT(increment_iteration()));
    disconnect(visual_window, SIGNAL(decrease_iteration()),
            this, SLOT(decrement_iteration()));
    disconnect(visual_window, SIGNAL(visual_window_closed()),
            this, SLOT(visual_window_closed()));
    disconnect(this, SIGNAL(iterationLoaded()),
            visual_window, SLOT(iterationLoaded()));
    disconnect(this, SIGNAL(startAnimation()),
            visual_window, SLOT(startAnimation()));
    disconnect(this, SIGNAL(stopAnimation()),
            visual_window, SLOT(stopAnimation()));
    disconnect(visual_window, SIGNAL(signal_startAnimation()),
            this, SLOT(slot_startAnimation()));
    disconnect(visual_window, SIGNAL(signal_stopAnimation()),
            this, SLOT(slot_stopAnimation()));
    disconnect(this, SIGNAL(takeSnapshotSignal()),
            visual_window, SLOT(takeSnapshot()));
    disconnect(visual_window, SIGNAL(imageStatus(QString)),
            this, SLOT(imageStatusSlot(QString)));
    disconnect(this, SIGNAL(takeAnimationSignal(bool)),
            visual_window, SIGNAL(takeAnimation(bool)));
    disconnect(this, SIGNAL(updateImagesLocationSignal(QString)),
            visual_window, SLOT(updateImagesLocation(QString)));
    disconnect(this, SIGNAL(stopAnimation()),
            visual_window, SLOT(stopAnimation()));
    disconnect(this, SIGNAL(restrictAxes(bool)),
            visual_window, SLOT(restrictAxes(bool)));
    disconnect(this, SIGNAL(updateDelayTime(int)),
            visual_window, SIGNAL(updateDelayTime(int)));

    opengl_window_open = false;
    ui->pushButton_OpenCloseVisual->setText("Open Visual Window");
    animation = false;
    ui->pushButton_Animate->setText("Start Animation - A");
    ui->pushButton_Animate->setEnabled(false);
    /* Set update viewpoint button to be false */
    ui->pushButton_updateViewpoint->setEnabled(false);
}

/*! \brief When the image dialog is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::image_dialog_closed() {
    disconnect(images_dialog, SIGNAL(image_dialog_closed()),
            this, SLOT(image_dialog_closed()));
    disconnect(images_dialog, SIGNAL(take_snapshot()),
            this, SLOT(takeSnapshotSlot()));
    disconnect(this, SIGNAL(imageStatusSignal(QString)),
            images_dialog, SLOT(imageStatus(QString)));
    disconnect(images_dialog, SIGNAL(takeAnimationSignal(bool)),
            this, SLOT(takeAnimationSlot(bool)));
    disconnect(images_dialog, SIGNAL(updateImagesLocation(QString)),
            this, SLOT(updateImagesLocationSlot(QString)));
    images_dialog_open = false;
    ui->pushButton_ImageSettings->setText("Open Image Settings");
}

/*! \brief When the time dialog is closed, disconnect all signal/slots and set variables.
 */
void MainWindow::time_dialog_closed() {
    disconnect(time_dialog, SIGNAL(time_dialog_closed()),
            this, SLOT(time_dialog_closed()));
    time_dialog_open = false;
    ui->pushButton_timeScale->setText("Open Time Settings");
    calcTimeScale();
}

/*! \brief When the iteration info dialog is closed, disconnect all signal/slots
 *  and set variables.
 */
void MainWindow::iterationInfoDialog_closed() {
    disconnect(this, SIGNAL(updateIterationInfoDialog()),
            iterationInfo_dialog, SLOT(update_info()));
    disconnect(iterationInfo_dialog, SIGNAL(iterationInfoDialog_closed()),
            this, SLOT(iterationInfoDialog_closed()));
    iterationInfo_dialog_open = false;
}

void MainWindow::restrict_axes_closed() {
    restrict_dimension_open = false;
    disconnect(restrictAxesDialog, SIGNAL(closed()),
            this, SLOT(restrict_axes_closed()));
    disconnect(this, SIGNAL(updatedAgentDimension()),
            restrictAxesDialog, SLOT(updatedAgentDimensions()));
    emit(restrictAxes(false));
}

/*! \brief When a graph window is closed, close all windows with the same graph name.
 *  \param graphName The graph name.
 */
void MainWindow::graph_window_closed(QString graphName) {
    closeGraphWindows(graphName);
}

/*! \brief Enable and disable the user interface when a file is open or closed.
 *  \param enable True for enabled, false for disabled.
 */
void MainWindow::enableInterface(bool enable) {
    /* Enable/Disable UI */
    emit(ui->groupBox->setEnabled(enable));
    emit(ui->groupBox_2->setEnabled(enable));
    emit(ui->groupBox_3->setEnabled(enable));
    emit(ui->groupBox_4->setEnabled(enable));
    /* Enable/Disable menu items */
    emit(ui->actionClose->setEnabled(enable));
    emit(ui->actionSave->setEnabled(enable));
    emit(ui->actionSave_As->setEnabled(enable));
}

/*! \brief Add a new plot to the graph model.
 */
void MainWindow::addPlot() {
    graph_settings_model->addPlot();
}

/*! \brief Create a new graph window, setup.
 */
void MainWindow::createGraphWindow(GraphWidget *graph_window) {
    graphs.append(graph_window);
    graph_window->updateData(iteration);
    graph_window->resize(720, 380);
    graph_window->show();
    connect(graph_window, SIGNAL(increase_iteration()),
            this, SLOT(increment_iteration()));
    connect(graph_window, SIGNAL(decrease_iteration()),
            this, SLOT(decrement_iteration()));
    connect(graph_window, SIGNAL(graph_window_closed(QString)),
            this, SLOT(graph_window_closed(QString)));
}
/*! \brief Set all rules with the graph name as disabled and close and
 *         remove all associated graph windows.
 *  \param graphName The graph name
 */
void MainWindow::closeGraphWindows(QString graphName) {
    graph_settings_model->setDisabled(graphName);

    /* Check open graphs */
    for (int i = 0; i < graphs.count(); i++) {
        if (QString::compare(graphs[i]->getGraph(), graphName) == 0) {
            graphs[i]->close();
            graphs.removeAt(i);
            i--;
        }
    }
}

/*! \brief Enable or disable a visual rule when the enabled cell
 *  of the visual rule table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::enabledRule(QModelIndex index) {
    /* If the enabled column */
    if (index.column() == 7) {
        /* Switch the enabled value */
        visual_settings_model->switchEnabled(index);
        /* Reread the iteration to apply the changes */
        readZeroXML();
    }
}

/*! \brief Show or hide a graph window when the enabled cell of the graph table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::enabledGraph(QModelIndex index) {
    bool enabled;

    if (index.column() == 5) {
        enabled = !(graph_settings_model->getPlot(index.row())->getEnable());
        graph_settings_model->switchEnabled(index);
        if (enabled) {
            GraphWidget * gw = new GraphWidget(&agents, &graph_style);
            gw->setGraph(graph_settings_model->
                    getPlot(index.row())->getGraph());
            QList<GraphSettingsItem *> subplots =
                    graph_settings_model->getPlotsInSameGraph(index);
            for (int i = 0; i < subplots.count(); i++)
                gw->addPlot(subplots.at(i));
            createGraphWindow(gw);
        } else {
            closeGraphWindows(graph_settings_model->
                    getPlot(index.row())->getGraph());
        }
    }
}

/*! \brief Handle a graph name change of a plot rule.
 *  \param gsi The GraphSettingsItem.
 *  \param oldGraph The old graph name
 *  \param newGraph The new graph name
 */
void MainWindow::plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph,
        QString newGraph) {
    for (int i = 0; i < graphs.count(); i++) {
        qDebug() << i << graphs[i]->getGraph() << newGraph;
        if (QString::compare(graphs[i]->getGraph(), oldGraph) == 0) {
            int rc = graphs[i]->removePlot(gsi);
            if (rc == 0) {
                closeGraphWindows(oldGraph);
            } else {
                graphs[i]->repaint();
            }
        }
        if (QString::compare(graphs[i]->getGraph(), newGraph) == 0) {
            graphs[i]->addPlot(gsi);
            graphs[i]->updateData(iteration);
            graphs[i]->repaint();
        }
    }
}

/*! \brief Show the colour dialog when the colour cell of the visual table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::getColourVisual(QModelIndex index) {
    if (index.column() == 6) {
        colourIndex = index;
        colour = qVariantValue<QColor>(index.data());
        QColor colourSave = colour;
        QColorDialog *colourDialog = new QColorDialog(this);
        colourDialog->setOption(QColorDialog::ShowAlphaChannel);
        colourDialog->setCurrentColor(colour);
        connect(colourDialog, SIGNAL(currentColorChanged(QColor)),
                this, SLOT(colourChanged(QColor)));

        int rc = colourDialog->exec();
        if (rc == QDialog::Rejected) {
        visual_settings_model->setData(index, qVariantFromValue(colourSave));
        }
        delete colourDialog;
    }
}

/*! \brief Update the visual settings model with any change in colour.
 *  \param c The new colour.
 */
void MainWindow::colourChanged(QColor c) {
    visual_settings_model->setData(colourIndex, qVariantFromValue(c));
}

/*! \brief Show the colour dialog when the
 *         colour cell of the graph table is clicked.
 *  \param index The index of the cell clicked.
 */
void MainWindow::getColourGraph(QModelIndex index) {
    if (index.column() == 4) {
        QColor colour = QColorDialog::getColor(
                qVariantValue<QColor>(index.data()));

        if (colour.isValid()) {
            graph_settings_model->setData(index, qVariantFromValue(colour));
        }
    }
}

/*! \brief Connect the add rule button with the data model.
 */
void MainWindow::addRule() {
    visual_settings_model->addRule();
}

/*! \brief Connect the delete rule button with the data model and
 *         keep deleting all selected rows.
 */
void MainWindow::deleteRule() {
     QModelIndexList indexList =
             ui->tableViewVisual->selectionModel()->selectedRows();

     while (indexList.count() > 0) {
         visual_settings_model->deleteRule(indexList.at(0));
         indexList = ui->tableViewVisual->selectionModel()->selectedRows();
     }
}

/*! \brief Connect the delete plot button with the data model and
 *         keep deleting all selected rows.
 */
void MainWindow::deletePlot() {
     QModelIndexList indexList =
             ui->tableViewGraph->selectionModel()->selectedRows();

     while (indexList.count() > 0) {
         graph_settings_model->deletePlot(indexList.at(0));
         indexList = ui->tableViewGraph->selectionModel()->selectedRows();
     }
}

/** \brief Provide a dialog to select the folder location of the 0.xml
 *         and make the path relative to the config xml location.
 */
void MainWindow::on_pushButton_LocationFind_clicked() {
    QString s = configPath;
    s.append("/");
    s.append(ui->lineEdit_ResultsLocation->text());
    /* Provide dialog to select folder */
    QString filepath =
            QFileDialog::getExistingDirectory(this,
                    tr("Select results data location..."),
                    s, QFileDialog::ShowDirsOnly);

    if (filepath.isEmpty())
        return;

    /* Return relative path from currentPath to location */
    QDir dir(configPath);
    QString s1 = dir.canonicalPath();
    QDir dir2(s1);
    s = dir2.relativeFilePath(filepath);
    ui->lineEdit_ResultsLocation->setText(s);

    // tryAndReadInAgentTypes();
    readZeroXML(); /* Read in new agent data */
}

/*! \brief Switch between the visual window being shown or hidden.
 */
void MainWindow::on_pushButton_OpenCloseVisual_clicked() {
    if (opengl_window_open == false) {
        /* Calculate viewpoint */
        resetVisualViewpoint();

        visual_window = new GLWidget(&xrotate, &yrotate, &xmove, &ymove, &zmove,
                restrictDimension, &orthoZoom);
        // Make the window destroy on close rather than hide
        visual_window->setAttribute(Qt::WA_DeleteOnClose);
        visual_window->resize(800, 600);
        visual_window->update_agents(&agents);
        visual_window->set_rules(visual_settings_model);
        visual_window->setIteration(&iteration);
        visual_window->setConfigPath(&configPath);
        visual_window->setTimeScale(timeScale);
        visual_window->setTimeString(&timeString);
        visual_window->setDimension(visual_dimension);

        /* Connect signals between MainWindow and visual_window */
        connect(this, SIGNAL(updateVisual()),
                visual_window, SLOT(updateGL()));
        connect(visual_window, SIGNAL(increase_iteration()),
                this, SLOT(increment_iteration()));
        connect(visual_window, SIGNAL(decrease_iteration()),
                this, SLOT(decrement_iteration()));
        connect(visual_window, SIGNAL(visual_window_closed()),
                this, SLOT(visual_window_closed()));
        connect(this, SIGNAL(iterationLoaded()),
                visual_window, SLOT(iterationLoaded()));
        connect(this, SIGNAL(startAnimation()),
                visual_window, SLOT(startAnimation()));
        connect(this, SIGNAL(stopAnimation()),
                visual_window, SLOT(stopAnimation()));
        connect(visual_window, SIGNAL(signal_startAnimation()),
                this, SLOT(slot_startAnimation()));
        connect(visual_window, SIGNAL(signal_stopAnimation()),
                this, SLOT(slot_stopAnimation()));
        connect(this, SIGNAL(takeSnapshotSignal()),
                visual_window, SLOT(takeSnapshot()));
        connect(visual_window, SIGNAL(imageStatus(QString)),
                this, SLOT(imageStatusSlot(QString)));
        connect(this, SIGNAL(takeAnimationSignal(bool)),
                visual_window, SLOT(takeAnimation(bool)));
        connect(this, SIGNAL(updateImagesLocationSignal(QString)),
                visual_window, SLOT(updateImagesLocation(QString)));
        connect(this, SIGNAL(stopAnimation()),
                visual_window, SLOT(stopAnimation()));
        connect(this, SIGNAL(restrictAxes(bool)),
                visual_window, SLOT(restrictAxes(bool)));
        connect(this, SIGNAL(updateDelayTime(int)),
                visual_window, SLOT(updateDelayTime(int)));

        if (restrict_dimension_open) emit( restrictAxes(true) );
        visual_window->show();
        visual_window->setFocus();
        ui->pushButton_OpenCloseVisual->setText("Close Visual Window");
        opengl_window_open = true;
        /* Set update viewpoint button to be false */
        ui->pushButton_updateViewpoint->setEnabled(true);
        ui->pushButton_Animate->setEnabled(true);
    } else {
        visual_window->close();
    }
}

/*! \brief Read the 0 xml defined by the current iteration.
 *  \return False if the file could not be found.
 */
int MainWindow::readZeroXML() {
    if (itLocked) return 0;

    itLocked = true;

    QString fileName;
    fileName.append(configPath);
    fileName.append("/");
    fileName.append(ui->lineEdit_ResultsLocation->text());
    fileName.append("/");
    fileName.append(QString().number(iteration));
    fileName.append(".xml");

    // qDebug() << "Opening file: " << fileName;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        // ui->spinBox->setValue(iteration);
        ui->label_5->setText(QString("! Error opening %1.xml").
                    arg(QString().number(iteration)));
        itLocked = false;
        return -1;
    }

    // used by graphs
    agents.clear();
    // used by iteration info dialog
    QHash<QString, int>::iterator i;
    for (i = agentTypeCounts.begin(); i != agentTypeCounts.end(); ++i) {
        i.value() = 0;
     // qDebug() << i.key() << ": " << i.value();
    }

    // used by visual
    for (int i = 0; i < visual_settings_model->rowCount(); i++) {
        visual_settings_model->getRule(i)->agents.clear();
    }

    ZeroXMLReader reader(&agents, &agentTypes, visual_settings_model, &ratio,
            agentDimension, &stringAgentTypes, &agentTypeCounts,
            &xoffset, &yoffset, &zoffset);
    if (!reader.read(&file)) {
        // ui->spinBox->setValue(iteration);
        ui->label_5->setText(
                    QString("! Error reading %1.xml").
                    arg(QString().number(iteration)));


        /* Make the path to the file look pretty */
         QDir dir(fileName);
         QString filePath = dir.canonicalPath();

         QMessageBox::warning(this, "FLAME Visualiser",
          tr("Cannot parse iteration file %1 at line %2, column %3:\n%4").arg(
                      filePath).arg(reader.lineNumber()).arg(
                      reader.columnNumber()).arg(reader.errorString()));
        itLocked = false;
         return -2;
    } else {
        itLocked = false;
         if (opengl_window_open) emit(iterationLoaded());
         ui->label_5->setText(
                 QString("Read %1.xml").arg(QString().number(iteration)));
    }

    if (iterationInfo_dialog_open) {
        emit(updateIterationInfoDialog());
    }

    return 1;
}

/*! \brief Change the iteration number to the number of the spin box.
 *  \param arg1 The value of the spin box
 */
void MainWindow::on_spinBox_valueChanged(int arg1) {
    // qDebug() << "on_spinBox_valueChanged" << arg1;
    if (iteration != arg1) {
        iteration = arg1;
        int rc = readZeroXML(); /* Read in new agent data */
        if (rc == 1) if (ui->checkBox_timeScale->isChecked()) calcTimeScale();
    }
}

/*! \brief Increment the iteration number, read in new agent data, set the spin box value, update all graphs.
 */
void MainWindow::increment_iteration() {
    int rc;
    /* increase iteration number */
    iteration++;
    /* try and read the iteration file
     * the parameter 1 means try and read in the agent data */
    rc = readZeroXML();
    /* return codes
       0 - itLocked is true
      -1 - error opening file
      -2 - error reading file
       1 - success
      */
    if (rc == -1) {  // Can't open file
        /* search forward '0' for next file */
        if (checkDirectoryForNextIteration(iteration, 0)) {
            // successful
            rc = readZeroXML();
        }
    }

    if (rc != 1) {  // unsuccessful open and read
        iteration--;
        if (animation) {
            slot_stopAnimation();
            emit(stopAnimation());
        }
    } else {
        if (ui->checkBox_timeScale->isChecked()) calcTimeScale();

        for (int i = 0; i < graphs.count(); i++) {
            graphs.at(i)->updateData(iteration);
        }

        if (restrict_dimension_open) emit(updatedAgentDimension());
    }

    ui->spinBox->setValue(iteration);
}

/*! \brief Decrement the iteration number, set the spin box value, read in new agent data.
 */
void MainWindow::decrement_iteration() {
    int rc;

    if (iteration > 0) iteration--;
    rc = readZeroXML();

    if (rc == -1) {  // Can't open file
        if (checkDirectoryForNextIteration(iteration, 1)) {
            rc = readZeroXML();
        }
    }

    ui->spinBox->setValue(iteration);

    if (rc == 1) {
        ui->spinBox->setValue(iteration);
        if (ui->checkBox_timeScale->isChecked()) calcTimeScale();

        if (restrict_dimension_open) emit(updatedAgentDimension());
    }
}

/*! \brief Provide a dialog to select a config file to open.
 */
void MainWindow::open_config_file() {
    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open config file..."),
                     "", tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     close_config_file();
     readConfigFile(fileName, 0);
}

bool MainWindow::checkDirectoryForNextIteration(int it, int flag) {
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
    for (int i = 0; i < list.size(); i++) {
        QString f = list.at(i);
        f.chop(4);
        bool s;
        int i = f.toInt(&s);
        if (s) ilist.append(i);
    }
    qSort(ilist);
    for (int i = 0; i < ilist.size(); i++) {
        if ((ilist.at(i) > it && flag == 0) ||
                (ilist.at(ilist.size()-1-i) < it && flag == 1)) {
            if (flag == 0) iteration = ilist.at(i);
            if (flag == 1) iteration = ilist.at(ilist.size()-1-i);
            return true;
        }
    }

    return false;
}

/*! \brief Read in a config file.
 *  \param fileName The file name
 *  \param it The iteration number to be set
 */
void MainWindow::readConfigFile(QString fileName, int it) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("FLAME Visualiser"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    ConfigXMLReader reader(visual_settings_model, graph_settings_model,
            &resultsData, timeScale, &ratio, &xrotate, &yrotate,
            &xmove, &ymove, &zmove, &delayTime, &orthoZoom, &visual_dimension);
    if (!reader.read(&file)) {
        QMessageBox::warning(this, tr("flame visualiser"),
         tr("Parse error in file %1 at line %2, column %3:\n%4").
         arg(fileName).
         arg(reader.lineNumber()).
         arg(reader.columnNumber()).
         arg(reader.errorString()));
        /* Clear anything read in */
        close_config_file();
    } else {
        if (visual_dimension == 2) on_actionOrthogonal_triggered();
        if (visual_dimension == 3) on_actionPerspective_triggered();

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

        ui->horizontalSlider_delay->setValue(
                static_cast<int>((1000-delayTime)/1000.0 * 99));
        // qDebug() << "slider value: " << ui->horizontalSlider_delay->value();

        if (ui->checkBox_timeScale->isChecked()) calcTimeScale();

        ui->lineEdit_ResultsLocation->setText(resultsData);
//        tryAndReadInAgentTypes();
        readZeroXML();

        /* For each new plot create a graph window */
        for (int i = 0; i < graph_settings_model->rowCount(); i++) {
        }

        enableInterface(true);
        fileOpen = true;
    }
    file.close();
}

/*! \brief Enable or disable the time scale UI.
 *  \param b The enable or disable flag
 */
void MainWindow::enableTimeScale(bool b) {
    timeScale->enabled = b;
    ui->checkBox_timeScale->setChecked(b);
    ui->lineEdit_timeScale->setEnabled(b);
    ui->pushButton_timeScale->setEnabled(b);

    /*QPalette palet;
    if (b) palet.setColor(ui->checkBox_timeScale->foregroundRole(),
            QColor(0, 0, 0));
    else
        palet.setColor(ui->checkBox_timeScale->foregroundRole(),
            QColor(120, 120, 120));
    ui->checkBox_timeScale->setPalette(palet);*/

    if (b) calcTimeScale();
    else
        emit(ui->lineEdit_timeScale->setText(""));
}

/*! \brief Open a new config file.
 */
void MainWindow::new_config_file() {
    QString fileName =
             QFileDialog::getSaveFileName(this, tr("New config file..."),
                                          "",
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     /* Close any current config */
     close_config_file();

     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("FLAME Visualiser"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     enableInterface(true);
     writeConfigXML(&file);
}

/*! \brief Provide a dialog to save a config file.
 */
void MainWindow::save_as_config_file() {
    QString configFile = "";
    if (fileOpen) {
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);
    }

    QString fileName =
             QFileDialog::getSaveFileName(this, tr("Save config file..."),
                                          configFile,
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("FLAME Visualiser"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     writeConfigXML(&file);
}

/*! \brief Save a config file.
 */
void MainWindow::save_config_file() {
    if (fileOpen == false) {
        save_as_config_file();
    } else {
        QString configFile;
        configFile.append(configPath);
        configFile.append("/");
        configFile.append(configName);

        QFile file(configFile);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("FLAME Visualiser"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(configFile)
                                 .arg(file.errorString()));
            return;
        }

        writeConfigXML(&file);
    }
}

/*! \brief Close the config file and disable the user interface.
 */
void MainWindow::close_config_file() {
    this->setWindowTitle("FLAME Visualiser - ");
    visual_settings_model->deleteRules();
    graph_settings_model->deletePlots();
    ui->lineEdit_ResultsLocation->setText("");
    agents.clear();
    agentTypes.clear();
    graphs.clear();
    enableInterface(false);
    fileOpen = false;
    timeScale->reset();
    enableTimeScale(false);
    iteration = 0;
    ui->spinBox->setValue(iteration);
    ui->label_5->setText("");
    configPath = "";
    configName = "";
    iteration = 0;
    xrotate = 0.0;
    yrotate = 0.0;
    xmove = 0.0;
    ymove = 0.0;
    zmove = -3.0;
    xoffset = 0.0;
    yoffset = 0.0;
    zoffset = 0.0;
    delayTime = 0;
    orthoZoom = 1.0;
    ui->pushButton_Animate->setText("Start Animation - A");
    ui->pushButton_Animate->setEnabled(false);
    animation = false;
    agentTypeCounts.clear();
    on_actionPerspective_triggered();
}

/*! \brief Write a config to an xml file.
 *  \param file The QFile to write to.
 *  \return False if error, true if worked.
 */
bool MainWindow::writeConfigXML(QFile * file) {
    fileOpen = true;

    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("flame_visualiser_config");
    stream.writeAttribute("version", "0.1");

    stream.writeStartElement("resultsData");  // resultsData
    stream.writeTextElement("directory", ui->lineEdit_ResultsLocation->text());
    stream.writeEndElement();  // resultsData

    stream.writeStartElement("timeScale");  // timeScale
    if (ui->checkBox_timeScale->isChecked())
        stream.writeTextElement("enable", "true");
    else
        stream.writeTextElement("enable", "false");
    stream.writeTextElement("milliseconds", QString("%1").arg(
            timeScale->millisecond));
    stream.writeTextElement("seconds", QString("%1").arg(timeScale->second));
    stream.writeTextElement("minutes", QString("%1").arg(timeScale->minute));
    stream.writeTextElement("hours", QString("%1").arg(timeScale->hour));
    stream.writeTextElement("days", QString("%1").arg(timeScale->day));
    if (timeScale->displayInVisual)
        stream.writeTextElement("displayTimeInVisual", "true");
    else
        stream.writeTextElement("displayTimeInVisual", "false");
    stream.writeEndElement();  // timeScale

    stream.writeStartElement("animation");  // animation
    stream.writeTextElement("delay", QString("%1").arg(delayTime));
    stream.writeEndElement();  // animation

    stream.writeStartElement("visual");
    stream.writeTextElement("view", QString("%1").arg(visual_dimension));
    stream.writeTextElement("ratio", QString("%1").arg(ratio));
    stream.writeTextElement("xrotate", QString("%1").arg(xrotate));
    stream.writeTextElement("yrotate", QString("%1").arg(yrotate));
    stream.writeTextElement("xmove", QString("%1").arg(xmove));
    stream.writeTextElement("ymove", QString("%1").arg(ymove));
    stream.writeTextElement("zmove", QString("%1").arg(zmove));
    stream.writeTextElement("orthoZoom", QString("%1").arg(orthoZoom));
    stream.writeStartElement("rules");
    for (int i = 0; i < this->visual_settings_model->rowCount(); i++) {
        VisualSettingsItem *vsitem = visual_settings_model->getRule(i);

        stream.writeStartElement("rule");  // rule
        stream.writeTextElement("agentType", vsitem->agentType());
        stream.writeStartElement("condition");  // condition
        if (vsitem->condition().enable) stream.writeTextElement(
                "enable", "true");
        else
            stream.writeTextElement("enable", "false");
        stream.writeStartElement("lhs");  // lhs
        stream.writeTextElement("type", "variable");
        stream.writeTextElement("variable", vsitem->condition().variable);
        stream.writeEndElement();  // lhs
        stream.writeTextElement("operator", vsitem->condition().op);
        stream.writeStartElement("rhs");  // rhs
        stream.writeTextElement("type", "value");
        stream.writeTextElement("value", QString("%1").arg(
                vsitem->condition().value));
        stream.writeEndElement();  // rhs
        stream.writeEndElement();  // condition
        stream.writeStartElement("x");  // x
        if (vsitem->x().useVariable)
            stream.writeTextElement("useVariable", "true");
        else
            stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->x().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(
                vsitem->x().opValue));
        stream.writeEndElement();  // x
        stream.writeStartElement("y");  // y
        if (vsitem->y().useVariable)
            stream.writeTextElement("useVariable", "true");
        else
            stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->y().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(
                vsitem->y().opValue));
        stream.writeEndElement();  // y
        stream.writeStartElement("z");  // z
        if (vsitem->z().useVariable) stream.writeTextElement(
                "useVariable", "true");
        else
            stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("variable", vsitem->z().positionVariable);
        stream.writeTextElement("offSet", QString("%1").arg(
                vsitem->z().opValue));
        stream.writeEndElement();  // z
        stream.writeStartElement("shape");  // shape
        stream.writeTextElement("object", vsitem->shape().getShape());
        if (QString::compare(vsitem->shape().getShape(), "sphere") == 0)
            stream.writeTextElement("quality", QString("%1").arg(
                    vsitem->shape().getQuality()));
        stream.writeTextElement("dimension", QString("%1").arg(
                vsitem->shape().getDimension()));
        if (vsitem->shape().getUseVariable())
            stream.writeTextElement("useVariable", "true");
        else
            stream.writeTextElement("useVariable", "false");
        stream.writeTextElement("dimensionVariable", QString("%1").arg(
                vsitem->shape().getDimensionVariable()));
        if (vsitem->shape().getFromCentreX())
            stream.writeTextElement("fromCentreX", "true");
        else
            stream.writeTextElement("fromCentreX", "false");
        stream.writeTextElement("dimensionY", QString("%1").arg(
                vsitem->shape().getDimensionY()));
        if (vsitem->shape().getUseVariableY())
            stream.writeTextElement("useVariableY", "true");
        else
            stream.writeTextElement("useVariableY", "false");
        stream.writeTextElement("dimensionVariableY", QString("%1").arg(
                vsitem->shape().getDimensionVariableY()));
        if (vsitem->shape().getFromCentreY())
            stream.writeTextElement("fromCentreY", "true");
        else
            stream.writeTextElement("fromCentreY", "false");
        stream.writeTextElement("dimensionZ", QString("%1").arg(
                vsitem->shape().getDimensionZ()));
        if (vsitem->shape().getUseVariableZ())
            stream.writeTextElement("useVariableZ", "true");
        else
            stream.writeTextElement("useVariableZ", "false");
        stream.writeTextElement("dimensionVariableZ", QString("%1").arg(
                vsitem->shape().getDimensionVariableZ()));
        if (vsitem->shape().getFromCentreZ())
            stream.writeTextElement("fromCentreZ", "true");
        else
            stream.writeTextElement("fromCentreZ", "false");
        stream.writeEndElement();  // shape
        stream.writeStartElement("colour");  // colour
        stream.writeTextElement("r", QString("%1").
                arg(vsitem->colour().red()));
        stream.writeTextElement("g", QString("%1").
                arg(vsitem->colour().green()));
        stream.writeTextElement("b", QString("%1").
                arg(vsitem->colour().blue()));
        stream.writeTextElement("a", QString("%1").
                arg(vsitem->colour().alpha()));
        stream.writeEndElement();  // colour
        if (vsitem->enabled()) stream.writeTextElement(
                "enable", "true");
        else
            stream.writeTextElement("enable", "false");
        stream.writeEndElement();  // rule
    }
    stream.writeEndElement();  // rules
    stream.writeEndElement();  // visual

    stream.writeStartElement("graph");
    for (int i = 0; i < this->graph_settings_model->rowCount(); i++) {
        GraphSettingsItem * gsitem = graph_settings_model->getPlot(i);

        stream.writeStartElement("plot");  // plot
        stream.writeTextElement("graphNumber", gsitem->getGraph());
        stream.writeStartElement("xAxis");  // xAxis
        stream.writeTextElement("type", gsitem->getXaxis());
        stream.writeEndElement();  // xAxis
        stream.writeStartElement("yAxis");  // yAxis
        stream.writeTextElement("type", "agent");
        stream.writeTextElement("agentType", gsitem->getYaxis());
        stream.writeEndElement();  // yAxis
        stream.writeStartElement("condition");  // condition
        if (gsitem->condition().enable)
            stream.writeTextElement("enable", "true");
        else
            stream.writeTextElement("enable", "false");
        stream.writeStartElement("lhs");  // lhs
        stream.writeTextElement("type", "variable");
        stream.writeTextElement("variable", gsitem->condition().variable);
        stream.writeEndElement();  // lhs
        stream.writeTextElement("operator", gsitem->condition().op);
        stream.writeStartElement("rhs");  // rhs
        stream.writeTextElement("type", "value");
        stream.writeTextElement("value", QString("%1").arg(
                gsitem->condition().value));
        stream.writeEndElement();  // rhs
        stream.writeEndElement();  // condition
        stream.writeStartElement("colour");  // colour
        stream.writeTextElement("r", QString("%1").arg(
                gsitem->getColour().red()));
        stream.writeTextElement("g", QString("%1").arg(
                gsitem->getColour().green()));
        stream.writeTextElement("b", QString("%1").arg(
                gsitem->getColour().blue()));
        stream.writeEndElement();  // colour
        stream.writeEndElement();  // plot
    }
    stream.writeEndElement();  // graph

    stream.writeEndDocument();
    file->close();
    if (file->error()) {
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

/*! \brief Handle any key press events.
 *  \param event The key press event
 */
void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
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
            on_pushButton_Animate_clicked();
            break;
        default:
            event->ignore();
            break;
    }
}

/*! \brief Automatically work out the offset of the scene to position
 *  centre at origin.
 */
void MainWindow::calcPositionOffset() {
    double smallest_x = 0.0;
    double largest_x = 0.0;
    double smallest_y = 0.0;
    double largest_y = 0.0;
    double smallest_z = 0.0;
    double largest_z = 0.0;

    for (int j = 0; j < visual_settings_model->rowCount(); j++) {
        for (int i= 0; i < visual_settings_model->getRule(j)->agents.count();
                i++) {
            if (smallest_x > visual_settings_model->getRule(j)->agents.at(i).x)
                smallest_x = visual_settings_model->getRule(j)->agents.at(i).x;
            if (smallest_y > visual_settings_model->getRule(j)->agents.at(i).y)
                smallest_y = visual_settings_model->getRule(j)->agents.at(i).y;
            if (largest_x  < visual_settings_model->getRule(j)->agents.at(i).x)
                largest_x  = visual_settings_model->getRule(j)->agents.at(i).x;
            if (largest_y  < visual_settings_model->getRule(j)->agents.at(i).y)
                largest_y  = visual_settings_model->getRule(j)->agents.at(i).y;
        }
    }

    /* Takes the middle position of x and y */
    xoffset = -(largest_x + smallest_x)/2.0;
    yoffset = -(largest_y + smallest_y)/2.0;
    zoffset = -(largest_z + smallest_z)/2.0;

    /*qDebug() << "xoffset: " << xoffset;
    qDebug() << "yoffset: " << yoffset;
    qDebug() << "zoffset: " << zoffset;*/
}

/*! \brief Automatically work out a good ratio to use to view agents visually.
 */
void MainWindow::calcPositionRatio() {
    double smallest = 0.0;
    double largest = 0.0;

    for (int j = 0; j < visual_settings_model->rowCount(); j++) {
        for (int i= 0; i < visual_settings_model->getRule(j)->agents.count();
                i++) {
            double x = visual_settings_model->getRule(j)->agents.at(i).x;
            double y = visual_settings_model->getRule(j)->agents.at(i).y;
            double size_x =
                visual_settings_model->getRule(j)->shape().getDimension()/2.0;
            double size_y = size_x;
            if (QString::compare("cube",
                visual_settings_model->getRule(j)->shape().getShape()) == 0)
                size_y =
                visual_settings_model->getRule(j)->shape().getDimensionY()/2.0;

            if (smallest > x-size_x) smallest = x-size_x;
            if (smallest > y-size_y) smallest = y-size_y;
            if (largest  < x+size_x) largest  = x+size_x;
            if (largest  < y+size_y) largest  = y+size_y;
        }
    }

    // qDebug() << smallest << largest;

    if (smallest < 0.0 && largest < -smallest)
        ratio = 1.0 / -smallest;
    else
        ratio = 1.0 / largest;

    /* Make a largest ratio */
    // if (ratio > 0.05) ratio = 0.05;

    // qDebug() << "ratio: " << ratio;
}

void MainWindow::slot_stopAnimation() {
    animation = false;
    ui->pushButton_Animate->setText("Start Animation - A");
}

void MainWindow::slot_startAnimation() {
    animation = true;
    ui->pushButton_Animate->setText("Stop Animation - A");
}

/*! \brief Emit animate signal when the animate button is pressed.
 */
void MainWindow::on_pushButton_Animate_clicked() {
    if (animation) {
        slot_stopAnimation();
        emit(stopAnimation());
    } else {
        slot_startAnimation();
        emit(startAnimation());
    }
}

/*! \brief Open or close the image settings dialog.
 */
void MainWindow::on_pushButton_ImageSettings_clicked() {
    if (images_dialog_open == false) {
        images_dialog_open = true;
        images_dialog = new ImagesDialog();

        connect(images_dialog, SIGNAL(image_dialog_closed()),
                this, SLOT(image_dialog_closed()));
        connect(images_dialog, SIGNAL(take_snapshot()),
                this, SLOT(takeSnapshotSlot()));
        connect(this, SIGNAL(imageStatusSignal(QString)),
                images_dialog, SLOT(imageStatus(QString)));
        connect(images_dialog, SIGNAL(takeAnimationSignal(bool)),
                this, SLOT(takeAnimationSlot(bool)));
        connect(images_dialog, SIGNAL(updateImagesLocation(QString)),
                this, SLOT(updateImagesLocationSlot(QString)));

        images_dialog->setConfigPath(&configPath);
        images_dialog->setLocation(ui->lineEdit_ResultsLocation->text());
        images_dialog->sendImageAniUpdate();
        images_dialog->show();
        ui->pushButton_ImageSettings->setText("Close Image Settings");
    } else {
        images_dialog->close();
    }
}

/*! \brief Emit take snapshot signal if snapshot button is pressed.
 */
void MainWindow::takeSnapshotSlot() {
    if (opengl_window_open) emit(takeSnapshotSignal());
}

void MainWindow::imageStatusSlot(QString s) {
    if (images_dialog_open) emit(imageStatusSignal(s));
}

void MainWindow::takeAnimationSlot(bool b) {
    if (opengl_window_open) emit(takeAnimationSignal(b));
}

void MainWindow::updateImagesLocationSlot(QString s) {
    if (opengl_window_open) emit(updateImagesLocationSignal(s));
}

void MainWindow::ruleUpdated(int /*row*/) {
    // Reread agents using updated rules
    if (opengl_window_open) readZeroXML();
}

void MainWindow::on_actionQuit_triggered() {
    if (opengl_window_open) visual_window->close();
    if (images_dialog_open) images_dialog->close();
    if (restrict_dimension_open) restrictAxesDialog->close();
    close();
}

void MainWindow::on_actionAbout_triggered() {
    QTextEdit *about = new QTextEdit(this);
    about->setWindowFlags(Qt::Dialog);
    about->setReadOnly(true);
    about->setAutoFormatting(QTextEdit::AutoNone);

    QString aboutText;
    aboutText.append("<h1>FLAME Visualiser</h1>");
    aboutText.append("<h3>Simon Coakley</h3>");
    aboutText.append("<h2>Version 3</h2>");
    aboutText.append("<h3>Changelog</h3>");
    /* Add new release notes here */
    aboutText.append("<h4>Version 3 (released 2012-02-22)</h4><ul>");
    aboutText.append("<li>Beta third release</li>");
    aboutText.append("<li>Added enable column for visual rules</li>");
    aboutText.append("<li>New agent types are automatically found in ");
    aboutText.append("new iteration files</li>");
    aboutText.append("<li>Added a reset viewpoint button</li>");
    aboutText.append("<li>Added orthogonal view option for visual ");
    aboutText.append("window</li>");
    aboutText.append("<li>Visual scene is now auto centred, no need ");
    aboutText.append("for offsets</li>");
    aboutText.append("<li>Added 'from centre' switch for shape size</li>");
    aboutText.append("<li>Missing data for graphs no longer taken to be ");
    aboutText.append("zero</li>");
    aboutText.append("<li>Lines drawn between all data points on a ");
    aboutText.append("graph</li>");
    aboutText.append("<li>Style menu added for graphs to select lines, ");
    aboutText.append("points, linespoints, or dots</li>");
    aboutText.append("<li>Visual menu items moved under Visual menu</li>");
    aboutText.append("<li>Changed the default shape and colour for ");
    aboutText.append("visual rules</li>");
    aboutText.append("<li>Removed offset labels from shape dialog</li>");
    aboutText.append("<li>Added iteration info dialog</li>");
    aboutText.append("<li>Improved error feedback</li>");
    aboutText.append("<li>Added application icon</li>");
    aboutText.append("<li>Bug fix: conditions are not drawn on graph ");
    aboutText.append("legend if not enabled</li>");
    aboutText.append("<li>Bug fix: cancel buttons work for dialog ");
    aboutText.append("editors</li></ul>");
    aboutText.append("<h4>Version 2 (released 2011-10-28)</h4><ul>");
    aboutText.append("<li>Beta second release</li>");
    aboutText.append("<li>Ability to change the quality of spheres ");
    aboutText.append("being drawn</li>");
    aboutText.append("<li>Ability to add a time scale that can be ");
    aboutText.append("displayed</li>");
    aboutText.append("<li>Ability to change the near clip plane</li>");
    aboutText.append("<li>Ability to pick an agent and its memory be ");
    aboutText.append("displayed</li>");
    aboutText.append("<li>Ability to move the centre of the visual scene ");
    aboutText.append("with the mouse</li>");
    aboutText.append("<li>Ability to restrict the drawing of agents on ");
    aboutText.append("different axes</li>");
    aboutText.append("<li>Program searches for iterations if they are not ");
    aboutText.append("sequential</li>");
    aboutText.append("<li>Program remembers the model last open and the ");
    aboutText.append("iteration number</li>");
    aboutText.append("<li>Program saves the visual scene rotation and ");
    aboutText.append("translation to the config file</li>");
    aboutText.append("<li>Program closes when the main window is closed</li>");
    aboutText.append("<li>Addition of a simple help dialog</li>");
    aboutText.append("<li>Bug fix: cubes and points were being drawn more ");
    aboutText.append("than once</li></ul>");
    aboutText.append("<h4>Version 1 (released 2011-10-07)</h4><ul><li>Beta ");
    aboutText.append("first release</li></ul>");

    about->setGeometry(50, 50, 600, 400);
    about->insertHtml(aboutText);
    about->moveCursor(QTextCursor::Start);
    about->show();
}

void MainWindow::on_pushButton_timeScale_clicked() {
    if (time_dialog_open == false) {
        time_dialog_open = true;
        time_dialog = new TimeDialog(timeScale);

        connect(time_dialog, SIGNAL(time_dialog_closed()),
                this, SLOT(time_dialog_closed()));

        time_dialog->show();

        ui->pushButton_timeScale->setText("Close Time Settings");
    } else {
        time_dialog->close();
    }
}

void MainWindow::calcTimeScale() {
    double tsecs;
    double tmsecs = timeScale->millisecond*iteration/1000.0;
    tmsecs = modf(tmsecs, &tsecs);
    int totalseconds = static_cast<int>(tsecs);
    totalseconds += timeScale->totalseconds*iteration;

    QString ts;
    size_t BufSize = 1000;
    char buf[BufSize];
    int days = static_cast<int>(totalseconds/86400);
    int hours = static_cast<int>((totalseconds%86400)/3600);
    int minutes = static_cast<int>(((totalseconds%86400)%3600)/60);
    int seconds = static_cast<int>(((totalseconds%86400)%3600)%60);
    if (days > 0 || timeScale->lowestScale == 4) {
        snprintf(buf, BufSize, "%3d day", days);
        ts.append(QString().fromAscii(buf));
        if (days > 1)
            ts.append("s");
        else
            ts.append(" ");
        ts.append(" ");
    }
    if (hours > 0 || timeScale->lowestScale < 4) {
        snprintf(buf, BufSize, "%02d hrs", hours);
        ts.append(QString().fromAscii(buf));
        // if(hours > 1) ts.append("s"); else ts.append(" ");
        ts.append(" ");
    }
    if (minutes > 0 || timeScale->lowestScale < 3) {
        snprintf(buf, BufSize, "%02d hrs", minutes);
        ts.append(QString().fromAscii(buf));
        // if(minutes > 1) ts.append("s"); else ts.append(" ");
        ts.append(" ");
    }
    if (timeScale->lowestScale == 1) {
        snprintf(buf, BufSize, "%02d s", seconds);
        ts.append(QString().fromAscii(buf));
    } else if (timeScale->lowestScale == 0) {
        snprintf(buf, BufSize, "%02.3f s", seconds+tmsecs);
        ts.append(QString().fromAscii(buf));
    }

    timeString = ts;

    emit(ui->lineEdit_timeScale->setText(ts));
}

void MainWindow::on_actionHelp_triggered() {
    QDesktopServices::openUrl(
        QUrl("http://www.flame.ac.uk/docs/flamevisualiser/v3/"));
}

void MainWindow::on_actionRestrict_Axes_triggered() {
    if (!restrict_dimension_open) {
        restrict_dimension_open = true;
        restrictAxesDialog = new RestrictAxesDialog(restrictDimension,
                agentDimension, restrictAgentDimension, &ratio);
        connect(restrictAxesDialog, SIGNAL(closed()),
                this, SLOT(restrict_axes_closed()));
        connect(this, SIGNAL(updatedAgentDimension()),
                restrictAxesDialog, SLOT(updatedAgentDimensions()));
        emit(restrictAxes(true));
    }
    restrictAxesDialog->show();
}

void MainWindow::on_horizontalSlider_delay_valueChanged(int value) {
    // qDebug() << "slider value: " << value;
    /* value range 0-99 */
    delayTime = (99-value)/99.0 * 1000.0; /* 1000 = 1s */
    // qDebug() << "delayTime: " << delayTime;
    emit(updateDelayTime(delayTime));
}

void MainWindow::on_actionIteration_Info_triggered() {
    /* If no iteration data window then create one */
    if (iterationInfo_dialog_open == false) {
        iterationInfo_dialog = new IterationInfoDialog(&agentTypeCounts,
                                                       agentDimension);
        iterationInfo_dialog->show();
        iterationInfo_dialog_open = true;
        connect(this, SIGNAL(updateIterationInfoDialog()),
                iterationInfo_dialog, SLOT(update_info()));
        connect(iterationInfo_dialog, SIGNAL(iterationInfoDialog_closed()),
                this, SLOT(iterationInfoDialog_closed()));
        emit(updateIterationInfoDialog());
    } else {
        iterationInfo_dialog->activateWindow();
    }
}

void MainWindow::resetVisualViewpoint() {
    // Set ratio to be 1
    ratio = 1.0;
    // Reset offsets
    xoffset = 0.0;
    yoffset = 0.0;
    zoffset = 0.0;
    // Read in agents with model dimensions
    readZeroXML();
    // Calculate offset
    calcPositionOffset();
    // Reread agents with offsets
    readZeroXML();
    // Calculate model to opengl dimension ratio
    calcPositionRatio();
    // Reread agents with opengl dimension using new ratio
    readZeroXML();
}

void MainWindow::on_pushButton_updateViewpoint_clicked() {
    visual_window->reset_camera();
    resetVisualViewpoint();
}

void MainWindow::on_actionPerspective_triggered() {
    ui->actionOrthogonal->setChecked(false);
    ui->actionPerspective->setChecked(true);
    visual_dimension = 3;
    if (opengl_window_open) visual_window->setDimension(3);
}

void MainWindow::on_actionOrthogonal_triggered() {
    ui->actionOrthogonal->setChecked(true);
    ui->actionPerspective->setChecked(false);
    visual_dimension = 2;
    if (opengl_window_open) visual_window->setDimension(2);
}

void MainWindow::updateAllGraphs() {
    int i;
    for (i = 0; i < graphs.size(); i++)
        graphs.at(i)->update();
}

void MainWindow::on_actionLines_triggered() {
    ui->actionLines->setChecked(true);
    ui->actionPoints->setChecked(false);
    ui->actionLinespoints->setChecked(false);
    ui->actionDots->setChecked(false);
    graph_style = 0;
    updateAllGraphs();
}

void MainWindow::on_actionPoints_triggered() {
    ui->actionLines->setChecked(false);
    ui->actionPoints->setChecked(true);
    ui->actionLinespoints->setChecked(false);
    ui->actionDots->setChecked(false);
    graph_style = 1;
    updateAllGraphs();
}

void MainWindow::on_actionLinespoints_triggered() {
    ui->actionLines->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionLinespoints->setChecked(true);
    ui->actionDots->setChecked(false);
    graph_style = 2;
    updateAllGraphs();
}

void MainWindow::on_actionDots_triggered() {
    ui->actionLines->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionLinespoints->setChecked(false);
    ui->actionDots->setChecked(true);
    graph_style = 3;
    updateAllGraphs();
}
