#include "Intersections.h"

#include <vector>
#include <math.h>
////////////////////////////
glm::vec3 Intersections::PhongLighting(Light li, glm::vec3 pt, glm::vec3 no, glm::vec3 colour, Ray r, bool is3D){
    glm::vec3 l = glm::fastNormalize(li.point - pt);
    glm::vec3 v = glm::fastNormalize(r.origin - pt);
    glm::vec3 n = glm::fastNormalize(no);

    glm::vec3 h = glm::fastNormalize(v + l);

    float diffuseParam = (li.strength * glm::dot(n, l));
    float specularParam  =(li.strength * pow(glm::dot(n, h), 10));

    // Negative colour value is meaningless!
    glm::vec3 ambient = colour;
    glm::vec3 diffuse  = glm::dot(n, l) > 1e-3 ? colour                       * diffuseParam   : glm::vec3(0,0,0);
    glm::vec3 specular = glm::dot(n, h) > 1e-3 ? glm::vec3(0.7f,0.7,0.7f)    * specularParam  : glm::vec3(0,0,0);
    
    if(is3D == false) return ambient + diffuse;
    else return ambient + diffuse + specular;   //only for Sphere (3D)
}

glm::vec4 Intersections::intersect_Sphere(Sphere s, Light l, Ray r){
    //|| (ro_a,ro_b,ro_c) + t(rd_a,rd_b,rd_c) - (o_a,o_b,o_c) ||^2 = radius^2
    // at^2 + bt + c = 0

    // a = (rd_a^2 + rd_b^2 + rd_c^2)
    float a = pow(glm::length(r.direction), 2);
    // b = 2*(rd_a * (ro_a - o_a) + rd_b * (ro_b - o_b) + rd_c * (ro_c - o_c))
    float b =   2 * (
                    (r.direction.x * (r.origin.x - s.centre.x)) +
                    (r.direction.y * (r.origin.y - s.centre.y)) +
                    (r.direction.z * (r.origin.z - s.centre.z))
                );
    // c = ((ro_a - o_a)^2 + (ro_b - o_b) ^2 + (ro_c - o_c) ^2 - radius^2)
    float c =   pow((r.origin.x - s.centre.x), 2) +
                pow((r.origin.y - s.centre.y), 2) +
                pow((r.origin.z - s.centre.z), 2) - 
                pow(s.radius, 2);

    if(b*b - 4*a*c < 0) return glm::vec4(NULL,NULL,NULL,NULL);
    //get 2 roots of the equation
    float t1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
    float t2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
    //select the closer one
    float t  = abs(t1) < abs(t2) ? t1 : t2;

    glm::vec3 point = r.origin + r.direction * t;
    glm::vec3 n = glm::fastNormalize(point - s.centre);
    normalTmp = n;

    Ray ray;
    ray.origin = point;
    ray.direction = glm::fastNormalize(l.point - point);
    
    glm::vec3 colour = PhongLighting(l, point, n, s.colour,r, true);

    return glm::vec4(colour, t);
}

glm::vec4 Intersections::intersect_Plane(Plane p, Light l, Ray r){
    //ray parallel to plane
    if(abs(glm::dot(p.normal, r.direction)) <= 0)  return glm::vec4(NULL,NULL,NULL,NULL);
    //std::cout<<"TRUE"<<std::endl;
    float distance = glm::dot(p.point - r.origin, p.normal)/glm::dot(p.normal, r.direction);
    if(distance <=  0 ) return glm::vec4(NULL,NULL,NULL,NULL);
    glm::vec3 point = r.origin + r.direction * distance;
    glm::vec3 colour = PhongLighting(l, point, p.normal, p.colour, r, false);

    return glm::vec4(colour,distance);
}

glm::vec4 Intersections::intersect_Triangle(Triangle t, Light l, Ray r){

    glm::vec3 n = glm::fastNormalize(glm::cross(t.C1-t.C0, t.C2-t.C0)); //normal vector of triangle(plane)
    normalTmp = n;
    // Intersect with triangle's plane
    Plane p; p.normal = n; p.point = t.C0;
    glm::vec4 plane = intersect_Plane(p,l,r);
    if(plane.w == (float)NULL) return plane;
    //intersection point bewteen ray and plane
    glm::vec3 point = r.origin + r.direction * plane.w;
    //Get barycentric coordinates
    float w1,w2,w3;
    w1 = glm::dot(n, cross(t.C1-t.C0, point-t.C0));
    w2 = glm::dot(n, cross(t.C2-t.C1, point-t.C1));
    w3 = glm::dot(n, cross(t.C0-t.C2, point-t.C2));
    //If intersection point is inside triangle
    if (w1 < 0 || w2 < 0 || w3 < 0 ) {
        return glm::vec4(NULL,NULL,NULL,NULL);
    }else
        return glm::vec4(PhongLighting(l,point,n,t.colour,r,false), plane.w);

}
//// S1
glm::vec4 Intersections::traceColour(Loader loader, Ray r){
    glm::vec4 point = glm::vec4(0.1,0.1,0.1,2048);    //Point further than 1024(max(Display Width, Display Height)) is not drawn
    Light l = loader.light.at(0);
    glm::vec4 temp;
    for(Sphere s : loader.sphere){
        temp = intersect_Sphere(s, l, r);
        if(temp.w != (float)NULL && temp.w >= 1e-3 && temp.w < point.w){
            point = temp;
            normalVec = normalTmp;
            reflectionType = s.type;
        }
    }
    for(Plane p : loader.plane){
        temp = intersect_Plane(p, l, r);
        if(temp.w != (float)NULL && temp.w >= 1e-3 && temp.w < point.w) {
            point = temp;
            normalVec = p.normal;
            reflectionType = p.type;
        }
    }
    for(Triangle t : loader.triangle){
        temp = intersect_Triangle(t, l, r);
        if (temp.w != (float)NULL && temp.w >= 1e-3 && temp.w < point.w) {
            point = temp;
            normalVec = normalTmp;
            reflectionType = t.type;
        }
    }
    return point;
}

