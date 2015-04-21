#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <fstream>
#include "./../utils/vectors.hpp"

#include <boost/bimap.hpp>

class PolicyForTest;

class Request {
typedef boost::bimap <int, std::string> bm_type;
enum NextAction { START, ACTORS, OBJECTS, RULES, END };

public:
  Request() {}
  Request(int actorId, int objectId): actorId(actorId), objectId(objectId) {
    nextAction = START;
  }

  // Request(int actorId, int objectId, const PolicyForTest &p): actorId(actorId), objectId(objectId) {
  //   actor = p.actorIdToString(actorId);
  //   object = p.objectIdToString(objectId);
  // }

  Request(std::ifstream *actorsData,
          std::ifstream *objectsData,
          std::ifstream *rulesData,
          std::ifstream *requestsData,
          std::ifstream *expectedData,
          PolicyForTest *policy):
    actorsData(actorsData), objectsData(objectsData), rulesData(rulesData),
    expectedData(expectedData), requestsData(requestsData) {
      policyPtr = policy;
  }



  int next();
  int nextActors();
  int nextObjects();
  int nextExpectedResult();
  int nextRules();


  void setIds(int actorId, int objectId);

  void setLabels(std::string actor, std::string object);

  friend std::ostream& operator<<(std::ostream& os, const Request& request);

  /* It will be arrays */
  int actorId;
  int objectId;
  std::string actor;
  std::string object;
  /* no copy constructor http://www.cplusplus.com/reference/fstream/ifstream/ifstream/ */
  std::ifstream *actorsData;
  std::ifstream *objectsData;
  std::ifstream *rulesData;
  std::ifstream *expectedData;
  std::ifstream *requestsData;
  PolicyForTest *policyPtr;

  NextAction nextAction;
  std::vector<int> effectiveRules;
  std::vector<int> highestRules;
  std::vector<int> deepestRules;

  std::vector<int> expEffectiveRules;
  std::vector<int> expHighestRules;
  std::vector<int> expDeepestRules;
  bool expIsAllowed;
};

#endif


