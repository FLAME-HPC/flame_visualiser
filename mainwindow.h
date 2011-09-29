#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "GLWidget.h"
#include "graphwidget.h"
#include "agent.h"
#include "agenttype.h"
#include "visualsettingsmodel.h"
#include "graphsettingsmodel.h"
#include "imagesdialog.h"

/** \namespace Ui
  * \brief
  */
namespace Ui {
    class MainWindow;
}

/** \class MainWindow
  * \brief Main window class.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void enableInterface(bool enable);

public slots:
    void increment_iteration();
    void decrement_iteration();
    void visual_window_closed();
    void image_dialog_closed();
    void graph_window_closed(QString graphName);
    void takeSnapshotSlot();
    void imageStatusSlot(QString);
    void takeAnimationSlot(bool);
    void updateImagesLocationSlot(QString);
    void colourChanged(QColor);

signals:
    void updateVisual();
    void unblock();
    void animate();
    void iterationLoaded();
    void takeSnapshotSignal();
    void imageStatusSignal(QString);
    void takeAnimationSignal(bool);
    void updateImagesLocationSignal(QString);
    void stopAnimation();

private slots:
    void on_pushButton_LocationFind_clicked();
    void on_pushButton_OpenCloseVisual_clicked();
    void on_spinBox_valueChanged(int arg1);
    void addRule();
    void deleteRule();
    void addPlot();
    void deletePlot();
    void open_config_file();
    void save_config_file();
    void save_as_config_file();
    void close_config_file();
    void getColourVisual(QModelIndex index);
    void getColourGraph(QModelIndex index);
    void enabledGraph(QModelIndex index);
    //void on_doubleSpinBox_ratio_valueChanged(double arg1);
    //void on_pushButton_AutoDetect_clicked();
    void plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph, QString newGraph);
    void on_pushButton_Animate_clicked();
    void on_pushButton_ImageSettings_clicked();

private:
    bool readZeroXML(int flag);
    bool writeConfigXML(QFile * file);
    void createGraphWindow(GraphWidget * graph_window);
    void readConfigFile(QString fileName);
    void closeGraphWindows(QString graphName);
    void calcPositionRatio();
    Ui::MainWindow *ui; ///< The User Interface
    bool opengl_window_open; ///< Indicates if the visual window is open
    bool images_dialog_open; ///< Indicates if the image settings window is open
    GLWidget *visual_window; ///< The visual window
    int iteration; ///< The current iteration number
    bool fileOpen; ///< Indicates if a file is open
    QList<Agent> agents; ///< The list of agents
    QList<AgentType> agentTypes; ///< The list of agent types
    QList<GraphWidget*> graphs; ///< The list of graph windows
    VisualSettingsModel * visual_settings_model; ///< The visual setting data model
    GraphSettingsModel * graph_settings_model; ///< The graph settings data model
    ImagesDialog * images_dialog; ///< The image settings dialog
    QString configPath; ///< The path to the config xml file
    QString configName; ///< The name of the config xml file
    QString resultsData; ///< The path to 0.xml data
    double ratio; ///< The visual location ratio
    bool itLocked;
    QColor colour;
    QModelIndex colourIndex;
};

#endif // MAINWINDOW_H
