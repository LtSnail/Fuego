#include "Model.h"

#include <filesystem>

#include "External/cgltf/cgltf.h"
#include "Renderer.h"
#include "fstream"

Fuego::Graphics::Model::Model(cgltf_data* data)
    : name(std::move(std::filesystem::path(data->scene->name).stem().string()))
    , mesh_count(data->meshes_count)
    , vertex_count(0)
    , indices_count(0)
{
    process_model(data, false);
}

Fuego::Graphics::Model::Model(std::string_view model_name)
    : name(model_name)
    , mesh_count(0)
    , vertex_count(0)
    , indices_count(0)
{
}

Fuego::Graphics::Model::Model(Model&& other) noexcept
    : name(std::move(other.name))
    , mesh_count(other.mesh_count)
    , vertex_count(other.vertex_count)
    , meshes(std::move(other.meshes))
    , indices_count(other.indices_count)
{
    other.mesh_count = 0;
    other.vertex_count = 0;
}

Fuego::Graphics::Model& Fuego::Graphics::Model::operator=(Model&& other) noexcept
{
    if (this != &other)
    {
        name = std::move(other.name);
        mesh_count = other.mesh_count;
        vertex_count = other.vertex_count;
        meshes = std::move(other.meshes);
        indices_count = other.indices_count;

        other.mesh_count = 0;
        other.vertex_count = 0;
        other.indices_count = 0;
    }
    return *this;
}

Fuego::Graphics::Model::Mesh::Mesh(cgltf_mesh* mesh, const Material* material, uint32_t mesh_index, std::vector<Fuego::Graphics::VertexData>& vertices,
                                   std::vector<uint32_t>& indices)
    : mesh_name(mesh->name)
    , vertex_count(0)
    , vertex_start(0)
    , vertex_end(0)
    , index_start(0)
    , index_end(0)
    , indices_count(0)
    , material(material)
{
    for (size_t i = 0; i < mesh->primitives_count; i++)
    {
        const cgltf_primitive primitive = mesh->primitives[i];
        indices_count += primitive.indices->count;
        for (size_t j = 0; j < primitive.attributes_count; j++)
        {
            if (primitive.attributes[j].type == cgltf_attribute_type_position)
            {
                vertex_count += primitive.attributes[j].data->count;
            }
        }
    }
    vertex_start = vertices.size();
    index_start = indices.size();

    vertices.reserve(vertices.size() + vertex_count);
    for (size_t i = 0; i < mesh->primitives_count; i++)
    {
        const cgltf_primitive primitive = mesh->primitives[i];
        const cgltf_accessor* primitive_indices_buffer = primitive.indices;

        const uint8_t* index_global_buffer = static_cast<const uint8_t*>(primitive_indices_buffer->buffer_view->buffer->data);
        size_t primitive_indecies_start_idx = primitive_indices_buffer->buffer_view->offset + primitive_indices_buffer->offset;
        const void* index_data = index_global_buffer + primitive_indecies_start_idx;

        const float* positions = nullptr;
        const float* normals = nullptr;
        const float* textcoords = nullptr;


        for (size_t j = 0; j < primitive.attributes_count; j++)
        {
            const cgltf_attribute& attribute = primitive.attributes[j];
            const cgltf_accessor* accessor = attribute.data;

            const uint8_t* attribute_global_buffer = static_cast<const uint8_t*>(accessor->buffer_view->buffer->data);
            size_t primitive_indecies_start_idx = accessor->buffer_view->offset + accessor->offset;
            const float* ptr = reinterpret_cast<const float*>(attribute_global_buffer + primitive_indecies_start_idx);

            if (attribute.type == cgltf_attribute_type_position)
                positions = ptr;
            else if (attribute.type == cgltf_attribute_type_normal)
                normals = ptr;
            else if (attribute.type == cgltf_attribute_type_texcoord)
                textcoords = ptr;
        }
        auto read_index = [&](size_t idx) -> uint32_t
        {
            if (primitive_indices_buffer->component_type == cgltf_component_type_r_16u)
                return reinterpret_cast<const uint16_t*>(index_data)[idx];
            else if (primitive_indices_buffer->component_type == cgltf_component_type_r_32u)
                return reinterpret_cast<const uint32_t*>(index_data)[idx];
            return 0;
        };

        std::unordered_map<uint32_t, uint32_t> map;
        for (size_t j = 0; j < primitive_indices_buffer->count; ++j)
        {
            uint32_t vi = read_index(j);
            if (map.contains(vi))
            {
                indices.push_back(map[vi]);
                continue;
            }
            VertexData v{};

            if (positions)
            {
                v.pos.x = positions[vi * 3 + 0];
                v.pos.y = positions[vi * 3 + 1];
                v.pos.z = positions[vi * 3 + 2];
            }
            if (normals)
            {
                v.normal.x = normals[vi * 3 + 0];
                v.normal.y = normals[vi * 3 + 1];
                v.normal.z = normals[vi * 3 + 2];
            }
            if (textcoords)
            {
                v.textcoord.x = textcoords[vi * 2 + 0];
                v.textcoord.y = textcoords[vi * 2 + 1];
            }

            vertices.push_back(v);
            uint32_t new_index = static_cast<uint32_t>(vertices.size() - 1);
            map[vi] = new_index;
            indices.push_back(new_index);
        }
    }
    vertex_end = vertices.size() - 1;
    index_end = indices.size() - 1;
}

