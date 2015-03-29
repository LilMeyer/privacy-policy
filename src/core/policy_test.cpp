#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <vector>

#include "./../models/rule.hpp"
#include "./../models/hierarchy.hpp"
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

class GraphTest : public CppUnit::TestFixture {

private:
  Hierarchy actors;
  Hierarchy objects;
  std::vector<Rule> rules;
  int actor;
  int object;
public:
  GraphTest() {
    int nbActors = 7;
    int nbObjects = 4;
    for(int i=0; i< nbActors; i++) {
      actors.addVertex(i);
    }
    for(int i=0; i< nbObjects; i++) {
      objects.addVertex(i);
    }
    actors.addEdge(0, 1);
    actors.addEdge(0, 2);
    actors.addEdge(1, 3);
    actors.addEdge(1, 4);
    actors.addEdge(2, 5);
    actors.addEdge(2, 6);

    objects.addEdge(0, 1);
    objects.addEdge(0, 2);
    objects.addEdge(1, 3);
    objects.addEdge(2, 3);
    actors.saveGraphviz("actorsGraphviz");
    objects.saveGraphviz("objectsGraphviz");

    rules.push_back(Rule(1, 0, 0, 3, false));
    rules.push_back(Rule(2, 0, 2, 1, false));
    rules.push_back(Rule(3, 0, 2, 2, true));
    rules.push_back(Rule(4, 0, 5, 3, false));

    /**
     * Avoid rule id = 0
     * All values are initialized to 0
     */
    std::vector<int> actorsRules(nbActors);
    actorsRules[0] = 1;
    actorsRules[2] = 1;
    actorsRules[5] = 1;

    /**
     * List of ancestors
     */
    // actor = 3;
    // object = 5;

    adjacency_list<> closure = actors.getTransitiveClosure();

    actors.printVertices();


    graph_traits <adjacency_list <> >::vertex_iterator i, end;
    graph_traits <adjacency_list <> >::adjacency_iterator  ai, a_end;
    property_map <adjacency_list <>, vertex_index_t >::type
      index_map = get(vertex_index, closure);


    // for (boost::tie(i, end) = vertices(actors); i != end; ++i) {
    //   std::cout << name[get(index_map, *i)];
    //   boost::tie(ai, a_end) = adjacent_vertices(*i, actors);
    //   if (ai == a_end) {
    //     std::cout << " has no children";
    //   } else {
    //     std::cout << " is the parent of ";
    //   }
    //   for (; ai != a_end; ++ai) {
    //     std::cout << name[get(index_map, *ai)];
    //     if (boost::next(ai) != a_end) {
    //       std::cout << ", ";
    //     }
    //   }
    //   std::cout << std::endl;
    // }


  }


  /**
   * Every rules that aim to this actor or his ancestors
   */
  // std::vector<Rule> effectiveRules() {

  // }



  static CppUnit::Test *suite() {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("GraphTest");

    suiteOfTests->addTest(new CppUnit::TestCaller<GraphTest>("Test1 - Unique Solution.",
            &GraphTest::test1));
    return suiteOfTests;
  }

  /* Setup method */
  void setUp() {}

  /* Teardown method */
  void tearDown() {}

protected:

  void test1() {
    std::cout << "start" << std::endl;

  }
};


int main(int argc, char* argv[]) {

  CppUnit::TextUi::TestRunner runner;
  std::cout << "Creating Test Suites:" << std::endl;
  runner.addTest(GraphTest::suite());
  std::cout<< "Running the unit tests."<< std::endl;
  runner.run();

  return 0;
}
