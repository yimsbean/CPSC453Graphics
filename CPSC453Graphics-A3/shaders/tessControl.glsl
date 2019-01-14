#version 410

layout(vertices=4) out;

in vec3 colour[];
out vec3 teColour[];	

void main()
{
	if(gl_InvocationID == 0){
		gl_TessLevelOuter[0] = 1;
		gl_TessLevelOuter[1] = 24;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	teColour[gl_InvocationID] = colour[gl_InvocationID];
}
