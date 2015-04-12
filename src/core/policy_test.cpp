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




class GraphTest : public CppUnit::TestFixture {

private:
  Hierarchy actors;
  Hierarchy objects;
  std::vector<Rule> rules;
  /* TODO: change to bimap et à intégrer dans Hierarchy !! */
  std::unordered_map<int, string> actorsUmap;
  std::unordered_map<int, string> objectsUmap;
  std::unordered_map<int, Rule> rulesUmap;
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

    actorsUmap.insert(std::pair<int, std::string>(0, "Lab"));
    actorsUmap.insert(std::pair<int, std::string>(1, "Microbio"));
    actorsUmap.insert(std::pair<int, std::string>(2, "Hemato"));
    actorsUmap.insert(std::pair<int, std::string>(3, "Strep"));
    actorsUmap.insert(std::pair<int, std::string>(4, "Test"));
    actorsUmap.insert(std::pair<int, std::string>(5, "VIH"));
    actorsUmap.insert(std::pair<int, std::string>(6, "Potassium"));

    objectsUmap.insert(std::pair<int, std::string>(0, "CHUS"));
    objectsUmap.insert(std::pair<int, std::string>(1, "Cardiologie"));
    objectsUmap.insert(std::pair<int, std::string>(2, "Urgence"));
    objectsUmap.insert(std::pair<int, std::string>(3, "Doctor B"));

    rules.push_back(Rule(1, 0, 0, 3, false));
    rules.push_back(Rule(2, 0, 2, 1, false));
    rules.push_back(Rule(3, 0, 2, 2, true));
    rules.push_back(Rule(4, 0, 5, 3, false));

    rulesUmap.insert(std::pair<int, Rule>(1, Rule(1, 0, 0, 3, false)));
    rulesUmap.insert(std::pair<int, Rule>(2, Rule(2, 0, 2, 1, false)));
    rulesUmap.insert(std::pair<int, Rule>(3, Rule(3, 0, 2, 2, false)));
    rulesUmap.insert(std::pair<int, Rule>(4, Rule(4, 0, 5, 3, false)));

    actors.printVertices();
    actors.printVerticesReverse();

    actors.toDotFile("actorsHierarchyClosure");
    actors.reverseToDotFile("actorsHierarchyClosureReverse");

    actors.printVertices();
    actors.printVerticesReverse();


    std::cout << "1 adj " << actors.adjacentIndexVertices(1) << std::endl;
    std::cout << "1 in " << actors.inAdjacentIndexVertices(1) << std::endl;
    std::cout << "3 adj" << actors.adjacentIndexVertices(3) << std::endl;
    std::cout << "3 in" << actors.inAdjacentIndexVertices(3) << std::endl;

    std::vector<int> effective = effectiveRules(5, 3);
    std::cout << "EffectiveRules:" << effective << std::endl;


    std::cout << "deepestRules:" << deepestRules(effective, 5) << std::endl;

    /**
     * Avoid rule id = 0
     * All values are initialized to 0
     */
    // std::vector<int> actorsRules(nbActors);
    // actorsRules[0] = 1;
    // actorsRules[2] = 1;
    // actorsRules[5] = 1;

    /**
     * List of ancestors
     */
    // actor = 3;
    // object = 5;

    // actors.printVertices();


    // std::cout << "Vertices from 0" << std::endl;



  }

  std::vector<int> effectiveRules(int actor, int object) {
    std::unordered_map<int, std::string>::const_iterator aIt = actorsUmap.find(actor);
    std::unordered_map<int, std::string>::const_iterator oIt = objectsUmap.find(object);
    std::cout << "Demande de " << aIt->second << " sur " << oIt->second << std::endl;
    std::vector<int> result, adjacent = actors.inAdjacentIndexVertices(actor);
    int l = rules.size(), m = adjacent.size();
    for(int i=0; i<l; i++) {
      if(rules[i].actor == actor) {
        result.push_back(rules[i].id);
        continue;
      }
      for(int j=0; j<m; j++) {
        // std::cout << adjacent[j] << std::endl;
        if(adjacent[j] == rules[i].actor) {
          result.push_back(rules[i].id);
        }
      }
    }
    return result;
  }


  std::vector<int> deepestRules(std::vector <int> rules, int actor) {
    std::unordered_map<int, std::string>::const_iterator aIt = actorsUmap.find(actor);
    std::unordered_map<int, Rule>::const_iterator rIt;
    std::unordered_map<int, std::vector<int> >::iterator it, it0;
    std::cout << "Actor " << aIt->second << "(" << aIt->first << ")" << std::endl;

    std::vector <int> result;
    int l = rules.size();
    if(l<=1) {
      return rules;
    }
    // If there is at least one rule on this actor
    for(int i=0; i<l; i++) {
      if(rules[i] == actor) {
        result.push_back(rules[i]);
      }
    }

    // Liste des acteurs ancêtres à actors :
    std::vector<int> adjacents = actors.inAdjacentIndexVertices(actor);
    std::cout << "adjacentIndexVertices" << adjacents << std::endl;

    std::vector<int> inRange;
    std::unordered_map<int, std::vector<int> > inRangeAdjacentsUmap;

    for(int i=0; i<l; i++) {
      if(vectorContains(adjacents, rules[i])) {
        inRange.push_back(rules[i]);
        inRangeAdjacentsUmap.insert(pair<int, std::vector<int> >
          (rules[i], actors.inAdjacentIndexVertices(rules [i])));
      }
    }
    cout << "inRange " << inRange << endl;
    /* Suppression de tous les moins spécifiques !! */
    std::vector<int> tmp;
    std::vector<int> toRemove;
    for(it = inRangeAdjacentsUmap.begin(); it != inRangeAdjacentsUmap.end(); it++) {
      cout << it->first << " ";
      tmp = it->second;
      for(unsigned int i = 0; i < tmp.size(); i++) {
        if(inRangeAdjacentsUmap.find(tmp[i]) != inRangeAdjacentsUmap.end()) {
          // remove de inrange
          toRemove.push_back(tmp[i]);
        }
        cout << tmp[i] << ",";
      }
      cout << endl;
    }





    if(result.size() > 0) {
      return result;
    }

    return result;
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
