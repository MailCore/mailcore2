#include "MCData.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unicode/ucsdet.h>
#include <libetpan/libetpan.h>

#include "MCString.h"
#include "MCHash.h"
#include "MCUtils.h"

#define DEFAULT_CHARSET "iso-8859-1"

using namespace mailcore;

void Data::allocate(unsigned int length)
{
    length ++;
    if (length < mAllocated)
        return;
    
    if (mAllocated == 0) {
        mAllocated = 4;
    }
    while (length > mAllocated) {
        mAllocated *= 2;
    }
    
    mBytes = (char *) realloc(mBytes, mAllocated);
}

void Data::reset()
{
    free(mBytes);
    mAllocated = 0;
    mLength = 0;
    mBytes = NULL;
}

Data::Data()
{
    mBytes = NULL;
    reset();
}

Data::Data(Data * otherData) : Object()
{
    mBytes = NULL;
    reset();
    appendData(otherData);
}

Data::Data(const char * bytes, unsigned int length)
{
    mBytes = NULL;
    reset();
    appendBytes(bytes, length);
}

Data::Data(int capacity)
{
    mBytes = NULL;
    reset();
    allocate(capacity);
}

Data::~Data()
{
    reset();
}

Data * Data::dataWithBytes(const char * bytes, unsigned int length)
{
    Data * result = new Data(bytes, length);
    return (Data *) result->autorelease();
}

char * Data::bytes()
{
    return mBytes;
}

unsigned int Data::length()
{
    return mLength;
}

void Data::appendData(Data * otherData)
{
    appendBytes(otherData->bytes(), otherData->length());
}

void Data::appendBytes(const char * bytes, unsigned int length)
{
    allocate(mLength + length);
    memcpy(&mBytes[mLength], bytes, length);
    mLength += length;
}

void Data::setBytes(const char * bytes, unsigned int length)
{
    reset();
    appendBytes(bytes, length);
}

void Data::setData(Data * otherData)
{
    reset();
    appendData(otherData);
}

String * Data::description()
{
    return String::stringWithUTF8Format("<%s:%p %i bytes>", MCUTF8(className()), this, length());
}

Object * Data::copy()
{
    return new Data(this);
}

bool Data::isEqual(Object * otherObject)
{
    Data * otherData = (Data *) otherObject;
    if (length() != otherData->length())
        return false;
    if (memcmp(bytes(), otherData->bytes(), mLength) != 0)
        return false;
    return true;
}

unsigned int Data::hash()
{
    return hashCompute(mBytes, mLength);
}

String * Data::stringWithDetectedCharset()
{
    String * result;
    result = stringWithDetectedCharset(NULL, false);
    return result;
}

String * Data::normalizeCharset(String * charset)
{
    if ((charset->caseInsensitiveCompare(MCSTR("iso-2022-jp")) == 0) ||
    (charset->caseInsensitiveCompare(MCSTR("iso-2022-jp-2")) == 0)) {
        charset = MCSTR("iso-2022-jp-2");
    }
    else if (charset->caseInsensitiveCompare(MCSTR("ks_c_5601-1987")) == 0) {
        charset = MCSTR("euckr");
    }
    else if ((charset->caseInsensitiveCompare(MCSTR("iso-8859-8-i")) == 0) ||
    (charset->caseInsensitiveCompare(MCSTR("iso-8859-8-e")) == 0)) {
        charset = MCSTR("iso-8859-8");
    }
    else if ((charset->caseInsensitiveCompare(MCSTR("GB2312")) == 0) ||
    (charset->caseInsensitiveCompare(MCSTR("GB_2312-80")) == 0)) {
        charset = MCSTR("GBK");
    }
    
    return charset->lowercaseString();
}

String * Data::stringWithCharset(const char * charset)
{
    String * result = new String(this, charset);
    if ((length() != 0) && (result->length() == 0)) {
        result->release();
        return NULL;
    }
    return (String *) result->autorelease();
}

