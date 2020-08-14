#include "world.hpp"

World::World(size_t worldLength, size_t worldWidth, size_t maxHeight)
	:_ng{std::chrono::high_resolution_clock::now().time_since_epoch().count()},
	_WORLDLENGTH{worldLength}, _WORLDWIDTH{worldWidth}, _WORLDHEIGHT{maxHeight}
{
	_theWorld.reserve(_WORLDLENGTH * _WORLDWIDTH);

	int blockHeight = -1;
	double xSeed, ySeed = -1.0;
	for (size_t i = 0; i < _WORLDLENGTH; i++)
	{
		for (size_t j = 0; j < _WORLDWIDTH; j++)
		{
			xSeed = static_cast<double>(i) / _noiseFrequency;
			ySeed = static_cast<double>(j) / _noiseFrequency;
			blockHeight = std::floor(_ng.perlinNoise(xSeed, ySeed) * _WORLDHEIGHT);
			_theWorld.emplace_back(i, blockHeight, j);
		}
	}

	glGenBuffers(1, &_instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * Cube::_translations.size(), Cube::_translations.data(), GL_STATIC_DRAW);

	//This big ugly chunk of code is necessary to use instance rendering
	//We're sending each instance array as a mat4 which consumes 4 consecutive layout locations
	for (size_t i = 0; i < Cube::getNumObjects(); i++)
	{
		GLuint tempVAO = _theWorld[i].getVAO();
		glBindVertexArray(tempVAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void World::render() const
{
	//for (const auto& cube : _theWorld)
	//{
	//	cube.render();
	//}

	glBindVertexArray(_theWorld[0].getVAO());
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, (GLsizei)Cube::getNumObjects());
	glBindVertexArray(0);
}



