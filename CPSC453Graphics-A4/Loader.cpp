#include "Loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <random>


std::vector<Light> light;
std::vector<Sphere> sphere;
std::vector<Plane> plane;
std::vector<Triangle> triangle;

void Loader::initialize(){
    light.clear();
    sphere.clear();
    plane.clear();
    triangle.clear();
}

void Loader::readFromSceneFile(const char* fileName){
    std::random_device rd;
    std::mt19937 RNG{ rd() };
    std::uniform_real_distribution<> sides(0.0, 1.0);

    std::ifstream file(fileName);
    std::string in, word;
    bool isInCurly = false;
    enum objType {LIGHT, SPHERE, PLANE, TRIANGLE};
    objType type;
    int pos;


    while(std::getline(file, in)){
        if(in.length() < 2 || in.at(0) == '#') continue;
        if(isInCurly){
            std::string mixture = in;
            while(std::getline(file, in)){
                pos = in.find("}");
                if(pos >= 0) break;
                else mixture += " " + in;
            }
            std::vector<float> split;
            std::istringstream iss(mixture);
            for(std::string s; iss >> s; )
                split.push_back(std::stof(s));

            if(type == LIGHT){
                Light l;
                l.point = glm::vec3(split.at(0),split.at(1),split.at(2));
                l.strength = 0.9;
                light.push_back(l);
            }else if(type == SPHERE){
                Sphere s;
                s.centre = glm::vec3(split.at(0),split.at(1),split.at(2));
                s.radius = split.at(3);
                s.colour = glm::vec3(split.at(4),split.at(5),split.at(6));
                s.type = split.at(7);
                sphere.push_back(s);
            }else if(type == PLANE){
                Plane p;
                p.normal = glm::vec3(split.at(0),split.at(1),split.at(2));
                p.point = glm::vec3(split.at(3),split.at(4),split.at(5));
                p.colour = glm::vec3(split.at(6),split.at(7),split.at(8));
                p.type = split.at(9);
                plane.push_back(p);
            }else if(type == TRIANGLE){
                Triangle t;
                t.C0 = glm::vec3(split.at(0),split.at(1),split.at(2));
                t.C1 = glm::vec3(split.at(3),split.at(4),split.at(5));
                t.C2 = glm::vec3(split.at(6),split.at(7),split.at(8));
                t.colour = glm::vec3(split.at(9),split.at(10),split.at(11));
                t.type = split.at(12);
                triangle.push_back(t);
            }
            isInCurly = false;
        }else{
            pos = in.find(" ");
            word = in.substr(0,pos);
            if(word == "light"){         type = LIGHT;    isInCurly = true;}
            else if(word == "sphere"){   type = SPHERE;   isInCurly = true;}
            else if(word == "plane"){    type = PLANE;    isInCurly = true;}
            else if(word == "triangle"){ type = TRIANGLE; isInCurly = true;}
            else{                                         isInCurly = false;}
        }
    }
}