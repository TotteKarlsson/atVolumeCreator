#ifndef atProjectManagerH
#define atProjectManagerH
#include <vector>
//---------------------------------------------------------------------------
using std::vector;

//!The project manager class manages projects.
class VolumeCreatorProject;

namespace VCL
{
	namespace Comctrls
	{
		class TTreeView;
    }
}


class ProjectManager
{
	public:
    						                        ProjectManager(TTreeView& tv);
 		virtual		 		                        ~ProjectManager();
        bool										createNewProject();
        bool										selectFirst();
        bool										selectNext();
        bool										selectPrevious();
        bool										selectLast();
		bool 										selectNode(VolumeCreatorProject* vcp);
	private:
    	TTreeView*			                        ProjectTView;
        vector<VolumeCreatorProject*>			 	mVCProjects;
		vector<VolumeCreatorProject*>::iterator		mCurrentVCProject;

};

#endif
