#include "gtest/gtest.h"
#include "HW_Config.h"

TEST(Hardware, hw_ver)
{
    int hw_ver = HW_VER;
    ASSERT_EQ(hw_ver, 65);
}
