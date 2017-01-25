#pragma hdrstop

#include <System.Classes.hpp>
#include <IdHTTP.hpp>
#include <sstream>
#include <vector>
#include "atRenderClient.h"
#include "mtkStringUtils.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace mtk;

const int HTTP_RESPONSE_OK = 200;

RenderClient::RenderClient(Idhttp::TIdHTTP* c,  const string& baseURL, const string& owner,
                            const string& project, const string& stack,
                            const string& imageType, int z, const RenderBox& box, double scale, const string& cacheFolder)
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

RenderClient::~RenderClient()
{
	delete mImageMemory;
}

string RenderClient::getProjectName()
{
	return mProject;
}

string RenderClient::setLocalCacheFolder(const string& f)
{
	mLocalCacheFolder = f;
}

TMemoryStream* RenderClient::getImage(int z)
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

vector<double> RenderClient::getOptimalXYBoxForZs(const vector<int>& zs)
{
	stringstream sUrl;
    sUrl << mBaseURL;
    sUrl << "/" << mOwner;
	sUrl << "/project/" << mProject;
    sUrl << "/stack/"<<mStackName<<"/z/0/bounds";

    Log(lInfo) << "Fetching from server using URL: "<<sUrl.str();

    double xMin(0), xMax(0), yMin(0), yMax(0);

    for(int z = 0; z < zs.size(); z++)
    {
        TStringStream* zstrings = new TStringStream;;
        mC->Get(sUrl.str().c_str(), zstrings);

        if( mC->ResponseCode == HTTP_RESPONSE_OK)
        {
            string s = stdstr(zstrings->DataString);
            vector<double> sBounds = parseBoundsResponse(s);

            if(z == 0)
            {
                xMin = sBounds[0];
                yMin = sBounds[1];
                xMax = sBounds[2];
                yMax = sBounds[3];
            }
            else
            {
                if(sBounds[0] < xMin)
                {
                    xMin = sBounds[0];
                }

                if(sBounds[1] < yMin)
                {
                    yMin = sBounds[1];
                }

                if(sBounds[0] > xMax)
                {
                    xMax = sBounds[2];
                }

                if(sBounds[1] > yMax)
                {
                    yMax = sBounds[3];
                }
            }

            Log(lInfo) << "Response: "<<s;
        }
        else
        {
            Log(lError) << "Failed fetching zs";
        }
    }

    vector<double> bounds(4);
    bounds[0] = xMin;
    bounds[1] = yMin;
    bounds[2] = xMax;
    bounds[3] = yMax;

    return bounds;
}

string RenderClient::getImageLocalPathAndFileName()
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

bool RenderClient::checkCacheForCurrentURL()
{
	return fileExists(getImageLocalPathAndFileName());
}

void RenderClient::clearImageMemory()
{
	delete mImageMemory;
    mImageMemory = NULL;
}

string RenderClient::getURLForZ(int z)
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

string RenderClient::getURL()
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

StringList RenderClient::getZs()
{
	StringList validZs = getValidZs();

    StringList missingZs;
    int nrOfMissingZs = 0;

    for(int i = 0; i < validZs.count(); i++)
    {
    	if(toInt(validZs[i]) != (i + nrOfMissingZs))
        {
        	missingZs.append(mtk::toString(i + nrOfMissingZs));
			nrOfMissingZs++;
        }
    }

    StringList all;
    all.append(validZs.asString(','));
	all.append(missingZs.asString(','));
	return all;
}

vector<int> RenderClient::getValidZs()
{
	StringList zs;
	stringstream sUrl;
    sUrl << mBaseURL;
    sUrl << "/" << mOwner;
	sUrl << "/project/" << mProject;
    sUrl << "/stack/"<<mStackName<<"/zValues";

    Log(lInfo) << "Fetching from server using URL: "<<sUrl.str();

    TStringStream* zstrings = new TStringStream;;
	mC->Get(sUrl.str().c_str(), zstrings);


    if( mC->ResponseCode == HTTP_RESPONSE_OK)
    {
        string s = stdstr(zstrings->DataString);
        Log(lInfo) << "Response: "<<s;
		s = stripCharacters("[]", s);
        zs.appendList(StringList(s,','));
    }
    else
    {
    	Log(lError) << "Failed fetching zs";
    }

	vector<int> zInts;
    for(int i = 0; i < zs.count(); i++)
    {
    	zInts.push_back(toInt(zs[i]));
    }

	return zInts;
}

vector<double> RenderClient::parseBoundsResponse(const string& _s)
{
	vector<double> bounds(4); //XminXMaxYMinYMax
    string s = stripCharacters("{}", _s);
    StringList l(s, ',');
    if(l.size() == 6)
    {
    	StringList xMin(l[0], ':');
    	StringList yMin(l[1], ':');
    	StringList xMax(l[3], ':');
    	StringList yMax(l[4], ':');

        bounds[0] = toDouble(xMin[1]);
        bounds[1] = toDouble(yMin[1]);
        bounds[2] = toDouble(xMax[1]);
        bounds[3] = toDouble(yMax[1]);
    }
    else
    {
    	Log(lError) << "Bad bounds format..";
    }
    return bounds;
}
