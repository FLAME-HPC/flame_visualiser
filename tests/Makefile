QT_464    = /usr/bin/qmake-4.6
QT_474    = /Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481    = /Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR = -spec macx-g++ "CONFIG+=test" flame_visualiser.pro
EXECUTABLE = "FLAME Visualiser.app/Contents/MacOS/FLAME Visualiser"
CPPLINT    = /Users/stc/workspace/flame/scripts/cpplint-qt.py

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
	make > /dev/null; \
	./$(EXECUTABLE)

clean:
	rm -rf cccc; \
	cd ..; \
	echo "clean up ..."; \
	make distclean > /dev/null

memtest: 4.8
	cd ..; \
	valgrind --leak-check=full ./$(EXECUTABLE)

CCCCSUMMARY = /Users/stc/workspace/flame/scripts/ccccsummary.py
cccc:
	cccc ../*.cpp ../*.h --outdir=cccc; \
	$(CCCCSUMMARY) cccc

.PHONY: cccc