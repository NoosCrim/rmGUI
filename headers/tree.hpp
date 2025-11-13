#include <vector>
#include <cinttypes>
#include <algorithm>

#include "types.hpp"
#pragma once

namespace rmGUI
{
    struct alignas(16) GraphicNodeProperties // stores GPU side properties of a Node
    {
        friend class GUITree;
        float4 color = {0.0f, 0.0f, 0.0f, 1.0f};
    private:
        float2 pos = {0.0f, 0.0f}, size = {0.0f, 0.0f};
    public:
        float height = 0.0f;
    };
    struct NodeProperties // stores CPU side properties of a Node
    {
        float2 absPos = {0.0f, 0.0f}, absSize = {0.0f, 0.0f};
        float2 relPos = {0.0f, 0.0f}, relSize = {0.0f, 0.0f};
    };
    class GUITree
    {
    public:
        void PrepareRenderData();
    private:
        class Node;
        uint32_t CreateBufferData(Node* node);
        void DeleteBufferData(Node* node);
        uint32_t PrepareRenderDataIter(Node* node, uint32_t i);
    private:
        class Node
        {
            friend class GUITree;
        private:
            GUITree &tree;
            Node* parent;
            uint32_t pIndex; // pIndex - index in parent
            uint32_t size = 1;
            std::vector<Node*> children;

            uint32_t gIndex; // gIndex - index to node's GraphicNodeProperties in nodeBufferData
        public:
            NodeProperties data; // NodeProperties of a node
        private:
            void UpdateSize()
            {
                size = 1;
                for(Node* child : children)
                    size += child->size;
            }
            void DetachFromParent()
            {
                if(this != parent->children.back())
                {
                    std::swap(parent->children[pIndex], parent->children.back());
                    parent->children[pIndex]->pIndex = pIndex;
                }
                parent->children.pop_back();
                parent = nullptr;
            }
            void AttachChild(Node* newChild)
            {
                newChild->pIndex = children.size();
                newChild->parent = this;
                children.push_back(newChild);
                ++size;
                if(parent)
                    parent->UpdateSize();
            }
        public:
            Node(Node* parent_) :
                tree(parent_->tree),
                parent(parent_),
                gIndex(tree.CreateBufferData(this))
            {
                parent->AttachChild(this);
            }
            Node(GUITree& tree_) :
                tree(tree_),
                parent(nullptr),
                gIndex(tree.CreateBufferData(this))
            {

            }
            virtual ~Node()
            {
                if(parent)
                    DetachFromParent();
                tree.DeleteBufferData(this);
                while(!children.empty())
                {
                    delete children.back();
                }
            }
            GraphicNodeProperties& gData() // GraphicNodeProperties of a node
            {
                return tree.nodeBufferData[gIndex];
            }
            const GraphicNodeProperties& gData() const
            {
                return tree.nodeBufferData[gIndex];
            }
            Node* CreateChild(auto&&... dataCtrArgs)
            {
                return new Node(this, dataCtrArgs...);
            }
        };
        
        std::vector<GraphicNodeProperties> nodeBufferData; // properties of each GUI node
        std::vector<Node*> nodeDataOwners; // i-th entry symbolizes owner of i-th entry in nodeBufferData
        std::vector<GLuint> indexBufferData; // indices of each GUI node properties in draw order
        Node root{*this};
    public:
        Node* GetRoot()
        {
            return &root;
        }
        const std::vector<GraphicNodeProperties>& GetNodeBufferData() const
        {
            return nodeBufferData;
        }
        const std::vector<GLuint>& GetIndexBufferData() const
        {
            return indexBufferData;
        }
    };
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












