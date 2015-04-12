#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <vector>
#include <unordered_map>
#include <string>

#include "./../models/rule.hpp"
#include "./../models/hierarchy.hpp"
#include "./../utils/vectors.hpp"
#include "policy.cpp"

/* Order is important ! */
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

// http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/quick_tour.html

using namespace boost;
using namespace std;


class HierarchyTest : public CppUnit::TestFixture {

private:
  Hierarchy hierarchy;

public:
  HierarchyTest() {
  }

  static CppUnit::Test *suite() {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("HierarchyTest");

    suiteOfTests->addTest(new CppUnit::TestCaller<HierarchyTest>("Test1 - Unique Solution.",
            &HierarchyTest::test1));
    return suiteOfTests;
  }

  /* Setup method */
  void setUp() {}

  /* Teardown method */
  void tearDown() {}

protected:

  void test1() {
    hierarchy.addElement(0, "Root");

  }
};


int main(int argc, char* argv[]) {

  CppUnit::TextUi::TestRunner runner;
  std::cout << "Creating Test Suites:" << std::endl;
  runner.addTest(HierarchyTest::suite());
  std::cout<< "Running the unit tests."<< std::endl;
  runner.run();

  return 0;
}
