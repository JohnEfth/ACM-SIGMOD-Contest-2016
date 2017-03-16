# ACM-SIGMOD-Contest-2016
Optimize the time of update and shortest path search in big graphs.

How to compile and run:

In order to compile write in the command line "make" so that the executable file will be created.
Then you can run the application with the command " ./project > out.txt " (it will write the results in the out.txt file)

How to make configurations:

For the application in order to run it needs to parameters:
-> Initialize Graph
-> Workload with commands to perform on the Graph

To change these parameters you go to the " config.txt " and there you give the name of the Graph and Workload file that you would like to be used. You will find these files in the directory config_files.

Few words about how the application works:

Firstly it creates the graph with adjacency lists. In order to access these lists in O(1) complexity I use an Index table that each cell has a Dynamic Hash Table allowing me to find the neightbors instantly.
Secondly in order to optimize the search of the shortest path I look my workloads from two perspectives, dynamic or static.
If the workload is dynamic the Graph has updates if it is static then has only search.

->If it is dynamic I create Weakly Connected Components so before I start searching I check if the two nodes are in the same component.
->If it is static I create Strongly Connected Components and with them I create a Grail Index which again will allow me to check before I start searching if the two nodes are in the same component.

Search function is created using Bidirectional BFS algorithm.  

Lastly in order to make these procedures faster I use threads , these threads are managed by a thread scheduler that distrubutes the jobs to the threads.

