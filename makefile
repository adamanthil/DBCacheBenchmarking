
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
	Column.cpp \
	BooleanExpression.cpp \
	Operand.cpp \
	Clause.cpp \
	PageLayout.cpp \
	DataCreator.cpp \
	MergeJoin.cpp \
	Query.cpp \
	Database.cpp \
	Partition.cpp \
	MaterializationLayout.cpp \
	TupleStreamReader.cpp \
	TupleStreamWriter.cpp \
	parser.cpp \
	scanner.cpp

release:
	g++ -o dblite -O3 -funroll-loops $(SRC) 

debug:
	g++ -g -o dblite.dbg -O0 $(SRC) 

generator:
	g++ -g -o genqueries GenerateQueries.cpp

clean:
	rm -rf *.o *~ *.out *.exe *.prog *dbl*
