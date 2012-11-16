///////////////////////////////////////////////////
// File:		MeshExporter.cpp
// Date:		20th August 2011
// Authors:		Matt Phillips
// Description:	ion Mesh exporter plugin for 3ds Max 2010
///////////////////////////////////////////////////

#include "MeshExporter.h"

static const int sVersion = 100;
static const char* sDescription = "ion::MeshExport - Mesh exporter for ion::engine";

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		// Hang on to this DLL's instance handle.
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}

	return(TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return sDescription;  
}

__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
  switch(i)
  {
    case 0:
		return &ion::toolchain::MeshExporter::sMeshExportClassDesc;
  }

  return 0;
}

__declspec( dllexport ) ULONG LibVersion()
{
	return Get3DSMAXVersion();
}

__declspec( dllexport ) int LibInitialize()
{
	// TODO: Perform initialization here.
	return TRUE;
}

__declspec( dllexport ) int LibShutdown()
{
	// TODO: Perform uninitialization here.
	return TRUE;
}

namespace ion
{
	namespace toolchain
	{
		MeshExportClassDesc MeshExporter::sMeshExportClassDesc;

		MeshExporter::MeshExporter()
		{

		}

		MeshExporter::~MeshExporter() 
		{

		}

		int MeshExporter::ExtCount()
		{
			// Returns the number of file name extensions supported by the plug-in.
			return 1;
		}

		const TCHAR *MeshExporter::Ext(int n)
		{		
			//#pragma message	(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
			return _T("ion.mesh");
		}

		const TCHAR *MeshExporter::LongDesc()
		{
			//#pragma message	(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\""))
			return _T(sDescription);
		}
	
		const TCHAR *MeshExporter::ShortDesc() 
		{			
			//#pragma message	(TODO("Return short ASCII description (i.e. \"Targa\")"))
			return _T("ion::mesh");
		}

		const TCHAR *MeshExporter::AuthorName()
		{			
			//#pragma message	(TODO("Return ASCII Author name"))
			return _T("Matt Phillips");
		}

		const TCHAR *MeshExporter::CopyrightMessage() 
		{	
			//#pragma message	(TODO("Return ASCII Copyright message"))
			return _T("(c) Matt Phillips 2011");
		}

		const TCHAR *MeshExporter::OtherMessage1() 
		{		
			//TODO: Return Other message #1 if any
			return _T("");
		}

		const TCHAR *MeshExporter::OtherMessage2() 
		{		
			//TODO: Return other message #2 in any
			return _T("");
		}

		unsigned int MeshExporter::Version()
		{				
			//#pragma message	(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
			return sVersion;
		}

		void MeshExporter::ShowAbout(HWND hWnd)
		{			
			// Optional
		}

		BOOL MeshExporter::SupportsOptions(int ext, DWORD options)
		{
			// TODO Decide which options to support.  Simply return
			// true for each option supported by each Extension 
			// the exporter supports.

			return TRUE;
		}

		int MeshExporter::GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
		{
			int i;
			if (i_currentNode == NULL)
			{
				i_currentNode = GetCOREInterface()->GetRootNode();
			}
			else // IGame will crash 3ds Max if it is initialised with the root node.
			{
				i_nodeTab.AppendNode(i_currentNode);
			}
			for (i = 0; i < i_currentNode->NumberOfChildren(); i++)
			{
				GetSceneNodes(i_nodeTab, i_currentNode->GetChildNode(i));
			}
			return i_nodeTab.Count();
		}


