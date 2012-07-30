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

GraphWidget::GraphWidget(QList<Agent*> *a, int * gs, TimeScale * ts, QWidget *parent)
    : QWidget(parent) {
    agents = a;
    topValue = 0;
    topIteration = 0;
    style = gs;
    timeScale = ts;
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

void GraphWidget::drawStylePoint(int type, int size, int x1, int y1,
                                 QPainter * painter) {
    if (type == 0) {
        /* cross */
        painter->drawLine(x1+size, y1+size, x1-size, y1-size);
        painter->drawLine(x1+size, y1-size, x1-size, y1+size);
    } else if (type == 1) {
        /* square */
        painter->drawLine(x1+size, y1+size, x1+size, y1-size);
        painter->drawLine(x1+size, y1-size, x1-size, y1-size);
        painter->drawLine(x1-size, y1-size, x1-size, y1+size);
        painter->drawLine(x1-size, y1+size, x1+size, y1+size);
    } else if (type == 2) {
        /* plus */
        painter->drawLine(x1, y1+size, x1, y1-size);
        painter->drawLine(x1+size, y1, x1-size, y1);
    } else if (type == 3) {
        /* diamond */
        painter->drawLine(x1, y1+size, x1+size, y1);
        painter->drawLine(x1+size, y1, x1, y1-size);
        painter->drawLine(x1, y1-size, x1-size, y1);
        painter->drawLine(x1-size, y1, x1, y1+size);
    } else if (type == 4) {
        /* up triangle */
        painter->drawLine(x1, y1-size, x1+size, y1+size);
        painter->drawLine(x1+size, y1+size, x1-size, y1+size);
        painter->drawLine(x1-size, y1+size, x1, y1-size);
    } else if (type == 5) {
        /* down triangle */
        painter->drawLine(x1, y1+size, x1+size, y1-size);
        painter->drawLine(x1+size, y1-size, x1-size, y1-size);
        painter->drawLine(x1-size, y1-size, x1, y1+size);
    }
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
    int xleft = 0;
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
        /* Add space for points symbol */
        text.append(" ");

        const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
                Qt::AlignLeft, text));

        if (*style == 1 || *style == 2) {
            drawStylePoint(j%6, 2, width - 8,
                           20+(j*bbox.height())-bbox.height()/4.0, &painter);
        }

        painter.drawText(width-bbox.width()-10, 20+(j*bbox.height()), text);

        if (xright > (width-bbox.width()-10)) xright = width-bbox.width()-10;
    }

    //painter.drawLine(QPoint(xright, 0), QPoint(xright, height));

    // Use topValue to position xright and xleft
    const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
            Qt::AlignLeft, QString("%1").arg(topValue)));
    xright -= bbox.width() + 10;

    //painter.drawLine(QPoint(xright, 0), QPoint(xright, height));

    /* Draw graph markers */
    // Y-axis
    painter.drawText(5, ytop-5, bbox.width(), 10, Qt::AlignRight | Qt::AlignVCenter,
            QString("%1").arg(topValue));
    xleft += bbox.width() + 15;
    painter.drawLine(xleft, ytop, xleft-5, ytop);
    /*int mod = 1;
    int tp = topValue;
    while(mod != 0 && tp > 0) {
        mod = tp%10;
        if(mod != 0) tp--;
    }
    qDebug() << tp;
    if(tp > 9) {
        int tip = ybottom-(((tp/static_cast<double>(topValue)))*(ybottom-ytop));
        painter.drawLine(xleft, tip, xleft-5, tip);
        painter.drawText(5, tip+5, QString("%1").arg(tp));
        int tip5 = ybottom-((((tp/2.0)/static_cast<double>(topValue)))*(ybottom-ytop));
        painter.drawLine(xleft, tip5, xleft-5, tip5);
        painter.drawText(5, tip5+5, QString("%1").arg(tp/2.0));

    }*/
    painter.drawText(5, ybottom-5, bbox.width(), 10, Qt::AlignRight | Qt::AlignVCenter,
            QString("0"));
    painter.drawLine(xleft, ybottom, xleft-5, ybottom);


    // X-axis
    // 0
    painter.drawLine(xleft, ybottom, xleft, ybottom+5);
    painter.drawText(xleft-15, ybottom+10, bbox.width(), 10, Qt::AlignCenter, QString("0"));
    // tics
    /*if (timeScale->days >= 4) {
        int mod = 1;
        int topDays = timeScale->days;
        while(mod != 0 && topDays > 0) {
            mod = topDays%4;
            if(mod != 0) topDays--;
        }

        int mdays = topDays/2.0;
        int x1 = xleft+( ((mdays)/static_cast<double>(topIteration))*(xright-xleft));
        painter.drawLine(x1, ybottom, x1, ybottom+5);
        const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
                Qt::AlignLeft, QString("%1").arg(mdays)));
        painter.drawText(x1-bbox.width(), ybottom+10, bbox.width(), 10, Qt::AlignCenter, QString("%1").arg(mdays));
    }*/
    // end
    painter.drawLine(xright, ybottom, xright, ybottom+5);
    if (plotsContainTimeScale()) {
        // Time
        const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
                Qt::AlignLeft, timeScale->timeString));
        //painter.drawRect(bbox.translated(xright, ybottom+10));
        painter.drawText(xright-5, ybottom+10, bbox.width(), bbox.height(), Qt::AlignCenter,
                timeScale->timeString);
        //qDebug() << timeScale->timeString;
    } else {
        // Iterations
        const QRect bbox(painter.boundingRect(QRect(0, 0, 0, 0),
                Qt::AlignLeft, QString("%1 iterations").arg(topIteration)));
        //painter.drawRect(bbox.translated(xright, ybottom+10));
        painter.drawText(xright-5, ybottom+10, bbox.width(), bbox.height(), Qt::AlignCenter,
                QString("%1 iterations").arg(topIteration));
    }
    /* Draw graph sides */
    painter.setPen(QPen(Qt::black));
    painter.drawLine(xleft, ytop, xleft, ybottom);
    painter.drawLine(xleft, ybottom, xright, ybottom);

    int last_valid_x;
    int last_valid_y;
    bool found_valid_point;
    /* Draw data */
    for (int j = 0; j < plots.count(); j ++) {
        painter.setPen(QPen(plots.at(j)->getColour()));
        last_valid_x = 0;
        last_valid_y = 0;
        found_valid_point = false;
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
                if (*style == 0 || *style == 2) {
                    if (i == 0)
                        painter.drawPoint(x1, y1);
                    else
                        painter.drawLine(x1, y1, last_valid_x, last_valid_y);
                    last_valid_x = x1;
                    last_valid_y = y1;
                }
                /* points style */
                if (*style == 1 || *style == 2) {
                    drawStylePoint(j%6, 2, x1, y1, &painter);
                }
                /* dots style */
                if (*style == 3) {
                    painter.drawPoint(x1, y1);
                }
            }
        }
        painter.drawText(xright+5, ybottom-(((data[j].back()/
                static_cast<double>(topValue)))*(ybottom-ytop))+5,
                QString("%1").arg(data[j].back()));
    }
}

