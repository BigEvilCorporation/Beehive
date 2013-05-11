#include "MeshExporter.h"
#include "core/File.h"
#include "renderer/Mesh.h"
#include "renderer/Skeleton.h"

#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnIkJoint.h> 
#include <maya/MFnSet.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin( obj, "Big Evil Corporation", "3.0", "Any");

	// Register the translator with the system
	return plugin.registerFileTranslator("ion::mesh export", "none", ion::toolchain::IonMeshExporter::creator, NULL, NULL);
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin( obj );
	return plugin.deregisterFileTranslator("ion::mesh export");
}

namespace ion
{
	namespace toolchain
	{
		void* IonMeshExporter::creator()
		{
			return new IonMeshExporter();
		}

		IonMeshExporter::IonMeshExporter()
		{
		}

		IonMeshExporter::~IonMeshExporter()
		{
		}

		bool IonMeshExporter::haveReadMethod() const
		{
			//Cannot read in files
			return false;
		}

		bool IonMeshExporter::haveWriteMethod() const
		{
			//Can write out files
			return true;
		}

		bool IonMeshExporter::haveNamespaceSupport() const
		{
			//No namespace support
			return false;
		}

		bool IonMeshExporter::haveReferenceMethod() const
		{
			//No reference method
			return false;
		}

		bool IonMeshExporter::allowMultipleFileOptimization() const
		{
			//No optimizations for selecting multiple files
			return false;
		}

		bool IonMeshExporter::canBeOpened() const
		{
			//Can't be opened
			return false;
		}

		MString IonMeshExporter::defaultExtension() const
		{
			return ".ion.mesh";
		}

		MPxFileTranslator::MFileKind IonMeshExporter::identifyFile(const MFileObject& filename, const char* buffer, short size) const
		{
			const char* name = filename.name().asChar();
			size_t nameLength = strlen(name);

			if((nameLength > 0) && !_stricmp(name + nameLength - 9, ".ion.mesh"))
				return kCouldBeMyFileType;
			else
				return kNotMyFileType;
		}

		MStatus IonMeshExporter::writer(const MFileObject& filename, const MString& optionsString, FileAccessMode mode)
		{
			//Create ion mesh
			ion::renderer::Mesh* ionMesh = new ion::renderer::Mesh();

			//Get list of currently selected objects
			MSelectionList selectedObjects;
			MGlobal::getActiveSelectionList(selectedObjects);

			//Iterate through all selected objects
			for(MItSelectionList objIterator(selectedObjects); !objIterator.isDone(); objIterator.next())
			{
				//Get object's path
				MDagPath objectPath;
				objIterator.getDagPath(objectPath);

				MItDag dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
				dagIterator.reset(objectPath.node(), MItDag::kDepthFirst, MFn::kInvalid);

				//Iterate through the object's DAG node hierarchy
				for( ; !dagIterator.isDone(); dagIterator.next())
				{
					//Get DAG node's path
					MDagPath dagPath;
					dagIterator.getPath(dagPath);

					//Get DAG function node
					MFnDagNode dagNode(dagPath);

					//If not an intermediate object, and isn't a transformed shape
					if(!dagNode.isIntermediateObject() && !dagPath.hasFn(MFn::kTransform))
					{
						//If object has a mesh
						if(dagPath.hasFn(MFn::kMesh))
						{
							//Create ion submesh
							ion::renderer::Mesh::SubMesh* ionSubMesh = ionMesh->CreateSubMesh();

							//Get Maya mesh
							MFnMesh fnMesh(dagPath);

							//Add vertices
							MFloatPointArray vertices;
							fnMesh.getPoints(vertices);

							for(int i = 0; i < vertices.length(); i++)
							{
								MFloatPoint vertex = vertices[i];
								ionSubMesh->AddVertex(ion::renderer::Vertex(vertex.x, vertex.y, vertex.z));
							}

							//Add faces
							for(MItMeshPolygon polyIterator(dagPath, MObject::kNullObj); !polyIterator.isDone(); polyIterator.next())
							{
								int numTriangles = 0;
								polyIterator.numTriangles(numTriangles);

								//Iterate through all triangles in polygon
								for(int i = 0; i < numTriangles; i++)
								{
									//Get triangle
									MPointArray points;
									MIntArray indices;
									polyIterator.getTriangle(i, points, indices);

									//Create ion face
									ion::renderer::Face ionFace;

									for(int j = 0; j < 3; j++)
									{
										//Set index
										ionFace.mIndices[j] = indices[j];

										//Set tex coord
										float2 texCoord;
										polyIterator.getUV(indices[j], texCoord);
										ionFace.mTexCoords[j] = ion::renderer::TexCoord(texCoord[0], texCoord[1]);
									}

									//Add face to submesh
									ionSubMesh->AddFace(ionFace);
								}
							}

							//Add normals
							MFloatVectorArray normals;
							fnMesh.getVertexNormals(false, normals);
							for(int i = 0; i < normals.length(); i++)
							{
								MFloatVector normal = normals[i];
								ionSubMesh->AddNormal(ion::Vector3(normal.x, normal.y, normal.z));
							}
						}
					}
				}
			}

			//Write to file
			MString fullPath = filename.fullName();
			const char* filename_cstr = fullPath.asChar();
			ionMesh->Save(filename_cstr);

			//Done with mesh
			delete ionMesh;

			return MS::kSuccess;
		}
	}
}