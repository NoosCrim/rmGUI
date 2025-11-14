// values representing certain corner types
const uint CornerShape_SQUARE = 0;
const uint CornerShape_ROUND = 1;
const uint CornerShape_BEVEL = 2;
const uint CornerShape_NOTCH = 3;
const uint CornerShape_SCOOP = 4;

// names for gui node corners in cornerShapePacked
const uint NodeSegment_TOP_LEFT = 0;
const uint NodeSegment_TOP_RIGHT = 1;
const uint NodeSegment_BOTTOM_RIGHT = 2;
const uint NodeSegment_BOTTOM_LEFT = 3;

const uint NodeSegment_TOP = 4;
const uint NodeSegment_RIGHT = 5;
const uint NodeSegment_BOTTOM = 6;
const uint NodeSegment_LEFT = 7;
const uint NodeSegment_MIDDLE = 8;
const uint NodeSegment_NONE = 9;

struct InstanceDataStruct
{
    vec4 color;
    vec2 pos;
	vec2 size;
	vec2 padding;
	uint cornerShapePacked;
    float height;
    vec2 tlCornerRelSize; 
	vec2 tlCornerAbsSize;
	vec2 trCornerRelSize; 
	vec2 trCornerAbsSize;
	vec2 brCornerRelSize; 
	vec2 brCornerAbsSize;
	vec2 blCornerRelSize; 
	vec2 blCornerAbsSize;
};


layout(std430, binding=0) buffer instanceDataBuffer
{
    InstanceDataStruct instanceData[];
};

layout(location=0) uniform ivec2 windowSize;

uint GetCornerShape(uint I, uint corner)
{
    uint offset = corner*8;
    return (instanceData[I].cornerShapePacked >> offset) & 0xFFu;
}