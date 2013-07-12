//
//  MCMessageConstantsPrivate.h
//  mailcore2
//
//  Created by Paul Young on 10/07/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCMESSAGECONSTANTSPRIVATE_H_
#define __MAILCORE_MCMESSAGECONSTANTSPRIVATE_H_

#ifdef __cplusplus

namespace mailcore {
    
    typedef enum {
        IMAPMessageRenderingTypeHTML,
        IMAPMessageRenderingTypeHTMLBody,
        IMAPMessageRenderingTypePlainText,
        IMAPMessageRenderingTypePlainTextBody
    } IMAPMessageRenderingType;
    
}

#endif

#endif
