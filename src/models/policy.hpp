#ifndef POLICY_H
#define POLICY_H

#include <algorithm>                 // for std::for_each
#include <iostream>                  // for std::cout
#include <string>
#include <unordered_map>
#include <utility>                   // for std::pair
#include <vector>

#include "rule.hpp"
#include "hierarchy.hpp"

/* Order is important ! */
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>



using namespace boost;
using namespace std;

class Policy {

typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
typedef adjacency_list <listS, listS, directedS, Index> Graph;
typedef graph_traits <Graph>::vertex_descriptor vertex_t;

public:
  Policy() {}

  void addActorVertices(std::vector<std::string> actorsVector) {
    for(unsigned int i = 0; i < actorsVector.size(); i++) {
      actors.addVertex(actorsVector[i]);
    }
  }

  void addObjectVertices(std::vector<std::string> objectsVector) {
    for(unsigned int i = 0; i < objectsVector.size(); i++) {
      objects.addVertex(objectsVector[i]);
    }
  }

  void addActorsEdge(int i, int j) {
    actors.addEdge(i, j);
  }

  void addObjectsEdge(int i, int j) {
    objects.addEdge(i, j);
  }

  void addActorsEdges(std::vector< pair<int, int> > v) {
    int l = v.size();
    for(int i=0; i<l; i++) {
      actors.addEdge(v[i].first, v[i].second);
    }
  }

  void addObjectsEdges(std::vector< pair<int, int> > v) {
    int l = v.size();
    for(int i=0; i<l; i++) {
      objects.addEdge(v[i].first, v[i].second);
    }
  }

  void addRule(Rule& r) {
    rulesM.insert(std::pair<int, Rule>(r.id, r));
  }

  Hierarchy actors;
  Hierarchy objects;
  std::unordered_map<int, Rule> rulesM;

private:
};

#endif
