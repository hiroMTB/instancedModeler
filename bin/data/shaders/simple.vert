#extension GL_EXT_gpu_shader4 : require

uniform sampler2D vtxtex;

int tex_w = 2048;
int tex_h = 2048;
float tex_w_f = float(tex_w);
float tex_h_f = float(tex_h);

void main(void)
{
    int y = (gl_InstanceID * 4) / tex_w;
    float y_f = float(y);
    float v = y_f / tex_h_f;
    
    mat4 mvp = mat4(texture2D(vtxtex,vec2(float((gl_InstanceID*4+0)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+1)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+2)%tex_w)/tex_w_f, v)),
                    texture2D(vtxtex,vec2(float((gl_InstanceID*4+3)%tex_w)/tex_w_f, v)) );

    vec4 inVertex = mvp * gl_Vertex;
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * inVertex;

}