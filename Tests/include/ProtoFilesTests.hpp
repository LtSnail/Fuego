#include "gtest/gtest.h"
#include "SceneObjects.pb.h"

using namespace proto::scene;

void CompareVec3(const Vec3& a, const Vec3& b)
{
    EXPECT_FLOAT_EQ(a.x(), b.x());
    EXPECT_FLOAT_EQ(a.y(), b.y());
    EXPECT_FLOAT_EQ(a.z(), b.z());
}

void CompareBaseSceneObject(const BaseSceneObject& a, const BaseSceneObject& b)
{
    EXPECT_EQ(a.name(), b.name());
    EXPECT_EQ(a.enabled(), b.enabled());
    EXPECT_EQ(a.id(), b.id());
}

TEST(SceneObjectsTest, Vec3Serialization)
{
    Vec3 vec;
    vec.set_x(1.0f);
    vec.set_y(2.0f);
    vec.set_z(3.0f);

    std::string serialized;
    ASSERT_TRUE(vec.SerializeToString(&serialized));

    Vec3 deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    CompareVec3(vec, deserialized);
}

TEST(SceneObjectsTest, Vec3EdgeCases)
{
    Vec3 vec;
    vec.set_x(FLT_MAX);
    vec.set_y(FLT_MIN);
    vec.set_z(0.0f);

    std::string serialized;
    ASSERT_TRUE(vec.SerializeToString(&serialized));

    Vec3 deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    CompareVec3(vec, deserialized);
}

TEST(SceneObjectsTest, Vec3EmptySerialization)
{
    Vec3 vec;
    std::string serialized;
    ASSERT_TRUE(vec.SerializeToString(&serialized));

    Vec3 deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    EXPECT_FLOAT_EQ(deserialized.x(), 0.0f);
    EXPECT_FLOAT_EQ(deserialized.y(), 0.0f);
    EXPECT_FLOAT_EQ(deserialized.z(), 0.0f);
}

TEST(SceneObjectsTest, SceneObjectSerialization)
{
    SceneObject scene;
    scene.mutable_base()->set_name("TestObject");
    scene.mutable_base()->set_enabled(true);
    scene.mutable_base()->set_id(42);

    scene.mutable_position()->set_x(10.0f);
    scene.mutable_position()->set_y(20.0f);
    scene.mutable_position()->set_z(30.0f);

    scene.mutable_rotation()->set_x(0.0f);
    scene.mutable_rotation()->set_y(90.0f);
    scene.mutable_rotation()->set_z(180.0f);

    std::string serialized;
    ASSERT_TRUE(scene.SerializeToString(&serialized));

    SceneObject deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    CompareBaseSceneObject(scene.base(), deserialized.base());
    CompareVec3(scene.position(), deserialized.position());
    CompareVec3(scene.rotation(), deserialized.rotation());
}

TEST(SceneObjectsTest, SceneObjectEmptySerialization)
{
    SceneObject scene;
    std::string serialized;
    ASSERT_TRUE(scene.SerializeToString(&serialized));

    SceneObject deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    EXPECT_EQ(deserialized.base().name(), "");
    EXPECT_FALSE(deserialized.base().enabled());
    EXPECT_EQ(deserialized.base().id(), 0);
}

TEST(SceneObjectsTest, ModelObjectSerialization)
{
    ModelObject model;
    model.mutable_scene_base()->mutable_base()->set_name("Model1");
    model.mutable_scene_base()->mutable_base()->set_enabled(false);
    model.mutable_scene_base()->mutable_base()->set_id(99);

    model.mutable_scene_base()->mutable_position()->set_x(5.0f);
    model.mutable_scene_base()->mutable_position()->set_y(15.0f);
    model.mutable_scene_base()->mutable_position()->set_z(25.0f);

    model.set_material("Metal");

    std::string serialized;
    ASSERT_TRUE(model.SerializeToString(&serialized));

    ModelObject deserialized;
    ASSERT_TRUE(deserialized.ParseFromString(serialized));

    CompareBaseSceneObject(model.scene_base().base(), deserialized.scene_base().base());

    CompareVec3(model.scene_base().position(), deserialized.scene_base().position());

    ASSERT_EQ(model.material(), deserialized.material());
}

TEST(SceneObjectsTest, InvalidDeserialization)
{
    std::string invalid_data = "corrupt_data";

    Vec3 vec;
    EXPECT_FALSE(vec.ParseFromString(invalid_data));

    SceneObject scene;
    EXPECT_FALSE(scene.ParseFromString(invalid_data));

    ModelObject model;
    EXPECT_FALSE(model.ParseFromString(invalid_data));
}

TEST(SceneObjectsTest, ModelObjectComparison)
{
    ModelObject a, b;
    a.mutable_scene_base()->mutable_base()->set_name("ObjectA");
    a.mutable_scene_base()->mutable_base()->set_enabled(true);
    a.mutable_scene_base()->mutable_base()->set_id(1);
    a.mutable_scene_base()->mutable_position()->set_x(10.0f);
    a.set_material("Material_1");

    b.mutable_scene_base()->mutable_base()->set_name("ObjectA");
    b.mutable_scene_base()->mutable_base()->set_enabled(true);
    b.mutable_scene_base()->mutable_base()->set_id(1);
    b.mutable_scene_base()->mutable_position()->set_x(10.0f);
    b.set_material("Material_1");

    CompareBaseSceneObject(a.scene_base().base(), b.scene_base().base());
    CompareVec3(a.scene_base().position(), b.scene_base().position());
}