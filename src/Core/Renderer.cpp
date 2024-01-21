#include "bgfx/bgfx.h"
#include "Renderer/Renderer.h"
#include <nanovg/nanovg.h>
#include <nanosvg/nanosvg.h>
namespace Terrasu {

	static PosUvTileVertex quadVertices[] =
	{


		{-1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f },  //0
		{ 1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f },  //1
		{-1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f },  //2
		{ 1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f },  //3
	};

	static const uint16_t quadTriList[] =
	{

		2, 3, 1,
		1, 0, 2,

	};
	class Renderer;
	class SimpleQuad {
	public:
		SimpleQuad() {
			PosUvTileVertex::init();


			m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(quadVertices, sizeof(quadVertices)),
				PosUvTileVertex::ms_layout);



			m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(quadTriList, sizeof(quadTriList))
			);

		}
		~SimpleQuad() {
			bgfx::destroy(m_ibh);
			bgfx::destroy(m_vbh);
		};
	private:
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;

		friend Renderer;
	};
}
namespace Terrasu
{
	bgfx::VertexLayout PosUvTileVertex::ms_layout;

	Renderer::Renderer()
	{
		quad = new SimpleQuad();
		m_nvg = nvgCreate(1, 0);
	}
	Renderer::~Renderer()
	{
	
	}
	
	void Renderer::DrawQuad(TransformComponent comp, const glm::mat4 transform, const Material& material)
	{
		// Set model matrix for rendering.
		bgfx::setTransform(glm::value_ptr(transform));

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, quad->m_vbh);
		bgfx::setIndexBuffer(quad->m_ibh);

		for (auto texture : material.textures)
		{
			bgfx::setTexture(0, texture.texColor, texture.handle);
		}

