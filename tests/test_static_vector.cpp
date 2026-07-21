#include <gtest/gtest.h>

#include "DH/StaticVector.hpp"
#include "SpyObject.hpp"

TEST(StaticVectorTests, DestructorCleansUpCorrectly) {
    DH::Test::SpyObject::reset();
    {
        DH::StaticVector<DH::Test::SpyObject, 10> vector;
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);
    }
    EXPECT_EQ(DH::Test::SpyObject::destructions, 3);
}

TEST(StaticVectorTests, ComplexMoveConstructorCallsCorrectly) {
    DH::Test::SpyObject::reset();
    {
        DH::StaticVector<DH::Test::SpyObject, 10> vector;
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);

        DH::StaticVector<DH::Test::SpyObject, 10> vector2 = std::move(vector);
    }
    EXPECT_EQ(DH::Test::SpyObject::move_constructions, 3);
    EXPECT_EQ(DH::Test::SpyObject::destructions, 3);
}

static_assert(std::is_trivially_copyable_v<DH::Test::TrivialData>);
TEST(StaticVectorTests, TrivialTypesCopiesCorrectly) {
    DH::StaticVector<DH::Test::TrivialData, 10> vector;
    vector.push_back({1, 3.14f});

    DH::StaticVector<DH::Test::TrivialData, 10> vector2 = vector;
    EXPECT_EQ(vector2[0].x, 1);
}