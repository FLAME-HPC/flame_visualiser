/*!
 * \file test_flame_visualiser.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Test suite for flame visualiser
 */
#include <QtTest/QtTest>
#include <QtGui/QApplication>
#include <QFileDialog>
#include "./mainwindow.h"

class TestVisualiser: public QObject {
    Q_OBJECT

  signals:
    void new_config_file();

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void create_new_config();
    void open_a_config();
    void save_a_config();
    void open_an_iteration();
    void adding_agent_types();

  private:
    MainWindow w;
    int rc;
};

void TestVisualiser::initTestCase() {
    /* Show main window */
    w.show();
}

void TestVisualiser::cleanupTestCase() {
    /* Close main window */
    w.close();
}

void TestVisualiser::create_new_config() {
    rc = w.create_new_config_file("");
    QCOMPARE(rc, 1);

    /* Cannot test unwritable file (at least across platforms) */
    /*rc = w.create_new_config_file("");
    QCOMPARE(rc, 2);*/

    rc = w.create_new_config_file("tests/models/new_config_file.xml");
    QCOMPARE(rc, 0);

    if (!QFile::remove("tests/models/new_config_file.xml")) {
        QWARN("Could not delete test file: tests/models/new_config_file.xml");
    }

    w.close_config_file();
}

void TestVisualiser::open_a_config() {
    rc = w.readConfigFile("tests/models/missing.xml", 0);
    QCOMPARE(rc, 1);

    rc = w.readConfigFile(
        "tests/models/malformed_config_xml/visual_config.xml", 0);
    QCOMPARE(rc, 2);

    rc = w.readConfigFile("tests/models/malformed_1_xml/visual_config.xml", 0);
    QCOMPARE(rc, 0);

    w.close_config_file();
}

void TestVisualiser::save_a_config() {
    rc = w.save_config_file_internal("");
    QCOMPARE(rc, 1);

    /* Cannot test unwritable file (at least across platforms) */
    /*rc = w.save_config_file_internal("");
    QCOMPARE(rc, 2);*/

    rc = w.save_config_file_internal("tests/models/new_config_file.xml");
    QCOMPARE(rc, 0);

    if (!QFile::remove("tests/models/new_config_file.xml")) {
        QWARN("Could not delete test file: tests/models/new_config_file.xml");
    }

    w.close_config_file();
}

void TestVisualiser::open_an_iteration() {
    rc = w.readConfigFile("tests/models/malformed_1_xml/visual_config.xml", 0);
    QCOMPARE(rc, 0);

    // Correct 0.xml
    w.iteration = 0;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);

    // Malformed 1.xml
    w.iteration = 1;
    rc = w.readZeroXML();
    QCOMPARE(rc, 2);

    // Non existent 2.xml
    w.iteration = 2;
    rc = w.readZeroXML();
    QCOMPARE(rc, 1);

    w.close_config_file();
}

void TestVisualiser::adding_agent_types() {
    rc = w.readConfigFile("tests/models/new_agent_types_added/visual_config.xml", 0);
    QCOMPARE(rc, 0);

    w.iteration = 0;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);
    QCOMPARE(w.agentTypes.size(), 1);
    QCOMPARE(w.agentTypes.at(0).name, QString("a"));

    w.iteration = 1;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);
    QCOMPARE(w.agentTypes.size(), 2);
    QCOMPARE(w.agentTypes.at(1).name, QString("b"));

    w.iteration = 2;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);
    QCOMPARE(w.agentTypes.size(), 3);
    QCOMPARE(w.agentTypes.at(2).name, QString("c"));

    w.iteration = 3;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);
    QCOMPARE(w.agentTypes.size(), 5);
    QCOMPARE(w.agentTypes.at(3).name, QString("d"));
    QCOMPARE(w.agentTypes.at(4).name, QString("e"));

    w.iteration = 4;
    rc = w.readZeroXML();
    QCOMPARE(rc, 0);
    QCOMPARE(w.agentTypes.size(), 5);

    w.close_config_file();
}

QTEST_MAIN(TestVisualiser)
#include "test_flame_visualiser.moc"
