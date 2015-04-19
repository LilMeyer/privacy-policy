#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include "./../utils/vectors.hpp"
#include "policy.hpp"

class Request {
public:
  Request() {}
  Request(int actorId, int objectId/*, const Policy &p*/): actorId(actorId), objectId(objectId) {
    // actor = p.actorIdToString(actorId);
    // object = p.objectIdToString(objectId);
  }

  void setIds(int actorId, int objectId) {
    actorId = actorId;
    objectId = objectId;
  }

  void setLabels(std::string actor, std::string object) {
    actor = actor;
    object = object;
  }

  friend std::ostream& operator<<(std::ostream& os, const Request& request);

  /* It will be arrays */
  int actorId;
  int objectId;
  std::string actor;
  std::string object;

  std::vector<int> effectiveRules;
  std::vector<int> highestRules;
  std::vector<int> deepestRules;

};

#endif

