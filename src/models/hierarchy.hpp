#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <iostream>                  // for std::cout
#include <typeinfo>
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <vector>
#include <map>
#include <string>

/* Order is important ! */
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

class Hierarchy {
typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
typedef adjacency_list <listS, listS, directedS, Index> Graph;
typedef typename graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph>::vertex_descriptor vertex_t;
public:
  Hierarchy() {
    name[0] = "ab";
    name[1] = "b";
    name[2] = "c";
    name[3] = "d";
    name[4] = "e";
    name[5] = "f";
    name[6] = "g";
    name[7] = "h";
    name[8] = "i";
    name[9] = "j";
    name[10] = "k";
    name[11] = "l";
  }

  friend std::ostream& operator<<(std::ostream& os, const Hierarchy& h);

  void addVertex(int i) {
    verticesMap[i] = add_vertex(Index(i, Name('a' + i)), graph);
  }

  void addVertex(int i, std::string label) {
    addVertex(i);
    name[i] = label;
  }

  void addEdge(int i, int j) {
    std::map<int, vertex_t>::const_iterator vertexItI = verticesMap.find(i);
    std::map<int, vertex_t>::const_iterator vertexItJ = verticesMap.find(j);
    if (vertexItI == verticesMap.end()) {
      throw std::invalid_argument("Can't find vertex");
    }
    if (vertexItJ == verticesMap.end()) {
      throw std::invalid_argument("Can't find vertex");
    }
    add_edge(vertexItI->second, vertexItJ->second, graph);
  }

  void printVertices() {
    property_map < Graph, vertex_index_t >::type
      index_map = get(vertex_index, graph);
    graph_traits <Graph >::adjacency_iterator ai, a_end;

    std::cout << "List of vertices : " << std::endl;
    int index;
    for(tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
      index = get(index_map, *vi);
      std::cout << "Id: " << index << " " << name[index];
      std::cout << "->";
      boost::tie(ai, a_end) = adjacent_vertices(*vi, graph);
      if (ai == a_end) {
        std::cout << " ()" << std::endl;
      } else {
        std::cout << " (";
        for (; ai != a_end; ++ai) {
          std::cout << name[get(index_map, *ai)];
          if (boost::next(ai) != a_end) {
            std::cout << ", ";
          }
        }
        std::cout << ")" << std::endl;
      }
    }
  }

  std::vector<int> adjacentIndexVertices(int index) {
  }

  void transitiveClosure() {
    transitive_closure(graph, closure);
    std::cout << std::endl << "Graph Transitive closure+:" << std::endl;
    char name[] = "abcdefghij";
    print_graph(closure, name);
    isClosureCompute = true;
  }

  adjacency_list<> getTransitiveClosure() {
    if(!isClosureCompute) {
      transitiveClosure();
    }
    return closure;
  }

  void saveGraphviz(std::string fileName) {
    transitive_closure(graph, closure);
    std::ofstream out("build/" + fileName + ".dot");
    write_graphviz(out, closure, make_label_writer(name));
  }

private:
  std::string name[20];
  graph_traits<Graph>::vertex_iterator vi, vi_end;
  Graph graph;
  adjacency_list <> closure;
  bool isClosureCompute = false;
  std::map<int, vertex_t> verticesMap;
};

std::ostream& operator<<(std::ostream& os, const Hierarchy& h) {
  return os;
}

#endif

