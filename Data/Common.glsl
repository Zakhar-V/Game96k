#define MAX_INSTANCES 512
#define MAX_BONES 64
#define C_POLYGON_OFFSET 5e-4
#define C_SILHOUETTE_OFFSET 5e-4

#ifdef GLSL

#define UBUFFER(Id, Name) layout(binding = Id, std140) uniform U##Name
#define USAMPLER(Id, T, Name) layout(binding = Id) uniform T Name

#ifdef COMPILE_GS
#define GS_IN_SUFFIX []
#else
#define GS_IN_SUFFIX
#endif

#define _INOUT(S, IO, T, P, N) layout(location = S) IO T P##N
#define IN(S, T, N) _INOUT(S, in, T, In, N) GS_IN_SUFFIX
#define IN_FLAT(S, T, N) _INOUT(S, flat in, T, In, N) GS_IN_SUFFIX 
#define OUT_FLAT(S, T, N) _INOUT(S, flat out, T, Out, N) 
#define OUT(S, T, N) _INOUT(S, out, T, Out, N)
#define INOUT(S, T, N) IN(S, T, N); OUT(S, T, N)
#define INOUT_FLAT(S, T, N) IN_FLAT(S, T, N); OUT_FLAT(S, T, N)

#define _PerVertex gl_PerVertex { vec4 gl_Position; }

#ifdef COMPILE_VS
INOUT(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec4, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
IN(5, vec4, Weights);
IN(6, vec4, Indices);
INOUT(7, vec2, TexCoord2);
INOUT(8, vec3, Size);
IN(9, vec4, InstanceMat1);
IN(10, vec4, InstanceMat2);
IN(11, vec4, InstanceMat3);
IN(12, vec4, InstanceParams);
IN(13, int, FirstBone);

OUT(5, vec4, InstanceParams);
OUT(6, vec4, WorldPos);
OUT(9, vec3, Binormal);
OUT_FLAT(10, int, InstanceID);
OUT(11, float, LogZ);
OUT(12, mat4, WorldMat); // 13-16

out _PerVertex;
uniform int BaseInstanceID;
#endif  

#ifdef COMPILE_GS
IN(0, vec4, Pos);
INOUT(1, vec2, TexCoord);
INOUT(2, vec4, Color);
INOUT(3, vec3, Normal);
INOUT(4, vec4, Tangent);
INOUT(5, vec4, InstanceParams);
INOUT(6, vec4, WorldPos);
IN(7, vec2, TexSize);
IN(8, vec3, Size);
INOUT(9, vec3, Binormal);
INOUT_FLAT(10, int, InstanceID);
OUT(11, float, LogZ);
IN(12, mat4, WorldMat);

in _PerVertex gl_in[];
out _PerVertex;
#endif

#ifdef COMPILE_PS
IN(1, vec2, TexCoord);
IN(2, vec4, Color);
IN(3, vec3, Normal);
IN(4, vec3, Tangent);
IN(5, vec4, InstanceParams);
IN(6, vec4, WorldPos);
IN(9, vec3, Binormal);
IN_FLAT(10, int, InstanceID);
IN(11, float, LogZ);

OUT(0, vec4, Color);
OUT(1, vec4, Normal);
OUT(2, vec4, Material);
#endif

#define UNIT_X vec3(1, 0, 0)
#define UNIT_Y vec3(0, 1, 0)
#define UNIT_Z vec3(0, 0, 1)
#define VEC4_IDENTITY vec4(0, 0, 0, 1)

#endif

UBUFFER(1, Camera)
{
	layout(row_major) mat4 ViewMat;
	layout(row_major) mat4 ProjMat;
	layout(row_major) mat4 ViewProjMat;
	layout(row_major) mat4 InvViewProjMat;
	layout(row_major) mat3 NormMat; // inverse(mat3(ViewMat))
	vec4 CameraPos;
	vec4 Depth;	// near, far, C=constant, FC = 1.0/log(far*C + 1)
	ivec2 ScreenSize;
	vec2 InvScreenSize;
};

UBUFFER(2, RasterizerParams)
{
	float DepthBias; // default is 0 or 1e-5
	float SilhouetteOffset;
};

/*UBUFFER(2, InstanceMat)
{
	layout(row_major) mat4 WorldMat[MAX_INSTANCES];
};

UBUFFER(3, InstanceParams)
{
	mat2x4 InstanceParams[MAX_INSTANCES];  // shininess, emission
};

UBUFFER(4, BoneMat)
{
	layout(row_major) mat4 BoneMat[MAX_BONES];
};*/


