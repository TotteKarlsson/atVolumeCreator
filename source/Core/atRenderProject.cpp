#pragma hdrstop
#include "atRenderProject.h"
#include "mtkXMLUtils.h"
//---------------------------------------------------------------------------

using namespace mtk;



RenderProject::RenderProject(const string& owner, const string& project, const string& stack)
:
mOwner(owner),
mProject(project),
mStack(stack)
{
	mVCObjectType = (vcoRenderProject);
}

void RenderProject::setupForStack(const string& owner, const string& project, const string& stack)
{
	mOwner 		= owner;
    mProject 	= project;
    mStack 		= stack;
}


XMLElement* RenderProject::addToXMLDocumentAsChild(tinyxml2::XMLDocument& doc, XMLNode* docRoot)
{
    //Create XML for saving to file
    XMLElement* val = doc.NewElement("owner");
    val->SetText(mOwner.c_str());
    docRoot->InsertEndChild(val);

    val = doc.NewElement("project");
    val->SetText(mProject.c_str());
    docRoot->InsertEndChild(val);

    val = doc.NewElement("stack");
    val->SetText(mStack.c_str());
    docRoot->InsertEndChild(val);

    return val;
}

bool RenderProject::write()
{
//	if(mProcessSequence)
//    {
//    	return mProcessSequence->write();
//    }
	return false;
}

