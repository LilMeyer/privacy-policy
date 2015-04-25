//=======================================================================
// Copyright (c) 2015 Romain Cotte. All rights reserved.
//
// Distributed under the GNU Affero General Public License,
// Version 3.0. (See https://www.gnu.org/licenses/agpl-3.0.html)
//=======================================================================

#include <iostream>
#include <utility>        // for std::pair
#include "request.hpp"
#include "policyForTest.hpp"
#include <boost/bimap.hpp>

using namespace boost;

int Request::next() {
  /**
   * First, initialize actors and objects
   * then, advance until **next** is reached
   */
  switch(nextAction) {
    case START:
      nextActors();
      nextObjects();
      nextRules();
      break;
    case ACTORS:
      nextActors();
      break;
    case OBJECTS:
      nextObjects();
      break;
    case RULES:
      nextRules();
      break;
    case END:
      return 0;
    default:
      break;
  }

  std::vector<std::string> splitVec, subVec;

  std::size_t found;

  for (std::string line; std::getline(*requestsData, line);) {
    cout << "Line " << line << endl;
    found = line.find("next");
    if (found != std::string::npos) {
      if(line.find("actors") != std::string::npos) {
        nextAction = ACTORS;
      }
      if(line.find("objects") != std::string::npos) {
        nextAction = OBJECTS;
      }
      if(line.find("rules") != std::string::npos) {
        nextAction = RULES;
      }
      break;
    }
    if(line.find("end") != std::string::npos) {
      nextAction = END;
      break;
    }

    found = line.find("#");
    if (found != std::string::npos) {
      continue;
    }

    splitVec.clear();
    split(splitVec, line, is_any_of(";"), token_compress_on);
    if (splitVec.size()!=2) {
      continue;
    }
    actorId = policyPtr->actorStringToId(splitVec[0]);
    objectId = policyPtr->objectStringToId(splitVec[1]);
    actor = splitVec[0];
    object = splitVec[1];
    cout << "Request number #" << requestNumber << endl;
    requestNumber++;
  }

  expEffectiveRules.clear();
  expDeepestRules.clear();
  expHighestRules.clear();

  for (std::string line; std::getline(*expectedData, line);) {
    found = line.find("#");
    if (found != std::string::npos) {
      continue;
    }

    found = line.find("next");
    if (found != std::string::npos) {
      continue;
    }
    // cout << "line " << line << endl;
    splitVec.clear();
    split(splitVec, line, is_any_of(";"), token_compress_on);
    if (splitVec.size()!=4) {
      continue;
    }
    subVec.clear();
    split(subVec, splitVec[0], is_any_of(","), token_compress_on);
    for(unsigned int i=0; i<subVec.size(); i++) {
      expEffectiveRules.push_back(atoi(subVec[i].c_str()));
    }
    subVec.clear();
    split(subVec, splitVec[1], is_any_of(","), token_compress_on);
    for(unsigned int i=0; i<subVec.size(); i++) {
      expHighestRules.push_back(atoi(subVec[i].c_str()));
    }
    subVec.clear();
    split(subVec, splitVec[2], is_any_of(","), token_compress_on);
    for(unsigned int i=0; i<subVec.size(); i++) {
      expDeepestRules.push_back(atoi(subVec[i].c_str()));
    }

    if(splitVec[3] == "1") {
      expIsAllowed = true;
    } else {
      expIsAllowed = false;
    }

    break;
  }

  return 1;
}


int Request::nextExpectedResult() {
  return 0;
}


