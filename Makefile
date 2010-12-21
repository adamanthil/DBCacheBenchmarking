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

all: dblite benchmarking

dblite: $(OBJS) Application.o
	$(CXX) -o dblite ${OBJS} Application.o

benchmarking: genqueries benchmark

genqueries: $(OBJS) GenerateQueries.o 
	$(CXX) -g -o genqueries $(OBJS) GenerateQueries.o

benchmark: Benchmark.o $(OBJS)
	$(CXX) -g -o benchmark $(OBJS) Benchmark.o

GenerateQueries.o: GenerateQueries.cpp
	$(CXX) $(CXXFLAGS) -c GenerateQueries.cpp

Benchmark.o: Benchmark.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -c Benchmark.cpp

Application.o: Application.cpp
	$(CXX) $(CXXFLAGS) -c Application.cpp

cpp.o:
	$(CXX) $(CXXFLAGS) -s $< 

clean:
	rm -rf *.o

