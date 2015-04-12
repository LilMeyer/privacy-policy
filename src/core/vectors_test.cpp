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
#include "./../utils/vectors.hpp"

#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestSuite.h>


using namespace std;

class VectorsTest : public CppUnit::TestFixture {

public:
  VectorsTest()  { }

  static CppUnit::Test *suite() {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("VectorsTest");

    suiteOfTests->addTest(new CppUnit::TestCaller<VectorsTest>("Test1 - Unique Solution.",
            &VectorsTest::test1));
    return suiteOfTests;
  }

  /* Setup method */
  void setUp() {}

  /* Teardown method */
  void tearDown() {}

protected:
  void test1() {
    std::vector<int> v1 { 1, 2, 3 };
    std::vector<int> v2 { 2, 4, 5 };
    std::vector<int> expectedDiff { 1, 3 };
    std::vector<int> diff = vectorDiff(v1, v2);
    CPPUNIT_ASSERT(vectorContains(v1, 1));
    CPPUNIT_ASSERT(vectorContains(v1, 2));
    CPPUNIT_ASSERT(vectorContains(v1, 3));
    CPPUNIT_ASSERT(vectorContains(v2, 2));
    CPPUNIT_ASSERT(vectorContains(v2, 4));
    CPPUNIT_ASSERT(vectorContains(v2, 5));
    CPPUNIT_ASSERT(!vectorContains(v1, 4));
    CPPUNIT_ASSERT(vectorSameValues(diff, expectedDiff));
  }
};


int main(int argc, char* argv[]) {

  CppUnit::TextUi::TestRunner runner;
  std::cout << "Creating Test Suites:" << std::endl;
  runner.addTest(VectorsTest::suite());
  std::cout<< "Running the unit tests."<< std::endl;
  runner.run();

  return 0;
}
