#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include "atRenderClient.h"
#include "atROIHistory.h"

using namespace mtk;
extern string gAppName;
extern string gAppDataLocation;
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	this->Caption = vclstr(createWindowTitle("VolumeCreator", Application));
    this->DoubleBuffered = true;

	gLogger.setLogLevel(mLogLevel);
	mLogFileReader.start(true);
    mCurrentRB.setX1(mXCoordE->getValue());
    mCurrentRB.setY1(mYCoordE->getValue());
    mCurrentRB.setWidth(mWidthE->getValue());
    mCurrentRB.setHeight(mHeightE->getValue());
    mROIHistory.add(mOriginalRB);

    //Setup renderclient
    mRC.setBaseURL(mBaseUrlE->getValue());
    mRC.getProject().setupForStack(mOwnerE->getValue(), mProjectE->getValue(), mStackNameE->getValue());


    //Populate owners
    StringList o = mRC.getOwners();
    if(o.size())
    {
		populateDropDown(o, mOwnersCB);
    }

	//Select owner
    int index = mOwnersCB->Items->IndexOf(mOwnerE->Text);

    if(index > -1)
    {
		mOwnersCB->ItemIndex = index;
        mOwnersCB->OnChange(NULL);

        //Select last project
        index = mProjectsCB->Items->IndexOf(mProjectE->Text);
        if(index > -1)
        {
            mProjectsCB->ItemIndex = index;
            mProjectsCB->OnChange(NULL);

            //Then select last stack
            index = mStacksCB->Items->IndexOf(mStackNameE->Text);
            if(index > -1)
            {
                mStacksCB->ItemIndex = index;
                mStacksCB->OnChange(NULL);
            }
        }
    }


	enableDisableGroupBox(TestSSHGB, 		false);
	enableDisableGroupBox(StackGenerationGB,false);
	enableDisableGroupBox(PostProcessingGB, false);

    TabSheet5->TabVisible = false;
    TabSheet2->TabVisible = false;

    //Setup path for ssh
	TSSHFrame1->ScFileStorage->Path = vclstr(gAppDataLocation);
}

void TMainForm::setupIniFile()
{
	string fldr = getSpecialFolder(CSIDL_LOCAL_APPDATA);
	fldr =  joinPath(fldr, gAppName);

	if(!folderExists(fldr))
	{
		CreateDir(fldr.c_str());
	}

	mIniFileC->init(fldr);

	//For convenience and for option form, populate appProperties container
	mAppProperties.append(&mGeneralProperties);
}

bool TMainForm::setupAndReadIniParameters()
{
	mIniFileC->load();
	mGeneralProperties.setIniFile(mIniFileC->getIniFile());

	//Setup parameters
	mGeneralProperties.setSection("GENERAL");
	mGeneralProperties.add((BaseProperty*)  &mBottomPanelHeight.setup( 	            	"HEIGHT_OF_BOTTOM_PANEL",    	    205));
	mGeneralProperties.add((BaseProperty*)  &mLogLevel.setup( 	                    	"LOG_LEVEL",    	                lAny));

    mGeneralProperties.add((BaseProperty*)  &mBaseUrlE->getProperty()->setup(	        "BASE_URL", 	                    "http://ibs-forrestc-ux1.corp.alleninstitute.org:8081/render-ws/v1"));
    mGeneralProperties.add((BaseProperty*)  &mOwnerE->getProperty()->setup(		        "OWNER", 		                    "Sharmishtaas"));
    mGeneralProperties.add((BaseProperty*)  &mProjectE->getProperty()->setup(	        "PROJECT", 		                    "M270907_Scnn1aTg2Tdt_13"));
    mGeneralProperties.add((BaseProperty*)  &mStackNameE->getProperty()->setup(	        "STACK_NAME", 	                    "ALIGNEDSTACK_DEC12"));

    mGeneralProperties.add((BaseProperty*)  &mScaleE->getProperty()->setup(		        "SCALE", 			                0.02));
    mGeneralProperties.add((BaseProperty*)  &mXCoordE->getProperty()->setup(	        "VIEW_X_COORD",    	                0));
    mGeneralProperties.add((BaseProperty*)  &mYCoordE->getProperty()->setup(	        "VIEW_Y_COORD",    	                0));
    mGeneralProperties.add((BaseProperty*)  &mWidthE->getProperty()->setup(		        "VIEW_WIDTH", 		                0));
    mGeneralProperties.add((BaseProperty*)  &mHeightE->getProperty()->setup(	        "VIEW_HEIGHT", 		                0));
    mGeneralProperties.add((BaseProperty*)  &MinIntensity->getProperty()->setup(	    "MIN_INTENSITY", 		            0));
    mGeneralProperties.add((BaseProperty*)  &MaxIntensity->getProperty()->setup(	    "MAX_INTENSITY", 		            65535));

	mGeneralProperties.add((BaseProperty*)  &mImageCacheFolderE->getProperty()->setup(	"IMAGE_CACHE_FOLDER",  				"C:\\ImageCache"));

    //Stack Generation
	mGeneralProperties.add((BaseProperty*)  &VolumesFolder->getProperty()->setup(		"VOLUMES_ROOT_FOLDER",  	  		"/nas1/temp"));
	mGeneralProperties.add((BaseProperty*)  &SubFolder1->getProperty()->setup(			"VOLUMES_SUB_FOLDER_1",  	  		"temp"));
//	mGeneralProperties.add((BaseProperty*)  &SubFolder2->getProperty()->setup(			"VOLUMES_SUB_FOLDER_2",  	  		"temp"));
	mGeneralProperties.add((BaseProperty*)  &VolumesScaleE->getProperty()->setup(	   	"VOLUMES_SCALE",  	 		 		0.01));


	//Read from file. Create if file do not exist
	mGeneralProperties.read();

	//Update UI
    mBaseUrlE->update();
	mOwnerE->update();
	mProjectE->update();
    mStackNameE->update();
    mScaleE->update();
    mXCoordE->update();
    mYCoordE->update();
    mWidthE->update();
    mHeightE->update();
	MinIntensity->update();
	MaxIntensity->update();
	mImageCacheFolderE->update();
    VolumesFolder->update();
    SubFolder1->update();
//    SubFolder2->update();
    VolumesScaleE->update();
	mBottomPanel->Height = mBottomPanelHeight;
    return true;
}

