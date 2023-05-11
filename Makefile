# a: main.cpp
# 	g++ -o a.out main.cpp

# a: main.cpp MoPipeMode.cpp Database.cpp Mo.cpp CliMo.cpp AdapterCli.cpp CliContext.cpp MyString.cpp BitMap.cpp Value.cpp MoEthernet.cpp PlaceBuffer.cpp
# 	g++ -std=c++17 -Wfatal-errors -o a.out main.cpp MoPipeMode.cpp Database.cpp Mo.cpp CliMo.cpp AdapterCli.cpp CliContext.cpp MyString.cpp BitMap.cpp Value.cpp MoEthernet.cpp PlaceBuffer.cpp

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
	g++ -g -std=c++17 -Wfatal-errors -o mo.bin $(CPPLIST) && echo "mo.bin is created"
