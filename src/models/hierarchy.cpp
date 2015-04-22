#include "hierarchy.hpp"

#include <iostream>                  // for std::cout
#include <typeinfo>
#include <vector>
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <map>
#include <string>
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


Hierarchy::Hierarchy() {
  name[0] = "000";
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

int Hierarchy::loadFromFile(std::string fileName) {
  std::ifstream data(fileName);
  if(!data) {
    std::cerr << "No " << fileName << " file" << std::endl;
    return -1;
  }
  std::string str, line;
  std::vector<std::string> splitVec;

  int count = 0;
  for(std::string line; std::getline(data, line);) {

    std::size_t found = line.find("#");
    if (found == std::string::npos) {
      splitVec.clear();
      split(splitVec, line, is_any_of(" "));
      if (count == 1) {
        std::cout << "ajout de " << atoi(splitVec[0].c_str()) << " "
          << splitVec[1] << std::endl;
        addVertex(atoi(splitVec[0].c_str()), splitVec[1]);
      } else {
        for(unsigned int i=1; i<splitVec.size(); i++) {
          std::cout << atoi(splitVec[0].c_str()) << "->"
          << atoi(splitVec[i].c_str()) << std::endl;
          addEdge(atoi(splitVec[0].c_str()), atoi(splitVec[i].c_str()));
        }
      }
    } else {
      count++;
    }
  }
  return 0;
}


int Hierarchy::loadFromFileF2(std::string fileName) {
  std::ifstream data(fileName);
  if(!data) {
    std::cerr << "No " << fileName << " file" << std::endl;
    return -1;
  }

  int id = 0;
  std::vector<std::string> splitVec;
  bimap<int, std::string> verticesBimap;
  std::vector<pair<int, int> > edges;
  pair<int, int> p;

  for (std::string line; std::getline(data, line);) {
    splitVec.clear();
    split(splitVec, line, is_any_of("->"), token_compress_on);
    int l = splitVec.size();
    // cout << "l:" << l << endl;
    // for(int i=0; i<splitVec.size(); i++) {
    //   cout << "|" << splitVec[i];
    // }
    // cout << endl;

    if(l!=2) {
      continue;
    }
    // cout << "id:" << id << endl;
    // cout << splitVec[0] << " " << splitVec[1] << endl;

    bm_type::right_const_iterator rightIter;
    rightIter = verticesBimap.right.find(splitVec[0]);

    if (rightIter == verticesBimap.right.end()) {
      cout << "Insert (first element)" << id << " " << splitVec[0] << endl;
      verticesBimap.insert(bm_type::value_type(id, splitVec[0]));
      p.first = id;
      id++;
    } else {
      p.first = rightIter->second;
    }

    rightIter = verticesBimap.right.find(splitVec[1]);
    if (rightIter == verticesBimap.right.end()) {
      cout << "Insert (second element)" << id << " " << splitVec[1] << endl;
      verticesBimap.insert(bm_type::value_type(id, splitVec[1]));
      p.second = id;
      id++;
    } else {
      p.second = rightIter->second;
    }
    edges.push_back(p);
  }


  bm_type::left_const_iterator it;
  for(it = verticesBimap.left.begin(); it != verticesBimap.left.end(); it++) {
    // cout << it->first << "<-->" << it->second << endl;
    addVertex(it->first, it->second);
  }

  for(unsigned int i=0; i<edges.size(); i++) {
    cout << edges[i].first << " " << edges[i].second << endl;;
    addEdge(edges[i].first, edges[i].second);
  }


  return 0;
}

void Hierarchy::addVertex(int i) {
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

void Hierarchy::addVertex(int i, const std::string label) {
  addVertex(i);
  namesUmap.insert(std::pair<int, std::string>(i, label));
  namesBimap.insert(bm_type::value_type(i, label));
}

int Hierarchy::addVertex(const std::string label) {
  int i = ++maxId;
  addVertex(i, label);
  return i;
}

std::string Hierarchy::getVertexName(int i) {
  // bm_type::left_const_iterator it;
  // it = namesBimap.left.find(i);
  return namesBimap.left.find(i)->second;
}

int Hierarchy::getVertexId(std::string str) {
  // bm_type::right_const_iterator it;
  // it = namesBimap.right.find(str);
  return namesBimap.right.find(str)->second;
}

void Hierarchy::addEdge(int i, int j) {
  std::map<int, vertex_t>::const_iterator vertexItI = verticesMap.find(i);
  std::map<int, vertex_t>::const_iterator vertexItJ = verticesMap.find(j);
  std::map<int, vertex_t>::const_iterator vertexItRI = verticesMapReverse.find(i);
  std::map<int, vertex_t>::const_iterator vertexItRJ = verticesMapReverse.find(j);

  if (vertexItI == verticesMap.end()) {
    std::cout << "Vertex " << i << std::endl;
    throw std::invalid_argument("Can't find vertex");
  }
  if (vertexItJ == verticesMap.end()) {
    std::cout << "Vertex " << j << std::endl;
    throw std::invalid_argument("Can't find vertex");
  }
  add_edge(vertexItI->second, vertexItJ->second, graph);
  add_edge(vertexItRJ->second, vertexItRI->second, graphReverse);
  isClosureComputed = false;
}


std::vector<std::pair<int, std::vector<int> > > Hierarchy::getVertices(Graph &graph) {
  std::vector<std::pair<int, std::vector<int> > > result;
  std::vector<int> tmp;
  property_map <Graph, vertex_index_t>::type
    index_map = get(vertex_index, graph);
  graph_traits <Graph>::adjacency_iterator ai, a_end;

  int index;
  for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
    tmp.clear();
    index = get(index_map, *vi);
    boost::tie(ai, a_end) = adjacent_vertices(*vi, graph);
    if (ai != a_end) {
      for (; ai != a_end; ++ai) {
        tmp.push_back(get(index_map, *ai));
      }
    }
    result.push_back(std::pair<int, std::vector<int> >(index, tmp));
  }
  return result;
}

void Hierarchy::_printVertices(Graph& graph) {
  std::vector<std::pair<int, std::vector<int> > > result = getVertices(graph);
  for(unsigned int i=0; i<result.size(); i++) {
    std::cout << result[i].first << " ["
         << namesBimap.left.find(result[i].first)->second << "]->";
    int l = result[i].second.size();
    std::cout << "(";
    for(int j=0; j<l; j++) {
      if(j!=0) {
        std::cout << ", ";
      }
      std::cout << result[i].second[j];
    }
    std::cout << ")" << std::endl;
  }

}

void Hierarchy::printVertices() {
  _printVertices(graph);
}

void Hierarchy::printVerticesReverse() {
  _printVertices(graphReverse);
}


  /**
   * Index to vertex_descriptor : vertex(index, graph)
   * vertex_descripor to index : get(index_map, vertexIt)
   */
std::vector<int> Hierarchy::adjacentIndexVertices(int index) {
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

std::vector<int> Hierarchy::inAdjacentIndexVertices(int index) {
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


bool Hierarchy::hasAccess() {
  return true;
}

void Hierarchy::transitiveClosure() {
  transitive_closure(graph, closure);
  transitive_closure(graphReverse, closureReverse);
  // std::cout << std::endl << "Graph Transitive closure+:" << std::endl;
  // char name[] = "0123456789";
  // print_graph(closure, name);
  isClosureComputed = true;
}

adjacency_list<> Hierarchy::getTransitiveClosure() {
  if(!isClosureComputed) {
    transitiveClosure();
  }
  return closure;
}

void Hierarchy::toDotFile(std::string fileName) {
  if(!isClosureComputed) {
    transitiveClosure();
  }
  std::ofstream out(fileName + ".dot");
  std::unordered_map<int, std::string>::iterator it;
  /* A ajouter quand on ajoute un vertex!! */
  for(it = namesUmap.begin(); it != namesUmap.end(); it++) {
    name[it->first] = it->second;
  }
  write_graphviz(out, closure, make_label_writer(name));
}

void Hierarchy::toDotFileFromGraph(std::string fileName) {
  if(!isClosureComputed) {
    transitiveClosure();
  }
  std::ofstream out(fileName + ".dot");
  std::unordered_map<int, std::string>::iterator it;
  for(it = namesUmap.begin(); it != namesUmap.end(); it++) {
    name[it->first] = it->second;
  }
  // write_graphviz(out, closure, make_label_writer(name));
  write_graphviz(out, graph, make_label_writer(name));
}

void Hierarchy::reverseToDotFile(std::string fileName) {
  if(!isClosureComputed) {
    transitiveClosure();
  }
  std::ofstream out(fileName + ".dot");

  std::unordered_map<int, std::string>::iterator it;
  for(it = namesUmap.begin(); it != namesUmap.end(); it++) {
    name[it->first] = it->second;
  }

  write_graphviz(out, closureReverse, make_label_writer(name));
}
