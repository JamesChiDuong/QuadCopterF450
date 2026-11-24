#include "gtest/gtest.h"
#include "Config.h"

TEST(Customer, customer_type)
{
    int customer_type = CUSTOMER_TYPE;
    ASSERT_EQ(customer_type, 0);
}
