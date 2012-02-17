/*!
 * \file graphwidget.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of graph widget
 */
#include <QPainter>
#include <QDebug>
#include <QtGui/QMouseEvent>
#include <QMenuBar>
#include "./graphwidget.h"
#include "./condition.h"

GraphWidget::GraphWidget(QList<Agent> *a, QWidget *parent)
    : QWidget(parent) {
    agents = a;
    topValue = 0;
    topIteration = 0;
    style = 0;
    setFocus();

    QMenuBar *menu = new QMenuBar(this);
    QMenu * graphMenu = new QMenu("Graph");
    QMenu * styleMenu = new QMenu("style");

    linesAction = new QAction("lines", this);
    pointsAction = new QAction("points", this);
    linespointsAction = new QAction("linespoints", this);
    dotsAction = new QAction("dots", this);
    linesAction->setCheckable(true);
    pointsAction->setCheckable(true);
    linespointsAction->setCheckable(true);
    dotsAction->setCheckable(true);
    connect(linesAction, SIGNAL(triggered()),
            this, SLOT(clickStyleLines()));
    connect(pointsAction, SIGNAL(triggered()),
            this, SLOT(clickStylePoints()));
    connect(linespointsAction, SIGNAL(triggered()),
            this, SLOT(clickStyleLinespoints()));
    connect(dotsAction, SIGNAL(triggered()),
            this, SLOT(clickStyleDots()));
    styleMenu->addAction(linesAction);
    styleMenu->addAction(pointsAction);
    styleMenu->addAction(linespointsAction);
    styleMenu->addAction(dotsAction);

    graphMenu->addMenu(styleMenu);
    menu->addMenu(graphMenu);

    linesAction->setChecked(true);
}

void GraphWidget::closeEvent(QCloseEvent *event) {
    emit(graph_window_closed(graphName));
    event->accept();
}

void GraphWidget::clickStyleLines() {
    linesAction->setChecked(true);
    pointsAction->setChecked(false);
    linespointsAction->setChecked(false);
    dotsAction->setChecked(false);
    style = 0;
    repaint();
}

void GraphWidget::clickStylePoints() {
    linesAction->setChecked(false);
    pointsAction->setChecked(true);
    linespointsAction->setChecked(false);
    dotsAction->setChecked(false);
    style = 1;
    repaint();
}

void GraphWidget::clickStyleLinespoints() {
    linesAction->setChecked(false);
    pointsAction->setChecked(false);
    linespointsAction->setChecked(true);
    dotsAction->setChecked(false);
    style = 2;
    repaint();
}

void GraphWidget::clickStyleDots() {
    linesAction->setChecked(false);
    pointsAction->setChecked(false);
    linespointsAction->setChecked(false);
    dotsAction->setChecked(true);
    style = 3;
    repaint();
}

void GraphWidget::addPlot(GraphSettingsItem *gsi) {
    plots.append(gsi);
    QList<int> newData;
    data.append(newData);
}

int GraphWidget::removePlot(GraphSettingsItem *gsi) {
    for (int i = 0; i < plots.count(); i++) {
        if (plots.at(i) == gsi) plots.removeAt(i);
    }

    return plots.count();
}

