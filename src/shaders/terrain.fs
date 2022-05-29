#version 450

layout(location = 0) out vec4 f_color;

in vec3 g_normal;
in vec3 g_position;

struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
};

struct DirectionalLight {
	vec4 direction;

	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
};

struct PointLight {
	vec3 position;
	float linear;
	vec3 diffuse;
	float quadratic;
	vec3 specular;
	float alignment1;
	vec3 ambient;
	float alignment2;
};

struct SpotLight {
	vec3 position;
	float linear;
	vec3 direction;
	float quadratic;
	vec3 diffuse;
	float innerCone;
	vec3 specular;
	float outerCone;
	vec3 ambient;
	float alignment1;
};

layout(std430, binding = 1) buffer directionalLightBlock {
    DirectionalLight directionalLights[];
} b_directionalLights;

layout(std430, binding = 2) buffer pointLightBlock {
    PointLight pointLights[];
} b_pointLights;

layout(std430, binding = 3) buffer spotLightBlock {
    SpotLight spotLights[];
} b_spotLights;

Material u_material = Material(vec3(0.0f, 0.8f, 0.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f), 1.0f);

vec3 view;
vec3 diffuseColor = u_material.diffuse;

vec3 ambient = vec3(0.0f);
vec3 diffuse = vec3(0.0f);
vec3 specular = vec3(0.0f);

void directionalLight() {
	for (int i = 0; i < b_directionalLights.directionalLights.length(); i++) {
		DirectionalLight dirLight = b_directionalLights.directionalLights[i];

		vec3 light = -dirLight.direction.xyz;
		vec3 reflection = reflect(dirLight.direction.xyz, g_normal);

		ambient += diffuseColor * dirLight.ambient.xyz;
		diffuse += max(dot(g_normal, light), 0.0) * diffuseColor * dirLight.diffuse.xyz;
		specular += pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * dirLight.specular.xyz;
	}
}

void pointLight() {
	for (int i = 0; i < b_pointLights.pointLights.length(); i++) {
		PointLight pointLight = b_pointLights.pointLights[i];

		vec3 light = normalize(pointLight.position.xyz - g_position);
		vec3 reflection = reflect(-light, g_normal);
		
		float distance = length(pointLight.position.xyz - g_position);
		float attenuation = 1.0f / ((1.0f) + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
		
		ambient += attenuation * diffuseColor * pointLight.ambient.xyz;
		diffuse += attenuation * max(dot(g_normal, light), 0.0) * diffuseColor * pointLight.diffuse.xyz;
		specular += attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * pointLight.specular.xyz;
	}
}

void spotLight() {
	for (int i = 0; i < b_spotLights.spotLights.length(); i++) {
		SpotLight spotLight = b_spotLights.spotLights[i];

		vec3 light = normalize(spotLight.position.xyz - g_position);
		vec3 reflection = reflect(-light, g_normal);
		float theta = dot(light, spotLight.direction.xyz);
		
		float distance = length(spotLight.position.xyz - g_position);
		float attenuation = 1.0f / ((1.0f) + (spotLight.linear * distance) + (spotLight.quadratic * distance * distance));
		
		float epsilon = spotLight.innerCone - spotLight.outerCone;
		float intensity = clamp((theta - spotLight.outerCone) / epsilon, 0.0f, 1.0f) * int(theta > spotLight.outerCone);
		
		ambient += attenuation * spotLight.ambient.xyz * diffuseColor;
		diffuse += intensity * attenuation * max(dot(g_normal, light), 0.0) * diffuseColor * spotLight.diffuse.xyz;
		specular += intensity * attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * spotLight.specular.xyz;
	}
}

void main() {
    view = normalize(-g_position);

	directionalLight();
	pointLight();
	spotLight();

    f_color = vec4(ambient + diffuse + specular + u_material.emissive, 1.0f);
}