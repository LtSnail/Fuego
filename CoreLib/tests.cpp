#include <gtest/gtest.h>

#include "Uptr.h"

using namespace Fuego;


TEST(CoreLibTest, Uptr_DefaultConstructor)
{
    Uptr<int> Ptr;
    EXPECT_EQ(Ptr.Get(), nullptr);
}

TEST(CoreLibTest, Uptr_ConstructorWithPointer)
{
    int* RawPtr = new int(42);
    Uptr<int> Ptr(RawPtr);
    EXPECT_EQ(Ptr.Get(), RawPtr);
    EXPECT_EQ(*Ptr, 42);
}

TEST(CoreLibTest, Uptr_MoveConstructor)
{
    int* RawPtr = new int(42);
    Uptr<int> Ptr1(RawPtr);
    Uptr<int> Ptr2(std::move(Ptr1));

    EXPECT_EQ(Ptr2.Get(), RawPtr);
    EXPECT_EQ(Ptr1.Get(), nullptr);
    EXPECT_EQ(*Ptr2, 42);
}

TEST(CoreLibTest, Uptr_MoveAssignment)
{
    int* RawPtr1 = new int(42);
    int* RawPtr2 = new int(100);

    Uptr<int> Ptr1(RawPtr1);
    Uptr<int> Ptr2(RawPtr2);

    Ptr2 = std::move(Ptr1);

    EXPECT_EQ(Ptr2.Get(), RawPtr1);
    EXPECT_EQ(Ptr1.Get(), nullptr);
    EXPECT_EQ(*Ptr2, 42);
}

TEST(CoreLibTest, Uptr_Reset)
{
    int* RawPtr = new int(42);
    Uptr<int> Ptr(RawPtr);

    Ptr.Reset();
    EXPECT_EQ(Ptr.Get(), nullptr);

    int* newPtr = new int(100);
    Ptr.Reset(newPtr);
    EXPECT_EQ(Ptr.Get(), newPtr);
    EXPECT_EQ(*Ptr, 100);
}

TEST(CoreLibTest, Uptr_Release)
{
    int* RawPtr = new int(42);
    Uptr<int> Ptr(RawPtr);

    int* releasedPtr = Ptr.Release();
    EXPECT_EQ(releasedPtr, RawPtr);
    EXPECT_EQ(Ptr.Get(), nullptr);

    delete releasedPtr;
}

TEST(CoreLibTest, Uptr_Swap)
{
    int* RawPtr1 = new int(42);
    int* RawPtr2 = new int(100);

    Uptr<int> Ptr1(RawPtr1);
    Uptr<int> Ptr2(RawPtr2);

    Ptr1.Swap(Ptr2);

    EXPECT_EQ(Ptr1.Get(), RawPtr2);
    EXPECT_EQ(Ptr2.Get(), RawPtr1);
    EXPECT_EQ(*Ptr1, 100);
    EXPECT_EQ(*Ptr2, 42);
}

TEST(CoreLibTest, Uptr_BoolOperator)
{
    Uptr<int> Ptr;
    EXPECT_FALSE(Ptr);

    int* RawPtr = new int(42);
    Ptr.Reset(RawPtr);
    EXPECT_TRUE(Ptr);
}

TEST(CoreLibTest, UptrArray_ConstructorWithPointer)
{
    int* RawPtr = new int[5]{1, 2, 3, 4, 5};
    Uptr<int[]> Ptr(RawPtr, 5);

    EXPECT_EQ(Ptr.Get(), RawPtr);
    EXPECT_EQ(Ptr.Size(), 5);
    EXPECT_EQ(Ptr[0], 1);
    EXPECT_EQ(Ptr[4], 5);
}

TEST(CoreLibTest, UptrArray_Reset)
{
    int* RawPtr = new int[5]{1, 2, 3, 4, 5};
    Uptr<int[]> Ptr(RawPtr, 5);

    Ptr.Reset();
    EXPECT_EQ(Ptr.Get(), nullptr);
    EXPECT_EQ(Ptr.Size(), 0);

    int* newPtr = new int[3]{10, 20, 30};
    Ptr.Reset(newPtr, 3);
    EXPECT_EQ(Ptr.Get(), newPtr);
    EXPECT_EQ(Ptr.Size(), 3);
    EXPECT_EQ(Ptr[0], 10);
    EXPECT_EQ(Ptr[2], 30);
}

