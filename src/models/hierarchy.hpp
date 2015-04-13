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
#include <boost/bimap.hpp>

using namespace boost;
using namespace std;


struct sample_graph_writer {
  void operator()(std::ostream& out) const {
    out << "graph [bgcolor=lightgrey]" << std::endl;
    out << "node [shape=circle color=white]" << std::endl;
    out << "edge [style=dashed]" << std::endl;
  }
};


class Hierarchy {
typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
typedef adjacency_list <listS, listS, directedS, Index> Graph;
typedef typename graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph>::vertex_descriptor vertex_t;
typedef bimap< int, std::string> bm_type;

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
    if(i>maxId) {
      maxId = i;
    }
    /* insert ? */
    verticesMap[i] = add_vertex(Index(i, Name('a' + i)), graph);
    verticesMapReverse[i] = add_vertex(Index(i, Name('a' + i)), graphReverse);

    // Detect if the key already exists !
    // std::pair<MyMap::iterator, bool> res = map.insert(std::make_pair(key,value));
    // if ( ! res.second ) {
    //     cout << "key " <<  key << " already exists "
    //          << " with value " << (res.first)->second << endl;
    // } else {
    //     cout << "created key " << key << " with value " << value << endl;
    // }

  }

  void addVertex(int i, const std::string label) {
    addVertex(i);
    namesUmap.insert(std::pair<int, std::string>(i, label));
    namesBimap.insert(bm_type::value_type(i, label));
  }

  /**
   * Add a vertex without specifying an id
   */
  int addVertex(const std::string label) {
    int i = ++maxId;
    addVertex(i, label);
    return i;
  }

  std::string getVertexName(int i) {
    // bm_type::left_const_iterator it;
    // it = namesBimap.left.find(i);
    return namesBimap.left.find(i)->second;
  }

  int getVertexId(std::string str) {
    // bm_type::right_const_iterator it;
    // it = namesBimap.right.find(str);
    return namesBimap.right.find(str)->second;
  }

  void addEdge(int i, int j) {
    std::map<int, vertex_t>::const_iterator vertexItI = verticesMap.find(i);
    std::map<int, vertex_t>::const_iterator vertexItJ = verticesMap.find(j);
    std::map<int, vertex_t>::const_iterator vertexItRI = verticesMapReverse.find(i);
    std::map<int, vertex_t>::const_iterator vertexItRJ = verticesMapReverse.find(j);

    if (vertexItI == verticesMap.end()) {
      cout << "Vertex " << i << endl;
      throw std::invalid_argument("Can't find vertex");
    }
    if (vertexItJ == verticesMap.end()) {
      cout << "Vertex " << j << endl;
      throw std::invalid_argument("Can't find vertex");
    }
    add_edge(vertexItI->second, vertexItJ->second, graph);
    add_edge(vertexItRJ->second, vertexItRI->second, graphReverse);
    isClosureComputed = false;
  }


  void printVertices() {
    property_map <Graph, vertex_index_t>::type
      index_map = get(vertex_index, graph);
    graph_traits <Graph>::adjacency_iterator ai, a_end;

    std::cout << "List of vertices :" << std::endl;
    int index;
    for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
      index = get(index_map, *vi);
      std::cout << index << " [" << namesBimap.left.find(index)->second << "]";
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

  void printVerticesReverse() {
    property_map <Graph, vertex_index_t>::type
      index_map = get(vertex_index, graphReverse);
    graph_traits <Graph>::adjacency_iterator ai, a_end;

    std::cout << "List of vertices :" << std::endl;
    int index;
    for(boost::tie(vi, vi_end) = vertices(graphReverse); vi != vi_end; ++vi) {
      index = get(index_map, *vi);
      std::cout << index << " [" << namesBimap.left.find(index)->second << "]";
      std::cout << "->";
      boost::tie(ai, a_end) = adjacent_vertices(*vi, graphReverse);
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
    if(!isClosureComputed) {
      transitiveClosure();
    }

    std::vector<int> v;
    property_map <adjacency_list<>, vertex_index_t>::type
      index_map = get(vertex_index, closure);
    adjacency_list<>::vertex_descriptor vertexIt = vertex(index, closure);

    graph_traits <adjacency_list<>>::adjacency_iterator ai, a_end;
    boost::tie(ai, a_end) = adjacent_vertices(vertexIt, closure);
    if (ai == a_end) {
      return v;
    }
    for (; ai != a_end; ++ai) {
      v.push_back(get(index_map, *ai));
    }
    return v;
  }

  std::vector<int> inAdjacentIndexVertices(int index) {
    if(!isClosureComputed) {
      transitiveClosure();
    }

    std::vector<int> v;
    property_map <adjacency_list<>, vertex_index_t>::type
      index_map = get(vertex_index, closureReverse);
    adjacency_list<>::vertex_descriptor vertexIt = vertex(index, closureReverse);

    graph_traits <adjacency_list<> >::adjacency_iterator ai, a_end;
    boost::tie(ai, a_end) = adjacent_vertices(vertexIt, closureReverse);
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
    // std::cout << std::endl << "Graph Transitive closure+:" << std::endl;
    // char name[] = "0123456789";
    // print_graph(closure, name);
    isClosureComputed = true;
  }

  adjacency_list<> getTransitiveClosure() {
    if(!isClosureComputed) {
      transitiveClosure();
    }
    return closure;
  }

  void toDotFile(std::string fileName) {
    if(!isClosureComputed) {
      transitiveClosure();
    }
    std::ofstream out("build/" + fileName + ".dot");
    write_graphviz(out, closure, make_label_writer(name));
  }

  void reverseToDotFile(std::string fileName) {
    if(!isClosureComputed) {
      transitiveClosure();
    }
    std::ofstream out("build/" + fileName + ".dot");
    write_graphviz(out, closureReverse, make_label_writer(name));
  }


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

std::ostream& operator<<(std::ostream& os, const Hierarchy& h) {
  return os;
}

#endif

