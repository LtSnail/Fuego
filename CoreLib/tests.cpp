#include <gtest/gtest.h>

#include "SharedPtr.h"
#include "UniquePtr.h"

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
    struct CustomDeleter
    {
        void operator()(int* ptr)
        {
            delete ptr;
        }
    };
    UniquePtr<int, CustomDeleter> Ptr(new int(5));
    EXPECT_TRUE(Ptr);
}

TEST(CoreLibTests, UniquePtr_ArrayTest)
{
    UniquePtr<int[]> Ptr(new int[3]{1, 2, 3});
    EXPECT_EQ(Ptr[0], 1);
    EXPECT_EQ(Ptr[2], 3);
}

TEST(CoreLibTests, UniquePtr_CustomDeleterTypeTest)
{
    struct CustomDeleter
    {
        void operator()(int* ptr)
        {
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
    struct CustomDeleter
    {
        void operator()(int* ptr)
        {
            delete[] ptr;
        }
        int Check()
        {
            return 3;
        }
    };

    UniquePtr<int[], CustomDeleter> Ptr(new int[3]{1, 2, 3});
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
        UniquePtr<LeakChecker[]> Ptr(new LeakChecker[3]{InstanceCount, InstanceCount, InstanceCount});
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
    UniquePtr<CheckClassPtr> Ptr(new CheckClassPtr);
    EXPECT_EQ(Ptr->Call(), 11);
}

TEST(CoreLibTests, UniquePtr_ClassPtrDereferanceTest)
{
    UniquePtr<CheckClassPtr> Ptr(new CheckClassPtr);
    EXPECT_EQ((*Ptr).Call(), 11);
}

TEST(CoreLibTests, SharedPtr_DefaultConstructorTest)
{
    SharedPtr<int> Sp;
    EXPECT_EQ(Sp.Get(), nullptr);
    EXPECT_EQ(Sp.UseCount(), 0);
}

TEST(CoreLibTests, SharedPtr_ConstructorWithPointerTest)
{
    SharedPtr<int> Sp(new int(42));
    EXPECT_NE(Sp.Get(), nullptr);
    EXPECT_EQ(*Sp, 42);
    EXPECT_EQ(Sp.UseCount(), 1);
}

TEST(CoreLibTests, SharedPtr_DestructorReleasesResourceTest)
{
    SharedPtr<int>* Sp = new SharedPtr<int>(new int(42));
    delete Sp;
    SUCCEED();
}

TEST(CoreLibTests, SharedPtr_CopyConstructorTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = Sp1;
    EXPECT_EQ(Sp1.Get(), Sp2.Get());
    EXPECT_EQ(Sp1.UseCount(), 2);
}

TEST(CoreLibTests, SharedPtr_MoveConstructorTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = std::move(Sp1);
    EXPECT_EQ(Sp1.Get(), nullptr);
    EXPECT_EQ(Sp2.UseCount(), 1);
}

TEST(CoreLibTests, SharedPtr_CopyAssignmentTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = SharedPtr<int>();
    Sp2 = Sp1;
    EXPECT_EQ(Sp1.Get(), Sp2.Get());
    EXPECT_EQ(Sp1.UseCount(), 2);
}

TEST(CoreLibTests, SharedPtr_MoveAssignmentTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = SharedPtr<int>();
    Sp2 = std::move(Sp1);
    EXPECT_EQ(Sp1.Get(), nullptr);
    EXPECT_EQ(Sp2.UseCount(), 1);
}

TEST(CoreLibTests, SharedPtr_ArrayAccessTest)
{
    auto Sp = SharedPtr<int[]>(new int[3]{1, 2, 3});
    EXPECT_EQ(Sp[0], 1);
    EXPECT_EQ(Sp[1], 2);
    EXPECT_EQ(Sp[2], 3);
    Sp[1] = 42;
    EXPECT_EQ(Sp[1], 42);
}

TEST(CoreLibTests, WeakPtr_LockReturnsSharedPtrTest)
{
    auto Sp = SharedPtr<int>(new int(42));
    WeakPtr<int> Wp(Sp);
    auto Sp2 = Wp.Lock();
    EXPECT_EQ(Sp.Get(), Sp2.Get());
    EXPECT_EQ(Sp2.UseCount(), 2);
}

TEST(CoreLibTests, WeakPtr_ExpiredReturnsTrueTest)
{
    WeakPtr<int> Wp;
    {
        auto Sp = SharedPtr<int>(new int(42));
        Wp = Sp;
    }
    EXPECT_TRUE(Wp.Expired());
}

TEST(CoreLibTests, WeakPtr_ReleaseResetsWeakPtrTest)
{
    auto Sp = SharedPtr<int>(new int(42));
    WeakPtr<int> Wp(Sp);
    Wp.Release();
    EXPECT_TRUE(Wp.Expired());
    EXPECT_EQ(Wp.Lock().Get(), nullptr);
}

TEST(CoreLibTests, SharedPtr_ResetReleasesResourceTest)
{
    auto Sp = SharedPtr<int>(new int(42));
    Sp.Reset();
    EXPECT_EQ(Sp.Get(), nullptr);
    EXPECT_EQ(Sp.UseCount(), 0);
}

TEST(CoreLibTests, SharedPtr_SwapExchangesResourcesTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = SharedPtr<int>(new int(24));
    Sp1.Swap(Sp2);
    EXPECT_EQ(*Sp1, 24);
    EXPECT_EQ(*Sp2, 42);
}

TEST(CoreLibTests, SharedPtr_UseCountIncreasesOnCopyTest)
{
    auto Sp1 = SharedPtr<int>(new int(42));
    auto Sp2 = Sp1;
    EXPECT_EQ(Sp1.UseCount(), 2);
    EXPECT_EQ(Sp2.UseCount(), 2);
}

TEST(CoreLibTests, SharedPtr_UseCountDecreasesOnResetTest)
{
    auto Sp = SharedPtr<int>(new int(42));
    Sp.Reset();
    EXPECT_EQ(Sp.UseCount(), 0);
}

TEST(CoreLibTests, SharedPtrWeakPtr_SharedPtrFromWeakPtrTest)
{
    auto Sp = SharedPtr<int>(new int(42));
    WeakPtr<int> Wp(Sp);
    auto Sp2 = Wp.Lock();
    EXPECT_EQ(Sp.Get(), Sp2.Get());
    EXPECT_EQ(Sp2.UseCount(), 2);
}

TEST(CoreLibTests, SharedPtrWeakPtr_WeakPtrTracksResourceTest)
{
    WeakPtr<int> Wp;
    {
        auto Sp = SharedPtr<int>(new int(42));
        Wp = Sp;
        EXPECT_FALSE(Wp.Expired());
    }
    EXPECT_TRUE(Wp.Expired());
}

TEST(CoreLibTests, SharedPtr_CompareTwoNullPtrs)
{
    SharedPtr<int> Ptr1;
    SharedPtr<int> Ptr2;

    EXPECT_TRUE(Ptr1 == Ptr2);
    EXPECT_FALSE(Ptr1 != Ptr2);
    EXPECT_FALSE(Ptr1 < Ptr2);
    EXPECT_FALSE(Ptr1 > Ptr2);
    EXPECT_TRUE(Ptr1 <= Ptr2);
    EXPECT_TRUE(Ptr1 >= Ptr2);
}

TEST(CoreLibTests, SharedPtr_CompareSameObject)
{
    auto RawPtr = new int(10);
    SharedPtr<int> Ptr1(RawPtr);
    SharedPtr<int> Ptr2(Ptr1);

    EXPECT_TRUE(Ptr1 == Ptr2);
    EXPECT_FALSE(Ptr1 != Ptr2);
    EXPECT_FALSE(Ptr1 < Ptr2);
    EXPECT_FALSE(Ptr1 > Ptr2);
    EXPECT_TRUE(Ptr1 <= Ptr2);
    EXPECT_TRUE(Ptr1 >= Ptr2);
}

TEST(CoreLibTests, SharedPtr_CompareDifferentObjects)
{
    SharedPtr<int> Ptr1(new int(10));
    SharedPtr<int> Ptr2(new int(20));

    EXPECT_FALSE(Ptr1 == Ptr2);
    EXPECT_TRUE(Ptr1 != Ptr2);
    EXPECT_TRUE((Ptr1 < Ptr2) || (Ptr1 > Ptr2));
}

TEST(CoreLibTests, SharedPtr_CompareWithNullPtr)
{
    SharedPtr<int> Ptr(new int(10));
    SharedPtr<int> nPtr;

    EXPECT_FALSE(Ptr == nPtr);
    EXPECT_TRUE(Ptr != nPtr);
    EXPECT_TRUE(nPtr == nPtr);
    EXPECT_FALSE(nPtr != nPtr);
}

TEST(CoreLibTests, SharedPtr_CompareWithRawPointer)
{
    int* RawPtr = new int(10);
    SharedPtr<int> Ptr(RawPtr);

    EXPECT_TRUE(Ptr == RawPtr);
    EXPECT_FALSE(Ptr != RawPtr);
}

TEST(CoreLibTests, UniquePtr_CompareTwoNullPtrs)
{
    UniquePtr<int> Ptr1;
    UniquePtr<int> Ptr2;

    EXPECT_TRUE(Ptr1 == Ptr2);
    EXPECT_FALSE(Ptr1 != Ptr2);
}

TEST(CoreLibTests, UniquePtr_CompareWithSelf)
{
    UniquePtr<int> Ptr(new int(10));

    EXPECT_TRUE(Ptr == Ptr);
    EXPECT_FALSE(Ptr != Ptr);
}

TEST(CoreLibTests, UniquePtr_CompareDifferentObjects)
{
    UniquePtr<int> Ptr1(new int(10));
    UniquePtr<int> Ptr2(new int(20));

    EXPECT_FALSE(Ptr1 == Ptr2);
    EXPECT_TRUE(Ptr1 != Ptr2);
}

TEST(CoreLibTests, UniquePtr_CompareWithNullPtr)
{
    UniquePtr<int> Ptr(new int(10));
    UniquePtr<int> nPtr;

    EXPECT_FALSE(Ptr == nPtr);
    EXPECT_TRUE(Ptr != nPtr);
    EXPECT_TRUE(nPtr == nPtr);
    EXPECT_FALSE(nPtr != nPtr);
}
