
#include <cmath>
#include <sstream>
#include <string>
#include <vector>


#define ASSERT_EQUALS(a,b) ASSERT_EQ(a, b)
#define ASSERT_NOT_EQUALS(a,b) ASSERT_NE(a, b)
#define ASSERT_LESS_THAN(a, b) ASSERT_LT(a, b)
#define ASSERT_NOT_LESS_THAN(a, b) ASSERT_GTE(a, b)
#define ASSERT_GREATER_THAN(a, b) ASSERT_GT(a, b)
#define ASSERT_NOT_GREATER_THAN(a, b) ASSERT_LTE(a, b)
#define ASSERT_LESS_THAN_OR_EQUALS(a, b) ASSERT_LTE(a, b)
#define ASSERT_GREATER_THAN_OR_EQUALS(a, b) ASSERT_GTE(a, b)

#define ASSERT_EQ(a, b) _ASSERT_COMPARISON(EQ, a, b)
#define ASSERT_NE(a, b) _ASSERT_COMPARISON(NE, a, b)
#define ASSERT_LT(a, b) _ASSERT_COMPARISON(LT, a, b)
#define ASSERT_LTE(a, b) _ASSERT_COMPARISON(LTE, a, b)
#define ASSERT_GT(a, b) _ASSERT_COMPARISON(GT, a, b)
#define ASSERT_GTE(a, b) _ASSERT_COMPARISON(GTE, a, b)


#define _ASSERT_COMPARISON(COMPARISON, a, b)                    \
  ComparisonAssertion_##COMPARISON(__FILE__, __LINE__, #a, #b, a, b);

#define DECLARE_COMPARISON_ASSERTION(NAME, OPERATOR)            \
  class ComparisonAssertion_##NAME  {                           \
  typedef void (ComparisonAssertion_##NAME::*bool_type)() const;\
  public:                                                       \
    template <typename A, typename B>                           \
    ComparisonAssertion_##NAME(                                 \
          const std::string& theFile,                           \
          unsigned theLine,                                     \
          const std::string& aExpression,                       \
          const std::string& bExpression,                       \
          const A& a,                                           \
          const B& b)  {                                        \
      if (a OPERATOR b) {                                       \
        return;                                                 \
      }                                                         \
      std::ostringstream os;                                    \
      std::cout << theFile << "@" << theLine << " : ";          \
      std::cout << "Expected " <<                               \
        aExpression << " " #OPERATOR " " << bExpression <<      \
        " (" << a << " " #OPERATOR " " << b << ")" << std::endl;\
      throw;                                                    \
    }                                                           \
  }

DECLARE_COMPARISON_ASSERTION(EQ, ==);
DECLARE_COMPARISON_ASSERTION(NE, !=);
DECLARE_COMPARISON_ASSERTION(LT, <);
DECLARE_COMPARISON_ASSERTION(LTE, <=);
DECLARE_COMPARISON_ASSERTION(GT, >);
DECLARE_COMPARISON_ASSERTION(GTE, >=);
#undef DECLARE_COMPARISON_ASSERTION
