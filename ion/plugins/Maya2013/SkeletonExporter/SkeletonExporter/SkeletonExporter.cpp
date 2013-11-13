#define ION_PLUGIN

#include "SkeletonExporter.h"
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
#include <maya/MFnMatrixData.h>
#include <maya/MFnSet.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin( obj, "Big Evil Corporation", "3.0", "Any");

	// Register the translator with the system
	return plugin.registerFileTranslator("ion::skeleton export", "none", ion::toolchain::IonSkeletonExporter::creator, NULL, NULL);
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin( obj );
	return plugin.deregisterFileTranslator("ion::skeleton export");
}

namespace ion
{
	namespace toolchain
	{
		void* IonSkeletonExporter::creator()
		{
			return new IonSkeletonExporter();
		}

		IonSkeletonExporter::IonSkeletonExporter()
		{
		}

		IonSkeletonExporter::~IonSkeletonExporter()
		{
		}

		bool IonSkeletonExporter::haveReadMethod() const
		{
			//Cannot read in files
			return false;
		}

		bool IonSkeletonExporter::haveWriteMethod() const
		{
			//Can write out files
			return true;
		}

		bool IonSkeletonExporter::haveNamespaceSupport() const
		{
			//No namespace support
			return false;
		}

		bool IonSkeletonExporter::haveReferenceMethod() const
		{
			//No reference method
			return false;
		}

		bool IonSkeletonExporter::allowMultipleFileOptimization() const
		{
			//No optimizations for selecting multiple files
			return false;
		}

		bool IonSkeletonExporter::canBeOpened() const
		{
			//Can't be opened
			return false;
		}

		MString IonSkeletonExporter::defaultExtension() const
		{
			return ".ion.skeleton";
		}

		MPxFileTranslator::MFileKind IonSkeletonExporter::identifyFile(const MFileObject& filename, const char* buffer, short size) const
		{
			const char* name = filename.name().asChar();
			size_t nameLength = strlen(name);

			if((nameLength > 0) && !_stricmp(name + nameLength - 9, ".ion.skeleton"))
				return kCouldBeMyFileType;
			else
				return kNotMyFileType;
		}

		void GetMatrixFromPlug(MFnIkJoint& joint, ion::Matrix4& matrix, const char* plugName)
		{
			MPlug plug = joint.findPlug(plugName);

			//Get matrix object
			MObject matrixObject;
			plug.getValue(matrixObject);

			//Extract matrix
			MFnMatrixData matrixData(matrixObject);
			MMatrix mayaMatrix = matrixData.matrix();

			//Convert to ion matrix
			float rawMatrix[4][4] = {{0.0f}};
			mayaMatrix.get(rawMatrix);
			matrix = ion::Matrix4((float*)rawMatrix);
		}

		void GetFloat3FromPlug(MFnIkJoint& joint, ion::Vector3& vector, const char* plugName)
		{
			MPlug plug = joint.findPlug(plugName);

			//Get component plugs
			MPlug plugX = plug.child(0);
			MPlug plugY = plug.child(1);
			MPlug plugZ = plug.child(2);

			//Get floats
			plugX.getValue(vector.x);
			plugY.getValue(vector.y);
			plugZ.getValue(vector.z);
		}

