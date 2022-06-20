shader_type spatial;
render_mode diffuse_toon, specular_toon;

uniform float size = 1.0;
uniform vec4 colour : source_color = vec4(1.0);

uniform float rim = 0.2;
uniform float metal = 0.0;
uniform float rough = 0.01;
uniform sampler2D _texture;

void vertex()
{
	VERTEX = VERTEX + (NORMAL * (size - 1.0));
}

void fragment() {
	// Place fragment code here.
	//RIM = rim;
	METALLIC = metal;
    ROUGHNESS = rough;
	vec4 sample = texture(_texture, UV);
	ALBEDO = colour.rgb * sample.rgb;
	//ALPHA = colour.a;
}
