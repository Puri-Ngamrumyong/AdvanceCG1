#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform float invert;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);
	vec3 inverted = vec3(1.0) - texColor.rgb;
	FragColor = vec4(mix(texColor.rgb, inverted, invert), texColor.a);
}