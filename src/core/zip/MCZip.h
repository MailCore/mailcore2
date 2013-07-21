#ifndef __MAILCORE_MCZIP_H_

#ifdef __cplusplus

#include <MailCore/MCMessageConstants.h>
#include <MailCore/MCBaseTypes.h>

namespace mailcore {
    ErrorCode CreateZipFileFromFolder(String * zipFilename, String * folder);
    
    String * CreateTemporaryZipFileFromFolder(String * folder);
    void RemoveTemporaryZipFile(String * zipFilename);
};

#endif

#endif
