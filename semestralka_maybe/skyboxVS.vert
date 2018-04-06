#version 140
//incoming inverse projection*view matrix 
uniform mat4 inversePVmatrix;
//incoming screen coordinats 
in vec2 screenCoord;
// outgoing texture coordinates
out vec3 texCoord_v;



void main() {
	vec4 farplaneCoord = vec4(screenCoord, 0.999999, 1);
	vec4 worldViewCoord = inversePVmatrix * farplaneCoord;
	texCoord_v = worldViewCoord.xyz / worldViewCoord.w;
	gl_Position = farplaneCoord;
}
