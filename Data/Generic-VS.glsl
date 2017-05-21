#ifdef TERRAIN
USAMPLER(3, sampler2D, TerrainHeightmap);
USAMPLER(4, sampler2D, TerrainNormalmap);
#endif

void main()
{
	OutInstanceID = BaseInstanceID + gl_InstanceID;
	OutPos = InPos;
	OutWorldMat = transpose(mat4(InInstanceMat1, InInstanceMat2, InInstanceMat3, VEC4_IDENTITY));
		
	OutTexCoord = InTexCoord;
	OutColor = InColor;

#ifdef SPRITE
	OutWorldPos = InPos;			
	OutSize = InSize;
	OutTexCoord2 = (InTexCoord2 - InTexCoord);
#else

#ifdef TERRAIN
	OutTexCoord += InInstanceParams.xy;
	OutPos.y *= texture(TerrainHeightmap, OutTexCoord).r;
	OutNormal = normalize((texture(TerrainNormalmap, OutTexCoord).xyz * 2 - 1) * NormMat);
	//OutNormal = normalize(InNormal * NormMat);
#else
	OutNormal = normalize(InNormal * NormMat);
#endif

	// TODO: tangent, binormal
	
#ifdef SKINNED
	OutWorldPos = OutPos; // TODO:
#else
	OutWorldPos = OutWorldMat * vec4((OutPos.xyz + InNormal * SilhouetteOffset), 1);
#endif // SKINNED
	
#endif // SPRITE

	gl_Position = ViewProjMat * OutWorldPos;
	OutLogZ = gl_Position.w * Depth.z + 1;
}