String * Data::stringWithDetectedCharset(String * hintCharset, bool isHTML)
{
	String * result;
    String * charset;
    
    if (hintCharset == NULL) {
        charset = charsetWithFilteredHTML(isHTML);
    }
    else {
        charset = charsetWithFilteredHTML(isHTML, hintCharset);
    }
    
    if (charset == NULL) {
        charset = MCSTR(DEFAULT_CHARSET);
    }
    
    charset = normalizeCharset(charset);
    
    /*
    if (charset->isEqual(MCSTR("iso-2022-jp-2"))) {
        const char * theBytes;
        Data * data;
        
        theBytes = bytes();
        data = this;
        if (length() > 0) {
            unsigned int idx;
            
            idx = length();
            while ((theBytes[idx - 1] == '\n') || (theBytes[idx - 1] == '\r')) {
                idx --;
                if (idx == 0)
                    break;
            }
            
            if (idx != length()) {
                data = Data::dataWithBytes(theBytes, idx);
            }
        }
        result = data->stringWithCharset("iso-2022-jp-2");
        if (result == NULL) {
            result = data->stringWithCharset("iso-2022-jp");
        }
        
        return result;
    }
     */
    
    result = stringWithCharset(charset->UTF8Characters());
    if (result == NULL) {
        result = stringWithCharset("iso-8859-1");
    }
    if (result == NULL) {
        result = stringWithCharset("windows-1252");
    }
    if (result == NULL) {
        result = stringWithCharset("utf-8");
    }
    if (result == NULL) {
        result = MCSTR("");
    }
    
	return result;
}

String * Data::charsetWithFilteredHTMLWithoutHint(bool filterHTML)
{
    UCharsetDetector * detector;
    const UCharsetMatch * match;
    UErrorCode err = U_ZERO_ERROR;
    const char * cName;
    String * result;
    
    detector = ucsdet_open(&err);
    ucsdet_setText(detector, bytes(), length(), &err);
    ucsdet_enableInputFilter(detector, filterHTML);
    match = ucsdet_detect(detector, &err);
    if (match == NULL) {
        ucsdet_close(detector);
        return NULL;
    }
    
    cName = ucsdet_getName(match, &err);
    
    result = String::stringWithUTF8Characters(cName);
    ucsdet_close(detector);
    
    return result;
}

String * Data::charsetWithFilteredHTML(bool filterHTML, String * hintCharset)
{
    if (hintCharset == NULL)
        return charsetWithFilteredHTMLWithoutHint(filterHTML);
    
    const UCharsetMatch ** matches;
    int32_t matchesCount;
    UCharsetDetector * detector;
    UErrorCode err = U_ZERO_ERROR;
    String * result;
    
    hintCharset = hintCharset->lowercaseString();
    
    detector = ucsdet_open(&err);
    ucsdet_setText(detector, bytes(), length(), &err);
    ucsdet_enableInputFilter(detector, filterHTML);
    matches = ucsdet_detectAll(detector,  &matchesCount, &err);
    if (matches == NULL) {
        ucsdet_close(detector);
        return hintCharset;
    }
    if (matchesCount == 0) {
        ucsdet_close(detector);
        return hintCharset;
    }
    
    result = NULL;
    
    for(int32_t i = 0 ; i < matchesCount ; i ++) {
        const char * cName;
        String * name;
        int32_t confidence;
        
        cName = ucsdet_getName(matches[i], &err);
        name = String::stringWithUTF8Characters(cName);
        name = name->lowercaseString();
        confidence = ucsdet_getConfidence(matches[i], &err);
        if ((confidence >= 50) && name->isEqual(hintCharset)) {
            result = name;
            break;
        }
    }
    
    if (result == NULL) {
        int32_t maxConfidence;
        
        maxConfidence = 49;
        
        for(int32_t i = 0 ; i < matchesCount ; i ++) {
            const char * cName;
            String * name;
            int32_t confidence;
            
            cName = ucsdet_getName(matches[i], &err);
            confidence = ucsdet_getConfidence(matches[i], &err);
            name = String::stringWithUTF8Characters(cName);
            if (confidence > maxConfidence) {
                result = name;
                maxConfidence = confidence;
            }
        }
    }
    ucsdet_close(detector);
    
    if (result == NULL)
        result = hintCharset;
    
    return result;
}

Data * Data::dataWithContentsOfFile(String * filename)
{
    int r;
    size_t read_items;
    struct stat stat_buf;
    FILE * f;
    char * buf;
    Data * data;
    
    f = fopen(filename->fileSystemRepresentation(), "rb");
    if (f == NULL) {
        return NULL;
    }

    r = fstat(fileno(f), &stat_buf);
    if (r < 0) {
        fclose(f);
        return NULL;
    }
    
    buf = (char *) malloc(stat_buf.st_size);
    
    read_items = fread(buf, 1, stat_buf.st_size, f);
    if ((off_t) read_items != stat_buf.st_size) {
        free(buf);
        fclose(f);
        return NULL;
    }
    
    data = Data::dataWithBytes(buf, (unsigned int) stat_buf.st_size);
    free(buf);
    
    fclose(f);
    
    return data;
}

