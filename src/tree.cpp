#include "tree.hpp"
namespace rmGUI
{
    void Node::UpdateSize()
    {
        size = 1;
        for(Node* child : children)
            size += child->size;
    }
    void Node::DetachFromParent()
    {
        if(this != parent->children.back())
        {
            std::swap(parent->children[pIndex], parent->children.back());
            parent->children[pIndex]->pIndex = pIndex;
        }
        parent->children.pop_back();
        parent = nullptr;
    }
    void Node::AttachChild(Node* newChild)
    {
        newChild->pIndex = children.size();
        newChild->parent = this;
        children.push_back(newChild);
        ++size;
        if(parent)
            parent->UpdateSize();
    }
    Node::Node(Node* parent_) :
        tree(parent_->tree),
        parent(parent_),
        gIndex(tree.CreateBufferData(this))
    {
        parent->AttachChild(this);
    }
    Node::Node(GUITree& tree_) :
        tree(tree_),
        parent(nullptr),
        gIndex(tree.CreateBufferData(this))
    {

    }
    Node::~Node()
    {
        if(parent)
            DetachFromParent();
        tree.DeleteBufferData(this);
        while(!children.empty())
        {
            delete children.back();
        }
    }
    GraphicNodeProperties& Node::gData()
    {
        return tree.nodeBufferData[gIndex];
    }
    const GraphicNodeProperties& Node::gData() const
    {
        return tree.nodeBufferData[gIndex];
    }
    Node* Node::CreateChild()
    {
        return new Node(this);
    }

    uint32_t GUITree::CreateBufferData(Node* node) // returns index of created buffer data
    {
        nodeBufferData.emplace_back();
        nodeDataOwners.push_back(node);
        return nodeBufferData.size() - 1;
    }
    void GUITree::DeleteBufferData(Node* node)
    {
        if(node->gIndex != nodeBufferData.size()-1)
        {
            nodeDataOwners.back()->gIndex = node->gIndex;
            std::swap(nodeBufferData[node->gIndex], nodeBufferData.back());
            std::swap(nodeDataOwners[node->gIndex], nodeDataOwners.back());
        }
        nodeBufferData.pop_back();
        nodeDataOwners.pop_back();
    }
    void GUITree::PrepareRenderData()
    {
        root.gData().pos = root.data.absPos;
        root.gData().size = root.data.absSize;
        indexBufferData.resize(root.size);
        uint32_t i = 0;
        indexBufferData[i++] = root.gIndex;
        for(Node* child : root.children)
            i = PrepareRenderDataIter(child, i);
    }
    uint32_t GUITree::PrepareRenderDataIter(Node* node, uint32_t i)
    {
        node->gData().pos = node->data.absPos + node->data.relPos * node->parent->gData().size + node->parent->gData().pos;
        node->gData().size = node->data.absSize + node->data.relSize * node->parent->gData().size;
        indexBufferData[i++] = node->gIndex;
        for(Node* child : node->children)
            i = PrepareRenderDataIter(child, i);
        return i;

    }
}