shader_type spatial;
render_mode diffuse_toon, specular_toon, cull_front;

uniform float size = 1.0;
uniform vec4 colour : source_color = vec4(1.0);

uniform float rim = 0.2;
uniform float metal = 0.0;
uniform float rough = 0.01;

void vertex()
{
	VERTEX = VERTEX * size;
}

void fragment() {
	// Place fragment code here.
	//RIM = rim;
	METALLIC = metal;
    ROUGHNESS = rough;
	ALBEDO = colour.rgb;
	//ALPHA = colour.a;
}
