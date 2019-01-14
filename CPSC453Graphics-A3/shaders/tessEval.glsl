#version 410

layout(isolines, cw) in;

in vec3 teColour[];
out vec3 colour;

uniform int pointCount;

void main()
{
	float u = gl_TessCoord.x;

	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec4 p3 = gl_in[3].gl_Position;

	vec3 c0 = teColour[0];
	vec3 c1 = teColour[1];
	vec3 c2 = teColour[2];
	vec3 c3 = teColour[3];

	if(pointCount == 3){
		gl_Position = u*u*p2   + 2*u*(1.0-u)*p1   + (1.0-u)*(1.0-u)*p0;
		colour = u*u*c2   + 2*u*(1.0-u)*c1   + (1.0-u)*(1.0-u)*c0;
	}else if (pointCount == 4){
		gl_Position = u*u*u*p3 + 3*u*u*(1.0-u)*p2 + 3*u*(1.0-u)*(1.0-u)*p1 + (1.0-u)*(1.0-u)*(1.0-u)*p0;
		colour	= u*u*u*c3 + 3*u*u*(1.0-u)*c2 + 3*u*(1.0-u)*(1.0-u)*c1 + (1.0-u)*(1.0-u)*(1.0-u)*c0;
	}else{
		gl_Position = vec4(gl_TessCoord,1.0);
		colour = vec3(0.8,0.8,0.8);
	}	
}
