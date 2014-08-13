#ifndef MAILCORE_MCUTILS_H

#define MAILCORE_MCUTILS_H

#ifdef __cplusplus

#define MC_SAFE_RETAIN(o) ((o) != NULL ? (o)->retain() : NULL)
#define MC_SAFE_COPY(o) ((o) != NULL ? (o)->copy() : NULL)

#define MC_SAFE_RELEASE(o) \
    do { \
        if ((o) != NULL) { \
            (o)->release(); \
            (o) = NULL; \
        } \
    } while (0)

#define MC_SAFE_REPLACE_RETAIN(type, mField, value) \
    do { \
        MC_SAFE_RELEASE(mField); \
        mField = (type *) MC_SAFE_RETAIN(value); \
    } while (0)

#define MC_SAFE_REPLACE_COPY(type, mField, value) \
    do { \
        MC_SAFE_RELEASE(mField); \
        mField = (type *) MC_SAFE_COPY(value); \
    } while (0)

#define MCSTR(str) mailcore::String::uniquedStringWithUTF8Characters("" str "")

#define MCUTF8(str) MCUTF8DESC(str)
#define MCUTF8DESC(obj) ((obj) != NULL ? (obj)->description()->UTF8Characters() : NULL )

#define MCLOCALIZEDSTRING(key) key

#define MCISKINDOFCLASS(instance, class) (dynamic_cast<class *>(instance) != NULL)

#endif

#endif
