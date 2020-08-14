#include <vector>
#include <tuple>
#include <chrono>
#include <iostream>

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

#include "noisegenerator.hpp"
#include "cube.hpp"

class World
{
public:
	World();
	void render() const;

private:
	NoiseGenerator _ng;

	//Holds all coordinates to be filled with cubes
	std::vector<Cube> _theWorld;

	GLuint _instanceVBO;

	const size_t _WORLDLENGTH = 100;
	const size_t _WORLDWIDTH = 100;
	const size_t _WORLDHEIGHT = 50;
};

//glm::mat4* positions = new glm::mat4[Cube::getNumObjects()];
//
//for (size_t i = 0; i < Cube::getNumObjects(); i++)
//{
//    glm::mat4 temp{ 1.f };
//
//    temp = glm::translate(temp, Cube::_translations[i]);
//
//    positions[i] = temp;
//}