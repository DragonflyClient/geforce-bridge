#pragma once

#include <jni.h>
/* Header for class dev_decobr_mcgeforce_bindings_MCGeForceHelper */

#ifndef _Included_dev_decobr_mcgeforce_bindings_MCGeForceHelper
#define _Included_dev_decobr_mcgeforce_bindings_MCGeForceHelper
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
	 * Method:    init
	 * Signature: ()V
	 */
	JNIEXPORT jlong JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_init
	(JNIEnv*, jobject);

	/*
	 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
	 * Method:    setVideoHighlight
	 * Signature: (Ljava/lang/String;Ljava/lang/String;II)V
	 */
	JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_setVideoHighlight
	(JNIEnv*, jobject, jlong, jstring, jstring, jint, jint);

	/*
 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
 * Method:    addGroup
 * Signature: (Ljava/lang/String;Ljava/lang/String)V
 */
	JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_addGroup
	(JNIEnv*, jobject, jlong, jstring, jstring);

	/*
	 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
	 * Method:    showHighlightsEditor
	 * Signature: (Ljava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_showHighlightsEditor
	(JNIEnv*, jobject, jlong, jstring);

	/*
	 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
	 * Method:    getNumOfHighlights
	 * Signature: (Ljava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_getNumOfHighlights
	(JNIEnv*, jobject, jlong, jstring);

	/*
	 * Class:     dev_decobr_mcgeforce_bindings_MCGeForceHelper
	 * Method:    closeGroup
	 * Signature: (Ljava/lang/String;Ljava/lang/Boolean;II)V
	 */
	JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_closeGroup
	(JNIEnv*, jobject, jlong, jstring, jboolean);

#ifdef __cplusplus
}
#endif
#endif

#include <iostream>
#include <gfesdk/os_defines.h>
#include <gfesdk/sdk_types.h>
#include <gfesdk/sdk_types_ipc.h>
#include <gfesdk/config.h>
#include <gfesdk/isdk.h>
#include <gfesdk/highlights/ihighlights.h>
#include <functional>
#include <Windows.h>

using namespace std;

class JNIHelper {
public:
	/*!
	 * \param env[in] - A valid JNIEnv returned from a JNI Call
	 * \param obj[in] - A valid jobject returned from a JNI call
	 */
	void init(JNIEnv* env, jobject obj);

	jmethodID getMethod(const char* methodName, const char* signature);

	jmethodID getStaticMethod(const char* methodName, const char* signature);

	/*!
	 * \param methodName[in] - The name of the method to be called
	 * \param signature[in]  - The signature of the method to be called
	 */
	void callVoidMethod(jmethodID method, ...);

	/*!
	 * \param methodName[in] - The name of the method to be called
	 * \param signature[in]  - The signature of the method to be called
	 */
	void callStaticVoidMethod(jmethodID method, ...);

	jstring convertCharArrayToString(const char* charArray);
};

class MCGeForce {
public:
	long init(void);
	void addGroup(NVGSDK_HANDLE* handle, const char* id, const char* name);
	void setVideoHighlight(NVGSDK_HANDLE* handle, const char* id, const char* groupID, int start, int end);
	void showHighlightsEditor(NVGSDK_HANDLE* handle, const char* groupID);
	void closeGroup(NVGSDK_HANDLE* handle, const char* groupID, bool deleteHighlights);
	void getNumOfHighlights(NVGSDK_HANDLE* handle, const char* groupID);
};