#ifndef RULE_H
#define RULE_H

#include <iostream>

class Rule {
public:
  Rule(const int id, const int priority, const int actor, const int object,
       const bool permission)
    : id(id), priority(priority), actor(actor), object(object),
      permission(permission) {}

  friend std::ostream& operator<<(std::ostream& os, const Rule& rule);


  int id;
  int priority;
  int actor;
  int object;
  bool permission;


};

#endif

