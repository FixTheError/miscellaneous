In order to run this program, open the project in visual studio and select "start without debugging" from the debug tab. 
You will pe prompted to select an input file, then a source and destination vertex. From there the program will output
the name of the algorithm it will use, the shortest path, and the weight of the shortest path.

Note:
I had trouble trying to figure out how to allow traversal against a directed edge.
DAG shortest path algorithm crashes the program when a destination is chosen which cannot be reached by following outward
edges from the source vertex.