bool GraphWidget::plotsContainTimeScale() {
    bool ts = false;
    for (int j = 0; j < plots.count(); j ++) {
        if(plots.at(j)->getXaxis() == "time scale") ts = true;
    }
    return ts;
}

void GraphWidget::updateData(int it) {
    int count;

    if (it > topIteration) topIteration = it;

    for (int j = 0; j < plots.count(); j++) {
        count = 0;

        for (int i = 0; i < agents->count(); i++) {
            if (QString::compare(agents->at(i)->agentType,
                    plots.at(j)->getYaxis()) == 0) {
                /* If a condition is enabled then check it */
                if (plots.at(j)->condition().enable) {
                    for (int k = 0; k < agents->at(i)->tags.count(); k++) {
                        if (QString::compare(plots.at(j)->condition().variable,
                                agents->at(i)->tags.at(k)) == 0) {
                            if (plots.at(j)->condition().op == "==" &&
                                    agents->at(i)->values.at(k).toDouble() ==
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "!=" &&
                                    agents->at(i)->values.at(k).toDouble() !=
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == ">" &&
                                    agents->at(i)->values.at(k).toDouble() >
                            plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "<" &&
                                    agents->at(i)->values.at(k).toDouble() <
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == ">=" &&
                                    agents->at(i)->values.at(k).toDouble() >=
                                    plots.at(j)->condition().value) count++;
                            else if (plots.at(j)->condition().op == "<=" &&
                                    agents->at(i)->values.at(k).toDouble() <=
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
        case Qt::Key_A:
            emit(signal_toggleAnimation());
            break;
        default:
            event->ignore();
            break;
    }
}
