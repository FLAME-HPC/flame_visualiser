QT_464    = /usr/bin/qmake-4.6
QT_474    = /Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481    = /Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR = -spec macx-g++ test_flame_visualiser.pro
SOURCES   = qtest/test_flame_visualiser.cpp \
            ../main.cpp\
            ../mainwindow.cpp \
            ../glwidget.cpp \
            ../zeroxmlreader.cpp \
            ../visualsettingsmodel.cpp \
            ../visualsettingsitem.cpp \
            ../configxmlreader.cpp \
            ../agenttypedelegate.cpp \
            ../shapedelegate.cpp \
            ../shapedialog.cpp \
            ../shape.cpp \
            ../colourdelegate.cpp \
            ../positiondelegate.cpp \
            ../positiondialog.cpp \
            ../position.cpp \
            ../condition.cpp \
            ../conditiondelegate.cpp \
            ../conditiondialog.cpp \
            ../graphsettingsmodel.cpp \
            ../graphsettingsitem.cpp \
            ../graphwidget.cpp \
            ../enableddelegate.cpp \
            ../graphdelegate.cpp \
            ../imagesdialog.cpp \
            ../timedialog.cpp \
            ../agentdialog.cpp \
            ../restrictaxesdialog.cpp \
            ../iterationinfodialog.cpp
HEADERS   = ../mainwindow.h \
            ../glwidget.h \
            ../zeroxmlreader.h \
            ../agent.h \
            ../visualsettingsmodel.h \
            ../visualsettingsitem.h \
            ../configxmlreader.h \
            ../agenttype.h \
            ../agenttypedelegate.h \
            ../shapedelegate.h \
            ../shapedialog.h \
            ../shape.h \
            ../colourdelegate.h \
            ../positiondelegate.h \
            ../positiondialog.h \
            ../position.h \
            ../condition.h \
            ../conditiondelegate.h \
            ../conditiondialog.h \
            ../graphsettingsmodel.h \
            ../graphsettingsitem.h \
            ../graphwidget.h \
            ../enableddelegate.h \
            ../graphdelegate.h \
            ../imagesdialog.h \
            ../timedialog.h \
            ../timescale.h \
            ../agentdialog.h \
            ../restrictaxesdialog.h \
            ../dimension.h \
            ../iterationinfodialog.h
EXECUTABLE = test_flame_visualiser.app/Contents/MacOS/test_flame_visualiser
CPPLINT    = /Users/stc/workspace/flame/flame_visualiser/tags/3/tests/ccplint/cpplint.py
CCCC       = cccc

lint:
	$(CPPLINT) $(SOURCES) $(HEADERS)

4.8:
	cd qtest; \
	echo "qmake ..."; \
	$(QT_481) $(QMAKE_PAR); \
	echo "make ..."; \
	make > /dev/null; \
	./$(EXECUTABLE)

4.7:
	cd qtest; \
	echo "qmake ..."; \
	$(QT_474) $(QMAKE_PAR); \
	echo "make ..."; \
	make > /dev/null; \
	./$(EXECUTABLE)

clean:
	rm -rf cccc; \
	cd qtest; \
	echo "clean up ..."; \
	make distclean > /dev/null; \
	rm *.png

memtest: 4.8
	cd qtest; \
	valgrind --leak-check=full ./$(EXECUTABLE)

CCCCSUMMARY = /Users/stc/workspace/flame/scripts/ccccsummary.py
cccc:
	cccc $(SOURCES) $(HEADERS) --outdir=cccc; \
	$(CCCCSUMMARY) cccc

.PHONY: cccc