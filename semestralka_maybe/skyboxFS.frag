#version 140
// sampler for texture access
uniform samplerCube skyboxSampler;
// incoming fragment texture coordinates
in vec3 texCoord_v;
// outgoing fragment color
out vec4 color_f;

uniform bool fogIsOn;


void main() {
	// fragment color is given only by the texture
	color_f = texture(skyboxSampler, texCoord_v);

	if (fogIsOn){
	
		color_f += vec4(0.0f, 0.3f, 0.0f, 1.0f);
	
	}

}