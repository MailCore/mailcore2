#ifndef MAILCORE_MCACTIVESYNCPRIVATE_H

#define MAILCORE_MCACTIVESYNCPRIVATE_H

#define MC_INIT_STRING_FIELD(field) field = NULL
#define MC_RELEASE_STRING_FIELD(field) MC_SAFE_RELEASE(field)
#define MC_SET_STRING_FIELD(field, value) MC_SAFE_REPLACE_COPY(String, field, value)
#define MC_GET_STRING_FIELD(field) return field
#define MC_SET_OBJECT_FIELD(type, field, value) MC_SAFE_REPLACE_COPY(type, field, value)
#define MC_GET_OBJECT_FIELD(field) return field

#endif
