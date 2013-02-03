//
//  MCDateFormatter.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __testUI__MCDateFormatter__
#define __testUI__MCDateFormatter__

#include <mailcore/MCBaseTypes.h>
#include <unicode/udat.h>

#ifdef __cplusplus

namespace mailcore {
    
    class String;
    
    enum DateFormatStyle {
        DateFormatStyleFull = UDAT_FULL,
        DateFormatStyleLong = UDAT_LONG,
        DateFormatStyleMedium = UDAT_MEDIUM,
        DateFormatStyleShort = UDAT_SHORT,
        DateFormatStyleNone = UDAT_NONE,
    };
    
    class DateFormatter : public Object {
    public:
		DateFormatter();
		virtual ~DateFormatter();
		
		static DateFormatter * dateFormatter();
        
        virtual void setDateStyle(DateFormatStyle style);
        virtual DateFormatStyle dateStyle();
        
        virtual void setTimeStyle(DateFormatStyle style);
        virtual DateFormatStyle timeStyle();
        
        virtual void setLocale(String * locale);
        virtual String * locale();
        
        virtual void setTimezone(String * timezone);
        virtual String * timezone();
        
        virtual void setDateFormat(String * dateFormat);
        virtual String * dateFormat();
        
        virtual String * stringFromDate(time_t date);
        virtual time_t dateFromString(String * dateString);
        
    private:
        UDateFormat * mDateFormatter;
        DateFormatStyle mDateStyle;
        DateFormatStyle mTimeStyle;
        String * mDateFormat;
        String * mTimezone;
        String * mLocale;
        
        void prepare();
    };
    
}

#endif

#endif /* defined(__testUI__MCDateFormatter__) */
