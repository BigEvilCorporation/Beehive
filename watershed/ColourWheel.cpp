///////////////////////////////////////////////////
// File:		ColourWheel.h
// Date:		5th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - Colour wheel
///////////////////////////////////////////////////

#include "ColourWheel.h"

namespace ws
{	
	ColourWheel::ColourWheel(ion::renderer::Scene& scene, const ion::Vector2& position, float outerRadius, float innerRadius)
	{
		mOuterRadius = outerRadius;
		mInnerRadius = innerRadius;
		mIsOpen = false;

		//Create material
		mMaterial = new ion::renderer::Material();

		//Assign vertex colours to ambient colour
		mMaterial->AssignVertexColour(ion::renderer::Material::Ambient);

		//Disable depth test, write, and culling
		mMaterial->SetDepthTest(false);
		mMaterial->SetDepthWrite(false);
		mMaterial->SetCullMode(ion::renderer::Material::None);

		//Create 2d mesh
		mMesh = new ion::renderer::Primitive(scene, ion::renderer::Primitive::Proj2D);

		//Create scene node
		mSceneNode = new ion::renderer::SceneNode(scene);
		mSceneNode->SetPosition(ion::Vector3(position.x, position.y, 0.0f));

		//Create selector
		mSelectorMesh = new ion::renderer::Primitive(scene, ion::renderer::Primitive::Proj2D);
		mSelectorSceneNode = new ion::renderer::SceneNode(scene);
		mSelectorSceneNode->SetPosition(ion::Vector3(position.x, position.y, 0.0f));

		mSelectorMesh->Begin(NULL, ion::renderer::Primitive::Line);
		mSelectorMesh->AddVertex(ion::renderer::Vertex(0.0f, 0.0f, 0.0f));
		mSelectorMesh->AddVertex(ion::renderer::Vertex(0.0f, outerRadius, 0.0f));
		mSelectorMesh->End();
	}

	void ColourWheel::AddEntry(Entry& entry)
	{
		mEntries.push_back(entry);
		Rebuild();
	}

	void ColourWheel::RemoveEntry(Entry& entry)
	{
		mEntries.remove(entry);
		Rebuild();
	}

	void ColourWheel::Open()
	{
		if(!IsOpen())
		{
			mSceneNode->Attach(*mMesh);
			mSelectorSceneNode->Attach(*mSelectorMesh);
			mIsOpen = true;
		}
	}

	void ColourWheel::Close()
	{
		if(IsOpen())
		{
			mSceneNode->Detach(*mMesh);
			mSelectorSceneNode->Detach(*mSelectorMesh);
			mIsOpen = false;
		}
	}

	bool ColourWheel::IsOpen() const
	{
		return mIsOpen;
	}

	void ColourWheel::SetSelectionAngle(float angle)
	{
		ion::Quaternion orientation(0.0f, 0.0f, 1.0f, ion::maths::DegreesToRadians(angle));
		mSelectorSceneNode->SetOrientation(orientation);
	}

	const ColourWheel::Entry& ColourWheel::GetCurrentSelection()
	{
		return *mEntries.begin();
	}

	void ColourWheel::Rebuild()
	{
		const int numSegments = 64;
		const float segmentLength = ion::maths::DegreesToRadians(360.0f) / (float)numSegments;

		//Build colour table
		std::vector<ion::Colour> colours;
		for(std::list<Entry>::iterator it = mEntries.begin(), end = mEntries.end(); it != end; ++it)
		{
			colours.push_back(it->mColour);
		}

		const int numColours = colours.size();
		const int numSegmentsPerColour = numSegments / numColours;

		//Clear primitive
		mMesh->Clear();

		//Begin constructing triangles
		mMesh->Begin(mMaterial, ion::renderer::Primitive::Triangle);

		//Outer ring of vertices
		for(int i = 0; i < numSegments; i++)
		{
			mMesh->AddVertex(ion::renderer::Vertex(
				sin(segmentLength * (float)i) * mOuterRadius,
				cos(segmentLength * (float)i) * mOuterRadius, 
				0.0f));

			mMesh->AddColour(colours[(int)((float)numColours * (float)i / (float)numSegments)]);
		}

		//Inner ring of vertices
		for(int i = 0; i < numSegments; i++)
		{
			mMesh->AddVertex(ion::renderer::Vertex(
				sin(segmentLength * (float)i) * mInnerRadius,
				cos(segmentLength * (float)i) * mInnerRadius,
				0.0f));

			mMesh->AddColour(colours[(int)((float)numColours * (float)i / (float)numSegments)]);
		}

		//Faces
		for(int i = 0; i < numSegments; i++)
		{
			mMesh->AddIndex(i);
			mMesh->AddIndex((i + 1) % numSegments);
			mMesh->AddIndex(i + numSegments);

			mMesh->AddIndex(i + 1);
			mMesh->AddIndex((i + numSegments + 1) % (numSegments * 2));
			mMesh->AddIndex(i + numSegments);
		}

		mMesh->End();
	}
}
