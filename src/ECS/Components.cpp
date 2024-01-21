#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Renderer/Renderer.h"

void Terrasu::Drawable::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{
}

void Terrasu::SpriteComponent::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{
	if (comp.Scissors.z != 0)
		bgfx::setScissor(comp.Scissors.x, comp.Scissors.y, comp.Scissors.z, comp.Scissors.w);
	renderer->DrawQuad(comp,transform, material);
}

void Terrasu::TextComponent::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{
	//TransformComponent& tr = transform;
	//if (!tr.Scale.x || !tr.Scale.y || !tr.Scale.z)
		//continue;
	//tr.Scale /= (m_screenheight / cam.height);
	//tr.Rotation.x = 3.1415f;
	//tr.Scale.y *= -1;
	//Entity temp{ entity,this };


	bgfx::setTransform(glm::value_ptr(transform));
	//child.first->GetComponent<TransformComponent>().Scissors = { poin.x - scl.x / 2, poin.y - scl.y / 2,scl.x,scl.y };
	//comp.Scissors.
	if(comp.Scissors.z != 0)
	bgfx::setScissor(comp.Scissors.x , comp.Scissors.y, comp.Scissors.z, comp.Scissors.w);
	//tr.Rotation.x = 0;
	//tr.Scale *= (m_screenheight / cam.height);
	text->Render();
}



void Terrasu::SpriteLottieComponent::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{
	order += 1;
	if (order > image->totalFrame())
	{
		order = 0;
	}
	image->renderSync(order, *surface);

	//auto mat = m_assetManager->CreateMaterial("texturedQuad");
	surface->buffer();
	auto mem = bgfx::makeRef(surface->buffer(), surface->height() * surface->bytesPerLine());
	auto handle = bgfx::createTexture2D(
		uint16_t(surface->width())
		, uint16_t(surface->height())
		, 1 < 1
		, 1
		, bgfx::TextureFormat::Enum::BGRA8
		, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE
		, mem
	);
	Texture* tex = new Texture();
	tex->handle = handle;
	tex->texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	//mat.textures.push_back(*tex);
	//renderer->DrawQuad(transform, mat);
}

void Terrasu::SpineComponent::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{
}

void Terrasu::SvgComponent::Draw(TransformComponent comp, glm::mat4 transform, CameraComponent& camera, Renderer* renderer)
{

	renderer->DrawSvg(comp,transform,*this,camera.scren_w, camera.screen_h,camera);
}

Terrasu::TextComponent::~TextComponent()
{

	if(text->m_staticText.idx < 65)
	delete text;

}
