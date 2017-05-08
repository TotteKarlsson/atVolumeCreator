#ifndef atFetchImagesThreadH
#define atFetchImagesThreadH
#include "mtkThread.h"
#include "mtkStringList.h"
//---------------------------------------------------------------------------

using mtk::StringList;
using mtk::Thread;

class FetchImagesThread : public mtk::Thread
{
	public:
							FetchImagesThread(const StringList& urls = StringList(""), const string& cacheRoot = "p:\\cache");
		void				setup(const StringList& urls, const string& cacheFolder);
		virtual void        run();
        void				assignUrls(const StringList& urls);
		void				worker();
		bool				setCacheRoot(const string& cr);
	private:
    	StringList			mImageURLs;
        string				mCacheRootFolder;

};

#endif
