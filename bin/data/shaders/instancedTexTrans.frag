#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : require

//varying vec3 normal, screenSpaceNormal;
//flat varying vec3  screenSpaceflatNormal;

varying vec3 lightDir, eyeVec;
flat varying vec3  flatNormal;
varying vec2 texCoord;

varying vec4 customColor;

void main( void )
{
    vec4 myColor = customColor;
    myColor.a = 1.0;

	vec4 final_color =
	(myColor * gl_FrontMaterial.ambient) +
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

    
	vec3 selectedNormal;
	selectedNormal = flatNormal;
	
	vec3 N = normalize(selectedNormal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0),
		gl_FrontMaterial.shininess );

		final_color += gl_LightSource[0].specular *
		gl_FrontMaterial.specular *	specular;
	}

    gl_FragColor = final_color;
}

