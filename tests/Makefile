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
OS        = win
QT_481    = "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\qmake.exe"
QT_474    = "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\qmake.exe"
QMAKE_PAR = "CONFIG+=test" "CONFIG+=release" flame_visualiser.pro
EXECUTABLE = "release\FLAME Visualiser.exe"
endif

all: 4.8

lint:
	$(CPPLINT) ../*.cpp ../*.h

4.8:
ifeq ($(OS),win)
	cd ..; \
	$(QT_481) $(QMAKE_PAR); \
	make; \
	cp "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\QtCore4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\QtGui4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\QtOpenGL4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.8.1\mingw\bin\QtTest4.dll" release; \
	./$(EXECUTABLE)
else
	cd ..; \
	$(QT_481) $(QMAKE_PAR); \
	make; \
	./$(EXECUTABLE)
endif

4.7:
ifeq ($(OS),win)
	cd ..; \
	$(QT_474) $(QMAKE_PAR); \
	make; \
	cp "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\QtCore4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\QtGui4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\QtOpenGL4.dll" release; \
	cp "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\QtTest4.dll" release; \
	mv "release\FLAME_Visualiser.exe" "release\FLAME Visualiser.exe"; \
	./$(EXECUTABLE)
else
	cd ..; \
	$(QT_474) $(QMAKE_PAR); \
	make; \
	./$(EXECUTABLE)
endif

clean:
ifeq ($(OS),win)
	rm -rf cccc; \
	cd ..; \
	rm -rf debug; \
	rm -rf release
else
	rm -rf cccc; \
	cd ..; \
	make distclean; \
	rm .flamevisualisersettings
endif

memtest:
	cd ..; \
	valgrind --leak-check=full ./$(EXECUTABLE)

CCCCSUMMARY = /Users/stc/workspace/flame/scripts/ccccsummary.py
cccc:
	cccc ../*.cpp ../*.h --outdir=cccc; \
	$(CCCCSUMMARY) cccc

.PHONY: cccc