void GraphWidget::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);

    // painter.setWindow( 0, 0, 800, 200 );

    // painter.setRenderHint( QPainter::Antialiasing, true);
    // painter.setPen(QPen(gsitem->getColour(), 1, Qt::SolidLine,
    // Qt::RoundCap, Qt::MiterJoin));

    QSize windowSize = size();
    int width = windowSize.width();
    int height = windowSize.height();
    int xleft = 40;
    int xright = width-40;
    int ytop = 20;
    int ybottom = height-40;

    painter.fillRect(0, 0, width, height, QBrush(Qt::white));

    /* Draw legend */
    for (int j = 0; j < plots.count(); j ++) {
        painter.setPen(QPen(plots.at(j)->getColour()));
        QString text = QString("%1").arg(plots[j]->getYaxis());
        if (plots[j]->condition().enable)
        text.append(QString(" (%2)").arg(plots[j]->condition().getString()));

        const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
                Qt::AlignLeft, text));

        painter.drawText(width-bbox.width()-10, 20+(j*bbox.height()), text);

        if (xright > (width-bbox.width()-40)) xright = width-bbox.width()-40;
    }

    int last_valid_x;
    int last_valid_y;
    bool found_valid_point = false;
    /* Draw data */
    for (int j = 0; j < plots.count(); j ++) {
        painter.setPen(QPen(plots.at(j)->getColour()));
        for (int i = 0; i < data.at(j).count(); i++) {
            /* First point is current data point */
            int x1 = xleft+( (i/static_cast<double>(topIteration))*
                    (xright-xleft));
            int y1 = ybottom-( ((data.at(j).at(i)/
                    static_cast<double>(topValue)))*(ybottom-ytop) );

            if (!found_valid_point && dataExists[i]) {
                last_valid_x = x1;
                last_valid_y = y1;
                found_valid_point = true;
            }

            /* Second point is last data point */
            if (dataExists[i]) {
                /* lines style */
                if (style == 0 || style == 2) {
                    if (i == 0)
                        painter.drawPoint(x1, y1);
                    else
                        painter.drawLine(x1, y1, last_valid_x, last_valid_y);
                    last_valid_x = x1;
                    last_valid_y = y1;
                }
                /* points style */
                if (style == 1 || style == 2) {
                    int size = 2;
                    painter.drawLine(x1+size, y1+size, x1-size, y1-size);
                    painter.drawLine(x1+size, y1-size, x1-size, y1+size);
                }
                /* dots style */
                if (style == 3) {
                    painter.drawPoint(x1, y1);
                }
            }
        }
        painter.drawText(xright+5, ybottom-(((data[j].back()/
                static_cast<double>(topValue)))*(ybottom-ytop))+5,
                QString("%1").arg(data[j].back()));
    }

    /* Draw graph sides */
    painter.setPen(QPen(Qt::black));
    painter.drawLine(xleft, ytop, xleft, ybottom);
    painter.drawLine(xleft, ybottom, xright, ybottom);
    /* Draw graph markers */
    painter.drawLine(xleft, ytop, xleft-5, ytop);
    painter.drawText(5, ytop-5, 30, 10, Qt::AlignCenter,
            QString("%1").arg(topValue));
    painter.drawLine(xright, ybottom, xright, ybottom+5);
    painter.drawText(xright-15, ybottom+10, 30, 10, Qt::AlignCenter,
            QString("%1").arg(topIteration));
}

void GraphWidget::updateData(int it) {
    int count;

    if (it > topIteration) topIteration = it;

    for (int j = 0; j < plots.count(); j++) {
        count = 0;

        for (int i = 0; i < agents->count(); i++) {
            if (QString::compare(agents->at(i).agentType,
                    plots.at(j)->getYaxis()) == 0) {
                /* If a condition is enabled then check it */
                if (plots.at(j)->condition().enable) {
                    for (int k = 0; k < agents->at(i).tags.count(); k++) {
                        if (QString::compare(plots.at(j)->condition().variable,
                                agents->at(i).tags.at(k)) == 0) {
                            if (plots.at(j)->condition().op == "==" &&
                                    agents->at(i).values.at(k).toDouble() ==
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "!=" &&
                                    agents->at(i).values.at(k).toDouble() !=
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == ">" &&
                                    agents->at(i).values.at(k).toDouble() >
                            plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "<" &&
                                    agents->at(i).values.at(k).toDouble() <
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == ">=" &&
                                    agents->at(i).values.at(k).toDouble() >=
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "<=" &&
                                    agents->at(i).values.at(k).toDouble() <=
                                    plots.at(j)->condition().value) count++;
                        }
                    }
                } else {
                    count++;
                }
            }
        }

        while (data.at(j).count() < it+1) data[j].append(0);
        data[j][it] = count;

        if (count > topValue) topValue = count;
    }

    while (dataExists.count() < it+1) dataExists.append(false);
    dataExists[it] = true;

    repaint();
}

void GraphWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Z:
            emit(decrease_iteration());
            break;
        case Qt::Key_X:
            emit(increase_iteration());
            break;
        default:
            event->ignore();
            break;
    }
}
