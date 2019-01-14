// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage1
in vec2 uv;

// first output is mapped to the framebuffer's colour index by default
layout(location = 0) out vec4 FragmentColour;
//layout(location = 0) out vec4 frameColour;

uniform int grayscale1, grayscale2, grayscale3, sepiaTone;
uniform int vertical, horizontal, unsharp;

uniform sampler2DRect imageTexture;


uniform int kernelSize, render1stTime_f;
uniform float kernelConstant,diff2;

vec4 gaussianEffects_x(in sampler2DRect inTexture){

    vec4 Colour = vec4(0);
    //1D, x-axis
    Colour = vec4(texture(inTexture, vec2(uv.x, uv.y)).rgba*1.0*(kernelConstant+diff2));
    for(int i=1; i <= int(kernelSize/2); ++i){
        float multiplier = 1.0*kernelConstant*pow(2.7182818,-1.0*i*i);
        Colour += vec4(texture(inTexture, vec2(uv.x+i, uv.y)).rgba*(multiplier+diff2));
        Colour += vec4(texture(inTexture, vec2(uv.x-i, uv.y)).rgba*(multiplier+diff2));
    }
    return Colour;
}
vec4 gaussianEffects_y(in sampler2DRect inTexture){
    vec4 Colour = vec4(0);
    Colour = vec4(texture(inTexture, vec2(uv.x, uv.y)).rgba*1.0*(kernelConstant+diff2));
    for(int i=1; i <= int(kernelSize/2); ++i){
        float multiplier = 1.0*kernelConstant*pow(2.7182818,-1.0*i*i);
        Colour += vec4(texture(inTexture, vec2(uv.x, uv.y+i)).rgba*(multiplier+diff2));
        Colour += vec4(texture(inTexture, vec2(uv.x, uv.y-i)).rgba*(multiplier+diff2));
    }
    return Colour;
}

vec4 sepiaToneEffect(in vec4 inColour){
    vec4 Colour = inColour;
    if(sepiaTone == 1) {
      Colour.x = 0.393f*Colour.x + 0.769f*Colour.y + 0.189f*Colour.z;
      Colour.y = 0.349f*Colour.x + 0.686f*Colour.y + 0.168f*Colour.z;
      Colour.z = 0.272f*Colour.x + 0.534f*Colour.y + 0.131f*Colour.z;
    }
    return Colour;
}
vec4 grayscale1Effect(in vec4 inColour){
    vec4 Colour = inColour;
    if(grayscale1 == 1) {
        float l = 0.333f*Colour.x + 0.333f*Colour.y + 0.333f*Colour.z;
        Colour.x = Colour.y = Colour.z = l;
    }
    return Colour;
}
vec4 grayscale2Effect(in vec4 inColour){
    vec4 Colour = inColour;
    if(grayscale2 == 1) {
        float l = 0.299f*Colour.x + 0.587f*Colour.y + 0.114f*Colour.z;
        Colour.x = Colour.y = Colour.z = l;
    }
    return Colour;
}
vec4 grayscale3Effect(in vec4 inColour){
    vec4 Colour = inColour;
    if(grayscale3 == 1) {
        float l = 0.213f*Colour.x + 0.715f*Colour.y + 0.072f*Colour.z;
        Colour.x = Colour.y = Colour.z = l;
    }
    return Colour;
}
vec4 verticalEffect(in vec4 inColour){
    vec4 Colour = inColour;
    if(vertical == 1) {
        vec4 tl = texture(imageTexture, vec2(uv.x-1, uv.y+1));
        vec4 tr = texture(imageTexture, vec2(uv.x+1, uv.y+1));
        vec4 l1 = texture(imageTexture, vec2(uv.x-1, uv.y));
        vec4 r1 = texture(imageTexture, vec2(uv.x+1, uv.y));
        vec4 ll = texture(imageTexture, vec2(uv.x-1, uv.y-1));
        vec4 lr = texture(imageTexture, vec2(uv.x+1, uv.y-1));
        Colour = tl + 2*l1 + ll - tr - 2*r1 - lr;
    }
    return Colour;
}
vec4 horizontalEffect(in vec4 inColour){
    vec4 Colour = inColour;
    if(horizontal == 1) {
        vec4 tl = texture(imageTexture, vec2(uv.x-1, uv.y+1));
        vec4 tm = texture(imageTexture, vec2(uv.x, uv.y+1));
        vec4 tr = texture(imageTexture, vec2(uv.x+1, uv.y+1));
        vec4 ll = texture(imageTexture, vec2(uv.x-1, uv.y-1));
        vec4 lm = texture(imageTexture, vec2(uv.x, uv.y-1));
        vec4 lr = texture(imageTexture, vec2(uv.x+1, uv.y-1));
        Colour = -tl - 2*tm -tr + ll + 2*lm +lr;
    }
    return Colour;
}
vec4 unsharpEffect(in vec4 inColour){
    vec4 Colour = inColour;
    if(unsharp == 1) {
        vec4 tm = texture(imageTexture, vec2(uv.x, uv.y+1));
        vec4 l1 = texture(imageTexture, vec2(uv.x-1, uv.y));
        vec4 r1 = texture(imageTexture, vec2(uv.x+1, uv.y));
        vec4 lm = texture(imageTexture, vec2(uv.x, uv.y-1));
        Colour =  -tm - l1 + 5*Colour - r1 - lm;
    }
    return Colour;
}
void main(void)
{
    if(render1stTime_f == 1){
        if(kernelSize != 0) 
            FragmentColour = gaussianEffects_y(imageTexture);
        else
            FragmentColour = texture(imageTexture, uv);
    }else{
        vec4 Colour;
        if(kernelSize != 0) 
            Colour =  gaussianEffects_x(imageTexture);
        else
            Colour = texture(imageTexture, uv);
        FragmentColour = sepiaToneEffect(grayscale3Effect(grayscale2Effect(grayscale1Effect(unsharpEffect(horizontalEffect(verticalEffect(Colour)))))));
    }
}
