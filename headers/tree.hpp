#include <vector>
#include <cinttypes>
#include <algorithm>
#include <GL/glew.h>

#include "types.hpp"
#pragma once

namespace rmGUI
{
    enum class CornerShape : uint32_t
    {
        SQUARE,
        ROUND,
        BEVEL,
        NOTCH,
        SCOOP
    };
    enum NodeCorner : uint32_t // bitmasks for gui node corners in cornerShapePacked
    {
        TOP_LEFT = 0xFFu,
        TOP_RIGHT = 0xFFu << 8,
        BOTTOM_RIGHT = 0xFFu << 16,
        BOTTOM_LEFT = 0xFFu << 24
    };

    struct alignas(16) GraphicNodeProperties // stores GPU side properties of a Node
    {
        friend class GUITree;
        float4 color = {0.0f, 0.0f, 0.0f, 1.0f};
    private:
        float2 pos = {0.0f, 0.0f}, size = {0.0f, 0.0f};
    private:
        float _padding[2];
        uint32_t cornerShapePacked;
    public:
        float height = 0.0f;
        float2  tlCornerRelSize = {0.0f,0.0f}, tlCornerAbsSize = {0.0f, 0.0f},
                trCornerRelSize = {0.0f,0.0f}, trCornerAbsSize = {0.0f, 0.0f},
                brCornerRelSize = {0.0f,0.0f}, brCornerAbsSize = {0.0f, 0.0f},
                blCornerRelSize = {0.0f,0.0f}, blCornerAbsSize = {0.0f, 0.0f};
        
    public:
        inline void SetCornerShape(NodeCorner cornerMask, CornerShape shape)
        {
            cornerShapePacked = (cornerShapePacked & ~(uint32_t)cornerMask) | 
            ((uint32_t)cornerMask & (
                (uint32_t)shape | 
                (uint32_t)shape << 8 |
                (uint32_t)shape << 16 |
                (uint32_t)shape << 24
            ));
        }
        inline CornerShape GetCornerShape(NodeCorner cornerMask)
        {
            uint8_t offset = std::countr_zero((uint32_t)cornerMask);
            return (CornerShape)((cornerShapePacked & (uint32_t)cornerMask) >> offset);
        }
    };
    struct NodeProperties // stores CPU side properties of a Node
    {
        float2 absPos = {0.0f, 0.0f}, absSize = {0.0f, 0.0f};
        float2 relPos = {0.0f, 0.0f}, relSize = {0.0f, 0.0f};
    };

    class GUITree;
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
        void UpdateSize();
        void DetachFromParent();
        void AttachChild(Node* newChild);
    public:
        Node(Node* parent_);
        Node(GUITree& tree_);
        virtual ~Node();
        GraphicNodeProperties& gData(); // GraphicNodeProperties of a node
        const GraphicNodeProperties& gData() const;
        Node* CreateChild();
    };
    class GUITree
    {
        friend class Node;
    public:
        void PrepareRenderData();
    private:
        uint32_t CreateBufferData(Node* node);
        void DeleteBufferData(Node* node);
        uint32_t PrepareRenderDataIter(Node* node, uint32_t i);
    private:
        std::vector<GraphicNodeProperties> nodeBufferData; // properties of each GUI node
        std::vector<Node*> nodeDataOwners; // i-th entry symbolizes owner of i-th entry in nodeBufferData
        std::vector<GLuint> indexBufferData; // indices of each GUI node properties in draw order
        Node root{*this};
    public:
        inline Node* GetRoot()
        {
            return &root;
        }
        inline const std::vector<GraphicNodeProperties>& GetNodeBufferData() const
        {
            return nodeBufferData;
        }
        inline const std::vector<GLuint>& GetIndexBufferData() const
        {
            return indexBufferData;
        }
    };
}












