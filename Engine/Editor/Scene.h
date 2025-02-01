#pragma once

#include <fupch.h>

#include <list>

#include "glm/glm.hpp"


namespace Fuego::Editor
{
class Scene
{
public:
private:
    uint16_t objects_amount;
};

class SceneObject
{
public:
    virtual ~SceneObject();

private:
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    bool enabled;
};

class TreeNode
{
public:
    TreeNode();
    bool operator==(const TreeNode& other) const;
    virtual ~TreeNode() = default;

    virtual void AddChildFront(TreeNode&& child);
    virtual void AddChildBack(TreeNode&& child);
    virtual void RemoveChild(TreeNode& child);


protected:
    std::list<TreeNode> children;
    uint32_t id;
    static uint32_t s_id;

private:
    static uint32_t GetID();
};

class Root : public TreeNode
{
public:
    Root();

protected:
private:
};

class Node : public TreeNode
{
public:
    Node();

protected:
private:
    TreeNode* parent;
};

}  // namespace Fuego::Editor