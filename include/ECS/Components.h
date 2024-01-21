#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Renderer/Material.h"
#include "Renderer/Vertex.h"
#include "Asset/SpineAnimation.h"
#include <unordered_map>

#include "rlottie/rlottie.h"
#include <set>
#include <nanovg/nanovg.h>

namespace Terrasu {

	struct CameraComponent {
	public:
		uint32_t backgroundcolor = 7777777;;
		glm::vec3 eye = { 0.0f, 0.0f, -25.0f };
		float width = 32, height = 18;
		int scren_w, screen_h;
	};
	struct TransformComponent {
	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec4 Scissors = {0,0,0,0};

		glm::mat4 finalTransform;

		std::string parentuuid;
		uint32_t parent = -1;  // Reference to parent entity
		std::set<uint32_t> children;


	};
	struct TagComponent {
	public:
		std::string tag;
		std::string UUID;
	};

	struct Drawable {
	public:
		int order;
		virtual void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer);
	};


	struct SvgComponent : public Drawable
	{
		std::string name;
		NVGcolor fill_color = {999.0f,999.0f,999.0f,999.0f};
		NVGcolor stroke_color = { 999.0f,999.0f,999.0f,999.0f };
		NSVGimage* image;
		void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer) override;
	};
	struct SpriteComponent: public Drawable {
	public:
		void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer);
		Material material;


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



	struct TextComponent: public Drawable {
	public:
		void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer);
		Text* text = nullptr;
		NVGcolor color = { 0.0f,0.0f,0.0f,1.0f };
		~TextComponent();
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

	struct SpineComponent : public Drawable {
	public:
		SpineAnimation* image;
		void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer) ;
	};


	struct SpriteLottieComponent: public Drawable {
	public:
		void Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer) ;
		std::unique_ptr<rlottie::Animation> image;
		rlottie::Surface* surface;
		std::unique_ptr<uint32_t[]> buffer;



	};
}