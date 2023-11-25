#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Material.h"
#include "SimpleQuad.h"
#include <unordered_map>
namespace Terrasu {

	struct TransformComponent {
	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };


		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};
	struct TagComponent {
	public:
		std::string tag;

	};

	struct SpriteComponent {
	public:
		Material material;
		int order = 0;

	};

	static std::unordered_map<char, glm::vec2> CharToCoord =
	{
		{'a',{36,18}},
		{'b',{37,18}},
		{'c',{38,18}},
		{'d',{39,18}},
		{'e',{40,18}},
		{'f',{41,18}},
		{'g',{42,18}},
		{'h',{43,18}},
		{'i',{44,18}},
		{'j',{45,18}},
		{'k',{46,18}},
		{'l',{47,18}},
		{'m',{48,18}},

		{'n',{36,19}},
		{'o',{37,19}},
		{'p',{38,19}},
		{'q',{39,19}},
		{'r',{40,19}},
		{'s',{41,19}},
		{'t',{42,19}},
		{'u',{43,19}},
		{'v',{44,19}},
		{'w',{45,19}},
		{'x',{46,19}},
		{'y',{47,19}},
		{'z',{48,19}},

		{' ',{1,0}},

		{'0',{36,17}},
		{'1',{37,17}},
		{'2',{38,17}},
		{'3',{39,17}},
		{'4',{40,17}},
		{'5',{41,17}},
		{'6',{42,17}},
		{'7',{43,17}},
		{'8',{44,17}},
		{'9',{45,17}},
		{':',{46,17}},
		{'.',{47,17}},
		{'%',{48,17}},


	};

	struct TextUIComponent
	{

	private:
		PosUvTileVertex* verts = new PosUvTileVertex[4 * 50];
		uint16_t* sqrTriList = new uint16_t[6 * 50];
	public:
		bgfx::DynamicVertexBufferHandle m_vbh;
		bgfx::DynamicIndexBufferHandle m_ibh;

		std::string text;
		Material material;

		TextUIComponent()
		{
			m_vbh = bgfx::createDynamicVertexBuffer(
				bgfx::makeRef(verts, sizeof(PosUvTileVertex) * 4 * 50)
				, PosUvTileVertex::ms_layout
			);

			m_ibh = bgfx::createDynamicIndexBuffer(
				bgfx::makeRef(sqrTriList, sizeof(uint16_t) * 6 * 50)
			);
		}
		void UpdateText(std::string text)
		{
			this->text = text;
			for (size_t i = 0; i < 4 * 50; i++)
			{
				verts[i] = { 0,0,0,0 };
			}
			std::fill_n(sqrTriList, 6 * 50, 0);
			int i = 0;
			for (char onechar : text)
			{

				auto cords = CharToCoord[onechar];
				uint16_t temp[] =
				{

					2, 3, 1,
					1, 0, 2,

				};
				for (size_t j = 0; j < sizeof(temp); j++)
				{
					sqrTriList[j + i * 6] = temp[j] + i * 4;
				}

				verts[i * 4] = { -1.0f + i * 1.25f,		1.0f, 1.0f,  0.0f,	cords.x,cords.y };
				verts[i * 4 + 1] = { 1.0f + i * 1.25f,  1.0f, 0.0f,  0.0f,	cords.x,cords.y };
				verts[i * 4 + 2] = { -1.0f + i * 1.25f,-1.0f, 1.0f,  1.0f,	cords.x,cords.y };
				verts[i * 4 + 3] = { 1.0f + i * 1.25f, -1.0f, 0.0f,  1.0f,  cords.x,cords.y };

				i++;
			}


			if (sizeof(verts) == 0)
				return;


			bgfx::update(m_vbh, 0, bgfx::makeRef(verts, sizeof(PosUvTileVertex) * 4 * 50));
			bgfx::update(m_ibh, 0, bgfx::makeRef(sqrTriList, sizeof(uint16_t) * 6 * 50));

		}
	};


	struct CameraComponent {
	public:
		glm::vec3 eye = { 0.0f, 0.0f, -25.0f };
		float width = 32, height = 18;
	};

	struct SimplePhysicsComponent {
	public:
		glm::vec4 Rect = { 0.0f, 0.0f, 1.0f, 1.0f };
		glm::vec3 Speed = glm::vec3(0.0f);

		enum State {
			Dynamic,
			Static,
			PassThrough,
		} state = Dynamic;
	};


}