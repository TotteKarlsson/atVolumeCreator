#ifndef atRemoteScripterH
#define atRemoteScripterH
#include "atATObject.h"
#include "dslStringList.h"
#include <string>
#include "ScSSHChannel.hpp"
//---------------------------------------------------------------------------

//!Class that sets up and run remote scripts through a ssh conenction
//!Remote script execution involve creataion of a full command string that
//!starts with a command, followed by a script name and script arguments

using dsl::StringList;
using std::string;

//---------------------------------------------------------------------------
namespace at
{

using Scsshchannel::TScSSHShell;

class RemoteScripter : public ATObject
{
        public:
                                                RemoteScripter(const string& scriptFileName, TScSSHShell* ssh = NULL);
                                                ~RemoteScripter();

            string 	                            createFullRemoteCommand();
            void	                            setRemoteCommand(const string& cmd);
            string                              getRemoteCommand(){return mRemoteCommand;}
            void                                useConnection(TScSSHShell* c){mSSHShell = c;}

            void                                addArgument(const string& argument);
            StringList                          getArguments();
            void                                run();
            void                                clearArguments();



        protected:

                                                //!The remote command is everyting before the actual script
            string                              mRemoteCommand;

            string                              mScriptName;

            StringList                          mArguments;

                                                //!The ssh connection
			TScSSHShell*						mSSHShell;
};

}
#endif
