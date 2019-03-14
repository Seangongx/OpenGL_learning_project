#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//uniform vec3 objectColor;//���ýṹ����
//uniform vec3 lightColor;//���ýṹ����
//uniform vec3 lightPos;//�����ڽṹ��������
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

uniform sampler2D tex;//��������ͼ��ע��������ı�������Ҫ�뺯��texture��ͬ��glsl�޷�����
uniform sampler2D tex_spec;//�������ͼ

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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//ע��shininess������������������setvec3ֵ��Ӧ���ϻ������ֵ�Ч��
	vec3 specular = texture(tex_spec, TexCoord).rgb * spec * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0); //* vec4(result, 1.0);
}