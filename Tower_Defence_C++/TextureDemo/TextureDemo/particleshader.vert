// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t;
in vec2 uv;
out vec2 uv_interp;

// Uniform (global) buffer
uniform mat4 x;
uniform float time;
uniform int type;

// Attributes forwarded to the fragment shader
out vec4 color_interp;


void main()
{
	vec4 ppos;
	float acttime;
	float speed = 2.0;
	float gravity = 6.0;
	acttime = mod(time + t*2.0, 2.0);
//	acttime = mod(time,4);
	if(type == 1){
		float speed = 2.0;
		float gravity = 8.0;
		ppos = vec4(vertex.x+dir.x*acttime*speed+0.5*gravity*acttime*acttime , vertex.y+dir.y*acttime*speed*0.5, 0.0, 1.0);
	}
	else if(type == 2){
		acttime = mod(time,4);
		float speed = 15.0;
		ppos = vec4(vertex.x+dir.x*acttime*speed, vertex.y+dir.y*acttime*speed, 0.0, 1.0);
	}else if (type == 3){
		acttime = mod(time+ t*4.0, 4.0);
		float speed = 0.0;
		float gravity = 9.0;
		ppos = vec4(vertex.x+dir.x*acttime*speed+0.5*gravity*acttime*acttime , vertex.y+dir.y*acttime*speed, 0.0, 1.0);
		if((vertex.x+dir.x*acttime*speed+0.5*gravity*acttime*acttime)*(vertex.x+dir.x*acttime*speed+0.5*gravity*acttime*acttime)
		+ (vertex.y+dir.y*acttime*speed)*(vertex.y+dir.y*acttime*speed) > 400){
			ppos = vec4(10000.0,10000.0,0.0,1.0);
		}
	}else{
		float speed = 2.0;
		float gravity = 10.0;
		acttime = mod(time + t*2.0, 2.0);
		ppos = vec4(vertex.x+dir.x*acttime*speed+0.5*gravity*acttime*acttime*0.33 , vertex.y+dir.y*acttime*speed*0.05, 0.0, 1.0);
	}
    gl_Position = x*ppos;
	
    color_interp = vec4(uv,1.0, 1.0);
	uv_interp = uv;
};