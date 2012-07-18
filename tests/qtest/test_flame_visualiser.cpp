/*!
 * \file test_flame_visualiser.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Test suite for flame visualiser
 */
#include <QtTest/QtTest>
#include <QtGui/QApplication>
#include "../../mainwindow.h"

class TestVisualiser: public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void open_missing_model();
    void open_malformed_model();
    void open_missing_submodel();
    void open_malformed_submodel();

  private:
    MainWindow w;
};

void TestVisualiser::initTestCase() {
    /* Show main window */
    w.show();
}

void TestVisualiser::cleanupTestCase() {
    /* Close main window */
    w.close();
}

void TestVisualiser::open_missing_model() {
    int rc = w.openModel_internal("../models/missing.xml", true);
    QVERIFY(rc == 1);
}

void TestVisualiser::open_malformed_model() {
    int rc = w.openModel_internal("../models/malformed_xml.xml", true);
    QVERIFY(rc == 2);
}

void TestVisualiser::open_missing_submodel() {
    int rc = w.openModel_internal("../models/missing_submodel.xml", true);
    QVERIFY(rc == 2);
}

void TestVisualiser::open_malformed_submodel() {
    int rc = w.openModel_internal("../models/malformed_submodel.xml", true);
    QVERIFY(rc == 2);
}

void TestVisualiser::open_test_model() {
    int rc = w.openModel_internal("../models/conditions.xml", true);
    QVERIFY(rc == 0);
}

QTEST_MAIN(TestVisualiser)
#include "test_flame_visualiser.moc"
