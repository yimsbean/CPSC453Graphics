// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;


uniform float factor;
uniform float rads;
uniform vec2 ratio;

uniform vec2 diff;
uniform int render1stTime_v;

vec4 scale2D(in vec4 vertex){
    mat2 tmat = mat2(pow(factor,0.5),0,0,pow(factor,0.5));
    vertex.xy = vertex.xy*tmat;
    return vertex;
}

vec4 rotate2D(in vec4 vertex){
    mat2 tmat = mat2(cos(rads), -sin(rads), sin(rads), cos(rads));
    vertex.xy = vertex.xy*tmat;
    return vertex;
}


vec4 fitToSquare2D(in vec4 vertex){
    mat2 tmat = mat2(ratio[0],0,0,ratio[1]);
    vertex.xy = vertex.xy*tmat;
    return vertex;
}

vec4 translation2D(in vec4 vertex){
    mat4 tmat = mat4(
                    1,0,0,-diff[0],
                    0,1,0,+diff[1],
                    0,0,1,0,
                    0,0,0,1);
    vertex = vertex*tmat;
    return vertex;
}
void main(){   
    if(render1stTime_v == 1)
        gl_Position = vec4(VertexPosition.xy,0.0,1.0);
    else
        gl_Position = scale2D(rotate2D(translation2D(fitToSquare2D(vec4(VertexPosition.xy,0.0, 1.0))))) ;
    // assign output colour to be interpolated
    uv = VertexUV;
}