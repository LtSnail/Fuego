#include "Scene.h"

#include "../FileSystem/FileSystem.h"
#include "Application.h"

#pragma region Concepts

#pragma endregion

namespace Fuego
{
uint32_t BaseSceneObject::s_id = 0;

void Node::RemodeChild(std::list<Node>::iterator it)
{
    children.erase(it);
}

uint32_t BaseSceneObject::GetID()
{
    // TODO: to think about race conditions. std::atomic / mutex
    return ++s_id;
}

Scene::Scene(const std::string& scene_name)
    : scene_name(scene_name)
    , objects_amount(0)
{
    FU_CORE_TRACE("Scene ctor");

    root_obj = AddObject<SceneFolder>(nullptr, "Root Folder");
}
Scene::~Scene()
{
    FU_CORE_TRACE("Scene destructor");
    delete root_obj;
}

BaseSceneObject* Scene::FindObject(std::string_view object_name) const
{
    auto it = string_to_object_map.find(object_name);
    return (it != string_to_object_map.end()) ? it->second : nullptr;
}

void Scene::RemoveObject(std::string_view object_name)
{
    BaseSceneObject* obj = FindObject(object_name);
    if (!obj)
        return;

    std::list<Node>::iterator obj_node_it = GetNodeIt(obj);
    Node* parent_node = obj_node_it->GetParent();
    if (!parent_node)
    {
        children.erase(obj_node_it);
    }
    else
    {
        parent_node->RemodeChild(obj_node_it);
    }
    object_to_node_map.erase(obj);
    string_to_object_map.erase(object_name);
    objects_amount--;
}

BaseSceneObject* Scene::Root()
{
    return static_cast<BaseSceneObject*>(root_obj);
}
void Scene::SaveSceneToFile(const std::string& file_name)
{
    auto fs = ServiceLocator::instance().GetService<Fuego::FS::FileSystem>();
    std::string scene_file_name = file_name + ".fu_scene";
    fs->FUCreateFile(scene_file_name, "Scenes");
}

std::list<Node>::iterator Scene::GetNodeIt(BaseSceneObject* obj)
{
    auto it = object_to_node_map.find(obj);
    return (it != object_to_node_map.end()) ? it->second : it->second->GetParent()->GetChildren().end();
}

BaseSceneObject::BaseSceneObject(Scene* master_scene, const std::string& name, bool enabled)
    : name(name)
    , enabled(enabled)
    , master_scene(master_scene)
{
    id = BaseSceneObject::GetID();
    FU_CORE_INFO("BaseSceneObject: {0} ctor", this->name);
}

SceneFolder::SceneFolder(Scene* master_scene, const std::string& folder_name)
    : BaseSceneObject(master_scene, folder_name, true)
{
    FU_CORE_INFO("SceneFolder: {0} ctor", ((BaseSceneObject*)this)->GetName());
}
SceneObject::SceneObject(Scene* master_scene, const std::string& name, glm::vec3 pos, glm::vec3 rot)
    : BaseSceneObject(master_scene, name, true)
    , position(pos)
    , rotation(rot)
{
    FU_CORE_INFO("SceneObject: {0} ctor", ((BaseSceneObject*)this)->GetName());
}
ModelObject::ModelObject(Scene* master_scene, const std::string& name, glm::vec3 pos, glm::vec3 rot, const Material* mat)
    : SceneObject(master_scene, name, pos, rot)
    , material(mat)
{
    FU_CORE_INFO("ModelObject: {0} ctor", ((BaseSceneObject*)this)->GetName());
}

void Node::PrintNode() const
{
    // TODO
}

}  // namespace Fuego
