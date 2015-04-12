//=======================================================================
// Copyright (c) 2015 Romain Cotte. All rights reserved.
//
// Distributed under the GNU Affero General Public License,
// Version 3.0. (See https://www.gnu.org/licenses/agpl-3.0.html)
//=======================================================================

#include <boost/thread.hpp>
// #include <boost/graph/graph_traits.hpp>


// #include <iostream>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

struct struct1 {

  struct1(const int a) : m_a(a) {}
  void print() {
    std::cout << "Value of a " << m_a << std::endl;
  }

private:
  int m_a;
};

struct rule {
  rule(const int priority, const int actor, const int object)
    : priority(priority), actor(actor), object(object) { }

  void print() {
    std::cout << "Priority: " << priority << std::endl;
    std::cout << "Actor: " << actor << std::endl;
    std::cout << "Object: " << object << std::endl;
  }

private:
  int priority;
  int actor;
  int object;
};

class ClassTest {
private:

public:

};




// boost::mutex mutex;
// int counter=0;

// void change_count() {
//   boost::mutex::scoped_lock lock(mutex);
//   int i = ++counter;
//   std::cout << "count == " << i << std::endl;
// }

int main(int argc, char* argv[]) {
  struct1 s(10);
  s.print();
  rule r(1, 2, 3);
  r.print();

/*  const int num_threads = 4;
  boost::thread_group thrds;
  for (int i=0; i < num_threads; ++i)
    thrds.create_thread(&change_count);
  thrds.join_all();*/

  return 0;
}
