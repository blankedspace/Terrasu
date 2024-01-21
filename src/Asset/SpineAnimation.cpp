#include "bgfx/bgfx.h"
#include "Renderer/Renderer.h"
#include "Asset/SpineAnimation.h"
#include "spine/Skin.h"
#include "spine/ClippingAttachment.h"
#include "spine/MeshAttachment.h"

#include <spine/Extension.h>


spine::SpineExtension* spine::getDefaultExtension() {
	return new spine::DefaultSpineExtension();
}


namespace Terrasu {

	SpineAnimation::SpineAnimation(const std::string& name) :
		mName(name), mAtlas(0), mState(0), mStateData(0), mSkeleton(0), mSpeed(1), mPlaying(false), mFlipX(false)
	{
		auto ass = Terrasu::AssetManager();
		mAtlas = new SpineAtlas(name, ass);// gAnimationHost.GetAtlas(mName);

		spine::SkeletonJson* skeletonJson = new spine::SkeletonJson(mAtlas->mAtlas);

		int length = 0;
		auto s = ass.ReadFileStr("Assets/" + name + ".json");
		length = s.size();

		spine::SkeletonData* skeletonData = skeletonJson->readSkeletonData(s.c_str());
		assert(skeletonData);
		delete skeletonJson;

		spine::Bone::setYDown(true);
		//worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);


		height = skeletonData->getHeight();
		mSkeleton = new spine::Skeleton(skeletonData);
		mStateData = new spine::AnimationStateData(skeletonData);
		mState = new spine::AnimationState(mStateData);
		mState->setRendererObject(this);
		auto Animation = mStateData->getSkeletonData()->findAnimation("walk");
		mState->setAnimation(0, Animation, true);

		spine::Skin skin("mix-and-match");
		skin.addSkin(skeletonData->findSkin("skin-base"));
		skin.addSkin(skeletonData->findSkin("nose/short"));
		skin.addSkin(skeletonData->findSkin("eyelids/girly"));
		skin.addSkin(skeletonData->findSkin("eyes/violet"));
		skin.addSkin(skeletonData->findSkin("hair/brown"));
		skin.addSkin(skeletonData->findSkin("clothes/hoodie-orange"));
		skin.addSkin(skeletonData->findSkin("legs/pants-jeans"));
		skin.addSkin(skeletonData->findSkin("accessories/bag"));
		skin.addSkin(skeletonData->findSkin("accessories/hat-red-yellow"));

		mSkeleton->setSkin(&skin);

		mSkeleton->setSlotsToSetupPose();
		mSkeleton->updateWorldTransform();

		mSkeleton->setSlotsToSetupPose();
		
		mSkeleton->setPosition(0, 0);
		mSkeleton->updateWorldTransform();

		mState->setAnimation(0, "idle", true);
		//spSkeleton_update(mSkeleton, 0);
		mPlaying = true;

		quadIndices.add(0);
		quadIndices.add(1);
		quadIndices.add(2);
		quadIndices.add(2);
		quadIndices.add(3);
		quadIndices.add(0);

		material == nullptr;

	}

	void SpineAnimation::Update(float timeElapsed)
	{
		if (mPlaying)
		{
			//mSkeleton.u
			//spSkeleton_update(mSkeleton, timeElapsed / 1000); // timeElapsed - ms, Spine использует время в секундах
			mState->update(timeElapsed / 1000);
			mState->apply(*mSkeleton);
			mSkeleton->updateWorldTransform();
		}
	}

	void SpineAnimation::Render(Terrasu::Renderer& m_renderer)
	{
		vertexArray.clear();
		if (mSkeleton->getColor().a == 0) return;

		Terrasu::PosUvTileVertex vertex;
		for (unsigned i = 0; i < mSkeleton->getSlots().size(); ++i) {
			spine::Slot& slot = *mSkeleton->getDrawOrder()[i];

			spine::Attachment* attachment = slot.getAttachment();
			if (!attachment) continue;
			// Early out if the slot color is 0 or the bone is not active
			if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
				clipper.clipEnd(slot);
				continue;
			}

			spine::Vector<float>* vertices = &worldVertices;
			spine::Vector<float>* uvs = NULL;
			spine::Vector<unsigned short>* indices = NULL;
			int indicesCount = 0;
			spine::Color* attachmentColor;

			if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
				spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;
				attachmentColor = &regionAttachment->getColor();

				// Early out if the slot color is 0
				if (attachmentColor->a == 0) {
					clipper.clipEnd(slot);
					continue;
				}

				worldVertices.setSize(8, 0);
				regionAttachment->computeWorldVertices(slot, worldVertices, 0, 2);
				uvs = &regionAttachment->getUVs();
				indices = &quadIndices;
				indicesCount = 6;
				texture = (Terrasu::Texture*)((spine::AtlasRegion*)regionAttachment->getRegion())->page->texture;

			}
			else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
				spine::MeshAttachment* mesh = (spine::MeshAttachment*)attachment;
				attachmentColor = &mesh->getColor();

