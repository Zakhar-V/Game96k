#if defined(COMBINE2) || defined(COMBINE4) || defined(COMBINE6)

USAMPLER(0, sampler2DArray, Albedo);
USAMPLER(1, sampler2DArray, NormalMap);
USAMPLER(2, sampler2DArray, ParamsMap); // metallic, smoothness, emission, lighting model
USAMPLER(3, sampler2DArray, CombineMap);

#else // COMBINE

USAMPLER(0, sampler2D, Albedo);
USAMPLER(1, sampler2D, NormalMap);
USAMPLER(2, sampler2D, DetailAlbedo);
USAMPLER(3, sampler2D, DetailNormalMap);

#endif // COMBINE

USAMPLER(5, sampler2D, Ramp);
USAMPLER(6, sampler2D, EnvlMap);

vec4 sample(vec4 k[3], float t)
{
	float t2 = (t - floor(t));
	int i1 = int(t2 * 3) % 3;
	int i2 = (i1 + 1) % 3;
	t2 = t2 * 3 - i1;
	return mix(k[i1], k[i2], t2);
}


void main()
{
	gl_FragDepth = log(InLogZ) * (Depth.w - DepthBias);

	//vec4 mtlColor = InstanceParams[InInstanceID][0];
	//vec4 mtlParams = InstanceParams[InInstanceID][1];

	OutColor = InColor;
	//OutColor = mtlColor * InColor; // TODO: material color

#if defined(COMBINE2) || defined(COMBINE4) || defined(COMBINE6)
#ifdef ALBEDO
	OutColor = vec4(0);
#else
	OutColor = vec4(1);
#endif
	
	const int num =
#ifdef COMBINE2
		2;
#elif defined(COMBINE4)
		4;
#else
		6;
#endif
	float w = 1 / (float)num;
	for (int i = 0; i < num; ++i)
	{
		vec2 c = texture(CombineMap, vec3(InTexCoord.xy, (float)i));
		vec3 tc = vec3(InTexCoord.xy, c.x);
#ifdef ALBEDO
		OutColor += texture(Albedo, tc) * c.y * w;
#endif

#ifdef NORMAL_MAP
	// TODO	
#endif

#ifdef PARAMS_MAP
	// TODO
#endif
	}

#else // COMBINE

#ifdef ALBEDO
	OutColor *= texture(Albedo, InTexCoord);
	//OutColor = texture(Albedo, InTexCoord) * 2 - 1;
#endif

#ifdef NORMAL_MAP
	// TODO
#ifdef DETAIL_NORMAL_MAP
	// TODO
#endif
#endif

#endif // COMBINE

#ifdef ENV_MAP
	// TODO
#endif
	//OutColor = vec4(InNormal, 1);
	//OutColor = vec4(InTexCoord.x, InTexCoord.y, 0, 1);
	//OutColor *= vec4(gl_FragDepth) * vec4(InNormal, 1);
	//OutColor.r = clamp(dot(normalize(InNormal * inverse(NormMat)), vec3(-1, 0, 0)), 0, 1);
	//OutColor.g = OutColor.r;
	//OutColor.b = OutColor.r;

	float AlphaThreshold = 0.01f;
	if (OutColor.a <= AlphaThreshold)
		discard;

	OutNormal = vec4(InNormal, 1);
	OutMaterial = vec4(0, 0, 0, 1); // mtlParams;
}