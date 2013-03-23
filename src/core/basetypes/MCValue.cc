#include "MCValue.h"
#include "MCValuePrivate.h"

#include <string.h>
#include <stdlib.h>

#include "MCString.h"
#include "MCHash.h"
#include "MCUtils.h"

using namespace mailcore;

Value::Value()
{
    mType = VALUE_TYPE_NONE;
    memset(&mValue, 0, sizeof(mValue));
}

Value::Value(Value * other)
{
    memcpy(&mValue, &other->mValue, sizeof(mValue));
    mType = other->mType;
    if (mType == VALUE_TYPE_DATA_VALUE) {
        mValue.dataValue.data = (char *) malloc(mValue.dataValue.length);
        memcpy(mValue.dataValue.data, other->mValue.dataValue.data, mValue.dataValue.length);
    }
}

Value::~Value()
{
    if (mType == VALUE_TYPE_DATA_VALUE) {
        free(mValue.dataValue.data);
    }
}

String * Value::description()
{
    switch (mType) {
        case VALUE_TYPE_BOOL_VALUE:
        if (mValue.boolValue) {
            return MCSTR("true");
        }
        else {
            return MCSTR("false");
        }
        case VALUE_TYPE_CHAR_VALUE:
        return String::stringWithUTF8Format("%i", (int) mValue.charValue);
        case VALUE_TYPE_UNSIGNED_CHAR_VALUE:
        return String::stringWithUTF8Format("%u", (unsigned int) mValue.unsignedCharValue);
        case VALUE_TYPE_SHORT_VALUE:
        return String::stringWithUTF8Format("%i", (int) mValue.shortValue);
        case VALUE_TYPE_UNSIGNED_SHORT_VALUE:
        return String::stringWithUTF8Format("%u", (unsigned int) mValue.unsignedShortValue);
        case VALUE_TYPE_INT_VALUE:
        return String::stringWithUTF8Format("%i", mValue.intValue);
        case VALUE_TYPE_UNSIGNED_INT_VALUE:
        return String::stringWithUTF8Format("%u", mValue.unsignedIntValue);
        case VALUE_TYPE_LONG_VALUE:
        return String::stringWithUTF8Format("%li", mValue.longValue);
        case VALUE_TYPE_UNSIGNED_LONG_VALUE:
        return String::stringWithUTF8Format("%lu", mValue.unsignedLongValue);
        case VALUE_TYPE_LONG_LONG_VALUE:
        return String::stringWithUTF8Format("%lli", mValue.longLongValue);
        case VALUE_TYPE_UNSIGNED_LONG_LONG_VALUE:
        return String::stringWithUTF8Format("%llu", mValue.unsignedLongLongValue);
        case VALUE_TYPE_FLOAT_VALUE:
        return String::stringWithUTF8Format("%f", (double) mValue.floatValue);
        case VALUE_TYPE_DOUBLE_VALUE:
        return String::stringWithUTF8Format("%f", mValue.doubleValue);
        case VALUE_TYPE_POINTER_VALUE:
        return String::stringWithUTF8Format("%p", mValue.pointerValue);
        case VALUE_TYPE_DATA_VALUE:
        return String::stringWithUTF8Format("<Value:%p:data>", this);
        default:
        return String::stringWithUTF8Format("<Value:%p:unknown>", this);
    }
}

bool Value::isEqual(Object * otherObject)
{
    Value * otherValue = (Value *) otherObject;
    if (otherValue->mType != mType)
        return false;
    if (mType == VALUE_TYPE_DATA_VALUE) {
        if (mValue.dataValue.length != otherValue->mValue.dataValue.length)
            return false;
        if (memcmp(&otherValue->mValue.dataValue.data, &mValue.dataValue.data, mValue.dataValue.length) != 0)
            return false;
    }
    else {
        if (memcmp(&otherValue->mValue, &mValue, sizeof(mValue)) != 0)
            return false;
    }
    return true;
}

unsigned int Value::hash()
{
    return hashCompute((const char *) &mValue, sizeof(mValue));
}

Object * Value::copy()
{
    return new Value(this);
}

Value * Value::valueWithBoolValue(bool value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_BOOL_VALUE;
    result->mValue.boolValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithCharValue(char value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_CHAR_VALUE;
    result->mValue.charValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithUnsignedCharValue(unsigned char value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_UNSIGNED_CHAR_VALUE;
    result->mValue.unsignedCharValue = value;
    return (Value *) result->autorelease();
}

///////////////////////

Value * Value::valueWithIntValue(int value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_INT_VALUE;
    result->mValue.intValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithUnsignedIntValue(unsigned int value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_UNSIGNED_INT_VALUE;
    result->mValue.unsignedIntValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithLongValue(long value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_LONG_VALUE;
    result->mValue.longValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithUnsignedLongValue(unsigned long value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_UNSIGNED_LONG_VALUE;
    result->mValue.unsignedLongValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithLongLongValue(long long value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_LONG_LONG_VALUE;
    result->mValue.longLongValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithUnsignedLongLongValue(unsigned long long value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_UNSIGNED_LONG_LONG_VALUE;
    result->mValue.unsignedLongLongValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithFloatValue(float value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_FLOAT_VALUE;
    result->mValue.floatValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithDoubleValue(double value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_DOUBLE_VALUE;
    result->mValue.doubleValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithPointerValue(void * value)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_POINTER_VALUE;
    result->mValue.pointerValue = value;
    return (Value *) result->autorelease();
}

Value * Value::valueWithData(const char * value, int length)
{
    Value * result = new Value();
    result->mType = VALUE_TYPE_DATA_VALUE;
    result->mValue.dataValue.data = (char *) malloc(length);
    memcpy(result->mValue.dataValue.data, value, length);
    result->mValue.dataValue.length = length;
    return (Value *) result->autorelease();
}

bool Value::boolValue()
{
    return mValue.boolValue;
}

char Value::charValue()
{
    return mValue.charValue;
}

unsigned char Value::unsignedCharValue()
{
    return mValue.unsignedCharValue;
}

short Value::shortValue()
{
    return mValue.shortValue;
}

unsigned short Value::unsignedShortValue()
{
    return mValue.unsignedShortValue;
}

int Value::intValue()
{
    return mValue.intValue;
}

unsigned int Value::unsignedIntValue()
{
    return mValue.unsignedIntValue;
}

long Value::longValue()
{
    return mValue.longValue;
}

unsigned long Value::unsignedLongValue()
{
    return mValue.unsignedLongValue;
}

long long Value::longLongValue()
{
    return mValue.longLongValue;
}

unsigned long long Value::unsignedLongLongValue()
{
    return mValue.unsignedLongLongValue;
}

float Value::floatValue()
{
    return mValue.floatValue;
}

double Value::doubleValue()
{
    return mValue.doubleValue;
}

void * Value::pointerValue()
{
    return mValue.pointerValue;
}

void Value::dataValue(const char ** p_value, int * p_length)
{
    * p_value = mValue.dataValue.data;
    * p_length = mValue.dataValue.length;
}

int Value::type()
{
    return mType;
}
