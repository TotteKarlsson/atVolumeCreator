#pragma hdrstop
#include "atRenderService.h"
#include <System.Classes.hpp>
#include <IdHTTP.hpp>
#include <sstream>
#include <vector>
#include "mtkStringUtils.h"
#include "mtkVCLUtils.h"

//---------------------------------------------------------------------------

using namespace std;
using namespace mtk;


RenderService::RenderService(Idhttp::TIdHTTP* c,  const string& baseURL, const string& owner,
                            const string& project, const string& stack,
                            const string& imageType, int z, RenderBox& box, double scale)
:
mC(c),
mBaseURL(baseURL),
mOwner(owner),
mProject(project),
mStackName(stack),
mImageType(imageType),
mZ(z),
mRenderBox(box),
mScale(scale)
{
	mImageMemory = new TMemoryStream();
}

RenderService::~RenderService()

{
	delete mImageMemory;
}

TMemoryStream* RenderService::getImage()
{
	//First check if we already is having this data
    if(checkCacheForCurrentURL())
    {
    	mImageMemory->LoadFromFile(getFileForCurrentURL().c_str());
    }
    else
    {
    	mC->Get(getURLC(), mImageMemory);

        //Save to cache (in a thread)
        if(createFolder(getFilePath(getFileForCurrentURL())))
        {
            mImageMemory->SaveToFile(getFileForCurrentURL().c_str());
        }
    }

    return mImageMemory;
}

string RenderService::getFileForCurrentURL()
{
    vector<string> cachePaths = splitStringAtWord(getURL(), "/owner/");
    string currentPath;
    if(cachePaths.size() == 2)
    {
		string fldr = substitute(cachePaths[1],"/","\\\\");
		return joinPath("p:\\cache", fldr, "image.tiff");
    }

   	return currentPath;
}

bool RenderService::checkCacheForCurrentURL()
{
	return fileExists(getFileForCurrentURL());
}

void RenderService::clearImageMemory()
{
	delete mImageMemory;
    mImageMemory = NULL;

}

string RenderService::getURL()
{
	stringstream sUrl;//("http://ibs-forrestc-ux1.corp.alleninstitute.org:8080/render-ws/v1/owner/Sharmishtaas/project/M259292_Scnn1aTg2_1/stack/{0}/
    //z/{1}/box/5000,9000,1300,1300,{2}/tiff-image");

    sUrl << mBaseURL;
    sUrl << "/" << mOwner;
	sUrl << "/project/";
    sUrl << mProject;
    sUrl << "/stack/"<<mStackName;
    sUrl << "/z/"<<mZ;
    sUrl << "/box/"<<mRenderBox.X<<","<<mRenderBox.Y;
    sUrl << "," << mRenderBox.Width << ","<<mRenderBox.Height << ","<<mScale;
    sUrl << "/tiff-image";
	return sUrl.str();
}
