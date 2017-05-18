#pragma hdrstop
#include "atROIHistory.h"
//---------------------------------------------------------------------------


void ROIHistory::clear()
{
	mHistory.clear();
}

void ROIHistory::add(RenderBox& rb)
{
	mHistory.push_back(rb);
    mCurrent = mHistory.end();
    mCurrent--;
}

RenderBox* ROIHistory::current()
{
	if(mCurrent != mHistory.end())
    {
    	return &(*(mCurrent));
    }
    return NULL;
}

RenderBox* ROIHistory::first()
{
	return  mHistory.size() ? &(mHistory.front()) : NULL;
}

RenderBox* ROIHistory::last()
{
	return  mHistory.size() ? &(mHistory.back()) : NULL;
}

RenderBox* ROIHistory::next()
{
    if(mCurrent != mHistory.end())
    {
        mCurrent++;
        if(mCurrent != mHistory.end())
        {
            return &(*(mCurrent));
        }
    }
    return NULL;
}

RenderBox* ROIHistory::previous()
{
    if(mCurrent != mHistory.begin())
    {
        mCurrent--;
        return &(*(mCurrent));
    }
    return NULL;

}

