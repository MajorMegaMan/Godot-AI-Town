shader_type spatial;
render_mode unshaded, cull_front;

uniform vec4 color = vec4( 0.0, 0.0, 0.0, 1.0 );
uniform float thickness = 0.1f;

void vertex()
{
	VERTEX = VERTEX * (1.0 + thickness);
}

void fragment( )
{
	ALBEDO = color.rgb;
	ALPHA = color.a;
}
