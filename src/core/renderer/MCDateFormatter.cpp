//
//  MCDateFormatter.cpp
//  testUI
//
//  Created by DINH Viêt Hoà on 1/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCDateFormatter.h"
#include <stdlib.h>
#include <unicode/udat.h>

using namespace mailcore;

DateFormatter::DateFormatter()
{
    mDateFormatter = NULL;
    mDateStyle = DateFormatStyleMedium;
    mTimeStyle = DateFormatStyleMedium;
    mDateFormat = NULL;
    mTimezone = NULL;
    mLocale = NULL;
}

DateFormatter::~DateFormatter()
{
    if (mDateFormatter != NULL) {
        udat_close(mDateFormatter);
    }
    MC_SAFE_RELEASE(mDateFormat);
    MC_SAFE_RELEASE(mTimezone);
    MC_SAFE_RELEASE(mLocale);
}

DateFormatter * DateFormatter::dateFormatter()
{
    DateFormatter * result = new DateFormatter();
    result->autorelease();
    return result;
}


void DateFormatter::setDateStyle(DateFormatStyle style)
{
    mDateStyle = style;
}

DateFormatStyle DateFormatter::dateStyle()
{
    return mDateStyle;
}

void DateFormatter::setTimeStyle(DateFormatStyle style)
{
    mTimeStyle = style;
}

DateFormatStyle DateFormatter::timeStyle()
{
    return mTimeStyle;
}

void DateFormatter::setLocale(String * locale)
{
    MC_SAFE_REPLACE_COPY(String, mLocale, locale);
}

String * DateFormatter::locale()
{
    return mLocale;
}

void DateFormatter::setTimezone(String * timezone)
{
    MC_SAFE_REPLACE_COPY(String, mTimezone, timezone);
}

String * DateFormatter::timezone()
{
    return mTimezone;
}

void DateFormatter::setDateFormat(String * dateFormat)
{
    MC_SAFE_REPLACE_COPY(String, mDateFormat, dateFormat);
}

String * DateFormatter::dateFormat()
{
    return mDateFormat;
}

String * DateFormatter::stringFromDate(time_t date)
{
    prepare();
    if (mDateFormatter == NULL)
        return NULL;
    
    UErrorCode err = U_ZERO_ERROR;
    int32_t len = udat_format(mDateFormatter, ((double) date) * 1000., NULL, 0, NULL, &err);
    if(err != U_BUFFER_OVERFLOW_ERROR) {
        return NULL;
    }
    
    String * result;
    
    err = U_ZERO_ERROR;
    UChar * unichars = (UChar *) malloc((len + 1) * sizeof(unichars));
    udat_format(mDateFormatter, ((double) date) * 1000., unichars, len + 1, NULL, &err);
    result = new String(unichars, len);
    free(unichars);
    
    result->autorelease();
    return result;
}

time_t DateFormatter::dateFromString(String * dateString)
{
    prepare();
    if (mDateFormatter == NULL)
        return (time_t) -1;
    
    UErrorCode err = U_ZERO_ERROR;
    UDate date = udat_parse(mDateFormatter, dateString->unicodeCharacters(), dateString->length(),
                            NULL, &err);
    if (err != U_ZERO_ERROR) {
        return (time_t) -1;
    }
    
    return date / 1000.;
}

void DateFormatter::prepare()
{
    if (mDateFormatter != NULL)
        return;
    
    const UChar * tzID = NULL;
    int32_t tzIDLength = -1;
    const UChar * pattern = NULL;
    int32_t patternLength = -1;
    UErrorCode err = U_ZERO_ERROR;
    const char * locale = NULL;
    
    if (mTimezone != NULL) {
        tzID = mTimezone->unicodeCharacters();
        tzIDLength = mTimezone->length();
    }
    if (mDateFormat != NULL) {
        pattern = mDateFormat->unicodeCharacters();
        patternLength = mDateFormat->length();
    }
    if (mLocale != NULL) {
        locale = mLocale->UTF8Characters();
    }
    
    mDateFormatter = udat_open((UDateFormatStyle) mTimeStyle, (UDateFormatStyle) mDateStyle,
                               locale,
                               tzID, tzIDLength,
                               pattern, patternLength,
                               &err);
}
