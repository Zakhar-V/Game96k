USAMPLER(0, sampler2D, ColorMap);

void main()
{
	gl_FragDepth = log(InLogZ) * (Depth.w - DepthBias);

	vec4 mtlColor = InstanceParams[InInstanceID][0];
	vec4 mtlParams = InstanceParams[InInstanceID][1];

#ifdef TEXTURE
	OutColor = mtlColor *texture(ColorMap, InTexCoord);
#else
	//OutColor = mtlColor;
	//TODO: vert color
	OutColor = InColor;
#endif
	OutNormal = vec4(InNormal, 1);
	OutMaterial = mtlParams;
}