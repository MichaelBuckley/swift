#include "swift/Basic/Unicode.h"
#include "gtest/gtest.h"

using namespace swift::unicode;

TEST(IsSingleExtendedGraphemeCluster, Test1) {
  EXPECT_EQ(false, isSingleExtendedGraphemeCluster(""));
  EXPECT_EQ(true, isSingleExtendedGraphemeCluster("a"));
}
