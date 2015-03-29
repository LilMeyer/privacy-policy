#ifndef RULE_H
#define RULE_H

#include <iostream>

class Rule {
public:
  Rule(const int id, const int priority, const int actor, const int object,
       const bool permission)
    : id(id), priority(priority), actor(actor), object(object) {}
  friend std::ostream& operator<<(std::ostream& os, const Rule& rule);
private:
  int id;
  int priority;
  int actor;
  int object;
  bool permission;
};

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
  os << "Id: " << rule.id;
  os << " Priority: " << rule.priority;
  os << " Actor: " << rule.actor;
  os << " Object: " << rule.object;
  os << " Permission: " << rule.permission << std::endl;
  return os;
}

#endif

