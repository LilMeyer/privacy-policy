#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>

#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>

// http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/quick_tour.html

using namespace boost;

class GraphTest : public CppUnit::TestFixture {
typedef std::pair<int, int> Edge;
typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

struct Rule {
  Rule(const int priority, const int actor, const int object, bool permission)
    : priority(priority), actor(actor), object(object) { }

  void print() {
    std::cout << "Priority: " << priority << std::endl;
    std::cout << "Actor: " << actor << std::endl;
    std::cout << "Object: " << object << std::endl;
    std::cout << "Permission: " << permission << std::endl;
  }

private:
  int priority;
  int actor;
  int object;
  bool permission;
};

private:
  enum { A, B, C, D, E, F, G, N };
  const int num_vertices = N;
  // Declare actors and objects graphs
  Graph actorsHierarchy, objectsHierarchy;
  std::vector<Edge> actorsEdgeArray, objectsEdgeArray;
  std::vector<Rule> rules;
public:
  GraphTest() : actorsHierarchy(N), objectsHierarchy(N) {
    actorsEdgeArray.push_back(Edge(A, B));
    actorsEdgeArray.push_back(Edge(A, C));
    actorsEdgeArray.push_back(Edge(B, D));
    actorsEdgeArray.push_back(Edge(B, E));
    actorsEdgeArray.push_back(Edge(C, F));
    actorsEdgeArray.push_back(Edge(C, G));
    // add the edges to the graph object
    for (unsigned int i = 0; i < actorsEdgeArray.size(); ++i) {
      add_edge(actorsEdgeArray[i].first, actorsEdgeArray[i].second, actorsHierarchy);
    }

    // Declare objects graph
    objectsEdgeArray.push_back(Edge(A, B));
    objectsEdgeArray.push_back(Edge(A, C));
    objectsEdgeArray.push_back(Edge(B, D));
    objectsEdgeArray.push_back(Edge(B, E));
    objectsEdgeArray.push_back(Edge(C, F));
    objectsEdgeArray.push_back(Edge(C, G));

    for (unsigned int i = 0; i < objectsEdgeArray.size(); ++i) {
      add_edge(objectsEdgeArray[i].first, objectsEdgeArray[i].second, objectsHierarchy);
    }

    // Declare rules
    Rule r(0, 1, 2, false);
    rules.push_back(r);



    std::cout << "Hello" << A << std::endl;
  }
  // A -> B
  // A -> C
  // B -> D
  // B -> E
  // C -> F
  // C -> G

  bool accessToObject(int actor, int object) {
    return true;
  }


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
    std::cout << std::endl;







    // /* ACCESSING THE VERTEX SET */
    // typedef graph_traits<Graph>::vertex_descriptor Vertex;

    // // get the property map for vertex indices
    // typedef property_map<Graph, vertex_index_t>::type IndexMap;
    // IndexMap index = get(vertex_index, g);

    // std::cout << "vertices(g) = ";
    // typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    // std::pair<vertex_iter, vertex_iter> vp;
    // for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    //   Vertex v = *vp.first;
    //   std::cout << index[v] <<  " ";
    // }
    // std::cout << std::endl;

    // /* ACCESSING THE EDGE SET */
    // std::cout << "edges(g) = ";
    // graph_traits<Graph>::edge_iterator ei, ei_end;
    // for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    //     std::cout << "(" << index[source(*ei, g)]
    //               << "," << index[target(*ei, g)] << ") ";
    // std::cout << std::endl;




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
