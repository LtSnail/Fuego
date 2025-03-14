#pragma once

#include <fupch.h>

#include <list>
#include <type_traits>
#include <variant>

#include "glm/glm.hpp"

namespace Fuego::Editor
{
class Material;
class Root;
class BaseSceneObject;
class TreeNode;
class Node;

class Scene
{
public:
    Scene(const std::string& scene_name);
    ~Scene();

    void AddObject(TreeNode* parent, Node&& new_node);
    void AddObject(const std::string& parent_node, Node&& new_node);
    BaseSceneObject* FindObject(const std::string& object_name) const;
    TreeNode* FindNode(const std::string& object_name) const;

    inline Root* GetRootNode() const
    {
        return root;
    }

    void SaveSceneToFile(const std::string& file_name);

private:

    Root* root;
    std::string scene_name;
    std::string scene_version = "1.0";
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
    ModelObject(const std::string& name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), const Material* mat = nullptr);

private:
    const Material* material;
};

class TreeNode
{
public:
    TreeNode(BaseSceneObject* obj, uint16_t node_level);
    virtual ~TreeNode();

    bool operator==(const TreeNode& other) const;
    TreeNode(TreeNode&& other) noexcept;
    TreeNode& operator=(TreeNode&& other) noexcept;

    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;

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
    ~Root()
    {
        FU_CORE_TRACE("Root dctor");
    }

    Root(Root&& other)
        : TreeNode(std::move(other))
    {
        FU_CORE_TRACE("Root mode ctor");
    }
    Root& operator=(Root&& other)
    {
        TreeNode::operator=(std::move(other));
        FU_CORE_TRACE("Root move assigment operator");
        return *this;
    }

    Root(const Root& other) = delete;
    Root operator=(Root other) = delete;
};
class Node final : public TreeNode
{
public:
    Node(BaseSceneObject* obj, TreeNode* parent);
    ~Node()
    {
        FU_CORE_TRACE("Node destructor");
    }
    Node(Node&& other) noexcept;
    Node& operator=(Node&& other) noexcept;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    inline const TreeNode* GetParent() const
    {
        return parent;
    }
    virtual void PrintNode() const override;

private:
    TreeNode* parent;
};
}  // namespace Fuego::Editor