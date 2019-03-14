#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//uniform vec3 objectColor;//改用结构体了
//uniform vec3 lightColor;//改用结构体了
//uniform vec3 lightPos;//包含在结构体里面了
uniform vec3 viewPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform sampler2D tex;//漫反射贴图，注意采样器的变量名不要与函数texture相同，glsl无法区分
uniform sampler2D tex_spec;//镜面光贴图

void main()
{
	//float ambientStrength = 0.1;
    vec3 ambient = texture(tex, TexCoord).rgb * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * texture(tex, TexCoord).rgb * light.diffuse;

	//float specularStrength = 0.5;
	vec3 viewDir = normalize(light.position - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//注意shininess别打错，否则与主程序中setvec3值对应不上会出现奇怪的效果
	vec3 specular = texture(tex_spec, TexCoord).rgb * spec * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0); //* vec4(result, 1.0);
}