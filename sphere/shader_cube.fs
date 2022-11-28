#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform vec3 my_color;
void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
  // FragColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	FragColor.rgb = my_color;
	FragColor.a = 1.0f;
}