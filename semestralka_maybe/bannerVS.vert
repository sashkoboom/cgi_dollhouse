#version 140

uniform mat4 PVMmatrix;     // Projection * View * Model --> model to clip coordinates
uniform float time;         // used for simulation of moving lights (such as sun)

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing texture coordinates

float decay = 0.05; //slow done thw time

void main() {

	// vertex position after the projection (gl_Position is predefined output variable)
	gl_Position = PVMmatrix * vec4(position, 1.0);   // outgoing vertex in clip coordinates

	float localTime = time * decay;

	vec2 offset = vec2(localTime * 5, 0.0);
	texCoord_v = texCoord - offset;
}

