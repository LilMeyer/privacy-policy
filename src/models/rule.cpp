//=======================================================================
// Copyright (c) 2015 Romain Cotte. All rights reserved.
//
// Distributed under the GNU Affero General Public License,
// Version 3.0. (See https://www.gnu.org/licenses/agpl-3.0.html)
//=======================================================================

#include <iostream>
#include "rule.hpp"

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
  os << "Id: " << rule.id;
  os << " Priority: " << rule.priority;
  os << " Actor: " << rule.actor;
  os << " Object: " << rule.object;
  os << " Permission: " << (rule.permission ? "allowed" : "prohibited");
  return os;
}

