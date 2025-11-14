#include <vector>
#include <cinttypes>
#include <algorithm>
#include <GL/glew.h>

#include "types.hpp"
#pragma once

namespace rmGUI
{
    using NodeCorner = uint32_t;
    using NodeCornerMask = uint32_t;
    class Node;
    enum class CornerShape : uint32_t
    {
        SQUARE,
        ROUND,
        BEVEL,
        NOTCH,
        SCOOP
    };
    struct alignas(16) GraphicNodeProperties // stores GPU side properties of a Node
    {
        friend class GUITree;
        float4 color = {0.0f, 0.0f, 0.0f, 1.0f};
    private:
        float2 pos = {0.0f, 0.0f}, size = {0.0f, 0.0f};
    private:
        uint32_t cornerShapePacked;
    public:
        float height = 0.0f;
    private:
        float _padding[2];
        float2 cornerSize[4];
        
    public:
        inline void SetCornerShape(NodeCornerMask cornerMask, CornerShape shape)
        {
            cornerShapePacked = (cornerShapePacked & ~(uint32_t)cornerMask) | 
            ((uint32_t)cornerMask & (
                (uint32_t)shape | 
                (uint32_t)shape << 8 |
                (uint32_t)shape << 16 |
                (uint32_t)shape << 24
            ));
        }
        CornerShape GetCornerShape(NodeCorner corner);
    };
    struct NodeProperties // stores CPU side properties of a Node
    {
        float2 absPos = {0.0f, 0.0f}, absSize = {0.0f, 0.0f};
        float2 relPos = {0.0f, 0.0f}, relSize = {0.0f, 0.0f};
        float2 absCornerSize[4] = {{0,0},{0,0},{0,0},{0,0}};
        float2 relCornerSize[4] = {{0,0},{0,0},{0,0},{0,0}};
    };

    class GUITree;
    class Node
    {
    public:
        enum NodeCorner : uint32_t // bitmasks for gui node corners in cornerShapePacked
        {
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_RIGHT,
            BOTTOM_LEFT
        };
        enum NodeCornerMask : uint32_t
        {
            TOP_LEFT_MASK = 0xFFu << (uint32_t)TOP_LEFT * 8,
            TOP_RIGHT_MASK = 0xFFu << (uint32_t)TOP_RIGHT * 8,
            BOTTOM_RIGHT_MASK = 0xFFu << (uint32_t)BOTTOM_RIGHT * 8,
            BOTTOM_LEFT_MASK = 0xFFu << (uint32_t)BOTTOM_LEFT * 8
        };
        static constexpr uint32_t nodeCornerMask[4]
        {
            TOP_LEFT_MASK,
            TOP_RIGHT_MASK,
            BOTTOM_RIGHT_MASK,
            BOTTOM_LEFT_MASK
        };
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












