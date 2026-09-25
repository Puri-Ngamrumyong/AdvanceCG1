#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform float invert;
uniform float blackWhite;

void main()
{
	vec3 color = texture(texture1, TexCoord).rgb;

	// black & white
	float gray = dot(color, vec3(0.299, 0.587, 0.114));
	color = mix(color, vec3(gray), blackWhite);

	// invert
	color = mix(color, vec3(1.0) - color, invert);

	FragColor = vec4(color, 1.0);
}