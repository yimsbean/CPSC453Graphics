#include "Data.h"


float Data::Q1_vertices1[][2]={
        {1.0f, 1.0f},   	{2.0f,  -1.0f}, 	{0.0f,  -1.0f},
        {0.0f,  -1.0f}, 	{-2.0f,  -1.0f}, 	{-1.0f,  1.0f}, 
        {-1.0f,  1.0f}, 	{0.0f, 1.0f}, 		{1.0f,  1.0f},
        {1.2f,  0.5f},		{2.5f,  1.0f}, 		{1.3f,  -0.49f}
};
float Data::Q1_vertices2[][2] = {
        {1.0f, 1.0f}, {4.0f, 0.0f}, {6.0f, 2.0f}, 	{9.0f, 1.0f},
        {8.0f, 2.0f}, {0.0f, 8.0f}, {0.0f, -2.0f}, 	{8.0f, 4.0f},
        {5.0f, 3.0f}, {3.0f, 2.0f}, {3.0f, 3.0f}, 	{5.0f, 2.0f},
        {3.0f, 2.2f}, {3.5f, 2.7f}, {3.5f, 3.3f},	{3.0f, 3.8f},
        {2.8f, 3.5f}, {2.4f, 3.8f}, {2.4f, 3.2f},	{2.8f, 3.5f}
};
int Data::QuestionNumber = 0;


std::string Data::name = "Seung";

std::string Data::scrollText = "The quick brown fox jumps over the lazy dog.";

int Data::pointCount = 3;

float Data::scRate = 0;
bool Data::scEnabled = false;

float Data::scLocation_x = 0;
float Data::scLocation_y = 0;
float Data::scMaxLength = 5;


std::chrono::time_point<std::chrono::system_clock> Data::startTime = std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::system_clock> Data::currentTime = std::chrono::system_clock::now();

//Full Text available, but rendering all text is a torture for CPU
std::vector<std::string> Data::bonus1Text={
"Episode IV",
"A NEW HOPE",
"It is a peirod of civil war. Rebel Spaceshpis, striking from a hidden base, have won their first victory against the evil Galatic Empire.",
//"During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the DEATH STAR, an armored space station with enough power to destory an entire planet.",
//"Pursued by the Empire's sinister agents, Princess Leia races home aboard her starship, custodian of the stolen plans that can save her people and restore freedom the the galaxy...."
};