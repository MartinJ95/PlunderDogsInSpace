#version 400 core

struct pointLight
{
	vec3 m_position;
	vec3 m_intensity;
	float m_radius;
};

struct spotLight
{
	vec3 m_position;
	vec3 m_intensity;
	vec3 m_direction;
	float m_fieldOfView;
	float m_range;
};

in vec3 VertexPos;
in vec2 TexCoord;
in vec3 Position;
in vec3 Normal;
in vec3 Color;

uniform sampler2D Texture;
//uniform pointLight pointLights[100];
//uniform int numPointLights;
//uniform spotLight spotLights[100];
//uniform int numSpotLights;
uniform vec3 cameraPosition;
uniform vec3 ambientLighting;
uniform float health;
uniform float totalHealth;

vec3 surfaceDiffuse;
vec3 surfaceSpecular;
vec3 finalColour = vec3(0, 0, 0);
vec3 normal;

out vec4 out_color;

vec3 getPointLightDiffuse(pointLight light)
{
	float distance = length(light.m_position - Position);
	float attenuation = smoothstep(light.m_radius, light.m_radius / 2, distance);
	vec3 L = normalize(light.m_position - Position);
	float diffuse_intensity = max(0, dot(L, normal));
	return (diffuse_intensity * surfaceDiffuse) * attenuation;
}

vec3 getPointLightSpecular(pointLight light)
{
	float distance = length(light.m_position - Position);
	float attenuation = smoothstep(light.m_radius, light.m_radius / 2, distance);
	vec3 L = normalize(light.m_position - Position);
	vec3 viewDir = normalize(cameraPosition - Position);
	vec3 reflectDir = reflect(-L, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);
	return spec * surfaceSpecular * attenuation;
}

vec3 getSpotLightDiffuse(spotLight light)
{
	float distance = length(light.m_position - Position);
	float attenuation = smoothstep(light.m_range, light.m_range/2, distance);
	vec3 L = normalize(light.m_position - Position);
	vec3 s = -L;
	float fc = smoothstep(cos(0.5 * radians(light.m_fieldOfView)), 1, dot(s, light.m_direction));
	float diffuse_intensity = max(0, dot(L, normal));
	return (diffuse_intensity * surfaceDiffuse) * attenuation * fc;
}

vec3 getSpotLightSpecular(spotLight light)
{
	float distance = length(light.m_position - Position);
	float attenuation = smoothstep(light.m_range, 0, distance);
	vec3 L = normalize(light.m_position - Position);
	vec3 viewDir = normalize(cameraPosition - Position);
	vec3 reflectDir = reflect(-L, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);
	return spec * surfaceSpecular * attenuation;
}

void main()
{
	normal = normalize(Normal);

	//surfaceDiffuse = texture(Texture, TexCoord).xyz;

	//without specular
	//for (int i = 0; i < numPointLights; i++)
	//{
		//finalColour += pointLights[i].m_intensity * (getPointLightDiffuse(pointLights[i]));
	//}

	//with specular
	//for(int i = 0; i < numPointLights; i++)
	//{
	//	vec3 diffuse = getPointLightDiffuse(pointLights[i]);
	//	vec3 specular = getPointLightSpecular(pointLights[i]);
	//	finalColour += pointLights[i].m_intensity * (diffuse + specular);
	//}

	//without specular
	//for (int i = 0; i < numSpotLights; i++)
	//{
		//finalColour += spotLights[i].m_intensity * (getSpotLightDiffuse(spotLights[i]));
	//}

	//with specular
	//for(int i = 0; i < numSpotLights; i++)
	//{
	//	vec3 diffuse = getSpotLightDiffuse(spotLights[i]);
	//	vec3 specular = getSpotLightSpecular(spotLights[i]);
	//	finalColour += spotLights[i].m_intensity * (diffuse + specular);
	//}

	//vec3 reflectedLight = (ambientLighting * surfaceDiffuse) + finalColour;

	//gl_FragColor = vec4(1, 0, 0, 1);
	//gl_FragColor = vec4(reflectedLight, 1.0);
	//gl_FragColor = vec4(vec3(1, 1, 1) * normal, 1.0);
	//gl_FragColor = texture(Texture, TexCoord).xyz;
	//gl_FragColor = vec4(Color, 1.0);
	if((VertexPos.x+1)*0.5 < max(min(health, totalHealth),0.1f) / totalHealth)
	{
		out_color = vec4(0, 1, 0, 1);
	}
	else
	{
		out_color = vec4(1, 0, 0, 1);
	}
	//out_color = vec4(Color, 1.0);
//out_color = vec4((VertexPos.x + 1)*0.5, 0.f, 0.f, 1.f);
//out_color = vec4(health / totalHealth, 0.f, 0.f, 1.f);
}