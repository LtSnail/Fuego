#pragma once

#include <fupch.h>

#include <list>

#include "glm/glm.hpp"


namespace Fuego::Editor
{
class Material;
class Root;
class BaseSceneObject;
class TreeNode;

class Scene
{
public:
    Scene(const std::string& scene_name);
    ~Scene();

    void AddObject(TreeNode* node, BaseSceneObject* obj);
    void AddObject(const std::string& parent_node, BaseSceneObject* obj);
    BaseSceneObject* FindObject(const std::string& object_name) const;
    TreeNode* FindNode(const std::string& object_name) const;

    inline const Root* GetRootNode() const
    {
        return root;
    }

private:
    Root* root;
    std::string scene_name;
    uint16_t objects_amount;
    std::unordered_map<std::string, TreeNode*> objects_map;
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

    virtual void AddChildFront(TreeNode child);
    virtual void AddChildBack(TreeNode child);
    virtual void RemoveChild(TreeNode& child);
    std::list<TreeNode>& GetChildren()
    {
        return children;
    }
    uint16_t GetNodeLevel() const;
    inline BaseSceneObject* GetSceneObject() const
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
    Root(const std::string& root_name);
};
class Node final : public TreeNode
{
public:
    Node(BaseSceneObject* obj, TreeNode* parent);

    inline const TreeNode* GetParent() const
    {
        return parent;
    }
    virtual void PrintNode() const override;

private:
    TreeNode* parent;
};

}  // namespace Fuego::Editor