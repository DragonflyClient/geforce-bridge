#include "MCGeForce.h"

static JNIEnv* environment = nullptr;
static jclass clazz = nullptr;
static JavaVM* jvm = nullptr;

void JNIHelper::init(JNIEnv* env, jobject obj)
{
	environment = env;

	// Get a JVM Instance from the current JNI Environment
	env->GetJavaVM(&jvm);
	if (NULL == jvm) {
		printf("[GeForce Experience Native] (JNIHelper::init) ERROR: jvm is null");
		fflush(stdout);

		return;
	}

	clazz = environment->GetObjectClass(obj);
	if (NULL == clazz) {
		printf("[GeForce Experience Native] (JNIHelper::init) ERROR: clazz is null");
		fflush(stdout);

		return;
	}
}

jmethodID JNIHelper::getMethod(const char* methodName, const char* signature)
{
	if (environment == NULL) {
		printf("[GeForce Experience Native] (numberOfHighlightsCallback) environment is null\n");
		fflush(stdout);

		return nullptr;
	}

	return environment->GetMethodID(clazz, methodName, signature);
}

jmethodID JNIHelper::getStaticMethod(const char* methodName, const char* signature)
{
	if (environment == NULL) {
		printf("[GeForce Experience Native] (numberOfHighlightsCallback) environment is null\n");
		fflush(stdout);

		return nullptr;
	}

	return environment->GetStaticMethodID(clazz, methodName, signature);
}

void JNIHelper::callVoidMethod(jmethodID method, ...)
{
	if (environment == NULL) {
		printf("[GeForce Experience Native] (numberOfHighlightsCallback) environment is null\n");
		fflush(stdout);

		return;
	}

	if (method == NULL) {
		printf("[GeForce Experience Native] (JNIHelper::callVoidMethod) callback is null");
		fflush(stdout);
		return;
	}

	jvm->AttachCurrentThread((void**)&environment, nullptr);

	va_list args;
	va_start(args, method);

	environment->CallVoidMethod(clazz, method, args);
	jvm->DetachCurrentThread();

	va_end(args);
}

void JNIHelper::callStaticVoidMethod(jmethodID method, ...)
{
	if (environment == NULL) {
		printf("[GeForce Experience Native] (JNIHelper::callStaticVoidMethod) environment is null\n");
		fflush(stdout);

		return;
	}

	if (method == NULL) {
		printf("[GeForce Experience Native] (JNIHelper::callStaticVoidMethod) method is null");
		fflush(stdout);

		return;
	}

	fflush(stdout);

	va_list args;
	va_start(args, method);

	jvm->AttachCurrentThread((void**)&environment, nullptr);
	environment->CallStaticVoidMethod(clazz, method, args);
	jvm->DetachCurrentThread();

	jvm->DetachCurrentThread();
	va_end(args);
}

jstring JNIHelper::convertCharArrayToString(const char* charArray)
{
	return (environment)->NewStringUTF(charArray);
}
