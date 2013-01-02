#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : require
#extension GL_ARB_draw_instanced : enable

uniform sampler2D vtxtex;
uniform mat4 mymat;

varying vec3 normal, screenSpaceNormal, lightDir, eyeVec;
flat varying vec3  flatNormal;
flat varying vec3  screenSpaceflatNormal;
varying vec2 texCoord;

int tex_w = 1024;
int tex_h = 512;

float tex_w_f = float(tex_w);
float tex_h_f = float(tex_h);

void main()
{
    int y = (gl_InstanceID * 4) / tex_w;
    float y_f = float(y);
    float v = y_f / tex_h_f;

    mat4 mvp = mat4(texture2D(vtxtex,vec2(float((gl_InstanceID*4+0)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+1)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+2)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+3)%tex_w)/tex_w_f, v)) );
    
    // shading
    vec3 inNormal = gl_Normal;
    vec4 inVertex = mvp * gl_Vertex;
    screenSpaceNormal = (gl_ModelViewMatrix * vec4(inNormal,1.0)).xyz;
    screenSpaceflatNormal = (gl_ModelViewMatrix * vec4(inNormal,1.0)).xyz;
    normal = gl_NormalMatrix * inNormal;
    flatNormal = gl_NormalMatrix * inNormal;
    vec3 vVertex = vec3(gl_ModelViewMatrix * inVertex);
    lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
    eyeVec = -vVertex;
    texCoord = gl_MultiTexCoord0.xy;
    
    // position
    
  	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * mvp * gl_Vertex;
}
