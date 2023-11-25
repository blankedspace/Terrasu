#pragma once
#include "glm/glm.hpp"
namespace Terrasu {
	bool AABB(glm::vec2 first, glm::vec2 second, float widthf, float heightf, float widths, float heights)
	{

		float Aleft = first.x - widthf;
		float ARight = first.x + widthf;
		float ATop = first.y + heightf;
		float ABottom = first.y - heightf;


		float Bleft = second.x - widths;
		float BRight = second.x + widths;
		float BTop = second.y + heights;
		float BBottom = second.y - heights;


		bool AisToTheRightOfB = Aleft > BRight;
		bool AisToTheLeftOfB = ARight < Bleft;
		bool AisAboveB = ABottom > BTop;
		bool AisBelowB = ATop < BBottom;

		return (!(AisToTheRightOfB || AisToTheLeftOfB || AisAboveB ||AisBelowB));
	
	}
	glm::vec2 AABBnormal(glm::vec2 first, glm::vec2 second, float widthf, float heightf, float widths, float heights, float delta)
	{

		float Aleft = first.x - widthf;
		float ARight = first.x + widthf;
		float ATop = first.y + heightf;
		float ABottom = first.y - heightf;


		float Bleft = second.x - widths;
		float BRight = second.x + widths;
		float BTop = second.y + heights;
		float BBottom = second.y - heights;

		glm::vec2 normal = { 0,0 };

		delta += 0.05f;
		Aleft += delta;
		ARight -= delta;
		ATop -= delta;
		ABottom += delta;


		Bleft += delta;
		BRight -= delta;
		BTop -= delta;
		BBottom += delta;

		bool AisToTheRightOfB = Aleft > BRight;
		bool AisToTheLeftOfB = ARight < Bleft;
		bool AisAboveB = ABottom > BTop;
		bool AisBelowB = ATop < BBottom;

		normal = { AisToTheRightOfB - AisToTheLeftOfB, AisAboveB - AisBelowB };

		
		return normal;
	}


}