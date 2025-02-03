#pragma once

#include <fupch.h>

#include <list>

#include "glm/glm.hpp"


namespace Fuego::Editor
{
class Material;
class Root;

class Scene
{
public:
    Scene(const std::string& scene_name);
    ~Scene();

    inline const Root* GetRootNode() const
    {
        return root;
    }

private:
    Root* root;
    std::string scene_name;
    uint16_t objects_amount;
};
class BaseSceneObject
{
public:
    BaseSceneObject(const std::string& name, bool enabled);
    virtual ~BaseSceneObject() = default;

    inline const std::string& GetName() const
    {
        return name;
    }

private:
    std::string name;
    bool enabled;
};
class SceneFolder : public BaseSceneObject
{
public:
    SceneFolder(const std::string& folder_name);
};
class SceneObject : public BaseSceneObject
{
public:
    SceneObject(const std::string& name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));

private:
    glm::vec3 position;
    glm::vec3 rotation;
};
class ModelObject : public SceneObject
{
public:
    ModelObject(const std::string& name, glm::vec3 pos, glm::vec3 rot, const Material* mat);

private:
    const Material* material;
};


class TreeNode
{
public:
    TreeNode(BaseSceneObject* obj, uint16_t node_level);
    virtual ~TreeNode();

    bool operator==(const TreeNode& other) const;
    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;
    TreeNode(TreeNode&&) noexcept = default;
    TreeNode& operator=(TreeNode&&) noexcept = default;

    virtual void AddChildFront(TreeNode&& child);
    virtual void AddChildBack(TreeNode&& child);
    virtual void RemoveChild(TreeNode& child);

    uint16_t GetNodeLevel() const;
    inline const BaseSceneObject* GetSceneObject() const
    {
        return object;
    }

    virtual void PrintNode() const;

protected:
    static uint32_t GetID();
    uint16_t node_level;

private:
    BaseSceneObject* object;

    std::list<TreeNode> children;

    uint32_t id;
    static uint32_t s_id;
};
class Root final : public TreeNode
{
public:
    Root();
};
class Node final : public TreeNode
{
public:
    Node(const std::string& node_name, BaseSceneObject* obj, TreeNode* parent);

    inline const TreeNode* GetParent() const
    {
        return parent;
    }
    virtual void PrintNode() const override;

private:
    TreeNode* parent;
};

}  // namespace Fuego::Editor