		int	MeshExporter::DoExport(const TCHAR* name,ExpInterface* ei,Interface* i, BOOL suppressPrompts, DWORD options)
		{
			//Get IGame scene interface
			IGameScene *sceneInterface = GetIGameInterface();

			//Setup OpenGL coodinate system
			IGameConversionManager *convManager = GetConversionManager();
			convManager->SetCoordSystem(IGameConversionManager::IGAME_OGL);

			//Initialise IGame scene interface
			sceneInterface->InitialiseIGame(true);
			sceneInterface->SetStaticFrame(0);

			//Meshes and bones
			ion::renderer::Mesh ionMesh;
			Tab<IGameNode*> maxMeshes = sceneInterface->GetIGameNodeByType(IGameObject::IGAME_MESH);
			Tab<IGameNode*> maxBones = sceneInterface->GetIGameNodeByType(IGameObject::IGAME_BONE);

			//Retrieve meshes
			for (int i = 0; i < maxMeshes.Count(); i++)
			{
				IGameNode* maxNode = maxMeshes[i];
				IGameObject* maxObject = maxNode->GetIGameObject();
				if(maxObject->IsRenderable() && maxObject->GetIGameType() != IGameObject::IGAME_BONE)
				{
					//Convert to ion mesh
					ConvertMesh(maxNode, &ionMesh);
				}
			}

			//Retrieve bones
			/*
			for (int i = 0; i < maxBones.Count(); i++)
			{
				SYM::SYM_BONE Bone = MaxToSymBone(Bones[i]);
				if(Bone.ID >= 0) SymMesh.Bones[Bone.ID] = Bone;
			}
			*/

			std::stringstream debugStream;

			std::list<std::list<ion::renderer::Mesh::SubMesh*>>& ionSubMeshes = ionMesh.GetSubMeshes();
			if(ionSubMeshes.size() > 0)
			{
				//Write the file, get filesize
				CStr filename(name);	
				filename.toLower();
				u64 fileSize = ionMesh.Save(std::string(filename));

				//Generate message
				debugStream << "Exported ion::mesh:\n\n";

				//Error check
				if(fileSize > 0)
				{
					for(std::list<std::list<ion::renderer::Mesh::SubMesh*>>::iterator subMesh = ionSubMeshes.begin(), subMeshEnd = ionSubMeshes.end(); subMesh != subMeshEnd; ++subMesh)
					{
						for(std::list<ion::renderer::Mesh::SubMesh*>::iterator lod = subMesh->begin(), lodEnd = subMesh->end(); lod != lodEnd; lod++)
						{
							debugStream	<< "Submesh name: "		<< (*lod)->GetName()
										<< "\t\tMaterial name: " << (*lod)->GetMaterialName().c_str()
										<< "\t\tVertices: "		<< (*lod)->GetNumVertices()
										<< "\t\tFaces: "		<< (*lod)->GetNumFaces()
										<< "\n";
						}
					}

					/*
					for (std::map<int, SYM::SYM_BONE>::iterator i = SymMesh.Bones.begin(); i != SymMesh.Bones.end(); i++)
					{
						Stream	<< "Bone name: " << (*i).second.Name.c_str()
								<< "\tBone ID: " << (*i).first
								<< "\tParent ID: " << (*i).second.ParentID << "\n";
				
					*/

					debugStream << "\nMesh filesize: " << fileSize << "\n\n\n";
				}
				else
				{
					MessageBox(NULL, "Could not write mesh", "Error", MB_OK);
					debugStream << "***ERROR EXPORTING MESH: " << name << "\n\n";
				}
			}
			else
			{
				debugStream << "Could not export - no meshes selected";
			}

			MessageBox(NULL, debugStream.str().c_str(), "ion::mesh exporter", MB_OK);

			return TRUE;
		}