void Fuego::Graphics::Model::PostLoad(cgltf_data* data)
{
    process_model(data);
}

void Fuego::Graphics::Model::process_model(cgltf_data* data, bool async)
{
    mesh_count = data->meshes_count;

    auto renderer = ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>();
    auto assets_manager = ServiceLocator::instance().GetService<Fuego::AssetsManager>();
    meshes.reserve(mesh_count);
    materials.reserve(data->materials_count);

    int texture_index = MAXINT;
    std::map<uint32_t, const Texture*> loaded_textures;
    for (size_t i = 0; i < materials.capacity(); i++)
    {
        uint32_t solid_texture_idx = 0;
        std::shared_ptr<Fuego::ResourceHandle<Fuego::Graphics::Image2D>> image{nullptr};
        std::shared_ptr<Fuego::Graphics::Texture> texture{nullptr};

        if ((data->materials + i)->has_pbr_metallic_roughness)
        {
            auto current_material = data->materials + i;
            bool has_texture = false;
            cgltf_texture* base_color_texture = current_material->pbr_metallic_roughness.base_color_texture.texture;
            char* texture_name = nullptr;
            if (base_color_texture)
                has_texture = true;

            if (has_texture)
            {
                if (base_color_texture->image->name)
                    texture_name = base_color_texture->image->name;
                else if (base_color_texture->image->buffer_view->name)
                    texture_name = base_color_texture->image->buffer_view->name;

                if (!base_color_texture->image->uri && base_color_texture->image->buffer_view)
                {
                    // Embeded texture
                    auto image_buffer = base_color_texture->image->buffer_view;
                    std::string extension = std::filesystem::path(texture_name).extension().string();
                    uint32_t channels = 0;
                    if (extension.find("jpg") != extension.npos || extension.find("jpeg") != extension.npos)
                        channels = 3;
                    else if (extension.find("png") != extension.npos)
                        channels = 4;

                    unsigned char* image_data = reinterpret_cast<unsigned char*>(image_buffer->buffer->data) + image_buffer->offset;
                    if (async)
                        image = assets_manager->LoadImage2DFromMemoryAsync(texture_name, image_data, image_buffer->size, channels);
                    else
                        image = assets_manager->LoadImage2DFromMemory(texture_name, image_data, image_buffer->size, channels);
                }
                else if (base_color_texture->image->uri)
                {
                    // Texture somewhere in folder
                    image = assets_manager->Load<Image2D>(base_color_texture->image->uri, async);
                }
                texture = renderer->CreateGraphicsResource<Texture>(image->Resource()->Name());
            }
            else
            {
                cgltf_float* color = current_material->pbr_metallic_roughness.base_color_factor;
                int channels = 0;
                for (size_t i = 0; i < 4; i++)
                {
                    if (*(color + i) > 0)
                        ++channels;
                }
                std::string solid_texture_name = std::string(name) + "_Solid_Texture_" + std::to_string(solid_texture_idx);

                if (channels == 4)
                    texture = renderer->CreateGraphicsResource<Texture>(solid_texture_name, Color(*color, *(color + 1), *(color + 2), *(color + 3)), 128, 128);
                else if (channels == 3)
                    texture = renderer->CreateGraphicsResource<Texture>(solid_texture_name, Color(*color, *(color + 1), *(color + 2)), 128, 128);
                else if (channels == 2)
                    texture = renderer->CreateGraphicsResource<Texture>(solid_texture_name, Color(*color, *(color + 1)), 128, 128);
                else
                    texture = renderer->CreateGraphicsResource<Texture>(solid_texture_name, Color(*color), 128, 128);

                ++solid_texture_idx;
            }

            // TODO think about passing raw pointer or shared ptr to material
            auto material = Material::CreateMaterial(texture.get());
            materials.emplace_back(std::unique_ptr<Material>(material));
            loaded_textures.emplace(texture_index, texture.get());
        }
    }

    for (size_t i = 0; i < mesh_count; i++)
    {
        auto mesh = (data->meshes + i);

        FU_CORE_ASSERT(mesh->primitives[0].type == cgltf_primitive_type_triangles, "Mesh is not triangulated");

        uint32_t material_idx = mesh->primitives[0].material - data->materials;
        const Mesh* emplaced_mesh =
            meshes.emplace_back(std::make_unique<Fuego::Graphics::Model::Mesh>(mesh, materials[material_idx].get(), i, vertices, indices)).get();
        vertex_count += emplaced_mesh->GetVertexCount();
        indices_count += emplaced_mesh->GetIndicesCount();
    }
}
