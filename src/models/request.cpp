//=======================================================================
// Copyright (c) 2015 Romain Cotte. All rights reserved.
//
// Distributed under the GNU Affero General Public License,
// Version 3.0. (See https://www.gnu.org/licenses/agpl-3.0.html)
//=======================================================================

#include <iostream>
#include "request.hpp"

std::ostream& operator<<(std::ostream& os, const Request& request) {
  os << "Demande de : " << request.actor << " sur " << request.object << endl;
  os << "Highests " << request.highestRules << endl;
  os << "effectiveRules: " << request.effectiveRules << endl;
  os << "deepestRules: " << request.deepestRules << endl;
  return os;
}

