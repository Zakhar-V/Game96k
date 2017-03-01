void main()
{
	OutInstanceID = gl_InstanceID;
	OutPos = InPos;

#ifdef SKINNED
	OutWorldPos = InPos; // TODO:
#elif defined(PARTICLES) || defined(FSQUAD)
	OutWorldPos = InPos;
#else
	OutWorldPos = WorldMat[gl_InstanceID] * vec4((InPos.xyz + InNormal * SilhouetteOffset), 1);
#endif

	OutColor = InColor;
	OutTexCoord = InTexCoord;

#ifdef SPRITE
	OutSize = InSize;
	OutRot = InRot;
	OutTexCoord2 = (InTexCoord2 - InTexCoord);
#else
	OutNormal = normalize(InNormal * NormMat);
	// TODO: tangent, binormal
#endif

#ifdef FSQUAD
	gl_Position = OutWorldPos;
#else
	gl_Position = ViewProjMat * OutWorldPos;
#endif
	OutLogZ = gl_Position.w * Depth.z + 1;
}