int Request::nextRules() {
  policyPtr->rulesM.clear();

  std::vector<std::string> splitVec;
  bimap<int, std::string> verticesBimap;
  std::vector<pair<int, int> > edges;

  int priority, ruleId, actorId, objectId;
  bool permission;
  for (std::string line; std::getline(*rulesData, line);) {

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
    if (splitVec.size()!=6) {
      continue;
    }

    bm_type::right_const_iterator rIt;

    ruleId = atoi(splitVec[0].c_str());
    priority = atoi(splitVec[1].c_str());
    rIt = policyPtr->actors.namesBimap.right.find(splitVec[2]);
    if(rIt == policyPtr->actors.namesBimap.right.end()) {
      cout << "Actor " << splitVec[2] << " not found " << endl;
      continue;
    } else {
      actorId = policyPtr->actors.namesBimap.right.find(splitVec[2])->second;
    }

    rIt = policyPtr->objects.namesBimap.right.find(splitVec[3]);
    if(rIt == policyPtr->objects.namesBimap.right.end()) {
      cout << "Object " << splitVec[3] << " not found " << endl;
      continue;
    } else {
      objectId = policyPtr->objects.namesBimap.right.find(splitVec[3])->second;
    }

    permission = splitVec[4] == "permission";

    Rule r(ruleId, priority, actorId, objectId, permission);
    policyPtr->rulesM.insert(std::pair<int, Rule>(ruleId, r));
  }

  return policyPtr->rulesM.size() > 0;
}

int Request::nextActors() {
  int id = 0, l;
  std::vector<std::string> splitVec;
  bimap<int, std::string> verticesBimap;
  std::vector<pair<int, int> > edges;
  pair<int, int> p;

  for (std::string line; std::getline(*actorsData, line);) {
    splitVec.clear();
    split(splitVec, line, is_any_of(">"), token_compress_on);
    l = splitVec.size();

    if(l!=2) {
      continue;
    }

    bm_type::right_const_iterator rightIter;
    rightIter = verticesBimap.right.find(splitVec[0]);

    if (rightIter == verticesBimap.right.end()) {
      verticesBimap.insert(bm_type::value_type(id, splitVec[0]));
      p.first = id;
      id++;
    } else {
      p.first = rightIter->second;
    }

    rightIter = verticesBimap.right.find(splitVec[1]);
    if (rightIter == verticesBimap.right.end()) {
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
     policyPtr->actors.addVertex(it->first, it->second);
  }

  for(unsigned int i=0; i<edges.size(); i++) {
    policyPtr->actors.addEdge(edges[i].first, edges[i].second);
  }

  return 1;
}

int Request::nextObjects() {
  int id = 0, l;
  std::vector<std::string> splitVec;
  bimap<int, std::string> verticesBimap;
  std::vector<pair<int, int> > edges;
  pair<int, int> p;

  for (std::string line; std::getline(*objectsData, line);) {
    splitVec.clear();
    split(splitVec, line, is_any_of(">"), token_compress_on);
    l = splitVec.size();

    if(l!=2) {
      continue;
    }

    bm_type::right_const_iterator rightIter;
    rightIter = verticesBimap.right.find(splitVec[0]);

    if (rightIter == verticesBimap.right.end()) {
      verticesBimap.insert(bm_type::value_type(id, splitVec[0]));
      p.first = id;
      id++;
    } else {
      p.first = rightIter->second;
    }

    rightIter = verticesBimap.right.find(splitVec[1]);
    if (rightIter == verticesBimap.right.end()) {
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
     policyPtr->objects.addVertex(it->first, it->second);
  }

  for(unsigned int i=0; i<edges.size(); i++) {
     policyPtr->objects.addEdge(edges[i].first, edges[i].second);
  }

  return 1;
}

void Request::setIds(int actorId, int objectId) {
  actorId = actorId;
  objectId = objectId;
}

void Request::setLabels(std::string actor, std::string object) {
  actor = actor;
  object = object;
}

std::ostream& operator<<(std::ostream& os, const Request& request) {
  os << "Demande de : " << request.actor << " sur " << request.object << std::endl;
  os << "Highests " << request.highestRules << std::endl;
  os << "effectiveRules: " << request.effectiveRules << std::endl;
  os << "deepestRules: " << request.deepestRules << std::endl;
  return os;
}

