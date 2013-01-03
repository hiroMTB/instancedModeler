
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : require

varying vec3 normal, screenSpaceNormal, lightDir, eyeVec;
flat varying vec3  flatNormal;
flat varying vec3  screenSpaceflatNormal;
uniform float RenderNormals;
uniform float UseFlatShading;
varying vec2 texCoord;

uniform sampler2D tex;


void main( void )
{
	vec4 final_color =
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	vec3 selectedNormal;
	
	if (UseFlatShading==1.0){
		selectedNormal = flatNormal;
	} else {
		selectedNormal = normal;
	}

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

//		final_color += gl_LightSource[0].specular *
//		gl_FrontMaterial.specular *	specular;
	}
	
	//gl_FragColor = texture2D(tex, gl_TexCoord[0].xy);
    gl_FragColor = final_color;

	if (RenderNormals == 1.0){
		gl_FragColor = vec4((N + vec3(1.0, 1.0, 1.0)) / 2.0,1.0);
	}
    
}

