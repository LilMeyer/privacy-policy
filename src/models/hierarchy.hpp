#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <iostream>
#include <vector>
#include <utility> // for std::pair
#include <unordered_map>

/* Order is important ! */
#include <boost/algorithm/string.hpp>
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/bimap.hpp>

using namespace boost;
using namespace std;

class Hierarchy {
typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
// typedef property< vertex_color_t, default_color_type > Index;

typedef adjacency_list <listS, listS, directedS, Index> Graph;
typedef typename graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph>::vertex_descriptor vertex_t;
typedef bimap< int, std::string> bm_type;

public:
  Hierarchy();

  friend std::ostream& operator<<(std::ostream& os, const Hierarchy& h);

  int loadFromFile(std::string fileName);

  int loadFromFileF2(std::string fileName);

  void addVertex(int i);

  void addVertex(int i, const std::string label);

  /**
   * Add a vertex without specifying an id
   */
  int addVertex(const std::string label);

  std::string getVertexName(int i);

  int getVertexId(std::string str);

  void addEdge(int i, int j);


  std::vector<std::pair<int, std::vector<int> > > getVertices(Graph &graph);

  void _printVertices(Graph& graph);

  void printVertices();

  void printVerticesReverse();

  /**
   * Index to vertex_descriptor : vertex(index, graph)
   * vertex_descripor to index : get(index_map, vertexIt)
   */
  std::vector<int> adjacentIndexVertices(int index);

  std::vector<int> inAdjacentIndexVertices(int index);


  bool hasAccess();

  void transitiveClosure();

  adjacency_list<> getTransitiveClosure();

  void toDotFile(std::string fileName);
  void toDotFileFromGraph(std::string fileName);

  void write_graphviz_p();

  void reverseToDotFile(std::string fileName);

int maxId = -1;
std::string name[20];
std::unordered_map<int, std::string> namesUmap;
bm_type namesBimap;
Graph graphReverse;
Graph graph;

private:
  graph_traits<Graph>::vertex_iterator vi, vi_end;
  adjacency_list <> closure;
  adjacency_list <> closureReverse;
  bool isClosureComputed = false;
  std::map<int, vertex_t> verticesMap;
  std::map<int, vertex_t> verticesMapReverse;
};

#endif

