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
    name[0] = "0";
    name[1] = "1";
    name[2] = "2";
    name[3] = "3";
    name[4] = "4";
    name[5] = "5";
    name[6] = "6";
    name[7] = "7";
    name[8] = "8";
    name[9] = "9";
    name[10] = "10";
    name[11] = "11";
    name[12] = "12";
    name[13] = "13";
    name[14] = "14";
  }

  friend std::ostream& operator<<(std::ostream& os, const Hierarchy& h);

  void addVertex(int i) {
    verticesMap[i] = add_vertex(Index(i, Name('a' + i)), graph);
    verticesMapReverse[i] = add_vertex(Index(i, Name('a' + i)), graphReverse);
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
    add_edge(vertexItJ->second, vertexItI->second, graphReverse);
  }

  void printVertices() {
    property_map <Graph, vertex_index_t>::type
      index_map = get(vertex_index, graph);
    graph_traits <Graph>::adjacency_iterator ai, a_end;

    std::cout << "List of vertices : " << std::endl;
    int index;
    for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
      index = get(index_map, *vi);
      std::cout << index << " " << name[index];
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


  /**
   * Index to vertex_descriptor : vertex(index, graph)
   * vertex_descripor to index : get(index_map, vertexIt)
   */
  std::vector<int> adjacentIndexVertices(int index) {
    std::vector<int> v;
    property_map <Graph, vertex_index_t>::type
      index_map = get(vertex_index, graph);
    Graph::vertex_descriptor vertexIt = vertex(index, graph);

    graph_traits <Graph>::adjacency_iterator ai, a_end;
    boost::tie(ai, a_end) = adjacent_vertices(vertexIt, graph);
    if (ai == a_end) {
      return v;
    }
    for (; ai != a_end; ++ai) {
      v.push_back(get(index_map, *ai));
    }
    return v;
  }

  std::vector<int> inAdjacentIndexVertices(int index) {
    std::vector<int> v;
    property_map <Graph, vertex_index_t>::type
      index_map = get(vertex_index, graphReverse);
    Graph::vertex_descriptor vertexIt = vertex(index, graphReverse);

    graph_traits <Graph>::adjacency_iterator ai, a_end;
    boost::tie(ai, a_end) = adjacent_vertices(vertexIt, graphReverse);
    if (ai == a_end) {
      return v;
    }
    for (; ai != a_end; ++ai) {
      v.push_back(get(index_map, *ai));
    }
    return v;
  }


  // std::vector<int> inEdgeVertices(int index) {
  //   std::vector<int> result;
  //   Graph::in_edge_iterator in_begin, in_end;
  //   Graph::vertex_descriptor vertexIt = vertex(index, graph);
  //   boost::tie(in_begin, in_end) = in_edges(vertexIt, graph);
  //   for (; in_begin != in_end; ++in_begin) {
  //       std::cout << source(*in_begin, graph) << std::endl;
  //   }
  //   // std::cout << std::endl;
  //   return result;
  // }



  bool hasAccess() {
    return true;
  }

  void transitiveClosure() {
    transitive_closure(graph, closure);
    transitive_closure(graphReverse, closureReverse);
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

std::string name[20];
Graph graphReverse;
Graph graph;
private:
  graph_traits<Graph>::vertex_iterator vi, vi_end;
  adjacency_list <> closure;
  adjacency_list <> closureReverse;
  bool isClosureCompute = false;
  std::map<int, vertex_t> verticesMap;
  std::map<int, vertex_t> verticesMapReverse;
};

std::ostream& operator<<(std::ostream& os, const Hierarchy& h) {
  return os;
}

#endif