		void MeshExporter::ConvertMesh(IGameNode* maxNode, ion::renderer::Mesh* ionMesh)
		{
			IGameObject* maxObject = maxNode->GetIGameObject();
			IGameMesh* maxMesh = (IGameMesh*)maxObject;

			//Initialise face data
			maxMesh->InitializeData();

			if(maxMesh->GetNumberOfFaces() > 0)
			{
				//Create ion submesh
				ion::renderer::Mesh::SubMesh* ionSubMesh = ionMesh->CreateSubMesh();

				//Set name
				ionSubMesh->SetName(maxNode->GetName());

				//Set material
				IGameMaterial* material = maxMesh->GetMaterialFromFace(maxMesh->GetFace(0));
				if(material)
				{
					ionSubMesh->SetMaterialName(material->GetMaterialName());
				}

				int topIndex = 0;

				//Get all smoothing groups
				Tab<DWORD> smoothingGroups = maxMesh->GetActiveSmgrps();

				for(int i = 0; i < smoothingGroups.Count(); i++)
				{
					Tab<FaceEx*> maxFaces = maxMesh->GetFacesFromSmgrp(smoothingGroups[i]);

					if(maxFaces.Count() > 0)
					{
						//Normals array
						std::vector<ion::Vector3> ionNormals;
						ionNormals.resize(maxMesh->GetNumberOfVerts() * 3);

						//Max -> ion index map
						std::vector<int> ionIndexMap;
						ionIndexMap.resize(maxMesh->GetNumberOfVerts() * 3);
						for(int index = 0; index < ionIndexMap.size(); index++)
						{
							ionIndexMap[index] = -1;
						}

						std::vector<ion::renderer::Vertex> ionVertices;
						std::vector<ion::renderer::Face> ionFaces;

						//Collect all vertices used by this smoothing group, and reshuffle indices to match the new vertex array
						for(int j = 0; j < maxFaces.Count(); j++)
						{
							ion::renderer::Face ionFace;

							for(int k = 0; k < 3; k++)
							{
								//Get original index and vertex
								int maxIndex = maxFaces[j]->vert[k];
								Point3 maxVertex = maxMesh->GetVertex(maxIndex);

								//If this vertex hasn't been used
								if(ionIndexMap[maxIndex] == -1)
								{
									//Add it to the new vertex array
									ionVertices.push_back(ion::renderer::Vertex(maxVertex.x, maxVertex.y, maxVertex.z));

									//Set ion index
									ionIndexMap[maxIndex] = ionVertices.size() - 1;
								}

								//Set face index
								ionFace.mIndices[k] = ionIndexMap[maxIndex] + topIndex;

								//Add normal
								Point3 normal = maxMesh->GetNormal(maxFaces[j]->meshFaceIndex, k, true);
								ionNormals[ionIndexMap[maxIndex]] += ion::Vector3(normal.x, normal.y, normal.z);
							}

							//Add face
							ionFaces.push_back(ionFace);
						}

						topIndex += ionVertices.size();

						//Add vertices
						for(int j = 0; j < ionVertices.size(); j++)
						{
							ionSubMesh->AddVertex(ionVertices[j]);
						}

						//Add normals
						for(int j = 0; j < ionVertices.size(); j++)
						{
							ionSubMesh->AddNormal(ionNormals[j].Normalise());
						}

						//Add faces
						for(int j = 0; j < ionFaces.size(); j++)
						{
							ionSubMesh->AddFace(ionFaces[j]);
						}
					}
				}
			}

			/*
			//Loop through modifiers
			for (int i = 0; i < Object->GetNumModifiers(); i++)
			{
				IGameModifier *Modifier = Object->GetIGameModifier(i);

				//Find the skin modifier
				if (Modifier->IsSkin())
				{
					IGameSkin *Skin = (IGameSkin*) Modifier;
			
					//Export skinned verts
					for (int v = 0; v < Skin->GetNumOfSkinnedVerts(); v++)
					{
						int BoneCount = 0;

						for (int b = 0; b < Skin->GetNumberOfBones(v); b++)
						{
							if(b < 4)
							{
								Subset.Vertices[v].BoneIDs[BoneCount] = Skin->GetBoneID(v, b);
								Subset.Vertices[v].Weights[BoneCount] = Skin->GetWeight(v, b);
								BoneCount++;
							}
							else
							{
								static bool UserWarned = false;

								//Bone-to-vertex count exceeded
								if(!UserWarned) MessageBox(NULL, "Warning: Max bones per vertex exceeded, skinning data will not be exported correctly", "Symmetry Mesh Exporter", MB_OK);

								UserWarned = true;
							}
						}
					}
				}
			}
			*/
		}

		/*
		SYM::SYM_BONE MaxToSymBone(IGameNode *Node)
		{
			//Retrieve matrix
			Matrix3 Matrix = Node->GetWorldTM().ExtractMatrix3();

			//Get position
			Point3 Position = Matrix.GetTrans();

			//Get quaternion
			Quat Rotation(Matrix);

			//New bone
			SYM::SYM_BONE Bone;

			//Set position/rotation
			Bone.Position = SYM::SYM_VECTOR3(Position.x, Position.y, Position.z);
			Bone.Rotation = SYM::SYM_QUATERNION(Rotation.x, Rotation.y, Rotation.z, -Rotation.w);

			//Get ID
			Bone.ID = Node->GetNodeID();

			//Get name
			Bone.Name = Node->GetName();

			//Get parent bone
			if(Node->GetNodeParent())
			{
				IGameObject *ParentObj = Node->GetNodeParent()->GetIGameObject();
				if(ParentObj->GetIGameType() == IGameObject::IGAME_BONE) Bone.ParentID = Node->GetNodeParent()->GetNodeID();
			}

			//Return bone
			return Bone;
		}
		*/
	}
}