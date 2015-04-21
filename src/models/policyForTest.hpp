#ifndef POLICYFORTEST_H
#define POLICYFORTEST_H

#include <algorithm>                 // for std::for_each
#include <iostream>                  // for std::cout
#include <string>
#include <unordered_map>
#include <utility>                   // for std::pair
#include <vector>

#include "policy.hpp"
#include "request.hpp"

/* Order is important ! */
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


using namespace boost;
using namespace std;

class PolicyForTest: public Policy {

public:
  std::string folder;
  std::ifstream rulesData;
  std::ifstream actorsData;
  std::ifstream objectsData;
  std::ifstream requestsData;
  Request *request;

  PolicyForTest() {}

  PolicyForTest(std::string folder);

  void run();

  int loadFromFolder(std::string folder);

  void printRules();

private:
};

#endif
