#pragma once

using namespace std;
#include "VectorMath.h"
#include <math.h>



namespace game
{
	VectorMath::VectorMath(){}

	//finds the sum of the given vector
	float VectorMath::Magnitude(glm::vec3 vector)
	{
		return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	}

	glm::vec3 VectorMath::Normalize(glm::vec3 vector)
	{
		float vectorsMagnitude = Magnitude(vector);
		if(vectorsMagnitude != 0)
		{
			return glm::vec3((vector.x / vectorsMagnitude), (vector.y / vectorsMagnitude), (vector.z / vectorsMagnitude));
		}
		else
		{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	glm::vec3 VectorMath::Clamp(glm::vec3 vector, float maxMagnitude)
	{
		float vectorsMagnitude = Magnitude(vector);
		if (vectorsMagnitude > maxMagnitude) 
		{
			return glm::vec3((vector.x / vectorsMagnitude), (vector.y / vectorsMagnitude), (vector.z / vectorsMagnitude));
		}
		else
		{
			return vector;
		}
	}
	
	float VectorMath::Dot(glm::vec3 vectorA, glm::vec3 vectorB)
	{
		return ((vectorA.x * vectorB.x) + (vectorA.y * vectorB.y) + (vectorA.z * vectorB.z));
	}

	glm::vec3 VectorMath::VectorFromAtoB(glm::vec3 vectorA, glm::vec3 vectorB)
	{
		return glm::vec3((vectorA.x - vectorB.x), (vectorA.y - vectorB.y), (vectorA.z - vectorB.z));
	}

	float VectorMath::AngleFormAtoB(glm::vec3 vectorA, glm::vec3 vectorB)//i dont know why this works 
	{
		return (Dot(vectorA, vectorB) / (Magnitude(vectorA) * Magnitude(vectorB)));
	}

	glm::vec3 VectorMath::MoveATowardsB(glm::vec3 vectorA, glm::vec3 vectorB, float maxMoveMagnitude)
	{	
		return Clamp(-VectorFromAtoB(vectorA, vectorB), maxMoveMagnitude);
	}

	glm::vec3 VectorMath::CrossProduct(glm::vec3 vectorA, glm::vec3 vectorB)
	{
		glm::vec3 result = glm::vec3(0.0, 0.0, 0.0);
			
		result.x = vectorA.y * vectorB.z - vectorA.z * vectorB.y;
		result.y = vectorA.z * vectorB.x - vectorA.x * vectorB.z;
		result.z = vectorA.x * vectorB.y - vectorA.y * vectorB.x;

		return result;
	}

	
} 