		for (auto uniform : material.uniforms)
		{
			bgfx::setUniform(m_assetManager->CreateUniformHandle(uniform.m_name), glm::value_ptr(uniform.data));
		}
		//bgfx::setTexture(0, s_texColor, *TryLoadTexture(spr.textureName));
		//float t = 2.2;
		//glm::vec4 data = { spr.xTilesetSize, spr.yTilesetSize,spr.xTile,spr.yTile };
		//bgfx::setUniform(TileData, glm::value_ptr(data));
		//bgfx::setUniform(u_color, glm::value_ptr(spr.color));
		//bgfx::setUniform(u_fill, glm::value_ptr(glm::vec4{ spr.fill,0,0,0 }));
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			;

	
		// Set render states.
		bgfx::setState(state);
		// Submit primitive for rendering to view 0.
		auto prog = material.shader.handle;
		bgfx::submit(0, prog);
	}

	void Renderer::DrawText(TransformComponent comp, const glm::mat4 transform, const TextUIComponent& text)
	{

		auto& material = text.material;
		// Set model matrix for rendering.
		bgfx::setTransform(glm::value_ptr(transform));

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, text.m_vbh);
		bgfx::setIndexBuffer(text.m_ibh);


		for (auto texture : material.textures)
		{
			bgfx::setTexture(0, texture.texColor, texture.handle);
		}

		for (auto uniform : material.uniforms)
		{
			bgfx::setUniform(m_assetManager->CreateUniformHandle(uniform.m_name), glm::value_ptr(uniform.data));
		}
		//bgfx::setTexture(0, s_texColor, *TryLoadTexture(spr.textureName));
		//float t = 2.2;
		//glm::vec4 data = { spr.xTilesetSize, spr.yTilesetSize,spr.xTile,spr.yTile };
		//bgfx::setUniform(TileData, glm::value_ptr(data));
		//bgfx::setUniform(u_color, glm::value_ptr(spr.color));
		//bgfx::setUniform(u_fill, glm::value_ptr(glm::vec4{ spr.fill,0,0,0 }));
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			;
		// Set render states.
		bgfx::setState(state);
		// Submit primitive for rendering to view 0.
		auto prog = material.shader.handle;
		bgfx::submit(0, prog);

	}

	void Renderer::DrawVerts(std::vector<PosUvTileVertex>* vertexArray, const glm::mat4& transform, Material* material)
	{
		if (material == nullptr)
			material = &m_assetManager->CreateMaterial("texturedQuad");
		if (m_vbh.idx > 5000)
		{
			m_vbh = bgfx::createDynamicVertexBuffer(bgfx::makeRef(vertexArray->data(), vertexArray->size() * 6 * sizeof(float)), Terrasu::PosUvTileVertex::ms_layout);
		}

		bgfx::setTransform(glm::value_ptr(transform));
		//solve not overloaded load
		auto texture = m_assetManager->LoadTexture("Assets/mix-and-match-pro.png");
		bgfx::setTexture(0, texture->texColor, texture->handle, BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC);




		bgfx::update(m_vbh, 0, bgfx::makeRef(vertexArray->data(), vertexArray->size() * 6 * sizeof(float)));


		bgfx::setVertexBuffer(0, m_vbh);


		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			;
		// Set render states.
		bgfx::setState(state);
		//solve not overloaded load


		bgfx::submit(0, material->shader.handle);
	}
	float getLineCrossing(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
		glm::vec2 b = p2 - p0;
		glm::vec2 d = p1 - p0;
		glm::vec2 e = p3 - p2;
		float m = d.x * e.y - d.y * e.x;
		// Check if lines are parallel, or if either pair of points are equal
		if (fabsf(m) < 1e-6)
			return NAN;
		return -(d.x * b.y - d.y * b.x) / m;
	}
	NVGcolor getNVGColor(uint32_t color) {
		return nvgRGBA(
			(color >> 0) & 0xff,
			(color >> 8) & 0xff,
			(color >> 16) & 0xff,
			(color >> 24) & 0xff);
	}

	NVGpaint getPaint(NVGcontext* vg, NSVGpaint* p) {
		assert(p->type == NSVG_PAINT_LINEAR_GRADIENT || p->type == NSVG_PAINT_RADIAL_GRADIENT);
		NSVGgradient* g = p->gradient;
		assert(g->nstops >= 1);
		NVGcolor icol = getNVGColor(g->stops[0].color);
		NVGcolor ocol = getNVGColor(g->stops[g->nstops - 1].color);

		float inverse[6];
		nvgTransformInverse(inverse, g->xform);

		glm::vec2 s, e;

			// Is it always the case that the gradient should be transformed from (0, 0) to (0, 1)?
			nvgTransformPoint(&s.x, &s.y, inverse, 0, 0);
		nvgTransformPoint(&e.x, &e.y, inverse, 0, 1);


			NVGpaint paint;
		if (p->type == NSVG_PAINT_LINEAR_GRADIENT)
			paint = nvgLinearGradient(vg, s.x, s.y, e.x, e.y, icol, ocol);
		else
			paint = nvgRadialGradient(vg, s.x, s.y, 0.0, 160, icol, ocol);
		return paint;
	}

	void Renderer::DrawSvg(TransformComponent comp, const glm::mat4 transform, const SvgComponent& svg, float w, float h, const CameraComponent& camera)
	{
		nvgBeginFrame(m_nvg, float(w), float(h), 1.0f);
		nvgSave(m_nvg);  // Save the current transformation state
		// Move the image by dx and dy
		if(comp.Scissors.z != 0)
		nvgScissor(m_nvg,comp.Scissors.x, comp.Scissors.y, comp.Scissors.z, comp.Scissors.w);
		//to center
		auto transformationMatrix = transform;
		glm::vec3 translation = glm::vec3(transformationMatrix[3]);

		// Extract rotation
		glm::mat3 rotationMatrix = glm::mat3(transformationMatrix);
		glm::quat rotation = glm::quat_cast(rotationMatrix);

		// Extract scale using eigen decomposition
		glm::mat3 m = glm::mat3(transformationMatrix);
		glm::vec3 scale;
		glm::vec3 rotationOnlyScale = glm::vec3(
			glm::length(m[0]),
			glm::length(m[1]),
			glm::length(m[2])
		);

		//if (transformc.Scissors != glm::vec4{ 0, 0, 0, 0 })
			//nvgScissor(m_nvg, transformc.Scissors.x, transformc.Scissors.y, transformc.Scissors.z, transformc.Scissors.w);
		NSVGimage* svgi = static_cast<NSVGimage*>(svg.image);
		float xs = rotationOnlyScale.x * w / camera.width * 1 / svgi->width;
		float ys = rotationOnlyScale.y * h / camera.height * 1 / svgi->height;
		nvgScale(m_nvg, xs, ys);

		float xm = w/2 - xs * svgi->width / 2;
		float ym = h/2 - ys * svgi->height/2;
		nvgTranslate(m_nvg, xm/xs, ym/ys);

		float x = translation.x * (w/2) / camera.width;
		float y = translation.y * (-h/2) / camera.height;
		nvgTranslate(m_nvg, x / xs, y / ys);



		for (auto shape = svgi->shapes; shape != NULL; shape = shape->next) {

			if (shape->opacity < 1.0)
				nvgGlobalAlpha(m_nvg, shape->opacity);
			
			for (auto path = shape->paths; path != NULL; path = path->next) {
				nvgBeginPath(m_nvg);
				nvgMoveTo(m_nvg, path->pts[0] , path->pts[1]);
	

				for (int i = 0; i < path->npts - 1; i += 3) {

					float* p = &path->pts[i * 2];

					nvgBezierTo(m_nvg,
						p[2],
						p[3],
						p[4],
						p[5],
						p[6],
						p[7]);
				}
				
				if (path->closed)
					nvgClosePath(m_nvg);

				int crossings = 0;
				glm::vec2 p0 = glm::vec2(path->pts[0], path->pts[1]);
				glm::vec2 p1 = glm::vec2(path->bounds[0] - 1.0, path->bounds[1] - 1.0);
				// Iterate all other paths
				for (NSVGpath* path2 = shape->paths; path2; path2 = path2->next) {
					if (path2 == path)
						continue;

					// Iterate all lines on the path
					if (path2->npts < 4)
						continue;
					for (int i = 1; i < path2->npts + 3; i += 3) {
						float* p = &path2->pts[2 * i];
						// The previous point
						glm::vec2 p2 = glm::vec2(p[-2], p[-1]);
						// The current point
						glm::vec2 p3 = (i < path2->npts) ? glm::vec2(p[4], p[5]) : glm::vec2(path2->pts[0], path2->pts[1]);
						float crossing = getLineCrossing(p0, p1, p2, p3);
						float crossing2 = getLineCrossing(p2, p3, p0, p1);
						if (0.0 <= crossing && crossing < 1.0 && 0.0 <= crossing2) {
							crossings++;
						}
					}
				}

				if (crossings % 2 == 0)
					nvgPathWinding(m_nvg, NVG_SOLID);
				else
					nvgPathWinding(m_nvg, NVG_HOLE);

	
		

				// Fill shape
				if (shape->fill.type) {
					switch (shape->fill.type) {
					case NSVG_PAINT_COLOR: {
						NVGcolor color = getNVGColor(shape->fill.color);
						if (svg.fill_color.rgba[0] != 999) {
							color = svg.fill_color;
						}
						nvgFillColor(m_nvg, color);
		
					} break;
					case NSVG_PAINT_LINEAR_GRADIENT:
					case NSVG_PAINT_RADIAL_GRADIENT: {
						NSVGgradient* g = shape->fill.gradient;
						(void)g;
					
							for (int i = 0; i < g->nstops; i++) {
					
							}
						nvgFillPaint(m_nvg, getPaint(m_nvg, &shape->fill));
					} break;
					}
					nvgFill(m_nvg);
				}

				// Stroke shape
				if (shape->stroke.type) {
					nvgStrokeWidth(m_nvg, shape->strokeWidth);
					// strokeDashOffset, strokeDashArray, strokeDashCount not yet supported
					nvgLineCap(m_nvg, (NVGlineCap)shape->strokeLineCap);
					nvgLineJoin(m_nvg, (int)shape->strokeLineJoin);

					switch (shape->stroke.type) {
					case NSVG_PAINT_COLOR: {
						NVGcolor color = getNVGColor(shape->stroke.color);
						if (svg.stroke_color.a != 999) {
							color = svg.stroke_color;
						}
						nvgStrokeColor(m_nvg, color);
					} break;
					case NSVG_PAINT_LINEAR_GRADIENT: {
						// NSVGgradient *g = shape->stroke.gradient;
						// printf("		lin grad: %f\t%f\n", g->fx, g->fy);
					} break;
					}
					nvgStroke(m_nvg);
				}



			}

		}
		nvgRestore(m_nvg);
		nvgEndFrame(m_nvg);
	}


}