layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
#ifdef NM
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;
#endif

uniform mat4 viewProj;
uniform mat4 model;

out vec2 v_texCoord;
out vec3 v_pos;
#ifdef NM
out mat3 v_tbn;
#else
out vec3 v_normal;
#endif

void main()
{
    v_texCoord = texCoord;
    v_pos = (vec4(pos, 1.0) * model).xyz;
#ifdef NM
    v_tbn = transpose(mat3(model)) * mat3(tangent, bitangent, normal);
#else
    v_normal = normalize(normal * mat3(model));
#endif

    gl_Position = vec4(pos, 1.0) * model * viewProj;
}