TEST(CoreLibTest, UptrArray_Release)
{
    int* RawPtr = new int[5]{1, 2, 3, 4, 5};
    Uptr<int[]> Ptr(RawPtr, 5);

    int* releasedPtr = Ptr.Release();
    EXPECT_EQ(releasedPtr, RawPtr);
    EXPECT_EQ(Ptr.Get(), nullptr);
    EXPECT_EQ(Ptr.Size(), 0);

    delete[] releasedPtr;
}

TEST(CoreLibTest, UptrArray_Swap)
{
    int* RawPtr1 = new int[3]{1, 2, 3};
    int* RawPtr2 = new int[2]{10, 20};

    Uptr<int[]> Ptr1(RawPtr1, 3);
    Uptr<int[]> Ptr2(RawPtr2, 2);

    Ptr1.Swap(Ptr2);

    EXPECT_EQ(Ptr1.Get(), RawPtr2);
    EXPECT_EQ(Ptr2.Get(), RawPtr1);
    EXPECT_EQ(Ptr1.Size(), 2);
    EXPECT_EQ(Ptr2.Size(), 3);
    EXPECT_EQ(Ptr1[0], 10);
    EXPECT_EQ(Ptr2[2], 3);
}

TEST(CoreLibTest, UptrArray_BoolOperator)
{
    Uptr<int[]> Ptr;
    EXPECT_FALSE(Ptr);

    int* RawPtr = new int[5]{1, 2, 3, 4, 5};
    Ptr.Reset(RawPtr, 5);
    EXPECT_TRUE(Ptr);
}

TEST(CoreLibTest, Uptr_Comparison_SamePointers)
{
    int* RawPtr = new int(42);
    Uptr<int> Ptr1(RawPtr);
    Uptr<int> Ptr2(RawPtr);

    EXPECT_EQ(Ptr1 <=> Ptr2, std::strong_ordering::equal);
    EXPECT_FALSE(Ptr1 < Ptr2);
    EXPECT_FALSE(Ptr1 > Ptr2);
    EXPECT_TRUE(Ptr1 == Ptr2);

    Ptr1.Release();
}

// TEST(CoreLibTest, Uptr_Comparison_WithNullptr)
//{
//     Uptr<int> Ptr1;
//     int* RawPtr = new int(42);
//     Uptr<int> Ptr2(RawPtr);
//
//     EXPECT_LT(Ptr1 <=> Ptr2, 0);
//     EXPECT_GT(Ptr2 <=> Ptr1, 0);
//     EXPECT_EQ(Ptr1.Get(), nullptr);
// }

TEST(CoreLibTest, Uptr_Comparison_DifferentPointers)
{
    int* RawPtr1 = new int(42);
    int* RawPtr2 = new int(100);

    Uptr<int> Ptr1(RawPtr1);
    Uptr<int> Ptr2(RawPtr2);

    EXPECT_NE(Ptr1 <=> Ptr2, std::strong_ordering::equal);
    EXPECT_TRUE(Ptr1.Get() < Ptr2.Get() || Ptr1.Get() > Ptr2.Get());
}

TEST(CoreLibTest, UptrArray_PointerArithmetic)
{
    int* RawPtr = new int[5]{1, 2, 3, 4, 5};
    Uptr<int[]> Ptr(RawPtr, 5);

    EXPECT_EQ(Ptr[0], 1);
    EXPECT_EQ(Ptr[1], 2);
    EXPECT_EQ(Ptr[4], 5);

    EXPECT_EQ(RawPtr + 0, Ptr + 0);
    EXPECT_EQ(RawPtr + 1, Ptr + 1);
    EXPECT_EQ(RawPtr + 4, Ptr + 4);
}

TEST(CoreLibTest, SingleObject_DefaultConstruction)
{
    Uptr<int> ptr = MakeUnique<int>();
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 0);
}

TEST(CoreLibTest, SingleObject_CustomConstruction)
{
    Uptr<int> ptr = MakeUnique<int>(42);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 42);
}

struct TestStruct
{
    int a;
    float b;

    TestStruct(int x, float y)
        : a(x)
        , b(y)
    {
    }
};

TEST(MakeUniqueTest, SingleObject_CustomType)
{
    Uptr<TestStruct> ptr = MakeUnique<TestStruct>(10, 3.14f);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->a, 10);
    EXPECT_FLOAT_EQ(ptr->b, 3.14f);
}

// Проверка корректного освобождения памяти
TEST(MakeUniqueTest, SingleObject_MemoryDeallocation)
{
    Uptr<int> ptr = MakeUnique<int>(100);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 100);

    ptr.Reset();
    EXPECT_FALSE(ptr);
}
