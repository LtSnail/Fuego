#pragma once

#include <fupch.h>
#include <list>
#include "glm/glm.hpp"

namespace Fuego
{
class Material;
class Scene;

class BaseSceneObject
{
public:
    BaseSceneObject(Scene* master_scene, const std::string& name, bool enabled);
    virtual ~BaseSceneObject() = default;

    inline const std::string& GetName() const
    {
        return name;
    }
private:
    std::string name;
    bool enabled;
    Scene* master_scene;

    static uint32_t GetID();
    uint32_t id;
    static uint32_t s_id;
};
class SceneFolder : public BaseSceneObject
{
public:
    SceneFolder(Scene* master_scene, const std::string& folder_name);
};
class SceneObject : public BaseSceneObject
{
public:
    SceneObject(Scene* master_scene, const std::string & name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));

private:
    glm::vec3 position;
    glm::vec3 rotation;
};
class ModelObject : public SceneObject
{
public:
    ModelObject(Scene* master_scene, const std::string & name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), const Material* mat = nullptr);

private:
    const Material* material;
};

class Node
{
public:
    template<typename T>
    Node(Node* parent, T* obj)
        : parent(parent)
        , object(obj)
    {
        FU_CORE_TRACE("Node ctor");
        
    }
    ~Node()
    {
        FU_CORE_TRACE("DELETE: {0}", object->GetName());
    }

    std::list<Node>& GetChildren()
    {
        return children;
    }
    template <typename T>
    inline T* GetSceneObject()
    {
        return object;
    }

    void PrintNode() const;
    inline Node* GetParent()
    {
        return parent;
    }

    
    template <typename T>
    std::list<Node>::iterator AddChild(T scene_obj)
    {
        children.emplace_back(this, scene_obj);
        return std::prev(children.end());
    }
    void RemodeChild(std::list<Node>::iterator);

private:
    Node* parent;
    

    BaseSceneObject* object;

    std::list<Node> children;


};

class Scene
{

public:
    Scene(const std::string& new_scene_obj);
    ~Scene();

    /// <summary>
    /// Pass SceneObject constructor arguments without ptr to Scene, it passes by scene itself
    /// this is gonna be changed by mixin class soon
    /// </summary>
    template <typename T, typename... Args>
        requires std::is_base_of_v<BaseSceneObject, T>
    T* AddObject(BaseSceneObject* parent, Args&&... args)
    {
        T* scene_obj = new T(this, std::forward<Args>(args)...);

        std::list<Node>::iterator node_it{};
        if (parent)
        {
            std::list<Node>::iterator parent_node = object_to_node_map[parent];
            node_it = parent_node->AddChild(scene_obj);
        }
        else
        {
            children.emplace_back(nullptr, scene_obj);
            node_it = std::prev(children.end());
        }
        object_to_node_map.emplace(scene_obj, node_it);
        string_to_object_map.emplace(scene_obj->GetName(), scene_obj);
        objects_amount++;

        return scene_obj;
    }

    template <typename T, typename... Args>
        requires std::is_base_of_v<BaseSceneObject, T>
    T* AddObject(std::string_view parent_name, Args&&... args)
    {
        BaseSceneObject* parent = FindObject(parent_name);
        if (!parent)
            return nullptr;

        T* scene_obj = new T(this, std::forward<Args>(args)...);

        std::list<Node>::iterator parent_it = GetNodeIt(parent);

        std::list<Node>::iterator child_node_it = parent_it->AddChild(scene_obj);

        object_to_node_map.emplace(scene_obj, child_node_it);
        string_to_object_map.emplace(scene_obj->GetName(), scene_obj);
        objects_amount++;

        return scene_obj;
    }

    BaseSceneObject* FindObject(std::string_view object_name) const;

    void RemoveObject(std::string_view object_name);

    BaseSceneObject* Root();
    void SaveSceneToFile(const std::string& file_name);

private:

    SceneFolder* root_obj;
    std::string scene_name;
    std::string scene_version = "1.0";
    uint16_t objects_amount;
    std::unordered_map<BaseSceneObject*, std::list<Node>::iterator> object_to_node_map;
    std::unordered_map<std::string_view, BaseSceneObject*> string_to_object_map;
    std::list<Node>::iterator GetNodeIt(BaseSceneObject* obj);
    std::list<Node> children;
};

}  // namespace Fuego::Editor