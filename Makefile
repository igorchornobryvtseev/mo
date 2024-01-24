INCDIR = inc    # include dir

CPPLIST = main.cpp \
        AdapterCli.cpp \
        BitMap.cpp \
        CliContext.cpp \
        CliMo.cpp \
        Database.cpp \
        Mo.cpp \
        MoEthernet.cpp \
        MyString.cpp \
        PlaceBuffer.cpp \
        Value.cpp \
        MoPipeMode.cpp \
        MoSyncE.cpp \
        MoAaa.cpp

OBJLIST=$(CPPLIST:%.cpp=%.o)
OBJLIST=$(CPPLIST:.cpp=.o)

all: $(CPPLIST)
	g++ -g -std=c++17 -Wno-narrowing -o mo.bin -I $(INCDIR) $(CPPLIST) && echo "mo.bin is created"
