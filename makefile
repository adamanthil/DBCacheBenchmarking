
SRC = Application.cpp \
	FileManager.cpp \
	DiskPage.cpp \
	MemoryBlock.cpp \
	FileDescriptor.cpp \
	SequentialScan.cpp \
	BufferManager.cpp \
	IRelationalOperator.cpp \
	Predicate.cpp \
	Schema.cpp \
	Tuple.cpp \
	Projection.cpp \
	Attribute.cpp

all:
	g++ -g -O0 $(SRC)


