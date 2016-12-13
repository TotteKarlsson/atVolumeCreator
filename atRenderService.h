#ifndef atRenderServiceH
#define atRenderServiceH
#include <string>
//---------------------------------------------------------------------------

namespace Idhttp
{
class TIdHTTP;
}

namespace System { namespace Classes {
class TMemoryStream;
}}

using System::Classes::TMemoryStream;

using std::string;
class  RenderBox
{
	public:
	    RenderBox(int x = 0, int y = 0, int width = 0, int height = 0) : X(x), Y(y), Width(width), Height(height){}
	    int X;
    	int Y;
	    int Width;
    	int Height;
};

class RenderService
{
	public:
							            RenderService(Idhttp::TIdHTTP* c,
                        	            						const string& baseURL,
                        	            					  	const string& owner,
                        	            					  	const string& project,
                                                              	const string& stack,
                                                              	const string& imageType,
                                                              	int z,
                                                              	RenderBox& box,
                                                                double mScale);
							            ~RenderService();

		void				            clearImageMemory();
		string				            getURL();
		const char* 		            getURLC(){return getURL().c_str();}
        TMemoryStream*		            getImage(int z = 0);
        bool				            checkCacheForCurrentURL();
        string				            getImageLocalPathAndFileName();
        string							getProjectName();

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
};

#endif
