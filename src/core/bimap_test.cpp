#include <iostream>

#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>

// http://www.boost.org/doc/libs/1_57_0/libs/bimap/doc/html/index.html

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

using namespace std;
using namespace boost;

template< class Map, class CompatibleKey, class CompatibleData >
void use_it(Map & m,
            const CompatibleKey  & key,
            const CompatibleData & data) {
  typedef typename Map::value_type value_type;
  typedef typename Map::const_iterator const_iterator;

  m.insert(value_type(key,data));
  const_iterator iter = m.find(key);
  if (iter != m.end()) {
    CPPUNIT_ASSERT(iter->first == key);
    CPPUNIT_ASSERT(iter->second == data);
    std::cout << iter->first << " --> " << iter->second << endl;
  }
  m.erase(key);
}

typedef bimap< bimaps::set_of<std::string>, bimaps::set_of<int> > bimap_type;

class BimapTest : public CppUnit::TestFixture {
private:
  bimap_type bm;
public:
  BimapTest() {
  }

  static CppUnit::Test *suite() {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("BimapTest");

    suiteOfTests->addTest(new CppUnit::TestCaller<BimapTest>("Test1 - Unique Solution.",
            &BimapTest::test1));
    return suiteOfTests;
  }

  /* Setup method */
  void setUp() {}

  /* Teardown method */
  void tearDown() {}

protected:
  void test1() {

    // Standard map
    {
      typedef std::map< std::string, int > map_type;
      map_type m;

      use_it(m, "one", 1);
    }

    // Left map view
    {
      typedef bimap_type::left_map map_type;
      map_type & m = bm.left;

      use_it(m, "one", 1);
    }

    // Reverse standard map
    {
      typedef std::map< int, std::string > reverse_map_type;
      reverse_map_type rm;

      use_it(rm, 1, "one");
    }

    // Right map view
    {
      typedef bimap_type::right_map reverse_map_type;
      reverse_map_type & rm = bm.right;

      use_it(rm, 1, "one");
    }
  }
};


int main(int argc, char* argv[]) {

  CppUnit::TextUi::TestRunner runner;
  std::cout << "Creating Test Suites:" << std::endl;
  runner.addTest(BimapTest::suite());
  std::cout<< "Running the unit tests."<< std::endl;
  runner.run();

  return 0;
}
