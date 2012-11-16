///////////////////////////////////////////////////
// File:		SceneExporter.cpp
// Date:		22nd August 2011
// Authors:		Matt Phillips
// Description:	ion Scene exporter plugin for 3ds Max 2010
///////////////////////////////////////////////////

#include "SceneExporter.h"

static const int sVersion = 100;
static const char* sDescription = "ion::SceneExport - Scene exporter for ion::engine";

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
		return &ion::toolchain::SceneExporter::sSceneExportClassDesc;
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
		SceneExportClassDesc SceneExporter::sSceneExportClassDesc;

		SceneExporter::SceneExporter()
		{

		}

		SceneExporter::~SceneExporter() 
		{

		}

		int SceneExporter::ExtCount()
		{
			// Returns the number of file name extensions supported by the plug-in.
			return 1;
		}

		const TCHAR *SceneExporter::Ext(int n)
		{		
			//#pragma message	(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
			return _T("ion.scene");
		}

		const TCHAR *SceneExporter::LongDesc()
		{
			//#pragma message	(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\""))
			return _T(sDescription);
		}
	
		const TCHAR *SceneExporter::ShortDesc() 
		{			
			//#pragma message	(TODO("Return short ASCII description (i.e. \"Targa\")"))
			return _T("ion::scene");
		}

		const TCHAR *SceneExporter::AuthorName()
		{			
			//#pragma message	(TODO("Return ASCII Author name"))
			return _T("Matt Phillips");
		}

		const TCHAR *SceneExporter::CopyrightMessage() 
		{	
			//#pragma message	(TODO("Return ASCII Copyright message"))
			return _T("(c) Matt Phillips 2011");
		}

		const TCHAR *SceneExporter::OtherMessage1() 
		{		
			//TODO: Return Other message #1 if any
			return _T("");
		}

		const TCHAR *SceneExporter::OtherMessage2() 
		{		
			//TODO: Return other message #2 in any
			return _T("");
		}

		unsigned int SceneExporter::Version()
		{				
			//#pragma message	(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
			return sVersion;
		}

		void SceneExporter::ShowAbout(HWND hWnd)
		{			
			// Optional
		}

		BOOL SceneExporter::SupportsOptions(int ext, DWORD options)
		{
			// TODO Decide which options to support.  Simply return
			// true for each option supported by each Extension 
			// the exporter supports.

			return TRUE;
		}

		int SceneExporter::GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
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


		int	SceneExporter::DoExport(const TCHAR* name,ExpInterface* ei,Interface* i, BOOL suppressPrompts, DWORD options)
		{
			//Get IGame scene interface
			IGameScene *sceneInterface = GetIGameInterface();

			//Setup Direct3D coodinate system
			IGameConversionManager *convManager = GetConversionManager();
			convManager->SetCoordSystem(IGameConversionManager::IGAME_D3D);

			//Initialise IGame scene interface
			sceneInterface->InitialiseIGame(true);
			sceneInterface->SetStaticFrame(0);

			//ion scene
			ion::renderer::Scene ionScene;

			//If file already exists, open existing scene and clear mesh/light data
			CStr filename(name);	
			filename.toLower();
			if(io::FileExists(std::string(filename)))
			{
				MessageBox(NULL, "Scene file already exists - only mesh and light data will be overwritten, other file contents will be preserved", "ion::scene exporter", MB_OK);

				//Clear all submeshes
				(*ionScene.GetSceneMesh()->GetSubMeshes().begin()).clear();

				//Clear all lights
				ionScene.GetLights().clear();
			}

			//Get meshes and lights
			Tab<IGameNode*> maxMeshes = sceneInterface->GetIGameNodeByType(IGameObject::IGAME_MESH);
			Tab<IGameNode*> maxLights = sceneInterface->GetIGameNodeByType(IGameObject::IGAME_LIGHT);

			//Retrieve meshes
			for (int i = 0; i < maxMeshes.Count(); i++)
			{
				IGameNode* maxNode = maxMeshes[i];
				IGameObject* maxObject = maxNode->GetIGameObject();
				if(maxObject->IsRenderable() && maxObject->GetIGameType() != IGameObject::IGAME_BONE)
				{
					//Convert to ion SubMesh
					ConvertMesh(maxNode, ionScene.GetSceneMesh());
				}
			}

			//Retrieve lights
			for (int i = 0; i < maxLights.Count(); i++)
			{
				IGameNode* maxNode = maxLights[i];
				IGameObject* maxObject = maxNode->GetIGameObject();

				//Create ion light
				ion::renderer::Light* ionLight = new ion::renderer::Light(ionScene);

				//Convert to ion Light
				ConvertLight(maxNode, ionLight);

				//Add to scene
				ionScene.AddLight(*ionLight);
			}

			std::stringstream debugStream;

			//Write the file, get filesize
			u64 fileSize = ionScene.Save(std::string(filename));

			//Generate message
			debugStream << "Exported ion::scene:\n\n";

			//Error check
			if(fileSize > 0)
			{
				debugStream << "Exported " << ionScene.GetSceneMesh()->GetSubMeshes().size() << " scene meshes\n";
				debugStream << "Exported " << ionScene.GetLights().size() << " lights\n\n";

				debugStream << "Scene filesize: " << fileSize << "\n\n\n";
			}
			else
			{
				MessageBox(NULL, "Could not write scene", "Error", MB_OK);
				debugStream << "***ERROR EXPORTING SCENE: " << name << "\n\n";
			}

			MessageBox(NULL, debugStream.str().c_str(), "ion::scene exporter", MB_OK);

			return TRUE;
		}

		void SceneExporter::ConvertMesh(IGameNode* maxNode, ion::renderer::Mesh* ionMesh)
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
		}

		void SceneExporter::ConvertLight(IGameNode* maxNode, ion::renderer::Light* ionLight)
		{
			IGameObject* maxObject = maxNode->GetIGameObject();
			IGameLight* maxLight = (IGameLight*)maxObject;

			//Initialise export data
			maxLight->InitializeData();

			//Get matrix
			GMatrix matrix = maxNode->GetWorldTM(0.0f);

			//Get position/direction
			Point3 position = matrix.Translation();
			Point3 direction = matrix.GetRow(2);
			ionLight->SetPosition(ion::Vector3(position.x, position.y, position.z));
			ionLight->SetDirection(ion::Vector3(direction.x, direction.y, direction.z));

			//Get properties
			ionLight->CastShadows(maxLight->CastShadows());

			float maxDistance = 100.0f;
			float constant = 50.0f;
			float falloff = 1.0f;

			if(maxLight->GetLightAttenEnd())
			{
				maxLight->GetLightAttenEnd()->GetPropertyValue(maxDistance, 0.0f);
			}

			if(maxLight->GetLightDecayStart())
			{
				maxLight->GetLightDecayStart()->GetPropertyValue(constant, 0.0f);
			}

			if(maxLight->GetLightFallOff())
			{
				maxLight->GetLightFallOff()->GetPropertyValue(falloff, 0.0f);
			}

			ionLight->SetAttenuation(maxDistance, constant, falloff);

			Point3 maxColour;
			if(maxLight->GetLightColor())
			{
				maxLight->GetLightColor()->GetPropertyValue(maxColour, 0.0f);
				ion::ColourRGB ionColour = ion::ColourRGB(maxColour.x, maxColour.y, maxColour.z);
				ionLight->SetDiffuse(ionColour);
				ionLight->SetSpecular(ionColour);
			}

			//Determine type
			IGameLight::LightType maxLightType = maxLight->GetLightType();
			ion::renderer::Light::Type ionLightType = ion::renderer::Light::Point;

			switch(maxLightType)
			{
			case IGameLight::IGAME_OMNI:
				ionLightType = ion::renderer::Light::Point;
				break;

			case IGameLight::IGAME_FSPOT:
			case IGameLight::IGAME_TSPOT:
				ionLightType = ion::renderer::Light::Spot;
				break;

			case IGameLight::IGAME_DIR:
			case IGameLight::IGAME_TDIR:
				ionLightType = ion::renderer::Light::Directional;
				break;
			}

			ionLight->SetType(ionLightType);
		}
	}
}