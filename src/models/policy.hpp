#ifndef POLICY_H
#define POLICY_H

#include <algorithm>                 // for std::for_each
#include <iostream>                  // for std::cout
#include <string>
#include <unordered_map>
#include <utility>                   // for std::pair
#include <vector>

#include "./../utils/vectors.hpp"
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
public:
typedef bimap <int, std::string> bm_type;
typedef std::unordered_map <int, Rule> um_type;
typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
typedef adjacency_list <listS, listS, directedS, Index> Graph;
typedef graph_traits <Graph>::vertex_descriptor vertex_t;


  Policy() {}

  void addActorVertices(std::vector<std::string> actorsVector);

  void addObjectVertices(std::vector<std::string> objectsVector);

  void addActorEdge(int i, int j);

  void addObjectEdge(int i, int j);

  void addActorEdges(std::vector< pair<int, int> > v);

  void addObjectEdges(std::vector< pair<int, int> > v);

  void addRule(Rule& r);

  void addRules(std::vector<Rule> v);

  void changeRules(std::vector<Rule> v);

  std::vector<int> effectiveRules(int actor, int object);

  std::vector<int> highestRules(std::vector<int> rules);

  std::vector<int> deepestRules(std::vector <int> rules, int actor);

  bool sumModalities(std::vector<int> rulesId);

  bool isAllowed(int actorId, int objectId, std::string &str);

  bool isAllowed(std::string actor, std::string object, std::string &str);

  int actorStringToId(std::string &str);

  int objectStringToId(std::string &str);

  std::string actorIdToString(int id) const;

  std::string objectIdToString(int id) const;

  Hierarchy actors;
  Hierarchy objects;
  std::unordered_map<int, Rule> rulesM;

private:
};

#endif
