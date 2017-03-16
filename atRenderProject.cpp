#pragma hdrstop
#include "atRenderProject.h"
//---------------------------------------------------------------------------
RenderProject::RenderProject(const string& owner, const string& project, const string& stack)
:
mOwner(owner),
mProject(project),
mStack(stack)
{}

void RenderProject::setupForStack(const string& owner, const string& project, const string& stack)
{
	mOwner = owner;
    mProject = project;
    mStack = stack;
}

