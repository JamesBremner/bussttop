#source file search paths
VPATH = ./src 

# compiler include search paths
INCS = -I./include \
	-I../ravencode/pub/src \
	-I../raven-set \
	-I../KMeans/src \
	-I../../code_ext/boost/boost_1_84_0

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj
RDIR = ../ravencode/pub/src

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ = \
	$(ODIR)/main.o \
	$(ODIR)/KMeans.o $(ODIR)/cDataPoint.o

### COMPILE
$(ODIR)/KMeans.o: ../KMeans/src/KMeans.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)
$(ODIR)/cDataPoint.o: ../KMeans/src/cDataPoint.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

$(ODIR)/%.o: %.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

bats: $(OBJ)
	g++ $(OPT) \
	-o bin/stops.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

