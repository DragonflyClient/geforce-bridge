#include "MCGeForce.h"
#include <iostream>
#include <string>
#include <ctime>

static JNIEnv* environment = nullptr;
static jclass clazz = nullptr;
static JavaVM* jvm = nullptr;

void error(char text[]) {

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	std::string str(buffer);

	std::cout << "[" << str << "] [GeForce Bridge/ERROR]: " << text << "\n";

}

void info(char text[]) {

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	std::string str(buffer);

	std::cout << "[" << str << "] [GeForce Bridge/INFO]: " << text << "\n";

}

void warn(char text[]) {

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	std::string str(buffer);

	std::cout << "[" << str << "] [GeForce Bridge/WARN]: " << text << "\n";

}

void JNIHelper::init(JNIEnv* env, jobject obj)
{
	environment = env;

	// Get a JVM Instance from the current JNI Environment
	env->GetJavaVM(&jvm);
	if (NULL == jvm) {
		error("(JNIHelper::init) jvm is null");
		fflush(stdout);

		return;
	}

	clazz = environment->GetObjectClass(obj);
	if (NULL == clazz) {
		error("clazz is null");
		fflush(stdout);

		return;
	}
}

jmethodID JNIHelper::getMethod(const char* methodName, const char* signature)
{
	if (environment == NULL) {
		error("(JNIHelper::init) environment is null");
		fflush(stdout);

		return nullptr;
	}

	return environment->GetMethodID(clazz, methodName, signature);
}

jmethodID JNIHelper::getStaticMethod(const char* methodName, const char* signature)
{
	if (environment == NULL) {
		warn("(numberOfHighlightsCallback) environment is null");
		fflush(stdout);

		return nullptr;
	}

	return environment->GetStaticMethodID(clazz, methodName, signature);
}

void JNIHelper::callVoidMethod(jmethodID method, ...)
{
	if (environment == NULL) {
		warn("(numberOfHighlightsCallback) environment is null");
		fflush(stdout);

		return;
	}

	if (method == NULL) {
		warn("(JNIHelper::callVoidMethod) callback is null");
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
		warn("(JNIHelper::callStaticVoidMethod) environment is null");
		fflush(stdout);

		return;
	}

	if (method == NULL) {
		warn("(JNIHelper::callStaticVoidMethod) method is null");
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
