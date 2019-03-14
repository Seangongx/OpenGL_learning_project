#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;	//ambient��diffuseһ������ʡ����ambient,��������ͼ������ע��������ı�������Ҫ�뺯��texture��ͬ��glsl�޷�����
    sampler2D specular;	//�������ͼ
    float shininess;
}; 

struct Light {//�����������Զ��Ǿ����ǿ��ֵ�����Ը��ģ����ǻ������˹��յ�˥��
    vec3 position;//����ǵ��Դ��������Ҫ���ù�Դλ��
	vec3 direction;//���ֻ��ƽ�й⣬���ǲ���λ�ü����Դ�����ˣ��ĳ�ֱ�Ӹ���
	float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//uniform vec3 objectColor;//���ýṹ����
//uniform vec3 lightColor;//���ýṹ����
//uniform vec3 lightPos;//�����ڽṹ��������

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = texture(material.diffuse, TexCoord).rgb * light.ambient;

	vec3 lightDir = normalize(light.position - FragPos);//ֱ���ù��߷���

	// diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * texture(material.diffuse, TexCoord).rgb * light.diffuse;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//ע��shininess��������������������setvec3ֵ��Ӧ���ϻ������ֵ�Ч��
	vec3 specular = texture(material.specular, TexCoord).rgb * spec * light.specular;

	//spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;

	//attenuation
	float distance = length(light.position - FragPos); 
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;


	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0); //* vec4(result, 1.0);

}