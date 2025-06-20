#pragma once

#include <glm/glm.hpp>

template <typename T, typename U>
constexpr bool GetPureTypeAndCompare()
{
    return std::is_same_v<T, std::remove_cv_t<std::remove_reference_t<U>>>;
}

namespace Fuego::Graphics
{
class Shader;
class Material;

class ShaderObject
{
public:
    virtual ~ShaderObject() = default;

    template <typename T>
    bool Set(std::string_view name, const T& var)
    {
        if constexpr (GetPureTypeAndCompare<glm::vec3, T>())
        {
            return set_vec3f_impl(name, var);
        }
        else if constexpr (GetPureTypeAndCompare<glm::mat4, T>())
        {
            return set_mat4f_impl(name, var);
        }
        else if constexpr (GetPureTypeAndCompare<Texture, T>())
        {
            return set_text2d_impl(name, var);
        }
    }

    static ShaderObject* CreateShaderObject(Shader* vs, Shader* px);
    virtual void Use() const = 0;
    virtual void BindMaterial(const Material* material) = 0;

    virtual void Release() = 0;

protected:
    ShaderObject() = default;
    virtual bool set_vec3f_impl(std::string_view name, const glm::vec3& vec) = 0;
    virtual bool set_mat4f_impl(std::string_view name, const glm::mat4& matrix) = 0;
    virtual bool set_text2d_impl(std::string_view name, const Texture& texture) = 0;
};
}  // namespace Fuego::Graphics
