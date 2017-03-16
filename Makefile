OBJS 	= main.o index.o buffer.o list_node.o graph.o DynHash.o grail.o Component.o SCC.o HyperGraph.o ComponentWCC.o WCC.o  graph_manager.o scheduler.o iSearch.o
SOURCE	= main.cpp index-buffer/index.cpp index-buffer/buffer.cpp index-buffer/list_node.cpp graph/graph.cpp data/DynHash.cpp grail/grail.cpp scc/Component.cpp scc/SCC.cpp grail/HyperGraph.cpp wcc/ComponentWCC.cpp wcc/WCC.cpp graph/graph_manager.cpp scheduler/scheduler.cpp graph/iSearch.cpp
HEADER  = headers/index.h headers/list_node.h headers/buffer.h headers/graph.h headers/DynHash.h headers/grail.h headers/Component.h headers/SCC.h headers/HyperGraph.h headers/ComponentWCC.h headers/WCC.h headers/scheduler.h headers/graph_manager.h headers/iSearch.h
OUT  	= project
CC		= g++
FLAGS   = -g -c -lpthread -std=c++11 -Wall

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ -lpthread

# create/compile the individual files >>separately<< 
main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

index.o: index-buffer/index.cpp headers/index.h
	$(CC) $(FLAGS) index-buffer/index.cpp

buffer.o: index-buffer/buffer.cpp headers/buffer.h
	$(CC) $(FLAGS) index-buffer/buffer.cpp

list_node.o: index-buffer/list_node.cpp headers/list_node.h
	$(CC) $(FLAGS) index-buffer/list_node.cpp

graph.o: graph/graph.cpp headers/graph.h
	$(CC) $(FLAGS) graph/graph.cpp

DynHash.o: data/DynHash.cpp headers/DynHash.h
	$(CC) $(FLAGS) data/DynHash.cpp

grail.o: grail/grail.cpp headers/grail.h
	$(CC) $(FLAGS) grail/grail.cpp

Component.o: scc/Component.cpp headers/Component.h
	$(CC) $(FLAGS) scc/Component.cpp

SCC.o: scc/SCC.cpp headers/SCC.h
	$(CC) $(FLAGS) scc/SCC.cpp

HyperGraph.o: grail/HyperGraph.cpp headers/HyperGraph.h
	$(CC) $(FLAGS) grail/HyperGraph.cpp


ComponentWCC.o: wcc/ComponentWCC.cpp headers/ComponentWCC.h
	$(CC) $(FLAGS) wcc/ComponentWCC.cpp


WCC.o: wcc/WCC.cpp headers/WCC.h
	$(CC) $(FLAGS) wcc/WCC.cpp

graph_manager.o: graph/graph_manager.cpp headers/graph_manager.h
	$(CC) $(FLAGS) graph/graph_manager.cpp

scheduler.o: scheduler/scheduler.cpp headers/scheduler.h
	$(CC) $(FLAGS) scheduler/scheduler.cpp

iSearch.o: graph/iSearch.cpp headers/iSearch.h
	$(CC) $(FLAGS) graph/iSearch.cpp

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)
