#pragma once
#include <string>
#include "spine/Animation.h"
#include "spine/AnimationState.h"
#include "spine/Event.h"
#include "spine/RegionAttachment.h"
#include "Asset/SpineAtlas.h"
#include "spine/SkeletonJson.h"
#include "spine/SkeletonData.h"

#include "spine/Skeleton.h"
#include "spine/AnimationStateData.h"
#include "spine/Bone.h"
#include "spine/BoneData.h"
#include "spine/Vector.h"
#include "spine/SkeletonClipping.h"

#include <spine/Extension.h>
#include <spine/TextureLoader.h>
#include <string>

#include "glm/glm.hpp"
#include "Renderer/Vertex.h"
namespace Terrasu {
	class Renderer;
	class SpineAnimation
	{
	public:
		SpineAnimation(const std::string& name);
		~SpineAnimation();

		void			Update(float timeElapsed);
		void			Render(Renderer& m_renderer);
		void			Change(const std::string& name);
		void			Play(const std::string& skin, const std::string& animation, bool looped);
		void			Stop();
		Terrasu::AssetManager* m_assetManager;
		void			OnAnimationEvent(spine::AnimationState* state, int trackIndex, int type, spine::Event* event, int loopCount);
		glm::mat4 transform;
		std::string		mName;
		spine::Skeleton* mSkeleton;
	private:
		spine::Animation* GetAnimation(const std::string& name) const;
		void			FillSlotVertices(PosUvTileVertex* points, float x, float y, spine::Slot* slot, spine::RegionAttachment* attachment);


		std::string		mCurrentAnimation;
		SpineAtlas* mAtlas;
		spine::AnimationState* mState;
		spine::AnimationStateData* mStateData;
	

		spine::SkeletonClipping clipper;
		spine::Vector<float> worldVertices;
		spine::Vector<unsigned short> quadIndices;
		Terrasu::Texture* texture;
		Terrasu::Material* material = nullptr;

		std::vector<Terrasu::PosUvTileVertex> vertexArray;

		bool			mPlaying;

		float mSpeed = 1;
		bool mFlipX = false;
	};
}