/*!
 * \file mainwindow.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for main window
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QFile>
#include "./glwidget.h"
#include "./graphwidget.h"
#include "./agent.h"
#include "./agenttype.h"
#include "./visualsettingsmodel.h"
#include "./graphsettingsmodel.h"
#include "./imagesdialog.h"
#include "./timedialog.h"
#include "./timescale.h"
#include "./restrictaxesdialog.h"
#include "./dimension.h"
#include "./iterationinfodialog.h"

/*! \brief
  */
namespace Ui {
    class MainWindow;
}

/*! \brief Main window class.
  */
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /* For testing allow test class to access private functions */
    #ifdef TESTBUILD
    friend class TestVisualiser;
    #endif

  protected:
    void keyPressEvent(QKeyEvent *event);
    void enableInterface(bool enable);
    void closeEvent(QCloseEvent *event);

  public slots:
    void increment_iteration();
    void decrement_iteration();
    void visual_window_closed();
    void image_dialog_closed();
    void time_dialog_closed();
    void graph_window_closed(QString graphName);
    void takeSnapshotSlot();
    void imageStatusSlot(QString);
    void takeAnimationSlot(bool);
    void updateImagesLocationSlot(QString);
    void colourChanged(QColor);
    void backgroundColourChanged(QColor c);
    void calcTimeScale();
    void restrict_axes_closed();
    void slot_toggleAnimation();
    void iterationInfoDialog_closed();

  signals:
    void updateVisual();
    void unblock();
    void stopAnimation();
    void startAnimation();
    void iterationLoaded();
    void takeSnapshotSignal();
    void imageStatusSignal(QString);
    void takeAnimationSignal(bool);
    void updateImagesLocationSignal(QString);
    void restrictAxes(bool);
    void updatedAgentDimension();
    void updateDelayTime(int);
    void updateIterationInfoDialog();

  private slots:
    void on_pushButton_LocationFind_clicked();
    void on_pushButton_OpenCloseVisual_clicked();
    void on_spinBox_valueChanged(int arg1);
    void addRule();
    void deleteRule();
    void addPlot();
    void deletePlot();
    void new_config_file();
    void open_config_file();
    void save_config_file();
    void save_as_config_file();
    void close_config_file();
    void getColourVisual(QModelIndex index);
    void getColourGraph(QModelIndex index);
    void enabledGraph(QModelIndex index);
    void enabledRule(QModelIndex index);
    // void on_doubleSpinBox_ratio_valueChanged(double arg1);
    // void on_pushButton_AutoDetect_clicked();
    void plotGraphChanged(GraphSettingsItem * gsi, QString oldGraph,
            QString newGraph);
    void on_pushButton_Animate_clicked();
    void on_pushButton_ImageSettings_clicked();
    void ruleUpdated(int);
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_pushButton_timeScale_clicked();
    void enableTimeScale(bool);
    void on_actionHelp_triggered();
    void on_actionRestrict_Axes_triggered();
    void on_horizontalSlider_delay_valueChanged(int value);
    void on_actionIteration_Info_triggered();
    void on_pushButton_updateViewpoint_clicked();
    void on_actionPerspective_triggered();
    void on_actionOrthogonal_triggered();
    void on_actionLines_triggered();
    void on_actionPoints_triggered();
    void on_actionLinespoints_triggered();
    void on_actionDots_triggered();
    void on_actionBackground_triggered();

  private:
    void loadGIS();
    int save_config_file_internal(QString fileName);
    int create_new_config_file(QString fileName);
    int readZeroXML();
    bool writeConfigXML(QFile * file);
    void createGraphWindow(GraphWidget * graph_window);
    int readConfigFile(QString fileName, int it);
    void closeGraphWindows(QString graphName);
    void calcPositionOffsetAndRatio();
    void findLoadSettings();
    bool checkDirectoryForNextIteration(int it, int flag);
    void resetVisualViewpoint();
    void updateAllGraphs();
    Ui::MainWindow *ui;  /*!< The User Interface */
    bool opengl_window_open;  /*!< Indicates if the visual window is open */
    /*! Indicates if the image settings window is open */
    bool images_dialog_open;
    /*! Indicates if the time scale settings window is open */
    bool time_dialog_open;
    GLWidget *visual_window;  /*!< The visual window */
    int iteration;  /*!< The current iteration number */
    bool fileOpen;  /*!< Indicates if a file is open */
    QList<Agent*> agents;  /*!< The list of agents */
    QList<AgentType> agentTypes;  /*!< The list of agent types */
    /*! A string list of agent type names  */
    QStringList stringAgentTypes;
    /*! A hash table of agent types and their count in an iteration */
    QHash<QString, int> agentTypeCounts;
    QList<GraphWidget*> graphs;  /*!< The list of graph windows */
    /*! The visual setting data model */
    VisualSettingsModel * visual_settings_model;
    /*! The graph settings data model */
    GraphSettingsModel * graph_settings_model;
    ImagesDialog * images_dialog;  /*!< The image settings dialog */
    TimeDialog * time_dialog;  /*!< The image settings dialog */
    IterationInfoDialog * iterationInfo_dialog;
    bool iterationInfo_dialog_open;
    QString configPath;  /*!< The path to the config xml file */
    QString configName;  /*!< The name of the config xml file */
    QString resultsData;  /*!< The path to 0.xml data */
    double ratio;  /*!< The visual location ratio */
    bool itLocked;
    QColor colour;
    QModelIndex colourIndex;
    TimeScale * timeScale;
    QString timeString;
    float xrotate;
    float yrotate;
    float xmove;
    float ymove;
    float zmove;
    double xoffset;
    double yoffset;
    double zoffset;
    float orthoZoom;
    Dimension * restrictAgentDimension;
    Dimension * restrictDimension;
    Dimension * agentDimension;
    bool restrict_dimension_open;
    RestrictAxesDialog * restrictAxesDialog;
    bool animation;
    int delayTime; /*!< The animation delay time in millisecs */
    int visual_dimension;
    int graph_style;
    QColor visualBackground;
    bool openedValidIteration;
    int gis[1000][600];
};

#endif  // MAINWINDOW_H_
