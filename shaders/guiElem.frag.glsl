#version 460

#include "guiElem.defs.glsl"

in vec2 fragPos;
in vec4 fragCol;
flat in uint GUI_ID;

vec2 inNodePos = fragPos - instanceData[GUI_ID].pos;
vec2 inNodePos01 = inNodePos / instanceData[GUI_ID].size;
float topD = inNodePos.y;
float leftD = inNodePos.x;
float bottomD = instanceData[GUI_ID].size.y - inNodePos.y;
float rightD = instanceData[GUI_ID].size.x - inNodePos.x;
uint inSegmentTablePacked = 0;

uint GetCornerShape(uint corner)
{
    uint offset = corner*8;
    return (instanceData[GUI_ID].cornerShapePacked >> offset) & 0xFFu;
}

vec2 GetCornerPosOut(uint segment)
{
	switch(segment)
	{
		case NodeSegment_TOP_LEFT:
			return instanceData[GUI_ID].pos;
		case NodeSegment_TOP_RIGHT:
			return instanceData[GUI_ID].pos + vec2(instanceData[GUI_ID].size.x, 0);
		case NodeSegment_BOTTOM_RIGHT:
			return instanceData[GUI_ID].pos + instanceData[GUI_ID].size;
		case NodeSegment_BOTTOM_LEFT:
			return instanceData[GUI_ID].pos + vec2(0, instanceData[GUI_ID].size.y);
		default:
			return vec2(0,0);
	}
}
vec2 GetCornerPosIn(uint segment)
{
	switch(segment)
	{
		case NodeSegment_TOP_LEFT:
			return instanceData[GUI_ID].pos + instanceData[GUI_ID].cornerSize[NodeSegment_TOP_LEFT];
		case NodeSegment_TOP_RIGHT:
			return instanceData[GUI_ID].pos + 
                vec2(instanceData[GUI_ID].size.x - instanceData[GUI_ID].cornerSize[NodeSegment_TOP_RIGHT].x, instanceData[GUI_ID].cornerSize[NodeSegment_TOP_RIGHT].y);
		case NodeSegment_BOTTOM_RIGHT:
			return instanceData[GUI_ID].pos + instanceData[GUI_ID].size - instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_RIGHT];
		case NodeSegment_BOTTOM_LEFT:
			return instanceData[GUI_ID].pos + 
                vec2(instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_LEFT].x, instanceData[GUI_ID].size.y - instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_LEFT].y);
		default:
			return vec2(0,0);
	}
}
void InSegmentEval()
{
    // corners
    inSegmentTablePacked = inSegmentTablePacked | 
        ((topD < instanceData[GUI_ID].cornerSize[NodeSegment_TOP_LEFT].y && 
        leftD < instanceData[GUI_ID].cornerSize[NodeSegment_TOP_LEFT].x) ? 
            1u << NodeSegment_TOP_LEFT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((topD < instanceData[GUI_ID].cornerSize[NodeSegment_TOP_RIGHT].y && 
        rightD < instanceData[GUI_ID].cornerSize[NodeSegment_TOP_RIGHT].x) ? 
            1u << NodeSegment_TOP_RIGHT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((bottomD < instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_RIGHT].y && 
        rightD < instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_RIGHT].x) ? 
            1u << NodeSegment_BOTTOM_RIGHT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((bottomD < instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_LEFT].y 
        && leftD < instanceData[GUI_ID].cornerSize[NodeSegment_BOTTOM_LEFT].x) 
            ? 1u << NodeSegment_BOTTOM_LEFT : 0u);
}
bool InSegmentCheck(const uint segment)
{
    return ((inSegmentTablePacked >> segment) & 1u) == 1;
}
bool CornerNoDiscardCheck(uint segment)
{
    switch(GetCornerShape(segment))
    {
        case CornerShape_SQUARE:
            return true;
        case CornerShape_ROUND:
            return !InSegmentCheck(segment) || 
                    length((fragPos - GetCornerPosIn(segment))/instanceData[GUI_ID].cornerSize[segment]) <= 1.f;
        case CornerShape_BEVEL:
            vec2 t = (fragPos - GetCornerPosIn(segment))/instanceData[GUI_ID].cornerSize[segment];
            return !InSegmentCheck(segment) || 
                    abs(t.x) + abs(t.y) <= 1.f;
        case CornerShape_NOTCH:
            return !InSegmentCheck(segment);
        case CornerShape_SCOOP:
            return !InSegmentCheck(segment) ||
                    length((fragPos - GetCornerPosOut(segment))/instanceData[GUI_ID].cornerSize[segment]) >= 1.f;
    }
    return true;
}
bool CornerNoDiscardCheck()
{
    return  CornerNoDiscardCheck(NodeSegment_TOP_LEFT) &&
            CornerNoDiscardCheck(NodeSegment_TOP_RIGHT) &&
            CornerNoDiscardCheck(NodeSegment_BOTTOM_RIGHT) &&
            CornerNoDiscardCheck(NodeSegment_BOTTOM_LEFT);
}

out vec4 color;

void main()
{
    InSegmentEval();
    color = fragCol;
    if(!CornerNoDiscardCheck())
        discard;
    vec4 shaCol[5] = 
    {
        vec4(1,0,0,1),
        vec4(0,1,0,1),
        vec4(0,0,1,1),
        vec4(0,1,1,1),
        vec4(1,1,0,1)
    };
    /*for(int i = 0; i < 4; i++)
        if(InSegmentCheck(i))
            color = shaCol[GetCornerShape(i)];*/
}