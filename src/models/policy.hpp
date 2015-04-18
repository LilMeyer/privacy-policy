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

class Policy {

typedef bimap <int, std::string> bm_type;
typedef std::unordered_map <int, Rule> um_type;
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

  void addActorEdge(int i, int j) {
    actors.addEdge(i, j);
  }

  void addObjectEdge(int i, int j) {
    objects.addEdge(i, j);
  }

  void addActorEdges(std::vector< pair<int, int> > v) {
    int l = v.size();
    for(int i=0; i<l; i++) {
      actors.addEdge(v[i].first, v[i].second);
    }
  }

  void addObjectEdges(std::vector< pair<int, int> > v) {
    int l = v.size();
    for(int i=0; i<l; i++) {
      objects.addEdge(v[i].first, v[i].second);
    }
  }

  void addRule(Rule& r) {
    rulesM.insert(std::pair<int, Rule>(r.id, r));
  }

  void addRules(std::vector<Rule> v) {
    int l = v.size();
    for(int i=0; i<l; i++) {
      addRule(v[i]);
    }
  }

  std::vector<int> effectiveRules(int actor, int object) {
    bm_type::left_const_iterator aIt = actors.namesBimap.left.find(actor);
    bm_type::left_const_iterator oIt = objects.namesBimap.left.find(object);
    std::cout << "Demande de " << aIt->second << " sur " << oIt->second << std::endl;
    std::vector<int> result, adjacent = actors.inAdjacentIndexVertices(actor);
    // cout << "adjacent " << adjacent << endl;
    int m = adjacent.size();

    for(um_type::iterator it = rulesM.begin(); it != rulesM.end(); it++) {
      if(it->second.actor == actor) {
        result.push_back(it->second.id);
        continue;
      }
      for(int j=0; j<m; j++) {
        if(adjacent[j] == it->second.actor) {
          result.push_back(it->second.id);
        }
      }
    }
    return result;
  }

  std::vector<int> highestRules(std::vector<int> rules) {
    int l = rules.size();
    if(l<=1) {
      return rules;
    }

    int priority = 100000;
    std::unordered_map<int, Rule>::const_iterator rIt;

    for(int i=0; i<l; i++) {
      rIt = rulesM.find(rules[i]);
      if(rIt != rulesM.end()) {
        if(rIt->second.priority < priority) {
          priority = rIt->second.priority;
        }
      }
    }

    std::vector<int> ruleIds;
    for(int i=0; i<l; i++) {
      rIt = rulesM.find(rules[i]);
      if(rIt != rulesM.end()) {
        if(rIt->second.priority == priority) {
          ruleIds.push_back(rIt->first);
        }
      }
    }
    return ruleIds;
  }

  std::vector<int> deepestRules(std::vector <int> rules, int actor) {
    bm_type::left_const_iterator aIt = actors.namesBimap.left.find(actor);
    std::unordered_map<int, Rule>::const_iterator rIt;
    std::unordered_map<int, std::vector<int> >::iterator it, it0;
    std::cout << "Actor " << aIt->second << "(" << aIt->first << ")" << std::endl;

    std::vector <int> result;
    int l = rules.size();
    if(l<=1) {
      return rules;
    }
    // If there is at least one rule on this actor
    for(int i=0; i<l; i++) {
      /* rIt ne devrait pas être nul */
      rIt = rulesM.find(rules[i]);
      if(rIt != rulesM.end()) {
        // cout << rIt->first << "->" << rIt->second << endl;
        if(rIt->second.actor == actor) {
          result.push_back(rules[i]);
        }
      }
    }

    if(result.size() > 0) {
      return result;
    }

    // Liste des acteurs ancêtres à actors :
    std::vector<int> adjacents = actors.inAdjacentIndexVertices(actor);
    // std::cout << "adjacentIndexVertices" << adjacents << std::endl;

    std::vector<int> inRange;
    std::unordered_map<int, std::vector<int> > inRangeAdjacentsUmap;

    for(int i=0; i<l; i++) {
      if(vectorContains(adjacents, rules[i])) {
        inRange.push_back(rules[i]);
        inRangeAdjacentsUmap.insert(pair<int, std::vector<int> >
          (rules[i], actors.inAdjacentIndexVertices(rules[i])));
      }
    }
    cout << "inRange " << inRange << endl;
    /* Suppression de tous les moins spécifiques !! */
    std::vector<int> tmp;
    std::vector<int> toRemove;
    for(it = inRangeAdjacentsUmap.begin(); it != inRangeAdjacentsUmap.end(); it++) {
      cout << it->first << ": adjacents=";
      tmp = it->second;
      for(unsigned int i = 0; i < tmp.size(); i++) {
        if(inRangeAdjacentsUmap.find(tmp[i]) != inRangeAdjacentsUmap.end()) {
          // remove de inrange
          toRemove.push_back(tmp[i]);
        }
        cout << tmp[i] << ",";
      }
      cout << endl;
    }

    result = vectorDiff(inRange, toRemove);
    return result;
  }

