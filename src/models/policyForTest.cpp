
#include <algorithm>      // for std::for_each
#include <iostream>       // for std::cout
#include <string>
#include <unordered_map>
#include <utility>        // for std::pair
#include <vector>


#include "policyForTest.hpp"

#include "request.hpp"

/* Order is important ! */
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <cppunit/TestAssert.h>

using namespace boost;
using namespace std;


PolicyForTest::PolicyForTest(std::string folder): folder(folder) {
  loadFromFolder(folder);
}

void PolicyForTest::run() {
  cout << "---Policy test---" << endl;

  while(request->next()) {
    std::cout << "Request from : " << request->actor << " on "
              << request->object << std::endl;
    printRules();

    actors.toDotFile(folder + "actors");
    objects.toDotFile(folder + "objects");
    std::vector<int> ruleIds = effectiveRules(request->actorId, request->objectId);
    std::cout << "EffectiveRules:" << ruleIds << std::endl;
    std::cout << "Expected EffectiveRules:" << request->expEffectiveRules << std::endl;
    CPPUNIT_ASSERT(vectorSameValues(ruleIds, request->expEffectiveRules));
    ruleIds = deepestRules(ruleIds, request->actorId);
    std::cout << "Deepest:" << ruleIds << std::endl;
    std::cout << "Expected deepest:" << request->expDeepestRules << std::endl;
    CPPUNIT_ASSERT(vectorSameValues(ruleIds, request->expDeepestRules));

    std::cout << "IsAllowed:" << sumModalities(ruleIds) << std::endl;
    std::cout << "Expected isAllowed:" << request->expIsAllowed << std::endl;
    CPPUNIT_ASSERT(sumModalities(ruleIds) == request->expIsAllowed);
  }

}

int PolicyForTest::loadFromFolder(std::string folder) {
  std::ifstream *actorsData = new std::ifstream(folder + "actors.dat");
  if(!actorsData) {
    std::cerr << "No " << folder + "actors.dat file" << std::endl;
    return -1;
  }

  std::ifstream *objectsData = new std::ifstream(folder + "objects.dat");
  if(!objectsData) {
    std::cerr << "No " << folder + "objects.dat file" << std::endl;
    return -1;
  }

  std::ifstream *rulesData = new std::ifstream(folder + "rules.dat");
  if(!rulesData) {
    std::cerr << "No " << folder + "rules.dat file" << std::endl;
    return -1;
  }

  std::ifstream *expectedData = new std::ifstream(folder + "expected.dat");
  if(!expectedData) {
    std::cerr << "No " << folder + "expected.dat file" << std::endl;
    return -1;
  }


  std::ifstream *requestsData = new std::ifstream(folder + "requests.dat");
  if(!requestsData) {
    std::cerr << "No " << folder + "requests.dat file" << std::endl;
    return -1;
  }
  request = new Request(actorsData, objectsData, rulesData,
                        requestsData, expectedData, this);
  return 0;
}


void PolicyForTest::printRules() {
  std::unordered_map<int, Rule>::iterator it;
  for(it = rulesM.begin(); it != rulesM.end(); it++) {
    cout << it->second << endl;
  }
}

