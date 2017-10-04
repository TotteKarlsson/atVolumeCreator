#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkLogger.h"
#include "mtkVCLUtils.h"

using namespace mtk;

void __fastcall addRenderProject(TTreeNode* vcNode, RenderProject* rp, TTreeView* tv)
{
    if(!vcNode)
    {
    	Log(lError) <<"Node is NULL";
        return;
    }
    int nrOfChilds(0);

	TTreeNode* n = tv->Items->AddChild(vcNode, "");
    n->Text = "Render Project " + IntToStr(n->Index + 1);
	tv->Items->GetFirstNode()->Expand(true);
    tv->Select(n);

    VolumeCreatorProject* vcp = (VolumeCreatorProject*) vcNode->Data;
    if(vcp)
    {
	    tv->Items->AddObject(NULL, vcp->getProjectName().c_str(), (void*) vcp);

        //Add RenderProject as a child
    	RenderProject* rp = new RenderProject("", "" , "");
        vcp->addChild(rp);
    	vcp->setModified();
    }

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::NewProjectAExecute(TObject *Sender)
{
    if(mVCProject)
    {
        if(closeProject() != mrOk)
        {
        	return;
        }
    }

    mVCProject = createNewProject();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::AddRenderProjectExecute(TObject *Sender)
{
	//Create a render project and associate with current VC project
    TTreeNode* vcNode = ProjectTView->Items->GetFirstNode();
  	RenderProject* rp = new RenderProject("", "" , "");
	addRenderProject(vcNode, rp, ProjectTView);
}

VolumeCreatorProject* __fastcall TMainForm::createNewProject()
{

	//Check how many main nodes
    int nrOfVCPs = ProjectTView->Items->Count;

	string pName = "VC Project " + mtk::toString(nrOfVCPs);
	VolumeCreatorProject* vcp = new VolumeCreatorProject(pName);

    Log(lInfo) << "Created a new VolumeCreator project";
    return vcp;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ProjectStatusTimerTimer(TObject *Sender)
{

	if(mVCProject)
    {
       	SaveProjectA->Enabled = mVCProject->isModified() ? true : false;
    }
    else
    {
		SaveProjectA->Enabled = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FileOpen1Accept(TObject *Sender)
{
    string f(stdstr(FileOpen1->Dialog->FileName));
    if(mVCProject && mVCProject->isOpen())
    {
		if(closeProject() == mrOk)
        {
        	mVCProject->close();
        }
    }

    if(!mVCProject)
	{
		mVCProject = createNewProject();
    }

	if(mVCProject->loadFromFile(f))
    {
	    ProjFileLbl->Caption = string("Project File: " + f).c_str();
    	Log(lInfo) << "Loaded project file: "<<f;
        mVCProject->open();
    }

    ProjectTView->Items->AddObject(NULL, mVCProject->getProjectName().c_str(), (void*) mVCProject);
}

//---------------------------------------------------------------------------
int __fastcall TMainForm::closeProject()
{
	if(saveProject() == mrOk)
    {
        mVCProject->close();
        Log(lInfo) << "Closed project: "<<mVCProject->getFileName();
   	    ProjFileLbl->Caption = string("Project File: None").c_str();
        ProjectTView->Items->Clear();
        delete mVCProject;
        mVCProject = NULL;
        return mrOk;
    }
    else
    {
    	return mrCancel;
    }
}

int __fastcall TMainForm::saveProjectAs()
{
    //Set filename and filePath
    SaveDialog1->Execute();
    if(SaveDialog1->FileName.Length())
    {
        string fName = stdstr(SaveDialog1->FileName);

        if(fileExists(fName))
        {
            if(MessageDlg("Overwrite file?", mtWarning, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrCancel)
            {
                return mrCancel;
            }
        }
        mVCProject->setFileName(fName);
        mVCProject->save();
        Log(lInfo) << "Saved project: "<<mVCProject->getFileName();
	    ProjFileLbl->Caption = string("Project File: " + fName).c_str();
        return mrOk;
    }
    else
    {
        return mrCancel;
    }

    return mrOk;
}

int __fastcall TMainForm::saveProject()
{
	//If project don't have an assigned filename, open filesavefile dialog
    if(mVCProject && mVCProject->isNeverSaved())
    {
    	int res = MessageDlg("Save Project?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo<<mbCancel, 0);
        if(res == mrYes)
        {
        	return saveProjectAs();
        }
        else if(res == mrCancel)
        {
        	return mrCancel;
        }
    }
    else if(mVCProject)
    {
		mVCProject->save();
        Log(lInfo) << "Saved project: "<<mVCProject->getFileName();
        return mrOk;
    }
	return mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseProjectAExecute(TObject *Sender)
{
	closeProject();
}

void __fastcall TMainForm::CloseProjectAUpdate(TObject *Sender)
{
   	CloseProjectA->Enabled = mVCProject ? true : false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveProjectAsAUpdate(TObject *Sender)
{
	SaveProjectAsA->Enabled = mVCProject ? true : false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveProjectAUpdate(TObject *Sender)
{
	SaveProjectA->Enabled = (mVCProject && mVCProject->isModified()) ? true : false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveProjectAExecute(TObject *Sender)
{
	saveProject();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveProjectAsAExecute(TObject *Sender)
{
	saveProjectAs();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ProjectTViewContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled)
{
	if(ProjectTView->GetNodeAt(MousePos.X, MousePos.Y) == ProjectTView->TopItem)
    {
    	Handled = false;
    }
    else
    {
    	Handled = true;
    }

    if(ProjectTView->Items->GetFirstNode() == NULL)
    {
    	Handled = true;
    }
}


