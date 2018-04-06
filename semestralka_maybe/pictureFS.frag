#version 140

uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame

uniform sampler2D texSampler; // sampler for texture access

smooth in vec3 position_v;    // camera space fragment position
smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragment color

// there are 10 frames in the row, 2 rows total
uniform ivec2 pattern = ivec2(10, 2);
// one frame lasts 0.1s
uniform float frameDuration = 0.1f;

// callculate texture sample dependent on frame
vec4 sampleTexture(int frame) {

	vec2 offset = vec2(1.0) / vec2(pattern);
	vec2 texCoordBase = texCoord_v / vec2(pattern);
	//callculate coordinates of texture 
	vec2 texCoord = texCoordBase + vec2(frame%pattern.x, frame / pattern.x)*offset;
	return texture(texSampler, texCoord);

}


void main() {
	// frame of the texture to be used for explosion drawing
	float animDuration = frameDuration * 20.0f;
	float pause = 10.0f;
	float timeCalc = float(time - (int(time / (animDuration + pause))*(animDuration + pause)) - pause);
	timeCalc = timeCalc > 0 ? timeCalc : 0;

	float timeFrame = timeCalc / frameDuration;
	int frame = int(timeFrame);
	// sample proper frame of the texture to get a fragment color
	vec4 cColor = sampleTexture(frame);
	vec4 nColor = sampleTexture(frame + 1);
	float nextRatio = mod(timeFrame, 1.0f);
	color_f = mix(cColor, nColor, nextRatio);
	
}
