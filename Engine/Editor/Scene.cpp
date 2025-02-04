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
void TreeNode::AddChildFront(TreeNode child)
{
    children.push_front(std::move(child));
}
void TreeNode::AddChildBack(TreeNode child)
{
    children.emplace_back(std::move(child));
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
        FU_CORE_TRACE("Child: ");
        it->PrintNode();
    }
}

Root::Root(const std::string& root_name)
    : TreeNode(new SceneFolder(root_name), 0)
{
}

Node::Node(BaseSceneObject* obj, TreeNode* parent)
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


Scene::Scene(const std::string& scene_name)
    : scene_name(scene_name)
    , root(nullptr)
    , objects_amount(0)
{
    root = new Root("Root Folder");
    objects_map.emplace("Root Folder", root);
}
Scene::~Scene()
{
    delete root;
}
void Scene::AddObject(TreeNode* node, BaseSceneObject* obj)
{
    node->AddChildBack(TreeNode(obj, node->GetNodeLevel() + 1));
    TreeNode& new_child = node->GetChildren().back();
    objects_map.emplace(obj->GetName(), &new_child);
    objects_amount++;
    FU_CORE_TRACE("Scene object {0} was added to {1} node, parent: {2}, level: {3}", 
        obj->GetName(), new_child.GetNodeLevel(),
                  node->GetSceneObject()->GetName(),
                  node->GetNodeLevel());
}
void Scene::AddObject(const std::string& parent_node, BaseSceneObject* obj)
{
    TreeNode* node = FindNode(parent_node);
    node->AddChildBack(Node(obj, node));
    TreeNode& new_child = node->GetChildren().back();
    objects_map.emplace(obj->GetName(), &new_child);
    objects_amount++;
    FU_CORE_TRACE("Scene object {0} was added to {1} node, parent: {2}, level: {3}", obj->GetName(), new_child.GetNodeLevel(),
                  node->GetSceneObject()->GetName(), node->GetNodeLevel());
}
BaseSceneObject* Scene::FindObject(const std::string& object_name) const
{
    auto it = objects_map.find(object_name);
    if (it != objects_map.end())
    {
        return it->second->GetSceneObject();
    }
    return nullptr;
}

TreeNode* Scene::FindNode(const std::string& object_name) const
{
    auto it = objects_map.find(object_name);
    if (it != objects_map.end())
    {
        return it->second;
    }
    return nullptr;
}

}  // namespace Fuego::Editor