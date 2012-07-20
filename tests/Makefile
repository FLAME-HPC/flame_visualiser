# Find OS
UNAME := $(shell uname)
# Linux OS
ifeq ($(UNAME), Linux)
QT_481    = /usr/bin/qmake
QMAKE_PAR = "CONFIG+=test" flame_visualiser.pro
EXECUTABLE = "FLAME Visualiser"
# Mac OS
else ifeq ($(UNAME), Darwin)
QT_474    = /Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481    = /Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR = -spec macx-g++ "CONFIG+=test" flame_visualiser.pro
EXECUTABLE = "FLAME Visualiser.app/Contents/MacOS/FLAME Visualiser"
CPPLINT    = /Users/stc/workspace/flame/scripts/cpplint-qt.py
# Win OS
else ifeq ($(UNAME), MINGW32_NT-5.1)
QT_481    = "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\qmake.exe"
QT_474    = "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\qmake.exe"
QMAKE_PAR = "CONFIG+=test" "CONFIG+=release" flame_visualiser.pro
EXECUTABLE = "release\FLAME Visualiser.exe"
endif

all: 4.8

lint:
	$(CPPLINT) ../*.cpp ../*.h

4.8:
	cd ..; \
	echo "qmake ..."; \
	$(QT_481) $(QMAKE_PAR); \
	echo "make ..."; \
	make; \
	./$(EXECUTABLE)

4.7:
	cd ..; \
	echo "qmake ..."; \
	$(QT_474) $(QMAKE_PAR); \
	echo "make ..."; \
	make; \
	./$(EXECUTABLE)

clean:
	rm -rf cccc; \
	cd ..; \
	echo "clean up ..."; \
	make distclean > /dev/null; \
	rm .flamevisualisersettings

memtest: 4.8
	cd ..; \
	valgrind --leak-check=full ./$(EXECUTABLE)

CCCCSUMMARY = /Users/stc/workspace/flame/scripts/ccccsummary.py
cccc:
	cccc ../*.cpp ../*.h --outdir=cccc; \
	$(CCCCSUMMARY) cccc

.PHONY: cccc