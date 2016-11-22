/*!
 * \file main.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of \c main()
 */
#include <QtGui/QApplication>
#include "./mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

/*!
 * \mainpage FLAME Visualiser
 *
 * \section OVERVIEW Overview
 * The FLAME Visualiser is a generic visualiser for FLAME models that use the FLAME framework (http://www.flame.ac.uk).
 *
 * \section DESIGN Design
 * The GUI is split into 4 main sections:
 * -# The location of data
 * -# The visual rules
 * -# The graph plots
 * -# The iteration settings
 *
 * \section DATASTRUCT Data Structures
 * -# The visual rules table class \c VisualSettingsModel is an implementation of \c QAbstractTableModel
 *    - which holds objects of type \c VisualSettingsItem
 * -# The visual window class \c GLWidget is an implementation of \c QGLWidget
 *    - which can render OpenGL graphics
 * -# The image settings dialog class \c ImagesDialog handles the saving of images from the visual window
 * -# The graph plots table class \c GraphSettingsModel is an implementation of \c QAbstractTableModel
 *    - which holds objects of type \c GraphSettingsItem
 * -# The time settings dialog class \c TimeDialog handles the time associated with one time step
 * -# The rescrict axes dialog class \c RestrictAxesDialog handles the restriction of drawing agents on different axes
 *
 * The \c VisualSettingsItem is used to represent visual rules and contains:
 * - agent type
 * - \c Condition condition
 * - \c Position x
 * - \c Position y
 * - \c Position z
 * - \c Shape shape
 * - colour
 *
 * The \c GraphSettingsItem is used to represent graph plots and contains:
 * - graph name
 * - x axis
 * - y axis
 * - \c Condition condition
 * - colour
 * - enable
 *
 * \section DELEGATES Object Delegates
 * A delegate class handles the interaction of the user with editing objects within cells of table.
 * A dialog class can be associated with the editing of certain objects.
 *
 * -# Visual rules are represented by the class \c VisualSettingsItem.
 * It has associated delegates:
 * - \c AgentTypeDelegate
 * - \c ConditionDelegate
 * - \c PositionDelegate
 * - \c ShapeDelegate
 * - \c ColourDelegate (only used to draw within the cell)
 *
 * -# Graph plots are represented by the class \c GraphSettingsItem.
 * It has associated delegates:
 * - \c GraphDelegate
 * - \c AgentTypeDelegate
 * - \c ConditionDelegate
 * - \c ColourDelegate (only used to draw within the cell)
 *
 * \section ITERATION Ways to change the iteration
 * Main Window
 * -# Click Back and Forward buttons
 *  - back calls \c decrement_iteration
 *  - forward calls \c increment_iteration
 * -# Change number in spin box
 *  - calls \c on_spinBox_valueChanged
 * -# Press Z and X keys
 *   - Z calls \c decrement_iteration
 *   - X calls \c increment_iteration
 * -# Click Animation button
 *  - calls \c on_pushButton_Animate_clicked
 *   - calls \c slot_stopAnimation or \c slot_startAnimation
 *   - emits \c startAnimation or \c stopAnimation
 *    - calls \c startAnimation or \c stopAnimation in visual window
 * -# Press A
 *  - calls \c on_pushButton_Animate_clicked
 * Visual Window
 * -# Press Z and X keys
 *  - Z emits \c decrease_iteration
 *   - calls \c decrement_iteration in main window
 *  - X emits \c increase_iteration
 *   - calls \c increment_iteration in main window
 * -# Press A key
 *  - switched \c animation boolean
 *  - emits \c signal_stopAnimation or \c signal_startAnimation
 *   - calls \c slot_stopAnimation or \c slot_startAnimation in main window
 * -# Animation running
 *  - emits \c increase_iteration
 */