  bool sumModalities(std::vector<int> rulesId) {
    int l = rulesId.size();
    for(int i=0; i<l; i++) {
      if(!rulesM.find(rulesId[i])->second.permission) {
        return false;
      }
    }
    return true;
  }

  int loadFromFolder(std::string folder) {
    actors.loadFromFileF2(folder + "actors.dat");
    objects.loadFromFileF2(folder + "objects.dat");
    loadRulesFromFile(folder + "rules.dat");
    return 0;
  }

  int loadActorsFromFile(std::string fileName) {
    return actors.loadFromFileF2(fileName);
  }

  int loadObjectsFromFile(std::string fileName) {
    return objects.loadFromFileF2(fileName);
  }

  int loadRulesFromFile(std::string fileName) {
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

    int loadRequestsFromFile(std::string fileName) {
      std::ifstream data(fileName);
      if(!data) {
        std::cerr << "No " << fileName << " file" << std::endl;
        return -1;
      }

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



      }



      return 0;
    }



  void printRules() {
    std::unordered_map<int, Rule>::iterator it;
    for(it = rulesM.begin(); it != rulesM.end(); it++) {
      cout << it->second << endl;
    }
  }

  bool isAllowed(int actorId, int objectId, std::string &str) {
    std::vector<int> ruleIds = effectiveRules(actorId, objectId);
    cout << "EffectiveRules:" << ruleIds << endl;
    ruleIds = deepestRules(ruleIds, actorId);
    cout << "Deepest:" << ruleIds << endl;
    // return sumModalities(ruleIds);
    return true;
  }

  bool isAllowed(std::string actor, std::string object, std::string &str) {
    int actorId = actorStringToId(actor);
    int objectId = objectStringToId(object);
    return isAllowed(actorId, objectId, str);
  }

  int actorStringToId(std::string &str) {
    bm_type::right_const_iterator rIt;
    rIt = actors.namesBimap.right.find(str);
    if(rIt == actors.namesBimap.right.end()) {
      cout << "Actor " << str << " not found " << endl;
      throw "Error";
      return -1;
    } else {
      return actors.namesBimap.right.find(str)->second;
    }
  }

  int objectStringToId(std::string &str) {
    bm_type::right_const_iterator rIt;
    rIt = objects.namesBimap.right.find(str);
    if(rIt == objects.namesBimap.right.end()) {
      cout << "Object " << str << " not found " << endl;
      throw "Error";
      return -1;
    } else {
      return objects.namesBimap.right.find(str)->second;
    }
  }

  std::string actorIdToString(int id) {
    bm_type::left_const_iterator lIt;
    lIt = actors.namesBimap.left.find(id);
    if(lIt == actors.namesBimap.left.end()) {
      cout << "Actor " << id << " not found " << endl;
      throw "Error";
      return "";
    } else {
      return actors.namesBimap.left.find(id)->second;
    }
  }

  std::string objectIdToString(int id) {
    bm_type::left_const_iterator lIt;
    lIt = objects.namesBimap.left.find(id);
    if(lIt == objects.namesBimap.left.end()) {
      cout << "Actor " << id << " not found " << endl;
      throw "Error";
      return "";
    } else {
      return objects.namesBimap.left.find(id)->second;
    }
  }

  Hierarchy actors;
  Hierarchy objects;
  Request request;
  std::unordered_map<int, Rule> rulesM;

private:
};

#endif
