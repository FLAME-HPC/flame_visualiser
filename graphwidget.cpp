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
#include "./graphwidget.h"
#include "./condition.h"

GraphWidget::GraphWidget(QList<Agent> *a, QWidget *parent)
    : QWidget(parent) {
    agents = a;
    topValue = 0;
    topIteration = 0;
    setFocus();
}

void GraphWidget::closeEvent(QCloseEvent *event) {
    emit(graph_window_closed(graphName));
    event->accept();
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

    /* Draw data */
    for (int j = 0; j < plots.count(); j ++) {
        painter.setPen(QPen(plots.at(j)->getColour()));
        for (int i = 0; i < data.at(j).count(); i++) {
            int x1 = xleft+( (i/static_cast<double>(topIteration))*
                    (xright-xleft));
            int y1 = ybottom-( ((data.at(j).at(i)/
                    static_cast<double>(topValue)))*(ybottom-ytop) );

            if (i == 0) {
                painter.drawPoint(x1, y1);
            } else {
                int x2 = xleft+(((i-1)/static_cast<double>(topIteration))*
                        (xright-xleft));
                int y2 = ybottom-( ((data.at(j).at(i-1)/
                        static_cast<double>(topValue)))*(ybottom-ytop) );
                painter.drawLine(x1, y1, x2, y2);
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
