#include "MCActiveSyncOptions.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncOptions::init()
{
    mProtocolVersions = NULL;
    mCommands = NULL;
}

ActiveSyncOptions::ActiveSyncOptions()
{
    init();
}

ActiveSyncOptions::~ActiveSyncOptions()
{
    MC_SAFE_RELEASE(mProtocolVersions);
    MC_SAFE_RELEASE(mCommands);
}

void ActiveSyncOptions::setProtocolVersions(Array * /* String */ value)
{
    MC_SET_OBJECT_FIELD(Array, mProtocolVersions, value);
}

Array * /* String */ ActiveSyncOptions::protocolVersions()
{
    MC_GET_OBJECT_FIELD(mProtocolVersions);
}

void ActiveSyncOptions::setCommands(Array * /* String */ value)
{
    MC_SET_OBJECT_FIELD(Array, mCommands, value);
}

Array * /* String */ ActiveSyncOptions::commands()
{
    MC_GET_OBJECT_FIELD(mCommands);
}

Object * ActiveSyncOptions::copy()
{
    ActiveSyncOptions * result = new ActiveSyncOptions();
    result->setProtocolVersions(protocolVersions());
    result->setCommands(commands());
    return result;
}

String * ActiveSyncOptions::description()
{
    return String::stringWithUTF8Format("<%s:%p>", className()->UTF8Characters(), this);
}
