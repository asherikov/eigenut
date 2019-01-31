/**
    @file
    @author  Alexander Sherikov
    @copyright 2019 Alexander Sherikov. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#include <eigenut/all.h>
#include <gtest/gtest.h>


TEST(Inclusion, Inclusion)
{
    EXPECT_TRUE(true);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
