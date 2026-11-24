#include "gtest/gtest.h"
#include "Version.h"

TEST(Version, hardware_version)
{
    const char* hardware_version = HARDWARE_VERSION;
    ASSERT_EQ(hardware_version, "HW6.3D");
}

TEST(Model, model_name)
{
    int model_name = MODEL_NAME;
    ASSERT_EQ(model_name, 60);
}
