layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void MakeVertex(in vec3 offset, in vec2 coord)
{
	OutInstanceID = InInstanceID[0];
	OutWorldPos = InWorldMat[0] * vec4(InPos[0].xyz + offset, 1);
	OutTexCoord = InTexCoord[0] + InTexSize[0] * coord;
	OutColor = InColor[0];
	gl_Position = ViewProjMat * OutWorldPos;
	OutLogZ = gl_Position.w * Depth.z + 1;
	EmitVertex();
}

void main()
{
	mat3 mat = mat3(UNIT_X * InSize[0].x, UNIT_Y * InSize[0].y, UNIT_Z);
	
	//TODO: do rotate around z (InSize[0].z)

#ifdef BILLBOARD_Y
	mat = mat3(NormMat[0], UNIT_Y, UNIT_Z) * mat;
#elif defined(BILLBOARD)
	mat = NormMat * mat;
#endif	

	MakeVertex(mat * vec3(.5, .5, 0), vec2(1, 1)); // rb
	MakeVertex(mat * vec3(.5, -.5, 0), vec2(1, 0)); // rt
	MakeVertex(mat * vec3(-.5, .5, 0), vec2(0, 1)); // lb
	MakeVertex(mat * vec3(-.5, -.5, 0), vec2(0, 0)); // lt

	EndPrimitive();
}