static size_t uudecode(char * text, size_t size)
{
    unsigned int count = 0;
    char *b = text;		/* beg */
    char *s = b;			/* src */
    char *d = b;			/* dst */
    char *e = b+size;			/* end */
    int out = (*s++ & 0x7f) - 0x20;
    
    /* don't process lines without leading count character */
    if (out < 0)
        return size;
    
    /* don't process begin and end lines */
    if ((strncasecmp((const char *)b, "begin ", 6) == 0) ||
        (strncasecmp((const char *)b, "end",    3) == 0))
        return size;
    
    //while (s < e - 4)
    while (s < e)
    {
        int v = 0;
        int i;
        for (i = 0; i < 4; i += 1) {
            char c = *s++;
            v = v << 6 | ((c - 0x20) & 0x3F);
        }
        for (i = 2; i >= 0; i -= 1) {
            char c = (char) (v & 0xFF);
            d[i] = c;
            v = v >> 8;
        }
        d += 3;
        count += 3;
    }
    *d = (char) '\0';
    return count;
}

Data * Data::decodedDataUsingEncoding(Encoding encoding)
{
    const char * text;
    size_t text_length;
    
    text = bytes();
    text_length = length();
    
    switch (encoding) {
        case Encoding7Bit:
        case Encoding8Bit:
        case EncodingBinary:
        case EncodingOther:
        default:
        {
            return this;
        }
        case EncodingBase64:
        case EncodingQuotedPrintable:
        {
            char * decoded;
            size_t decoded_length;
            size_t cur_token;
            int mime_encoding;
            Data * data;
            
            switch (encoding) {
                default: //disable warning
                case EncodingBase64:
                    mime_encoding = MAILMIME_MECHANISM_BASE64;
                    break;
                case EncodingQuotedPrintable:
                    mime_encoding = MAILMIME_MECHANISM_QUOTED_PRINTABLE;
                    break;
            }
            
            cur_token = 0;
            mailmime_part_parse(text, text_length, &cur_token,
                                mime_encoding, &decoded, &decoded_length);
            data = Data::dataWithBytes(decoded, (unsigned int) decoded_length);
            mailmime_decoded_part_free(decoded);
            return data;
        }
        case EncodingUUEncode:
        {
            char * dup_data;
            size_t decoded_length;
            Data * data;
            char * current_p;
            
            data = Data::dataWithCapacity((unsigned int) text_length);
            
            dup_data = (char *) malloc(text_length);
            memcpy(dup_data, text, text_length);
            
            current_p = dup_data;
            while (1) {
                size_t length;
                char * p;
                char * p1;
                char * p2;
                char * end_line;
                
                p1 = strchr(current_p, '\n');
                p2 = strchr(current_p, '\r');
                if (p1 == NULL) {
                    p = p2;
                }
                else if (p2 == NULL) {
                    p = p1;
                }
                else {
                    if (p1 - current_p < p2 - current_p) {
                        p = p1;
                    }
                    else {
                        p = p2;
                    }
                }
                end_line = p;
                if (p != NULL) {
                    while ((size_t) (p - dup_data) < text_length) {
                        if ((* p != '\r') && (* p != '\n')) {
                            break;
                        }
                        p ++;
                    }
                }
                if (p == NULL) {
                    length = text_length - (current_p - dup_data);
                }
                else {
                    length = end_line - current_p;
                }
                if (length == 0) {
                    break;
                }
                decoded_length = uudecode(current_p, length);
                if (decoded_length != 0 && decoded_length < length) {
                    data->appendBytes(current_p, (unsigned int) decoded_length);
                }
                
                if (p == NULL)
                    break;
                
                current_p = p;
                while ((size_t) (current_p - dup_data) < text_length) {
                    if ((* current_p != '\r') && (* current_p != '\n')) {
                        break;
                    }
                    current_p ++;
                }
            }
            free(dup_data);
            
            return data;
        }
    }
}

Data * Data::data()
{
    return dataWithCapacity(0);
}

Data * Data::dataWithCapacity(int capacity)
{
    Data * result = new Data(capacity);
    return (Data *) result->autorelease();
}
