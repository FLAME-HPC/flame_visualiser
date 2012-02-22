/*!
 * \file graphwidget.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for graph widget
 */
#ifndef GRAPHWIDGET_H_
#define GRAPHWIDGET_H_

#include <QWidget>
#include "./agent.h"
#include "./graphsettingsitem.h"

class GraphWidget: public QWidget {
  Q_OBJECT

  public:
    GraphWidget(QList<Agent> *a = 0, int * gs = 0, QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void updateData(int it);
    void addPlot(GraphSettingsItem * gsi);
    int removePlot(GraphSettingsItem * gsi);
    void setGraph(QString g) { graphName = g; }
    QString getGraph() { return graphName; }

  signals:
    void increase_iteration();
    void decrease_iteration();
    void graph_window_closed(QString graphName);

  protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *);

  private slots:


  private:
    void drawStylePoint(int type, int size, int x1, int y1, QPainter *painter);
    QList<Agent> *agents;
    // GraphSettingsModel * gsmodel;
    QList<GraphSettingsItem*> plots;
    QList<QList<int> > data;
    /*! Flag for each iteration to say data exists */
    QList<bool> dataExists;
    int topValue;
    int topIteration;
    QString graphName;
    int * style;
};

#endif  // GRAPHWIDGET_H_
