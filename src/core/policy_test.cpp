#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <vector>

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
typedef std::pair <int, int> Edge;
typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
typedef property <vertex_name_t, char> Name;
typedef property <vertex_index_t, std::size_t, Name> Index;
typedef adjacency_list <listS, listS, directedS, Index> Graph_t;
typedef typename graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph_t>::vertex_descriptor vertex_t;

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
  // const int num_vertices = N;
  const int nVertices = 7;
  // Declare actors and objects graphs
  Graph actorsHierarchy, objectsHierarchy;
  adjacency_list <> actorsHierarchyClosure, objectsHierarchyClosure;

  std::vector<Edge> actorsEdgeArray, objectsEdgeArray;

  Graph_t actorsHierarchy_t, objectsHierarchy_t;
  std::vector <vertex_t> actorsVerts, objectsVerts;

  std::vector<Rule> rules;
public:
  GraphTest() : actorsHierarchy(nVertices), objectsHierarchy(nVertices),
                actorsVerts(nVertices), objectsVerts(nVertices) {

    char name[] = "abcdefgh";

    actorsEdgeArray.push_back(Edge(A, B));
    actorsEdgeArray.push_back(Edge(A, C));
    actorsEdgeArray.push_back(Edge(B, D));
    actorsEdgeArray.push_back(Edge(B, E));
    actorsEdgeArray.push_back(Edge(C, F));
    actorsEdgeArray.push_back(Edge(C, G));
    // // add the edges to the graph object
    for (unsigned int i = 0; i < actorsEdgeArray.size(); ++i) {
      add_edge(actorsEdgeArray[i].first, actorsEdgeArray[i].second, actorsHierarchy);
    }

    // // Declare objects graph
    objectsEdgeArray.push_back(Edge(A, B));
    objectsEdgeArray.push_back(Edge(A, C));
    objectsEdgeArray.push_back(Edge(B, D));
    objectsEdgeArray.push_back(Edge(B, E));
    objectsEdgeArray.push_back(Edge(C, F));
    objectsEdgeArray.push_back(Edge(C, G));

    for (unsigned int i = 0; i < objectsEdgeArray.size(); ++i) {
      add_edge(objectsEdgeArray[i].first, objectsEdgeArray[i].second, objectsHierarchy);
    }

    for (int i = 0; i < 7; ++i) {
      actorsVerts[i] = add_vertex(Index(i, Name('a' + i)), actorsHierarchy_t);
      objectsVerts[i] = add_vertex(Index(i, Name('a' + i)), objectsHierarchy_t);
    }

    add_edge(actorsVerts[0], actorsVerts[1], actorsHierarchy_t);
    add_edge(actorsVerts[0], actorsVerts[2], actorsHierarchy_t);
    add_edge(actorsVerts[1], actorsVerts[3], actorsHierarchy_t);
    add_edge(actorsVerts[1], actorsVerts[4], actorsHierarchy_t);
    add_edge(actorsVerts[2], actorsVerts[5], actorsHierarchy_t);
    add_edge(actorsVerts[2], actorsVerts[6], actorsHierarchy_t);

    add_edge(objectsVerts[0], objectsVerts[1], objectsHierarchy_t);
    add_edge(objectsVerts[0], objectsVerts[2], objectsHierarchy_t);
    add_edge(objectsVerts[1], objectsVerts[3], objectsHierarchy_t);
    add_edge(objectsVerts[1], objectsVerts[4], objectsHierarchy_t);
    add_edge(objectsVerts[2], objectsVerts[5], objectsHierarchy_t);
    add_edge(objectsVerts[2], objectsVerts[6], objectsHierarchy_t);

    // // Declare rules
    Rule r(0, 1, 2, false);
    rules.push_back(r);


    transitive_closure(actorsHierarchy_t, actorsHierarchyClosure);
    std::cout << std::endl << "Graph Transitive closure+:" << std::endl;
    print_graph(actorsHierarchyClosure, name);

    std::ofstream out("actorsHierarchyClosure.dot");
    write_graphviz(out, actorsHierarchyClosure, make_label_writer(name));

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
