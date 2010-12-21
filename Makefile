.SUFFIXES:
.SUFFIXES: .o .cpp

CXX=g++
CXXFLAGS=-O3 -funroll-loops
SRC=FileManager.cpp \
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

OBJS=$(SRC:.cpp=.o)

dblite: $(OBJS) Application.o
	g++ -o dblite ${OBJS} Application.o

benchmarking: $(OBJS) GenerateQueries.o Benchmark.o
	g++ -g -o genqueries $(OBJS) GenerateQueries.o
	g++ -g -o benchmark $(OBJS) Benchmark.o

GenerateQueries.o: GenerateQueries.cpp
	g++ $(CXXFLAGS) -c GenerateQueries.cpp

Benchmark.o: Benchmark.cpp $(SRC)
	g++ $(CXXFLAGS) -c Benchmark.cpp

Application.o: Application.cpp
	g++ $(CXXFLAGS) -c Application.cpp

cpp.o:
	g++ $(CXXFLAGS) -s $< 

