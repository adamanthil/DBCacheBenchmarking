
SRC = FileManager.cpp \
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
	Parser.cpp \
	Scanner.cpp \
	Settings.cpp

release:
	g++ -o dblite -O3 -funroll-loops Application.cpp $(SRC) 
	g++ -o benchmark -O3 -funroll-loops Benchmark.cpp $(SRC)
	g++ -o genqueries GenerateQueries.cpp

debug:
	g++ -g -o dblite.dbg -O0 Application.cpp $(SRC) 
	g++ -g -o benchmark Benchmark.cpp $(SRC)
	g++ -g -o genqueries GenerateQueries.cpp

# only compile data generator &  benchmark tool
benchmarking:
	g++ -g -o genqueries GenerateQueries.cpp
	g++ -g -o benchmark Benchmark.cpp $(SRC)

clean:
	rm -rf *.o *~ *.out *.exe *.prog *dbl* *genqueries*
