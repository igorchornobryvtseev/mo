INCDIR = inc    # include dir

CPPLIST = main.cpp \
        AdapterCli.cpp \
        BitMap.cpp \
        CliContext.cpp \
        CliMo.cpp \
        Database.cpp \
        Mo.cpp \
        MoEthernet.cpp \
        MoPipeMode.cpp \
        MyString.cpp \
        PlaceBuffer.cpp \
        Value.cpp

all: $(CPPLIST)
	g++ -g -std=c++17 -Wfatal-errors -o mo.bin -I $(INCDIR) $(CPPLIST) && echo "mo.bin is created"
