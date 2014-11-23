#include "MCWin32.h" // should be first include.

#include "MCData.h"

#define USE_UCHARDET 1

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#if USE_UCHARDET
#include <uchardet/uchardet.h>
#else
#include <unicode/ucsdet.h>
#endif
#include <libetpan/libetpan.h>
#if __APPLE__
#include <iconv.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "MCDefines.h"
#include "MCString.h"
#include "MCHash.h"
#include "MCUtils.h"
#include "MCHashMap.h"
#include "MCBase64.h"
#include "MCSet.h"

#define MCDATA_DEFAULT_CHARSET "iso-8859-1"

using namespace mailcore;

static int isPowerOfTwo (unsigned int x)
{
    return ((x != 0) && !(x & (x - 1)));
}

void Data::allocate(unsigned int length, bool force)
{
    if (length <= mAllocated)
        return;

    if (force) {
        mAllocated = length;
    }
    else {
        if (!isPowerOfTwo(mAllocated)) {
            mAllocated = 0;
        }
        if (mAllocated == 0) {
            mAllocated = 4;
        }
        while (length > mAllocated) {
            mAllocated *= 2;
        }
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
    allocate(length, true);
    appendBytes(bytes, length);
}

Data::Data(int capacity)
{
    mBytes = NULL;
    reset();
    allocate(capacity, true);
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

static String * normalizeCharset(String * charset)
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

static bool isHintCharsetValid(String * hintCharset)
{
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    static Set * knownCharset = NULL;
    
    pthread_mutex_lock(&lock);
    if (knownCharset == NULL) {
        knownCharset = new Set();
        
#if !USE_UCHARDET
        UCharsetDetector * detector;
        UEnumeration * iterator;
        UErrorCode err = U_ZERO_ERROR;
        
        detector = ucsdet_open(&err);
        iterator = ucsdet_getAllDetectableCharsets(detector, &err);
        while (1) {
            const char * validCharset = uenum_next(iterator, NULL, &err);
            if (err != U_ZERO_ERROR)
                break;
            if (validCharset == NULL)
                break;
            knownCharset->addObject(String::stringWithUTF8Characters(validCharset));
        }
        uenum_close(iterator);
        ucsdet_close(detector);
#else
        const char * charset_list[] = {
            "Big5",
            "EUC-JP",
            "EUC-KR",
            "x-euc-tw",
            "gb18030",
            "ISO-8859-8",
            "windows-1255",
            "windows-1252",
            "Shift_JIS",
            "UTF-8",
            "UTF-16",
            "HZ-GB-2312",
            "ISO-2022-CN",
            "ISO-2022-JP",
            "ISO-2022-KR",
            "ISO-8859-5",
            "windows-1251",
            "KOI8-R",
            "x-mac-cyrillic",
            "IBM866",
            "IBM855",
            "ISO-8859-7",
            "windows-1253",
            "ISO-8859-2",
            "windows-1250",
            "TIS-620",
        };
        for(unsigned int i = 0 ; i < sizeof(charset_list) / sizeof(charset_list[0]) ; i ++) {
            String * str = String::stringWithUTF8Characters(charset_list[i]);
            str = str->lowercaseString();
            knownCharset->addObject(str);
        }
#endif
    }
    pthread_mutex_unlock(&lock);
    
    if (hintCharset != NULL) {
        hintCharset = normalizeCharset(hintCharset);
        
        if (hintCharset->isEqual(MCSTR("tis-620"))) {
            return true;
        }
        else if (hintCharset->isEqual(MCSTR("koi8-r"))) {
            return true;
        }
        else if (hintCharset->isEqual(MCSTR("euc-kr"))) {
            return true;
        }
        else if (hintCharset->isEqual(MCSTR("windows-1256"))) {
            return true;
        }
        
        if (!knownCharset->containsObject(hintCharset)) {
            return true;
        }
    }
    
    return false;
}

String * Data::stringWithDetectedCharset(String * hintCharset, bool isHTML)
{
    String * result;
    String * charset;
    
    if (hintCharset != NULL) {
        hintCharset = normalizeCharset(hintCharset);
    }
    if (isHintCharsetValid(hintCharset)) {
        charset = hintCharset;
    }
    else {
        if (hintCharset == NULL) {
            charset = charsetWithFilteredHTML(isHTML);
        }
        else {
            charset = charsetWithFilteredHTML(isHTML, hintCharset);
        }
    }
    
    if (charset == NULL) {
        charset = MCSTR(MCDATA_DEFAULT_CHARSET);
    }
    
    charset = normalizeCharset(charset);
    
    // Remove whitespace at the end of the string to fix conversion.
    if (charset->isEqual(MCSTR("iso-2022-jp-2"))) {
        Data * data = this;
        result = data->stringWithCharset("iso-2022-jp-2");
        if (result == NULL) {
            result = data->stringWithCharset("iso-2022-jp");
        }
        if (result == NULL) {
            result = MCSTR("");
        }
        
        return result;
    }

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
#if !USE_UCHARDET
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
#else
  String * result = NULL;
  uchardet_t ud = uchardet_new();
  int r = uchardet_handle_data(ud, bytes(), length());
  if (r == 0) {
    uchardet_data_end(ud);
    const char * charset = uchardet_get_charset(ud);
    if (charset[0] != 0) {
      result = String::stringWithUTF8Characters(charset);
    }
  }
  uchardet_delete(ud);
  
  return result;
#endif
}

String * Data::charsetWithFilteredHTML(bool filterHTML, String * hintCharset)
{
    if (hintCharset == NULL)
        return charsetWithFilteredHTMLWithoutHint(filterHTML);
    
#if !USE_UCHARDET
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
#else
    String * result;
    uchardet_t ud = uchardet_new();
    int r = uchardet_handle_data(ud, bytes(), length());
    if (r == 0) {
        uchardet_data_end(ud);
        const char * charset = uchardet_get_charset(ud);
        if (charset[0] == 0) {
            result = hintCharset;
        }
        else {
            result = String::stringWithUTF8Characters(charset);
        }
    }
    else {
        result = hintCharset;
    }
    uchardet_delete(ud);
    
    return result;
#endif
}

void Data::takeBytesOwnership(char * bytes, unsigned int length)
{
    free(mBytes);
    mBytes = (char *) bytes;
    mLength = length;
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
    
    buf = (char *) malloc((size_t) stat_buf.st_size);
    
    read_items = fread(buf, 1, (size_t)  stat_buf.st_size, f);
    if ((off_t) read_items != stat_buf.st_size) {
        free(buf);
        fclose(f);
        return NULL;
    }
    
    data = Data::data();
    data->takeBytesOwnership(buf, (unsigned int) stat_buf.st_size);
    
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

String * Data::base64String()
{
    char * encoded = MCEncodeBase64(bytes(), length());
    String * result = String::stringWithUTF8Characters(encoded);
    free(encoded);
    return result;
}

HashMap * Data::serializable()
{
    HashMap * result = Object::serializable();
    result->setObjectForKey(MCSTR("data"), base64String());
    return result;
}

void Data::importSerializable(HashMap * serializable)
{
    setData(((String *) (serializable->objectForKey(MCSTR("data"))))->decodedBase64Data());
}

#if __APPLE__
static CFStringEncoding encodingFromCString(const char * charset)
{
    CFStringEncoding encoding;
    CFStringRef charsetString;
    CFDataRef charsetData;
    
    charsetData = CFDataCreate(NULL, (const UInt8 *) charset, strlen(charset));
    charsetString = CFStringCreateFromExternalRepresentation(NULL, charsetData, kCFStringEncodingUTF8);
    encoding = CFStringConvertIANACharSetNameToEncoding(charsetString);
    CFRelease(charsetString);
    CFRelease(charsetData);
    
    return encoding;
}

static size_t lepIConvInternal(iconv_t cd,
                               const char **inbuf, size_t *inbytesleft,
                               char **outbuf, size_t *outbytesleft,
                               char **inrepls, const char *outrepl)
{
    size_t ret = 0, ret1;
    char *ib = (char *) *inbuf;
    size_t ibl = *inbytesleft;
    char *ob = *outbuf;
    size_t obl = *outbytesleft;

    for (;;)
    {
        ret1 = iconv (cd, &ib, &ibl, &ob, &obl);
        if (ret1 != (size_t)-1)
            ret += ret1;
        if (ibl && obl && errno == EILSEQ)
        {
            if (inrepls)
            {
                /* Try replacing the input */
                char **t;
                for (t = inrepls; *t; t++)
                {
                    char *ib1 = *t;
                    size_t ibl1 = strlen (*t);
                    char *ob1 = ob;
                    size_t obl1 = obl;
                    iconv (cd, &ib1, &ibl1, &ob1, &obl1);
                    if (!ibl1)
                    {
                        ++ib, --ibl;
                        ob = ob1, obl = obl1;
                        ++ret;
                        break;
                    }
                }
                if (*t)
                    continue;
            }
            if (outrepl)
            {
                /* Try replacing the output */
                size_t n = strlen (outrepl);
                if (n <= obl)
                {
                    memcpy (ob, outrepl, n);
                    ++ib, --ibl;
                    ob += n, obl -= n;
                    ++ret;
                    continue;
                }
            }
        }
        *inbuf = ib, *inbytesleft = ibl;
        *outbuf = ob, *outbytesleft = obl;
        return ret;
    }
}

static int lepIConv(const char * tocode, const char * fromcode,
                    const char * str, size_t length,
                    char * result, size_t * result_len)

{
    size_t out_size;
    size_t old_out_size;
    iconv_t conv;
    char * p_result;
    int res;
    size_t r;

    conv = iconv_open(tocode, fromcode);
    if (conv == (iconv_t) -1) {
        res = MAIL_CHARCONV_ERROR_UNKNOWN_CHARSET;
        goto err;
    }

    out_size = length * 6;
    old_out_size = out_size;
    p_result = result;

    r = lepIConvInternal(conv, &str, &length,
                         &p_result, &out_size, NULL, "?");
    if (r == (size_t) -1) {
        res = MAIL_CHARCONV_ERROR_CONV;
        goto close_iconv;
    }
    
    iconv_close(conv);
    
    * result_len = old_out_size - out_size;
    * p_result = '\0';

    return MAIL_CHARCONV_NO_ERROR;
    
close_iconv:
    iconv_close(conv);
err:
    return res;
}

static int lepCFConv(const char * tocode, const char * fromcode,
                     const char * str, size_t length,
                     char * result, size_t * result_len)
{
    CFDataRef data;
    CFStringRef resultString;
    CFStringEncoding fromEncoding;
    CFStringEncoding toEncoding;
    CFDataRef resultData;

    fromEncoding = encodingFromCString(fromcode);
    toEncoding = encodingFromCString(tocode);
    if (fromEncoding == kCFStringEncodingInvalidId)
        return MAIL_CHARCONV_ERROR_UNKNOWN_CHARSET;
    if (toEncoding == kCFStringEncodingInvalidId)
        return MAIL_CHARCONV_ERROR_UNKNOWN_CHARSET;

    data = CFDataCreate(NULL, (const UInt8 *) str, length);
    resultString = CFStringCreateFromExternalRepresentation(NULL, data, fromEncoding);
    if (resultString == NULL) {
        CFRelease(data);
        return MAIL_CHARCONV_ERROR_CONV;
    }

    resultData = CFStringCreateExternalRepresentation(NULL, resultString, toEncoding, (UInt8) '?');

    unsigned int len;
    len = (unsigned int) CFDataGetLength(resultData);
    CFDataGetBytes(resultData, CFRangeMake(0, len), (UInt8 *) result);
    * result_len = len;
    result[len] = 0;
    
    CFRelease(resultData);
    CFRelease(resultString);
    CFRelease(data);
    
    return MAIL_CHARCONV_NO_ERROR;
}

static int lepMixedConv(const char * tocode, const char * fromcode,
                        const char * str, size_t length,
                        char * result, size_t * result_len)
{
    int r;
    
    if (strcasecmp(fromcode, "iso-2022-jp-2") == 0) {
        r = lepCFConv(tocode, fromcode, str, length,
                      result, result_len);
        if (r == MAIL_CHARCONV_NO_ERROR)
            return r;
    }
    
    r = lepIConv(tocode, fromcode, str, length,
                 result, result_len);
    if (r == MAIL_CHARCONV_NO_ERROR)
        return r;
    
    return r;
}
#endif

static void * createObject()
{
    return new Data();
}

INITIALIZE(Data)
{
    Object::registerObjectConstructor("mailcore::Data", &createObject);
#if __APPLE__
    extended_charconv = lepMixedConv;
#endif
}
