#ifndef atRenderClientH
#define atRenderClientH
#include <string>
#include "mtkStringList.h"
#include "mtkConstants.h"
#include <vector>

using std::vector;

//---------------------------------------------------------------------------

namespace Idhttp
{
	class TIdHTTP;
}

namespace System
{
	namespace Classes
	{
		class TMemoryStream;
	}
}

using System::Classes::TMemoryStream;
using mtk::StringList;
using mtk::gEmptyString;
using std::string;


class  RenderBox
{
	public:
	    RenderBox(int x = 0, int y = 0, int width = 0, int height = 0)
        : X(x), Y(y), Width(width), Height(height){}
	    int X;
    	int Y;
	    int Width;
    	int Height;
};

class RenderClient
{
	public:
							            RenderClient(Idhttp::TIdHTTP* c,
                        	            						const string& baseURL,
                        	            					  	const string& owner,
                        	            					  	const string& project,
                                                              	const string& stack,
                                                              	const string& imageType = "tiff-image",
                                                              	int z = 0,
                                                              	const RenderBox& box = RenderBox(),
                                                                double mScale = 0.01,
                                                                const string& cacheFolder = gEmptyString);
							            ~RenderClient();

		void				            clearImageMemory();
		string				            getURL();
		const char* 		            getURLC(){return getURL().c_str();}
        TMemoryStream*		            getImage(int z = 0);
        string							getURLForZ(int z);
        bool				            checkCacheForCurrentURL();
        string				            getImageLocalPathAndFileName();
        string							getProjectName();
        string							setLocalCacheFolder(const string& f);
        string							getLocalCacheFolder(){return mLocalCacheFolder;}
        StringList						getZs();
        vector<int>						getValidZs();
        vector<double>					getOptimalXYBoxForZs(const vector<int>& zs = vector<int>(0));

    private:
		Idhttp::TIdHTTP* 	            mC;
		TMemoryStream* 		            mImageMemory;
    	int				                mZ;
        double				            mScale;
        string			                mBaseURL;
        string			                mProject;
        string			                mOwner;
        string 			                mStackName;
        string 			                mImageType;
        RenderBox			            mRenderBox;
        string							mLocalCacheFolder;
        vector<double>					parseBoundsResponse(const string& s);
};

#endif
