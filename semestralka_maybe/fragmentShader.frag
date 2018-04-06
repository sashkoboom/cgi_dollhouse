#version 140
uniform bool isBanner;
//passing param using to callculate the fog
uniform sampler2D texSampler;
// sampler for the texture access
smooth in vec3 vertexNormal;
//vertex normal
smooth in vec3 vertexPosition;
// vertex position in world space
smooth in vec2 texCoord_v;
// incoming texture coordinates
uniform mat4 Vmatrix;
//view matrix
uniform float time;
// time used for simulation of moving


out vec4 color_f;
// outgoing fragment color


uniform vec4 spotPosition;
// reflector position (world coordinates)
uniform vec3 spotDirection;
// reflector direction (world coordinates)
uniform bool reflectorOn;
//flag for on/off reflector


//material
uniform vec3  ambient;
uniform vec3  diffuse;
uniform vec3  specular;
uniform float shininess;


uniform vec3 globalAmbient;
uniform vec3 globalDiffuse;
uniform vec3 globalSpecular;

uniform bool isStar;

struct Fog
{
	vec3 color;
	float density;
};

uniform bool fogIsOn;
Fog fog;

// structure for Light
struct Light {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  position;
	vec3  direction;
	vec3 color;
	vec3  spotDirection;
	// spotlight direction
	float spotCosCutOff;
	// cosine of the spotlight's half angle
	float spotExponent;
}
;
//structure for point light
struct PointLight{
	bool isOn;
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  position;
	vec3  color;
	float	pointConstantAtt;
	float	pointLinearAtt;
	float	pointQuadraticAtt;
};
//passing structures of point light
uniform PointLight point;
//lights:
Light directional;
Light reflector;

//calculate directional light
vec4 directionalLight(Light light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 vertexPosition, vec3 vertexNormal) {
	vec3 color = vec3(0.2f, 0.1f, 0.5f);
	vec3 L = normalize(light.position);
	vec3 R = reflect(-L, vertexNormal);
	vec3 V = normalize(-vertexPosition);
	float NdotL = max(0.0f, dot(vertexNormal, L));
	float RdotV = max(0.0f, dot(R, V));
	color += ambient * light.ambient;
	color += diffuse * light.diffuse * NdotL;
	color += specular * light.specular * pow(RdotV, shininess);
	return vec4(color, 1.0f);
}

//calculate reflector light
vec4 reflectorLight(Light light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 vertexPosition, vec3 vertexNormal) {
	vec3 ret = vec3(0.0f);
	vec3 L = normalize(light.position.xyz - vertexPosition);
	vec3 R = reflect(-L, vertexNormal);
	vec3 V = normalize(-vertexPosition);
	float NdotL = max(0.0f, dot(vertexNormal, -L));
	float RdotV = max(0.0f, dot(R, V));
	ret += ambient * light.ambient;
	ret += diffuse * light.diffuse * NdotL;
	ret += specular * light.specular * pow(RdotV, shininess);
	float spotCoef = max(0.0f, dot(-L, light.spotDirection));
	if (spotCoef < light.spotCosCutOff)
		ret *= 0.0f;
	else
		ret *= pow(spotCoef, light.spotExponent);
	return vec4(ret, 0.0f);
}

//calculate point light
//calculate point light
vec4 pointLight(PointLight point, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 vertexPosition, vec3 vertexNormal){
   
	
	point.position = (Vmatrix * vec4(point.position,1.0f)).xyz;
    vec3 color = vec3(0.5f);
    vec3 L = normalize(point.position - vertexPosition);
    vec3 R = reflect(-L, vertexNormal);
    vec3 V = normalize(-vertexPosition);
    float NdotL = max(0.0f, dot(vertexNormal, L));
    float RdotV = max(0.0f, dot(R, V));
    color += ambient * point.ambient;
    color += diffuse * point.diffuse * NdotL;
    color += specular * point.specular * pow(RdotV, shininess);
    float dist = length(point.position - vertexPosition);
    float attentuation = 1 / (point.pointConstantAtt + point.pointLinearAtt * dist + point.pointQuadraticAtt * dist * dist);
    color *= attentuation;
    
	return vec4(color, 1.0f);
}



// set up lights 
void setupLights() {
	//directional
	directional.ambient = globalAmbient;
	directional.diffuse = globalDiffuse;
	directional.specular = globalSpecular;
	directional.position = (Vmatrix * vec4(0.0f, 10.0f, -27.0f, 0.0f)).xyz;
	//reflector
	reflector.ambient = vec3(0.5f);
	reflector.diffuse = vec3(0.5f);
	reflector.specular = vec3(0.9f);
	reflector.spotCosCutOff = 0.98f;
	// bigger number -> smaller cone
	reflector.spotExponent = 2.0f;
	reflector.position = (Vmatrix * spotPosition).xyz;
	reflector.spotDirection = normalize((Vmatrix * vec4(spotDirection, 0.0)).xyz);
	
	
	fog.color = vec3(0.1f, 0.9f, 0.2f);
	fog.density = 0.5f;
}

vec4 applyFog(Fog fog, vec4 inputColor, vec3 vertexPosition)
{
	float density = fog.density;
	float distance = length(vertexPosition) / 4.0f;
	float intensity = clamp(exp(-pow(density * distance, 2.0f)), 0.0f, 1.0f);
	return vec4((intensity * inputColor.xyz + (1.0f - intensity) * fog.color), 1.0f);
}


void main()
{
	
	
	

	setupLights();
	vec3 globalAmbientLight = vec3(0.0f);
	vec4 color = vec4(ambient * globalAmbientLight, 0.0f);
	//callculate color
	color += directionalLight(directional, ambient, diffuse, specular, shininess, vertexPosition, vertexNormal);
	
	
	//callculate color with textures
	color = color * texture(texSampler, texCoord_v);
	//callculate color with reflector on	
	color += reflectorLight(reflector, ambient, diffuse, specular, shininess, vertexPosition, vertexNormal);
	
	int i = 0;
	//callculate color with pointlights

		color += pointLight(point, ambient, diffuse, specular, shininess, vertexPosition, vertexNormal);
	


	if(fogIsOn) {
		color += applyFog(fog, color, vertexPosition);
	}

	

	color_f = color;
	
	
};