#ifndef atRenderProjectH
#define atRenderProjectH
#include <string>
#include <vector>
//---------------------------------------------------------------------------

using std::string;
using std::vector;
class RenderProject
{
	public:
			    	                    RenderProject(const string& owner, const string& project, const string& stack);
		void							setupForStack(const string& owner, const string& project, const string& stack);
		string							getProjectName(){return mProject;}
		string							getProjectOwner(){return mOwner;}
		string							getCurrentStackname(){return mStack;}

    	string 		                    mOwner;
        string 		                    mProject;
        string                          mStack;
        vector<string>				   	mStacks;


};


#endif
