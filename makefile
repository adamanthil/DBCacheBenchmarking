
SRC = Application.cpp \
	FileManager.cpp \
	DiskPage.cpp \
	MemoryBlock.cpp \
	FileDescriptor.cpp \
	Table.cpp \
	SequentialScan.cpp \
	JoinOperator.cpp \
	NestedBlockJoin.cpp \
	BufferManager.cpp \
	IRelationalOperator.cpp \
	Schema.cpp \
	Tuple.cpp \
	Projection.cpp \
	Attribute.cpp \
	BooleanExpression.cpp \
	Operand.cpp \
	Clause.cpp \
	PageLayout.cpp \
	DataCreator.cpp \
	MergeJoin.cpp \
	Query.cpp

all:
	g++ -g -O0 $(SRC) 

clean:
	rm -rf *.o *~ *.out *.exe *.prog