glm::vec4 Intersections::blendColour(glm::vec4 colour1, glm::vec4 colour2){
    if(colour2.w == (float)NULL) return colour1;
    else if (colour2.w >= 2048) return blendColour(colour1, glm::vec4(0.2,0.2,0.2,2047));

    if(reflectionType == 2 )
        return glm::vec4((0.3*colour1.r + 0.7*colour2.r),(0.3*colour1.g + 0.7*colour2.g),(0.3*colour1.b + 0.7*colour2.b), colour1.w);
    else    //reflection = 1
        return glm::vec4((0.5*colour1.r + 0.5*colour2.r),(0.5*colour1.g + 0.5*colour2.g),(0.5*colour1.b + 0.5*colour2.b), colour1.w);
}

//// S2
glm::vec4 Intersections::traceReflection(glm::vec4 inColour, Loader loader, Ray r, int level){
    glm::vec4 point = glm::vec4(0.2,0.2,0.2,2048);
    if(level <= 0) return traceShadow(inColour,loader,r);
    Light l = loader.light.at(0);
    Ray newRay;
    newRay.origin = glm::vec3(
        r.origin.x + r.direction.x * inColour.w,
        r.origin.y + r.direction.y * inColour.w,
        r.origin.z + r.direction.z * inColour.w
    );
    newRay.direction = glm::fastNormalize(glm::vec3(
        (newRay.origin - r.origin) - 2 *glm::dot(newRay.origin - r.origin, normalVec) * normalVec
    ));

    glm::vec4 newColour;
    for(Sphere s : loader.sphere){
        newColour = intersect_Sphere(s, l, newRay);
        if(newColour.w != (float)NULL && newColour.w >= 1e-3 && newColour.w < point.w){
            point = newColour;
            normalVec = normalTmp;
            reflectionType = s.type;
        }
    }
    for(Plane p : loader.plane){
        newColour = intersect_Plane(p, l, newRay);
        if(newColour.w != (float)NULL && newColour.w >= 1e-3 && newColour.w < point.w) {
            point = newColour;
            normalVec = p.normal;
            reflectionType = p.type;
        }
    }
    for(Triangle t : loader.triangle){
        newColour = intersect_Triangle(t, l, newRay);
        if (newColour.w != (float)NULL && newColour.w >= 1e-3 && newColour.w < point.w) {
            point = newColour;
            normalVec = normalTmp;
            reflectionType = t.type;
        }
    }
    if(reflectionType == 0) return blendColour(inColour, traceShadow(point, loader, newRay));
    return blendColour(inColour,traceReflection(point, loader, newRay, level-1));
}

/////// S3
glm::vec4 Intersections::traceShadow(glm::vec4 finalColour, Loader loader, Ray r){
    Light l = loader.light.at(0);
    Ray shadowRay;
    shadowRay.origin = glm::vec3(
        r.origin.x + r.direction.x * finalColour.w,
        r.origin.y + r.direction.y * finalColour.w,
        r.origin.z + r.direction.z * finalColour.w
    );
    shadowRay.direction = glm::fastNormalize(glm::vec3(
        l.point.x - shadowRay.origin.x,
        l.point.y - shadowRay.origin.y,
        l.point.z - shadowRay.origin.z
    ));

    glm::vec4 temp;
    for(Sphere s : loader.sphere){
        temp = intersect_Sphere(s, l, shadowRay);
        if(temp.w != (float)NULL && temp.w >= 1e-3)
            return glm::vec4(finalColour.r * 0.4 , finalColour.g * 0.4, finalColour.b * 0.4, finalColour.w);
    }
    for(Plane p : loader.plane){
        temp = intersect_Plane(p, l, shadowRay);
        if(temp.w != (float)NULL && temp.w >= 1e-3 ) 
            return glm::vec4(finalColour.r * 0.4 , finalColour.g * 0.4, finalColour.b * 0.4, finalColour.w);
    }
    for(Triangle t : loader.triangle){
        temp = intersect_Triangle(t, l, shadowRay);
        if (temp.w != (float)NULL && temp.w >= 1e-3) 
            return glm::vec4(finalColour.r * 0.4 , finalColour.g * 0.4, finalColour.b * 0.4, finalColour.w);
    }
    return finalColour;
}

glm::vec4 Intersections::getColour(Loader loader, Ray r){
    normalVec = normalTmp = glm::vec3(0,0,0);
    reflectionType = 0;

    glm::vec4 c = traceColour(loader, r);
    if(reflectionType == 0)
        c = traceShadow(c, loader, r);
    else
        c = traceReflection(c,loader, r, 2);
    return c;
}