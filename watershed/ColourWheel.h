///////////////////////////////////////////////////
// File:		ColourWheel.h
// Date:		5th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - Colour wheel
///////////////////////////////////////////////////

#include "core/Colour.h"
#include "core/maths/Vector.h"
#include "renderer/Primitive.h"
#include "renderer/Scene.h"
#include "renderer/SceneNode.h"
#include "renderer/Material.h"

#include <list>

namespace ws
{
	class ColourWheel
	{
	public:
		class Entry
		{
		public:
			Entry(int id, const ion::Colour& colour)
			{
				mId = id;
				mColour = colour;
			}

			bool operator == (const Entry& rhs) const { return mId == rhs.mId; }

			int mId;
			ion::Colour mColour;
		};

		ColourWheel(ion::renderer::Scene& scene, const ion::Vector2& position, float outerRadius, float innerRadius);

		void AddEntry(Entry& entry);
		void RemoveEntry(Entry& entry);

		void Open();
		void Close();
		bool IsOpen() const;

		void SetSelectionAngle(float angle);
		const Entry& GetCurrentSelection();

	private:
		void Rebuild();

		float mOuterRadius;
		float mInnerRadius;

		ion::renderer::Material* mMaterial;
		ion::renderer::Primitive* mMesh;
		ion::renderer::SceneNode* mSceneNode;

		ion::renderer::Primitive* mSelectorMesh;
		ion::renderer::SceneNode* mSelectorSceneNode;

		std::list<Entry> mEntries;

		bool mIsOpen;
	};
}