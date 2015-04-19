
#include <algorithm>                 // for std::for_each
#include <iostream>                  // for std::cout
#include <string>
#include <unordered_map>
#include <utility>                   // for std::pair
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

using namespace boost;
using namespace std;


PolicyForTest::PolicyForTest(std::string folder) {
  loadFromFolder(folder);
}

void PolicyForTest::run() {
  cout << "---Policy test---" << endl;
  cout << request << endl;
}

int PolicyForTest::loadFromFolder(std::string folder) {
  actors.loadFromFileF2(folder + "actors.dat");
  objects.loadFromFileF2(folder + "objects.dat");
  loadRulesFromFile(folder + "rules.dat");
  loadRequestsFromFile(folder + "requests.dat");
  return 0;
}

int PolicyForTest::loadActorsFromFile(std::string fileName) {
  return actors.loadFromFileF2(fileName);
}

int PolicyForTest::loadObjectsFromFile(std::string fileName) {
  return objects.loadFromFileF2(fileName);
}

int PolicyForTest::loadRulesFromFile(std::string fileName) {
  std::ifstream data(fileName);
  if(!data) {
    std::cerr << "No " << fileName << " file" << std::endl;
    return -1;
  }

  std::vector<std::string> splitVec;
  bimap<int, std::string> verticesBimap;
  std::vector<pair<int, int> > edges;

  int l, priority, ruleId, actorId, objectId;
  bool permission;
  for (std::string line; std::getline(data, line);) {

    std::size_t found = line.find("next");
    if (found != std::string::npos) {
      break;
    }

    found = line.find("#");
    if (found != std::string::npos) {
      continue;
    }

    splitVec.clear();
    split(splitVec, line, is_any_of(";"), token_compress_on);
    l = splitVec.size();
    if(l!=6) {
      continue;
    }

    bm_type::right_const_iterator rIt;

    ruleId = atoi(splitVec[0].c_str());
    priority = atoi(splitVec[1].c_str());
    rIt = actors.namesBimap.right.find(splitVec[2]);
    if(rIt == actors.namesBimap.right.end()) {
      cout << "Actor " << splitVec[2] << " not found " << endl;
      continue;
    } else {
      actorId = actors.namesBimap.right.find(splitVec[2])->second;
    }

    rIt = objects.namesBimap.right.find(splitVec[3]);
    if(rIt == objects.namesBimap.right.end()) {
      cout << "Object" << splitVec[3] << " not found " << endl;
      continue;
    } else {
      objectId = objects.namesBimap.right.find(splitVec[3])->second;
    }

    permission = splitVec[4] == "permission";

    Rule r(ruleId, priority, actorId, objectId, permission);
    rulesM.insert(std::pair<int, Rule>(ruleId, r));
  }

  return 0;
}


int PolicyForTest::loadRequestsFromFile(std::string fileName) {
  std::ifstream data(fileName);
  if(!data) {
    std::cerr << "No " << fileName << " file" << std::endl;
    return -1;
  }

  std::vector<std::string> splitVec;
  int l;

  for (std::string line; std::getline(data, line);) {
    std::size_t found = line.find("next");
    if (found != std::string::npos) {
      break;
    }
    found = line.find("#");
    if (found != std::string::npos) {
      continue;
    }

    splitVec.clear();
    split(splitVec, line, is_any_of(";"), token_compress_on);
    l = splitVec.size();
    if(l!=2) {
      continue;
    }
    cout << "here " << splitVec[0] << endl;
    request = Request(actorStringToId(splitVec[0]),
                      objectStringToId(splitVec[1])/*,
                      this*/);
    cout << request << endl;
    break;
  }
  return 0;
}

void PolicyForTest::printRules() {
  std::unordered_map<int, Rule>::iterator it;
  for(it = rulesM.begin(); it != rulesM.end(); it++) {
    cout << it->second << endl;
  }
}

