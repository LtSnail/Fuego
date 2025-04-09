#pragma once
#include <cstdint>

namespace Fuego::Renderer
{
struct VertexLayout
{
public:
    enum DataType
    {
        FLOAT = 0,
        FLOAT_VEC2 = 1,
        FLOAT_VEC3 = 2,
        FLOAT_VEC4 = 3,
        INSIGNED_BYTE = 4,
        SHORT = 5
    };
    struct VertexAttribute
    {
        VertexAttribute(uint16_t ind, uint8_t comp_amount, DataType comp_type, bool enabled);
        uint16_t index;
        uint8_t components_amount;
        DataType components_type;
        uint16_t offset;
        bool is_enabled;
    };
    struct LayoutIterator
    {
    public:
        LayoutIterator(VertexLayout* master, VertexAttribute* attrib);
        inline uint16_t GetIndex()
        {
            return _attrib->index;
        }
        inline uint8_t GetComponentsAmount()
        {
            return _attrib->components_amount;
        }
        inline uint32_t GetAPIDatatype()
        {
            return _master->GetAPIDataType(_attrib->components_type);
        }
        inline uint16_t GetOffset()
        {
            return _attrib->offset;
        }
        inline bool GetIsEnabled()
        {
            return _attrib->is_enabled;
        }
        bool IsDone();

    private:
        friend struct VertexLayout;
        VertexAttribute* _attrib;
        VertexLayout* _master;
        bool is_done;
        uint16_t index;
    };


    VertexLayout();
    ~VertexLayout() = default;
    void AddAttribute(VertexAttribute attrib);
    void EnableAttribute(uint16_t attrib_index);
    void DisableAttribute(uint16_t attrib_index);
    uint32_t GetAPIDataType(DataType type);
    uint32_t GetSizeOfDataType(DataType type);
    inline uint16_t GetLayoutSize()
    {
        return layout_size;
    }
    LayoutIterator* GetIteratorBegin();
    LayoutIterator* GetNextIterator();

    void ReleaseIterator();

private:
    std::vector<VertexAttribute> attribs;
    uint16_t layout_size;
    LayoutIterator* _it;
};
}  // namespace Fuego::Renderer
