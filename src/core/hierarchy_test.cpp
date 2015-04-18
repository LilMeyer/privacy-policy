//=======================================================================
// Copyright (c) 2015 Romain Cotte. All rights reserved.
//
// Distributed under the GNU Affero General Public License,
// Version 3.0. (See https://www.gnu.org/licenses/agpl-3.0.html)
//=======================================================================

#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <vector>
#include <unordered_map>
#include <string>

#include "./../models/rule.hpp"
#include "./../models/hierarchy.hpp"
#include "./../utils/vectors.hpp"

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
    suiteOfTests->addTest(new CppUnit::TestCaller<HierarchyTest>("Test2 import.",
            &HierarchyTest::test2));
    suiteOfTests->addTest(new CppUnit::TestCaller<HierarchyTest>("Test3 import.",
            &HierarchyTest::test3));
    return suiteOfTests;
  }

  /* Setup method */
  void setUp() {}

  /* Teardown method */
  void tearDown() {}

protected:

  void test1() {
    int rootId = 0;
    string rootName = "Root";
    hierarchy.addVertex(rootId, rootName);
    CPPUNIT_ASSERT_EQUAL(rootName, hierarchy.getVertexName(rootId));
    CPPUNIT_ASSERT_EQUAL(rootId, hierarchy.getVertexId(rootName));

    hierarchy.getVertexId("doNotExists");

    int kingId = hierarchy.addVertex("King");
    int queenId = hierarchy.addVertex("Queen");
    int kingSonId = hierarchy.addVertex("KingSon");
    CPPUNIT_ASSERT_EQUAL(kingId, 1);
    CPPUNIT_ASSERT_EQUAL(queenId, 2);
    CPPUNIT_ASSERT_EQUAL(kingSonId, 3);
    hierarchy.addEdge(rootId, kingId);
    hierarchy.addEdge(rootId, queenId);
    hierarchy.addEdge(kingId, kingSonId);


    // Redirect cout. (à supprimer)
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    ostringstream strCout;
    cout.rdbuf(strCout.rdbuf());

    hierarchy.printVertices();

    string str1 = "0 [Root]->(1, 2)\n"
                 "1 [King]->(3)\n"
                 "2 [Queen]->()\n"
                 "3 [KingSon]->()\n";
    CPPUNIT_ASSERT(str1.compare(strCout.str()) == 0);

    strCout.str("");
    strCout.clear();

    hierarchy.printVerticesReverse();
    string str2 = "0 [Root]->()\n"
                  "1 [King]->(0)\n"
                  "2 [Queen]->(0)\n"
                  "3 [KingSon]->(1)\n";

    CPPUNIT_ASSERT(str2.compare(strCout.str()) == 0);

    // Restore old cout.
    cout.rdbuf(oldCoutStreamBuf);

    std::vector<int> v1 = { 3 };
    std::vector<int> v2 = { 0 };
    std::vector<int> v3 = { };
    std::vector<int> v4 = { 1, 0 };
    std::vector<int> v5 = { 1, 2, 3 };
    std::vector<int> r;

    r = hierarchy.adjacentIndexVertices(kingId);
    CPPUNIT_ASSERT(vectorSameValues(v1, r));
    r = hierarchy.inAdjacentIndexVertices(kingId);
    CPPUNIT_ASSERT(vectorSameValues(v2, r));

    r = hierarchy.adjacentIndexVertices(kingSonId);
    CPPUNIT_ASSERT(vectorSameValues(v3, r));
    r = hierarchy.inAdjacentIndexVertices(kingSonId);
    CPPUNIT_ASSERT(vectorSameValues(v4, r));

    r = hierarchy.adjacentIndexVertices(rootId);
    CPPUNIT_ASSERT(vectorSameValues(v5, r));
    r = hierarchy.inAdjacentIndexVertices(rootId);
    CPPUNIT_ASSERT(vectorSameValues(v3, r));

    hierarchy.toDotFile("hierarchy_test");
  }

  void test2() {
    Hierarchy example1;
    example1.loadFromFile("src/examples/graph1.dat");
    example1.toDotFile("example1");
  }

  void test3() {
    std::cout << "Test 3" << std::endl;
    Hierarchy example2;
    example2.loadFromFileF2("src/examples/graph1_f2.dat");
    example2.toDotFile("example1_f2");
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
