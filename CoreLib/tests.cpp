#include "UniquePtr.h"

#include <gtest/gtest.h>

using namespace Fuego;

TEST(CoreLibTests, UniquePtr_NullptrTest)
{
    UniquePtr<int> Ptr;
    EXPECT_EQ(Ptr.Get(), nullptr);
}

TEST(CoreLibTests, UniquePtr_GetRawTest)
{
    int* RawPtr = new int(5);
    UniquePtr<int> Ptr(RawPtr);
    EXPECT_EQ(Ptr.Get(), RawPtr);
}

TEST(CoreLibTests, UniquePtr_DereferenceTest)
{
    UniquePtr<int> Ptr(new int(10));
    EXPECT_EQ(*Ptr, 10);
}

TEST(CoreLibTests, UniquePtr_MoveTest)
{
    UniquePtr<int> Ptr1(new int(15));
    UniquePtr<int> Ptr2(std::move(Ptr1));
    EXPECT_EQ(Ptr1.Get(), nullptr);
    EXPECT_EQ(*Ptr2, 15);
}

TEST(CoreLibTests, UniquePtr_ResetTest)
{
    UniquePtr<int> Ptr(new int(20));
    Ptr.Reset();
    EXPECT_EQ(Ptr.Get(), nullptr);
}

TEST(CoreLibTests, UniquePtr_ReleaseTest)
{
    UniquePtr<int> Ptr(new int(25));
    int* RawPtr = Ptr.Release();
    EXPECT_EQ(Ptr.Get(), nullptr);
    EXPECT_EQ(*RawPtr, 25);
    delete RawPtr;
}

TEST(CoreLibTests, UniquePtr_CustomDeleterTest)
{
    struct CustomDeleter {
        void operator()(int* ptr) {
            delete ptr;
        }
    };
    UniquePtr<int, CustomDeleter> Ptr(new int(5));
    EXPECT_TRUE(Ptr);
}

TEST(CoreLibTests, UniquePtr_ArrayTest)
{
    UniquePtr<int[]> Ptr(new int[3] {1, 2, 3});
    EXPECT_EQ(Ptr[0], 1);
    EXPECT_EQ(Ptr[2], 3);
}

TEST(CoreLibTests, UniquePtr_CustomDeleterTypeTest)
{
    struct CustomDeleter {
        void operator()(int* ptr) {
            delete ptr;
        }
        int Check()
        {
            return 3;
        }
    };

    UniquePtr<int, CustomDeleter> Ptr(new int(10));
    auto Deleter = Ptr.GetDeleter();
    EXPECT_EQ(Deleter.Check(), 3);
}

TEST(CoreLibTests, UniquePtr_CustomDeleterArrayTypeTest)
{
    struct CustomDeleter {
        void operator()(int* ptr) {
            delete[] ptr;
        }
        int Check()
        {
            return 3;
        }
    };

    UniquePtr<int[], CustomDeleter> Ptr(new int[3] {1, 2, 3});
    auto Deleter = Ptr.GetDeleter();
    EXPECT_EQ(Deleter.Check(), 3);
}

TEST(CoreLibTests, UniquePtr_SwapTest)
{
    UniquePtr<int> ptr1(new int(1));
    UniquePtr<int> ptr2(new int(2));
    ptr1.Swap(ptr2);
    EXPECT_EQ(*ptr1, 2);
    EXPECT_EQ(*ptr2, 1);
}

class LeakChecker
{
public:
    LeakChecker(int& InstanceCout_)
        : InstanceCout(InstanceCout_)
    {
        ++InstanceCout;
    }

    ~LeakChecker()
    {
        --InstanceCout;
    }

    int GetInstanceCount() const
    {
        return InstanceCout;
    }

private:
    int& InstanceCout;
};

TEST(CoreLibTests, UniquePtr_MemoryLeakTest_1)
{
    int InstanceCout = 0;

    {
        UniquePtr<LeakChecker> Ptr(new LeakChecker(InstanceCout));
        EXPECT_EQ(Ptr->GetInstanceCount(), 1);
    }

    EXPECT_EQ(InstanceCout, 0);
}

TEST(CoreLibTests, UniquePtr_MemoryLeakTest_2)
{
    int InstanceCout = 0;

    UniquePtr<LeakChecker> Ptr(new LeakChecker(InstanceCout));
    EXPECT_EQ(Ptr->GetInstanceCount(), 1);
    Ptr.Reset();

    EXPECT_EQ(InstanceCout, 0);
}

TEST(CoreLibTests, UniquePtr_ArrayMemoryLeakTest)
{
    int InstanceCount = 0;

    {
        UniquePtr<LeakChecker[]> Ptr(new LeakChecker[3] { InstanceCount , InstanceCount , InstanceCount });
        EXPECT_EQ(Ptr[2].GetInstanceCount(), 3);
    }

    EXPECT_EQ(InstanceCount, 0);
}

TEST(CoreLibTests, UniquePtr_BoolCastTest)
{
    UniquePtr<int> Ptr(nullptr);
    EXPECT_FALSE(bool(Ptr));
}

class CheckClassPtr
{
public:
    int Call()
    {
        return 11;
    }
};

TEST(CoreLibTests, UniquePtr_ClassPtrTest)
{
    UniquePtr<CheckClassPtr> ptr (new CheckClassPtr);
    EXPECT_EQ(ptr->Call(), 11);
}

TEST(CoreLibTests, UniquePtr_ClassPtrDereferanceTest)
{
    UniquePtr<CheckClassPtr> ptr(new CheckClassPtr);
    EXPECT_EQ((*ptr).Call(), 11);
}
