#version 460

#include "guiElem.defs.glsl"

in vec2 fragPos;
in vec4 fragCol;
flat in uint GUI_ID;
flat in vec2 cornerSize[4];

float topD = fragPos.y - instanceData[GUI_ID].pos.y;
float leftD = fragPos.x - instanceData[GUI_ID].pos.x;
float bottomD = instanceData[GUI_ID].pos.y + instanceData[GUI_ID].size.y - fragPos.y;
float rightD = instanceData[GUI_ID].pos.x + instanceData[GUI_ID].size.x - fragPos.x;
uint inSegmentTablePacked = 0;
vec2 GetCornerPosOut(uint segment, uint I)
{
	switch(segment)
	{
		case NodeSegment_TOP_LEFT:
			return instanceData[I].pos;
		case NodeSegment_TOP_RIGHT:
			return instanceData[I].pos + vec2(instanceData[I].size.x, 0);
		case NodeSegment_BOTTOM_RIGHT:
			return instanceData[I].pos + instanceData[I].size;
		case NodeSegment_BOTTOM_LEFT:
			return instanceData[I].pos + vec2(0, instanceData[I].size.y);
		default:
			return vec2(0,0);
	}
}
vec2 GetCornerPosIn(uint segment, uint I)
{
	switch(segment)
	{
		case NodeSegment_TOP_LEFT:
			return instanceData[I].pos + cornerSize[NodeSegment_TOP_LEFT];
		case NodeSegment_TOP_RIGHT:
			return instanceData[I].pos + 
                vec2(instanceData[I].size.x - cornerSize[NodeSegment_TOP_RIGHT].x, cornerSize[NodeSegment_TOP_RIGHT].y);
		case NodeSegment_BOTTOM_RIGHT:
			return instanceData[I].pos + instanceData[I].size - cornerSize[NodeSegment_BOTTOM_RIGHT];
		case NodeSegment_BOTTOM_LEFT:
			return instanceData[I].pos + 
                vec2(cornerSize[NodeSegment_BOTTOM_LEFT].x, instanceData[I].size.y - cornerSize[NodeSegment_BOTTOM_LEFT].y);
		default:
			return vec2(0,0);
	}
}
void InSegmentEval()
{
    // corners
    inSegmentTablePacked = inSegmentTablePacked | 
        ((topD < cornerSize[NodeSegment_TOP_LEFT].y && leftD < cornerSize[NodeSegment_TOP_LEFT].x) ? 1u << NodeSegment_TOP_LEFT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((topD < cornerSize[NodeSegment_TOP_RIGHT].y && rightD < cornerSize[NodeSegment_TOP_RIGHT].x) ? 1u << NodeSegment_TOP_RIGHT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((bottomD < cornerSize[NodeSegment_BOTTOM_RIGHT].y && rightD < cornerSize[NodeSegment_BOTTOM_RIGHT].x) ? 1u << NodeSegment_BOTTOM_RIGHT : 0u);

    inSegmentTablePacked = inSegmentTablePacked | 
        ((bottomD < cornerSize[NodeSegment_BOTTOM_LEFT].y && leftD < cornerSize[NodeSegment_BOTTOM_LEFT].x) ? 1u << NodeSegment_BOTTOM_LEFT : 0u);
}
bool InSegmentCheck(const uint segment)
{
    return ((inSegmentTablePacked >> segment) & 1u) == 1;
}
bool CornerNoDiscardCheck(uint segment)
{
    switch(GetCornerShape(GUI_ID, segment))
    {
        case CornerShape_SQUARE:
            return true;
        case CornerShape_ROUND:
            return !InSegmentCheck(segment) || 
                    length((fragPos - GetCornerPosIn(segment, GUI_ID))/cornerSize[segment]) <= 1.f;
        case CornerShape_BEVEL:
            vec2 t = (fragPos - GetCornerPosIn(segment, GUI_ID))/cornerSize[segment];
            return !InSegmentCheck(segment) || 
                    abs(t.x) + abs(t.y) <= 1.f;
        case CornerShape_NOTCH:
            return !InSegmentCheck(segment);
        case CornerShape_SCOOP:
            return !InSegmentCheck(segment) ||
                    length((fragPos - GetCornerPosOut(segment, GUI_ID))/cornerSize[segment]) >= 1.f;
    }
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
}