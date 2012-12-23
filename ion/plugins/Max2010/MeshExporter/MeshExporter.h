///////////////////////////////////////////////////
// File:		MeshExporter.h
// Date:		20th August 2011
// Authors:		Matt Phillips
// Description:	ion Mesh exporter plugin for 3ds Max 2010
///////////////////////////////////////////////////

#include "../../renderer/Mesh.h"

#include <Max.h>
#include <IGame/IGame.h>
#include <IGame/IGameExport.h>
#include <Igame/IgameModifier.h>
#include <IGame/IConversionManager.h>

#include <point2.h>
#include <point3.h>

#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>

#include <sstream>

#define IonMeshExport_CLASS_ID	Class_ID(0x6fe7cddc, 0x529c08b0)

//Extern'd for DLL entry
HINSTANCE hInstance;

namespace ion
{
	namespace toolchain
	{
		//Forward declaration
		class MeshExportClassDesc;

		class MeshExporter : public SceneExport
		{
		public:
			MeshExporter();
			~MeshExporter();

			static MeshExportClassDesc sMeshExportClassDesc;

			virtual int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

			virtual int				ExtCount();					// Number of extensions supported
			virtual const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
			virtual const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
			virtual const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
			virtual const TCHAR *	AuthorName();				// ASCII Author name
			virtual const TCHAR *	CopyrightMessage();			// ASCII Copyright message
			virtual const TCHAR *	OtherMessage1();			// Other message #1
			virtual const TCHAR *	OtherMessage2();			// Other message #2
			virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
			virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
			virtual BOOL SupportsOptions(int ext, DWORD options);

			void ConvertMesh(IGameNode* maxNode, ion::renderer::Mesh* ionMesh);

		protected:
			int GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode =NULL);

		private:
			static HWND hParams;
		};

		class MeshExportClassDesc : public ClassDesc2
		{
		public:
			virtual int IsPublic() 							{ return TRUE; }
			virtual void* Create(BOOL /*loading = FALSE*/) 	{ return new MeshExporter(); }
			virtual const TCHAR *	ClassName() 			{ return _T("ion::MeshExporter"); }
			virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
			virtual Class_ID ClassID() 						{ return IonMeshExport_CLASS_ID; }
			virtual const TCHAR* Category() 				{ return _T("Export"); }

			virtual const TCHAR* InternalName() 			{ return _T("ion::MeshExporter"); }	// returns fixed parsable name (scripter-visible name)
			virtual HINSTANCE HInstance() 					{ return hInstance; }				// returns owning module handle
		};
	}
}