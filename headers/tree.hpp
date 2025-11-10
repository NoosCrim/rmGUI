#include <vector>
#include <cinttypes>
#include <algorithm>

#include "types.hpp"
#pragma once

namespace rmGUI
{
    struct alignas(16) NodeProperties
    {
        float4 color;
        float2 pos, size;
        float height;
    };
    template<typename T = NodeProperties>
    class GUITree
    {
    public:
        void RefreshIndexBufferData();
    private:
        class Node;
        uint32_t CreateBufferData(Node* node, auto&&... args);
        void DeleteBufferData(Node* node);
        uint32_t RefreshIndexBufferDataIter(Node* node, uint32_t i);
    private:
        class Node
        {
            friend class GUITree<T>;
            GUITree &tree;
            Node* parent;
            uint32_t pIndex; // pIndex - index in parent
            uint32_t size = 1;
            std::vector<Node*> children;

            uint32_t bIndex; // bIndex - index in nodeBufferData
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
            Node(Node* parent_, auto&&... dataCtrArgs) :
                tree(parent_->tree),
                parent(parent_),
                bIndex(tree.CreateBufferData(this, dataCtrArgs...))
            {
                parent->AttachChild(this);
            }
            Node(GUITree<T>& tree_, auto&&... dataCtrArgs) :
                tree(tree_),
                parent(nullptr),
                bIndex(tree.CreateBufferData(this, dataCtrArgs...))
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
            T& data()
            {
                return tree.nodeBufferData[bIndex];
            }
            void SetPosPixels(float2 pos)
            {
                data().pos = pos;
            }
            void SetPosRelative(float2 pos)
            {
                data().pos = parent->data().size * pos;
            }
            void SetSizePixels(float2 size)
            {
                data().size = size;
            }
            void SetSizeRelative(float2 size)
            {
                data().size = parent->data().size * size;
            }
            Node* CreateChild(auto&&... dataCtrArgs)
            {
                return new Node(this, dataCtrArgs...);
            }
        };
        
        std::vector<T> nodeBufferData; // properties of each GUI node
        std::vector<Node*> nodeDataOwners; // i-th entry symbolizes owner of i-th entry in nodeBufferData
        std::vector<GLuint> indexBufferData; // indices of each GUI node properties in draw order
        Node root{*this};
    public:
        Node* GetRoot()
        {
            return &root;
        }
        const std::vector<T>& GetNodeBufferData() const
        {
            return nodeBufferData;
        }
        const std::vector<GLuint>& GetIndexBufferData() const
        {
            return indexBufferData;
        }
    };
    template<typename T>
    uint32_t GUITree<T>::CreateBufferData(Node* node, auto&&... args) // returns index of created buffer data
    {
        nodeBufferData.emplace_back(args...);
        nodeDataOwners.push_back(node);
        return nodeBufferData.size() - 1;
    }
    template<typename T>
    void GUITree<T>::DeleteBufferData(Node* node)
    {
        if(node->bIndex != nodeBufferData.size()-1)
        {
            nodeDataOwners.back()->bIndex = node->bIndex;
            std::swap(nodeBufferData[node->bIndex], nodeBufferData.back());
            std::swap(nodeDataOwners[node->bIndex], nodeDataOwners.back());
        }
        nodeBufferData.pop_back();
        nodeDataOwners.pop_back();
    }
    template<typename T>
    void GUITree<T>::RefreshIndexBufferData()
    {
        indexBufferData.resize(root.size);
        RefreshIndexBufferDataIter(&root, 0);
    }
    template<typename T>
    uint32_t GUITree<T>::RefreshIndexBufferDataIter(Node* node, uint32_t i)
    {
        indexBufferData[i++] = node->bIndex;
        for(Node* child : node->children)
            i = RefreshIndexBufferDataIter(child, i);
        return i;

    }
}