		MStatus IonSkeletonExporter::writer(const MFileObject& filename, const MString& optionsString, FileAccessMode mode)
		{
			//Create ion skeleton
			ion::renderer::Skeleton* ionSkeleton = new ion::renderer::Skeleton();

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

					//If object has a skeleton joint
					if(dagPath.hasFn(MFn::kJoint))
					{
						//Get joint
						MFnIkJoint fnJoint(dagPath);

						//Get joint name
						MString jointName = fnJoint.name();

						//Joints must have names for serialization
						if(jointName.length() > 1)
						{
							//Create ion bone
							ion::renderer::Bone* ionBone = ionSkeleton->CreateBone(jointName.asChar());

							if(ionBone)
							{
								//Get joint's parent
								MObject parentObj = fnJoint.parent(0);

								MMatrix parentInvWorldBindMatrix = MMatrix::identity;
								//MMatrix mayaOrientMtx = MMatrix::identity;

								//Check if parent is another joint
								if(parentObj.hasFn(MFn::kJoint))
								{
									//Get joint
									MFnIkJoint fnParentJoint(parentObj);

									//Get inverse world bind matrix
									parentInvWorldBindMatrix = fnParentJoint.dagPath().inclusiveMatrixInverse();

									//Get parent's name
									MString parentName = fnParentJoint.name();

									if(parentName.length() > 1)
									{
										//Set ion bone's parent name
										ionBone->SetParentName(parentName.asChar());
									}
								}

								//Get world matrix
								MMatrix worldMatrix = dagPath.inclusiveMatrix();

								//Calc local matrix
								MMatrix localMatrix = worldMatrix * parentInvWorldBindMatrix;

								//Create ion matrices
								float rawMtx[4][4] = {{0}};
								worldMatrix.get(rawMtx);
								ion::Matrix4 ionWorldMatrix((float*)rawMtx);

								localMatrix.get(rawMtx);
								ion::Matrix4 ionLocalMatrix((float*)rawMtx);

								//Set world bind pose matrix
								ionBone->SetWorldBindPoseTransform(ionWorldMatrix);

								//Set local bind pose matrix
								ionBone->SetLocalBindPoseTransform(ionLocalMatrix);

								/*
								//Extract components from plugs
								ion::Vector3 translationVector;
								ion::Vector3 scaleVector;
								ion::Vector3 rotationEuler;
								ion::Vector3 jointOrientEuler;

								GetFloat3FromPlug(fnJoint, translationVector, "translate");
								GetFloat3FromPlug(fnJoint, scaleVector, "scale");
								GetFloat3FromPlug(fnJoint, rotationEuler, "rotate");
								GetFloat3FromPlug(fnJoint, jointOrientEuler, "jointOrient");

								//Convert to matrices
								ion::Matrix4 translationMatrix;
								ion::Matrix4 scaleMatrix;

								translationMatrix.SetTranslation(translationVector);
								scaleMatrix.SetScale(scaleVector);

								MEulerRotation mayaRotEuler(rotationEuler.x, rotationEuler.y, rotationEuler.z, MEulerRotation::kXYZ);
								MMatrix mayaRotMtx = mayaRotEuler.asMatrix();
								float rawMtx[4][4] = {{0}};
								mayaRotMtx.get(rawMtx);
								ion::Matrix4 rotationMatrix((float*)rawMtx);

								MEulerRotation mayaOrientEuler(jointOrientEuler.x, jointOrientEuler.y, jointOrientEuler.z, MEulerRotation::kXYZ);
								MMatrix mayaOrientMtx = mayaOrientEuler.asMatrix();
								mayaOrientMtx.get(rawMtx);
								ion::Matrix4 jointOrientMatrix((float*)rawMtx);

								//Calculate final local space matrix
								ion::Matrix4 localMatrix = rotationMatrix * jointOrientMatrix * translationMatrix * scaleMatrix;

								ionBone->SetLocalBindPoseTransform(localMatrix);
								*/
							}
						}
					}
				}
			}

			//Finalise skeleton (builds tree, calculates local bind pose transforms from world)
			ionSkeleton->Finalise();

			//Get filename
			MString fullPath = filename.fullName();
			const char* filenameCstr = fullPath.asChar();
			
			//Create and open file stream for writing
			ion::io::File file(filenameCstr, ion::io::File::OpenWrite);
			
			if(file.IsOpen())
			{
				//Create archive for serialising out
				ion::serialise::Archive archiveOut(file, ion::serialise::Archive::Out, ion::renderer::Skeleton::sSerialiseVersion);

				//Serialise
				archiveOut.Serialise(*ionSkeleton);

				//Close file
				file.Close();
			}

			//Done with skeleton
			delete ionSkeleton;

			return MS::kSuccess;
		}
	}
}