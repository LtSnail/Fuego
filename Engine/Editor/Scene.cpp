#include "Scene.h"

namespace Fuego::Editor
{
uint32_t TreeNode::s_id = 0;
uint32_t TreeNode::GetID()
{
    // TODO: to think about race conditions. std::atomic / mutex
    return ++s_id;
}

TreeNode::TreeNode(BaseSceneObject* obj, uint16_t node_level)
    : object(obj)
    , node_level(node_level)
    , id(GetID())
{
}
TreeNode::~TreeNode()
{
    delete object;
}
bool TreeNode::operator==(const TreeNode& other) const
{
    return this->id == other.id;
}
void TreeNode::AddChildFront(TreeNode&& child)
{
    children.push_front(std::move(child));
}
void TreeNode::AddChildBack(TreeNode&& child)
{
    children.push_back(std::move(child));
}
void TreeNode::RemoveChild(TreeNode& child)
{
    for (std::list<TreeNode>::iterator it = children.begin(); it != children.end(); it++)
    {
        if (*it == child)
        {
            children.erase(it);
        }
    }
}
uint16_t TreeNode::GetNodeLevel() const
{
    return node_level;
}
void TreeNode::PrintNode() const
{
    FU_CORE_TRACE("Node name: {0}, level: {1}", object->GetName(), node_level);
    for (std::list<TreeNode>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        FU_CORE_TRACE("Child: {0}", it->object->GetName());
    }
}
Root::Root()
    : TreeNode(new SceneFolder("Root Folder"), 0)
{
}
Node::Node(const std::string& node_name, BaseSceneObject* obj, TreeNode* parent)
    : TreeNode(obj, parent->GetNodeLevel() + 1)
    , parent(parent)
{
}
void Node::PrintNode() const
{
    TreeNode::PrintNode();
    Node* par = dynamic_cast<Node*>(parent);
    if (par)
    {
        FU_CORE_TRACE("Parent Node: {0}", par->GetSceneObject()->GetName());
    }
    else
    {
        FU_CORE_TRACE("Parent Node is Root node");
    }
}
BaseSceneObject::BaseSceneObject(const std::string& name, bool enabled)
    : name(name)
    , enabled(enabled)
{
    FU_CORE_INFO("BaseSceneObject: {0} ctor", this->name);
}
SceneFolder::SceneFolder(const std::string& folder_name)
    : BaseSceneObject(folder_name, true)
{
    FU_CORE_INFO("SceneFolder: {0} ctor", ((BaseSceneObject*)this)->GetName());
}
SceneObject::SceneObject(const std::string& name, glm::vec3 pos, glm::vec3 rot)
    : BaseSceneObject(name, true)
    , position(pos)
    , rotation(rot)
{
    FU_CORE_INFO("SceneObject: {0} ctor", ((BaseSceneObject*)this)->GetName());
}
ModelObject::ModelObject(const std::string& name, glm::vec3 pos, glm::vec3 rot, const Material* mat)
    : SceneObject(name, pos, rot)
    , material(mat)
{
    FU_CORE_INFO("ModelObject: {0} ctor", ((BaseSceneObject*)this)->GetName());
}


Scene::Scene(const std::string& scene_name, Root* root)
    : scene_name(scene_name)
    , root(root)
{
}

Scene::~Scene()
{
    delete root;
}

}  // namespace Fuego::Editor