$input a_position, a_color0, a_texcoord0, a_texcoord1
$output v_color0, v_texcoord0, v_texcoord1

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"
uniform vec4 TileData;

void main()
{
	gl_Position = mul(u_model[0], vec4(a_position, 1.0));
	v_color0.x = a_position.x;
	v_texcoord0.x = (1.0 - a_texcoord0.x/TileData[0]);
	v_texcoord0.y = (a_texcoord0.y/TileData[1]);
	v_texcoord0.x = v_texcoord0.x + (1.0 /TileData[0])*TileData[2];
	v_texcoord0.y = v_texcoord0.y + (1.0 /TileData[1])*TileData[3];
}
