#version 140
uniform mat4 matrix;		// Projection * View * Model  --> model to clip coordinates
uniform mat4 Vmatrix;       // View                       --> world to eye coordinates
uniform mat4 Mmatrix;       // Model                      --> model to world coordinates
uniform mat4 normalMatrix;	//incoming  normal matrix
uniform sampler2D texSampler;  // sampler for texture access


in vec3 vertex;  //vertex
in vec3 normal; // vertex normal
in vec2 texCoord;	// incoming texture coordinates                

smooth out vec3 vertexNormal;
smooth out vec3 vertexPosition;

smooth out vec2 texCoord_v;	// outgoing texture coordinates  
//point light
/*struct PointLight{

	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  position;
	vec3 color;
	float	pointConstantAtt;
	float	pointLinearAtt;
	float	pointQuadraticAtt;
};*/
//incoming array of structures point light, using  to light up the pumkins
//uniform PointLight points[10];
///uniform PointLight point;


void main() {

	vertexPosition = (Vmatrix * Mmatrix * vec4(vertex, 1.0)).xyz;
	vertexNormal = normalize((normalMatrix * vec4(normal, 0.0)).xyz);
	gl_Position = matrix * vec4(vertex, 1);

	texCoord_v = texCoord;
}