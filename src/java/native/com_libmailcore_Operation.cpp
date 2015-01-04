#include "com_libmailcore_Operation.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCOperation.h"
#include "MCDefines.h"

using namespace mailcore;

#define nativeType Operation
#define javaType nativeType

class JavaOperationCallback : public Object, public OperationCallback {
public:
    JavaOperationCallback(JNIEnv * env, jobject obj) {
        mEnv = env;
        mGlobalRef = env->NewGlobalRef(obj);
    }
    
    virtual ~JavaOperationCallback() {
        if (mGlobalRef != NULL) {
            mEnv->DeleteGlobalRef(mGlobalRef);
            mGlobalRef = NULL;
        }
    }
    
    virtual void operationFinished(Operation * op)
    {
        jclass cls = mEnv->GetObjectClass(mGlobalRef);
        jmethodID mid = mEnv->GetMethodID(cls, "callCallback", "()V");
        mEnv->CallVoidMethod(mGlobalRef, mid);
        
        mEnv->DeleteGlobalRef(mGlobalRef);
        mGlobalRef = NULL;
        
        this->release();
    }
    
private:
    JNIEnv * mEnv;
    jobject mGlobalRef;
};

JNIEXPORT void JNICALL Java_com_libmailcore_Operation_cancel
  (JNIEnv * env, jobject obj)
{
    MC_JAVA_NATIVE_INSTANCE->cancel();
}

JNIEXPORT jboolean JNICALL Java_com_libmailcore_Operation_isCancelled
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jboolean, isCancelled);
}

JNIEXPORT void JNICALL Java_com_libmailcore_Operation_nativeStart
  (JNIEnv * env, jobject obj)
{
    JavaOperationCallback * callback = new JavaOperationCallback(env, obj);
    MC_JAVA_NATIVE_INSTANCE->setCallback(callback);
    MC_JAVA_NATIVE_INSTANCE->start();
}

MC_JAVA_BRIDGE
