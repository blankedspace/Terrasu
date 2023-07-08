$input v_color0, v_texcoord0, v_texcoord1

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"
SAMPLER2D(s_texColor,  0);
uniform vec4 u_Color;

void main()
{
	vec4 color = texture2D(s_texColor, v_texcoord0.xy);
	gl_FragColor = color*u_Color;
}