				// Early out if the slot color is 0
				if (attachmentColor->a == 0) {
					clipper.clipEnd(slot);
					continue;
				}

				worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
				mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), worldVertices.buffer(), 0, 2);
				uvs = &mesh->getUVs();
				indices = &mesh->getTriangles();
				indicesCount = mesh->getTriangles().size();
				texture = (Terrasu::Texture*)((spine::AtlasRegion*)mesh->getRegion())->page->texture;

			}
			else if (attachment->getRTTI().isExactly(spine::ClippingAttachment::rtti)) {
				spine::ClippingAttachment* clip = (spine::ClippingAttachment*)slot.getAttachment();
				clipper.clipStart(slot, clip);
				continue;
			}
			else
				continue;

			uint8_t r = static_cast<uint8_t>(mSkeleton->getColor().r * slot.getColor().r * attachmentColor->r * 255);
			uint8_t g = static_cast<uint8_t>(mSkeleton->getColor().g * slot.getColor().g * attachmentColor->g * 255);
			uint8_t b = static_cast<uint8_t>(mSkeleton->getColor().b * slot.getColor().b * attachmentColor->b * 255);
			uint8_t a = static_cast<uint8_t>(mSkeleton->getColor().a * slot.getColor().a * attachmentColor->a * 255);



			if (clipper.isClipping()) {
				clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
				vertices = &clipper.getClippedVertices();
				uvs = &clipper.getClippedUVs();
				indices = &clipper.getClippedTriangles();
				indicesCount = clipper.getClippedTriangles().size();
			}


			for (int ii = 0; ii < indicesCount; ++ii) {
				int index = (*indices)[ii] << 1;
				vertex.m_x = (*vertices)[index] / height;
				vertex.m_y = (*vertices)[index + 1] / height;
				vertex.uv_x = (*uvs)[index];
				vertex.uv_y = (*uvs)[index + 1];
				vertex.m_xTile = 1;
				vertex.m_yTile = 1;
				vertexArray.push_back(vertex);
			}
			clipper.clipEnd(slot);


		}

		material = &m_assetManager->CreateMaterial("texturedQuad");
		m_renderer.DrawVerts(&vertexArray, transform, material);
		//target.draw(*vertexArray, states);
		clipper.clipEnd();

	}

	void SpineAnimation::Change(const std::string& name)
	{
		auto ass = Terrasu::AssetManager();
		auto s = ass.ReadFileStr("Assets/" + name + ".json");
		if(s == "")
		{
			return;
		}
		mAtlas = new SpineAtlas(name, ass);// gAnimationHost.GetAtlas(mName);

		spine::SkeletonJson* skeletonJson = new spine::SkeletonJson(mAtlas->mAtlas);
		spine::SkeletonData* skeletonData = skeletonJson->readSkeletonData(s.c_str());
		assert(skeletonData);
		delete skeletonJson;

		spine::Bone::setYDown(true);
		//worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);



		mSkeleton = new spine::Skeleton(skeletonData);
		mStateData = new spine::AnimationStateData(skeletonData);
		mState = new spine::AnimationState(mStateData);
		mState->setRendererObject(this);

		auto Animation = mStateData->getSkeletonData()->findAnimation("walk");
		mState->setAnimation(0, Animation, true);

		spine::Skin skin("mix-and-match");
		

		skin.addSkin(skeletonData->findSkin("nose/short"));
		skin.addSkin(skeletonData->findSkin("eyelids/girly"));
		skin.addSkin(skeletonData->findSkin("eyes/violet"));
		skin.addSkin(skeletonData->findSkin("hair/blue"));
		skin.addSkin(skeletonData->findSkin("clothes/hoodie-orange"));
		skin.addSkin(skeletonData->findSkin("legs/pants-jeans"));
		skin.addSkin(skeletonData->findSkin("accessories/bag"));
		skin.addSkin(skeletonData->findSkin("accessories/hat-red-yellow"));

		mSkeleton->setSkin(&skin);

		mSkeleton->setSlotsToSetupPose();
		mSkeleton->updateWorldTransform();

		mSkeleton->setSlotsToSetupPose();

		mSkeleton->setPosition(0, 0);
		mSkeleton->updateWorldTransform();

		mState->setAnimation(0, "dance", true);
		//spSkeleton_update(mSkeleton, 0);
		mPlaying = true;
	}

}