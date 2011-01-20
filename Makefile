LINT		= python cpplint.py
LINTFLAGS	= --filter=-legal

LINK		= g++

INCPATH 	= -I/opt/local/include/
LIBS		= -L/opt/local/lib/ -lcxcore -lcv -lhighgui

SOURCES		= main.cpp \
		  detector.cpp \
		  tools.cpp

HEADERS		= detected.h \
		  tools.h

OBJECTS		= $(SOURCES:.cpp=.o)

TARGET		= polyp_detector

.SUFFIXES: .cpp .o
.cpp.o:	
	$(LINT) $(LINTFLAGS) $<
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(TARGET) : $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(OBJCOMP) $(LIBS)

all: $(TARGET)

.PHONY: clean
clean:
	rm *.o
