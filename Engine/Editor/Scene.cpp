#include "Scene.h"

#include "../FileSystem/FileSystem.h"
#include "Application.h"

namespace Fuego::Editor
{
uint32_t TreeNode::s_id = 0;
uint32_t TreeNode::GetID()
{
    // TODO: to think about race conditions. std::atomic / mutex
    return ++s_id;
}

Scene::Scene(const std::string& scene_name)
    : scene_name(scene_name)
    , root(new Root("Root Folder"))
    , objects_amount(0)
{
    FU_CORE_TRACE("Scene ctor");
    objects_map.emplace(root->GetSceneObject()->GetName(), root);
}
Scene::~Scene()
{
    FU_CORE_TRACE("Scene destructor");
    delete root;
}
void Scene::AddObject(TreeNode* parent, Node&& new_node)
{
    TreeNode& moved_node = parent->GetChildren().emplace_back(std::move(new_node));

    objects_map.emplace(moved_node.GetSceneObject()->GetName(), &moved_node);
    objects_amount++;
    FU_CORE_TRACE("Scene object {0} was added to {1} node, parent: {2}, level: {3}", moved_node.GetSceneObject()->GetName(), moved_node.GetNodeLevel(),
                  parent->GetSceneObject()->GetName(), parent->GetNodeLevel());
}
void Scene::AddObject(const std::string& parent_node, Node&& new_node)
{
    TreeNode* parent = FindNode(parent_node);
    TreeNode& moved_node = parent->GetChildren().emplace_back(std::move(new_node));
    objects_map.emplace(moved_node.GetSceneObject()->GetName(), &moved_node);
    objects_amount++;
    FU_CORE_TRACE("Scene object: name {0}, level: {1} was added,  parent: {2}, level: {3}", moved_node.GetSceneObject()->GetName(), moved_node.GetNodeLevel(),
                  parent->GetSceneObject()->GetName(), parent->GetNodeLevel());
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
void Scene::SaveSceneToFile(const std::string& file_name)
{
    auto& fs = Fuego::Application::Get().FileSystem();
    std::string scene_file_name = file_name + ".fu_scene";
    fs.FUCreateFile(scene_file_name, "Scenes");
    fs.WriteToFile(scene_file_name, ParseScene().c_str());
}

std::string Scene::ParseScene() const
{
    return GetFormattedFUSONObject(FUSONObject<std::string>("Scene name", scene_name), FUSONObject<std::string>("Scene version", scene_version),
                                  FUSONObject<int>("Objects amount", objects_amount));
}

template <typename... T>
std::string Scene::GetFormattedFUSONObject(FUSONObject<T>... vars) const
{
    if (sizeof...(vars) == 0)
        return "";

    std::string result = "{\n";

    (ProcessVar(vars, result), ...);

    result.pop_back();
    result.pop_back();
    result += "\n}";
    return result;
}

template <typename T>
void Scene::ProcessVar(const FUSONObject<T>& obj, OUT std::string& str) const
{
    if constexpr (IsNumber<T>)
    {
        str.append("\t\"" + obj.key + "\": " + std::to_string(obj.value) + ",\n");
    }
    else if constexpr (StringLike<T>)
    {
        str.append("\t\"" + obj.key + "\": \"" + obj.value + "\",\n");
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

TreeNode::TreeNode(BaseSceneObject* obj, uint16_t node_level)
    : object(obj)
    , node_level(node_level)
    , id(GetID())
{
    FU_CORE_TRACE("TreeNode ctor");
}
TreeNode::~TreeNode()
{
    FU_CORE_TRACE("TreeNode dctor");
    delete object;
}
bool TreeNode::operator==(const TreeNode& other) const
{
    return this->id == other.id;
}
TreeNode::TreeNode(TreeNode&& other) noexcept
    : object(other.object)
    , node_level(other.node_level)
    , id(other.id)
    , children(std::move(other.children))
{
    FU_CORE_TRACE("TreeNode move ctor");
    other.object = nullptr;
    other.node_level = -1;
    other.id = 0;
}
TreeNode& TreeNode::operator=(TreeNode&& other) noexcept
{
    FU_CORE_TRACE("TreeNode move assigment ctor");

    if (this != &other)
    {
        object = nullptr;
        node_level = -1;
        id = 0;
        children.clear();

        object = other.object;
        node_level = other.node_level;
        id = other.id;
        children = std::move(other.children);

        other.object = nullptr;
        other.node_level = -1;
        other.id = 0;
        other.children.clear();
    }

    return *this;
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
    FU_CORE_TRACE("Root ctor");
}

Node::Node(BaseSceneObject* obj, TreeNode* parent)
    : TreeNode(obj, parent->GetNodeLevel() + 1)
    , parent(parent)
{
    FU_CORE_TRACE("Node ctor");
}
Node::Node(Node&& other) noexcept
    : TreeNode(std::move(other))
    , parent(other.parent)
{
    other.parent = nullptr;
    FU_CORE_TRACE("Node move ctor");
}
Node& Node::operator=(Node&& other) noexcept
{
    FU_CORE_TRACE("Node move assignment operator");

    if (this != &other)
    {
        TreeNode::operator=(std::move(other));
        parent = other.parent;

        other.parent = nullptr;
    }
    return *this;
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
}  // namespace Fuego::Editor