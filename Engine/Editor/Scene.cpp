#include "Scene.h"

namespace Fuego::Editor
{
uint32_t TreeNode::s_id = 0;
uint32_t TreeNode::GetID()
{
    // TODO: to think about race conditions. std::atomic / mutex
    return ++s_id;
}

TreeNode::TreeNode()
    : id(GetID())
{
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


}  // namespace Fuego::Editor