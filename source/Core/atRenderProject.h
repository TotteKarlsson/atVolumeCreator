#ifndef atRenderProjectH
#define atRenderProjectH
#include <string>
#include <vector>
#include "atVolumeCreatorProject.h"
//---------------------------------------------------------------------------

using std::string;
using std::vector;

class RenderProject : public VolumeCreatorProject
{
	public:
			    	                    RenderProject(const string& owner, const string& project, const string& stack);
		void							setupForStack(const string& owner, const string& project, const string& stack);
		string							getProjectName(){return mProject;}
		string							getProjectOwner(){return mOwner;}
		string							getCurrentStackName(){return mStack;}

        								//!ProjectManagement
		virtual bool 	   	            write();
        virtual mtk::XMLElement*        addToXMLDocumentAsChild(mtk::XMLDocument& doc, mtk::XMLNode* docRoot);


    	string 		                    mOwner;
        string 		                    mProject;
        string                          mStack;
        vector<string>				   	mStacks;

	protected:

};


#endif
