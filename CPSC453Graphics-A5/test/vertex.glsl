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
layout(location = 1) in vec3 VertexColour;
// ---
layout(location = 2) in vec2 VertexTextureCoord;
layout(location = 3) in vec3 VertexNormal;
// ---

// output to be interpolated between vertices and passed to the fragment stage
out vec4 Colour;
// ---
out vec2 TextureCoord;
out vec3 Normal;
out vec3 FragPos;
// ---

// uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // assign vertex position without modification
    gl_Position = proj*view*model*vec4(VertexPosition, 1.0);

    // assign output colour to be interpolated
	TextureCoord = VertexTextureCoord;
    // ---
	Normal = (model*vec4(VertexNormal, 0)).xyz;
    FragPos = (model*vec4(VertexPosition, 1)).xyz;
    // ---
}
