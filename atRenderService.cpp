#pragma hdrstop
#include "atRenderService.h"
#include <System.Classes.hpp>
#include <IdHTTP.hpp>
#include <sstream>
#include <vector>
#include "mtkStringUtils.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace mtk;

RenderService::RenderService(Idhttp::TIdHTTP* c,  const string& baseURL, const string& owner,
                            const string& project, const string& stack,
                            const string& imageType, int z, RenderBox& box, double scale, const string& cacheFolder)
:
mC(c),
mBaseURL(baseURL),
mOwner(owner),
mProject(project),
mStackName(stack),
mImageType(imageType),
mZ(z),
mRenderBox(box),
mScale(scale),
mLocalCacheFolder(cacheFolder)
{
	mImageMemory = new TMemoryStream();
}

RenderService::~RenderService()
{
	delete mImageMemory;
}

string RenderService::getProjectName()
{
	return mProject;
}

string RenderService::setLocalCacheFolder(const string& f)
{
	mLocalCacheFolder = f;
}

TMemoryStream* RenderService::getImage(int z)
{
	mZ = z;

	if(!mImageMemory)
    {
		mImageMemory = new TMemoryStream();
    }
	//First check if we already is having this data
    if(checkCacheForCurrentURL())
    {
        Log(lInfo) << "Fetching from cache";
    	mImageMemory->LoadFromFile(getImageLocalPathAndFileName().c_str());
    }
    else
    {
        Log(lInfo) << "Fetching from server";
    	mC->Get(getURLC(), mImageMemory);

        //Save to cache (in a thread)
        if(createFolder(getFilePath(getImageLocalPathAndFileName())))
        {
            mImageMemory->SaveToFile(getImageLocalPathAndFileName().c_str());
        }
    }

    return mImageMemory;
}

string RenderService::getImageLocalPathAndFileName()
{
    vector<string> cachePaths = splitStringAtWord(getURL(), "/owner/");
    string currentPath;
    if(cachePaths.size() == 2)
    {
		string fldr = substitute(cachePaths[1],"/","\\\\");
		return joinPath(mLocalCacheFolder, fldr, "image.tiff");
    }

   	return currentPath;
}

bool RenderService::checkCacheForCurrentURL()
{
	return fileExists(getImageLocalPathAndFileName());
}

void RenderService::clearImageMemory()
{
	delete mImageMemory;
    mImageMemory = NULL;
}

string RenderService::getURLForZ(int z)
{
	stringstream sUrl;
    sUrl << mBaseURL;
    sUrl << "/" << mOwner;
	sUrl << "/project/" << mProject;
    sUrl << "/stack/"<<mStackName;
    sUrl << "/z/"<<z;
    sUrl << "/box/"<<mRenderBox.X<<","<<mRenderBox.Y << "," << mRenderBox.Width << ","<<mRenderBox.Height << ","<<mScale;
    sUrl << "/tiff-image";
	return sUrl.str();
}

string RenderService::getURL()
{
	//("http://ibs-forrestc-ux1.corp.alleninstitute.org:8080/render-ws/v1/owner/Sharmishtaas/project/M259292_Scnn1aTg2_1/stack/{0}/
    //z/{1}/box/5000,9000,1300,1300,{2}/tiff-image");
	stringstream sUrl;
    sUrl << mBaseURL;
    sUrl << "/" << mOwner;
	sUrl << "/project/" << mProject;
    sUrl << "/stack/"<<mStackName;
    sUrl << "/z/"<<mZ;
    sUrl << "/box/"<<mRenderBox.X<<","<<mRenderBox.Y << "," << mRenderBox.Width << ","<<mRenderBox.Height << ","<<mScale;
    sUrl << "/tiff-image";
	return sUrl.str();
}
