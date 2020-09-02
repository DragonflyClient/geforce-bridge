#include "MCGeForce.h"

static MCGeForce instance;
static JNIHelper jniHelper;

static jmethodID callbackMethodID;
static jmethodID numberOfHighlightsCallbackMethodID;

void asyncCallback(NVGSDK_RetCode ret, void* v) {
	jniHelper.callStaticVoidMethod(callbackMethodID, "(I)V", (int)ret);
}

long MCGeForce::init(void)
{	
	NVGSDK_HANDLE* handle;
	NVGSDK_Scope scopes[] = { NVGSDK_SCOPE_HIGHLIGHTS, NVGSDK_SCOPE_HIGHLIGHTS_VIDEO, NVGSDK_SCOPE_HIGHLIGHTS_SCREENSHOT };
	NVGSDK_ScopePermission scopePermissions[3];

	NVGSDK_RetCode returnCode;

	NVGSDK_CreateInputParams createParams = { 0 };
	createParams.appName = "Minecraft";
	createParams.scopeTable = &scopes[0];
	createParams.scopeTableSize = 3;
	createParams.pollForCallbacks = false;
	
	NVGSDK_CreateResponse response;
	response.scopePermissionTable = &scopePermissions[0];
	response.scopePermissionTableSize = 3;

	returnCode = NVGSDK_Create(&handle, &createParams, &response);
	jniHelper.callStaticVoidMethod(jniHelper.getStaticMethod("createCallback", "(ILjava/lang/String;)V"), (int)returnCode, jniHelper.convertCharArrayToString(response.gfeVersionStr));

	if (NVGSDK_SUCCEEDED(returnCode)) {
		NVGSDK_Scope scopes[] = { NVGSDK_SCOPE_HIGHLIGHTS, NVGSDK_SCOPE_HIGHLIGHTS_VIDEO, NVGSDK_SCOPE_HIGHLIGHTS_SCREENSHOT };
		NVGSDK_ScopePermission scopePermissions[3];

		NVGSDK_RequestPermissionsParams reqParams;
		reqParams.scopeTable = &scopes[0];
		reqParams.scopeTableSize = 3;

		NVGSDK_RequestPermissionsAsync(handle, &reqParams, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
		addGroup(handle, "mcdragonfly", "Dragonfly");

		NVGSDK_Highlight highlights[3] = { 0 };
		NVGSDK_LocalizedPair highlight1Translation[1] = { { "en-US", "Kill" } };
		highlights[0].id = "mcdrgn_kill";
		highlights[0].userInterest = true;
		highlights[0].highlightTags = NVGSDK_HIGHLIGHT_TYPE_ACHIEVEMENT;
		highlights[0].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_GOOD;
		highlights[0].nameTable = &highlight1Translation[0];
		highlights[0].nameTableSize = 1;

		NVGSDK_LocalizedPair highlight2Translation[1] = { { "en-US", "Win" } };
		highlights[1].id = "mcdrgn_win";
		highlights[1].userInterest = true;
		highlights[1].highlightTags = NVGSDK_HIGHLIGHT_TYPE_MILESTONE;
		highlights[1].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_VERY_GOOD;
		highlights[1].nameTable = &highlight2Translation[0];
		highlights[1].nameTableSize = 1;

		NVGSDK_LocalizedPair highlight3Translation[1] = { { "en-US", "Death" } };
		highlights[2].id = "mcdrgn_death";
		highlights[2].userInterest = true;
		highlights[2].highlightTags = NVGSDK_HIGHLIGHT_TYPE_INCIDENT;
		highlights[2].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_BAD;
		highlights[2].nameTable = &highlight3Translation[0];
		highlights[2].nameTableSize = 1;
		
		NVGSDK_HighlightConfigParams params = { 0 };
		params.highlightDefinitionTable = &highlights[0];
		params.highlightTableSize = 3;
		params.defaultLocale = "en-US";
		NVGSDK_Highlights_ConfigureAsync(handle, &params, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
	}

	return (long) handle;
}

void MCGeForce::addGroup(NVGSDK_HANDLE* handle, const char* id, const char* name)
{
	NVGSDK_LocalizedPair group1Translation[1] = { { "en-US", name } };
	NVGSDK_HighlightOpenGroupParams ogParams = { 0 };
	ogParams.groupId = id;
	ogParams.groupDescriptionTable = group1Translation;
	ogParams.groupDescriptionTableSize = 1;
	NVGSDK_Highlights_OpenGroupAsync(handle, &ogParams, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
}

void MCGeForce::setVideoHighlight(NVGSDK_HANDLE* handle, const char* id, const char* groupID, int start, int end)
{
	NVGSDK_VideoHighlightParams vhParams = { 0 };
	vhParams.highlightId = id;
	vhParams.groupId = groupID;
	vhParams.startDelta = start;
	vhParams.endDelta = end;
	NVGSDK_Highlights_SetVideoHighlightAsync(handle, &vhParams, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
}

void MCGeForce::showHighlightsEditor(NVGSDK_HANDLE* handle, const char* groupID)
{
	NVGSDK_GroupView groupView = { 0 };
	groupView.groupId = groupID;

	NVGSDK_SummaryParams summaryParams = { 0 };
	summaryParams.groupSummaryTable = { &groupView };
	summaryParams.groupSummaryTableSize = 1;
	NVGSDK_Highlights_OpenSummaryAsync(handle, &summaryParams, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
}

void MCGeForce::closeGroup(NVGSDK_HANDLE* handle, const char* groupID, bool deleteHighlights) {
	NVGSDK_HighlightCloseGroupParams closeParams = { 0 };
	closeParams.destroyHighlights = deleteHighlights;
	closeParams.groupId = groupID;

	NVGSDK_Highlights_CloseGroupAsync(handle, &closeParams, reinterpret_cast<NVGSDK_EmptyCallback>(asyncCallback), NULL);
}

void numberOfHighlightsCallback(NVGSDK_RetCode code, NVGSDK_Highlights_NumberOfHighlights const* ret, void* arg) {
	jniHelper.callStaticVoidMethod(numberOfHighlightsCallbackMethodID, "(I)V", (int)ret->numberOfHighlights);
}

void MCGeForce::getNumOfHighlights(NVGSDK_HANDLE* handle, const char* groupID)
{
	NVGSDK_GroupView groupParams = { 0 };
	groupParams.groupId = groupID;

	NVGSDK_Highlights_GetNumberOfHighlightsAsync(handle, &groupParams,
                                                 reinterpret_cast<NVGSDK_Highlights_GetNumberOfHighlightsCallback>(numberOfHighlightsCallback), NULL);
}

JNIEXPORT jlong JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_init
(JNIEnv* env, jobject obj) {	
	jniHelper.init(env, obj);

	callbackMethodID = jniHelper.getStaticMethod("callback", "(I)V");
	numberOfHighlightsCallbackMethodID = jniHelper.getStaticMethod("numberOfHighlightsCallback", "(I)V");

	return instance.init();
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_setVideoHighlight
(JNIEnv* env, jobject obj, jlong arg1, jstring arg2, jstring arg3, jint arg4, jint arg5) {
	const char* arg2native = env->GetStringUTFChars(arg2, 0);
	const char* arg3native = env->GetStringUTFChars(arg3, 0);

	instance.setVideoHighlight((NVGSDK_HANDLE *) arg1, arg2native, arg3native, arg4, arg5);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_addGroup
(JNIEnv* env, jobject obj, jlong arg1, jstring arg2, jstring arg3) {
	const char* arg2native = env->GetStringUTFChars(arg2, 0);
	const char* arg3native = env->GetStringUTFChars(arg3, 0);

	instance.addGroup((NVGSDK_HANDLE*)arg1, arg2native, arg3native);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_showHighlightsEditor
(JNIEnv* env, jobject obj, jlong arg1, jstring arg2) {
	const char* arg2native = env->GetStringUTFChars(arg2, 0);

	instance.showHighlightsEditor((NVGSDK_HANDLE*)arg1, arg2native);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_closeGroup
(JNIEnv* env, jobject obj, jlong arg1, jstring arg2, jboolean arg3) {
	const char* arg2native = env->GetStringUTFChars(arg2, 0);

	instance.closeGroup((NVGSDK_HANDLE*)arg1, arg2native, (bool) arg3);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_getNumOfHighlights
(JNIEnv* env, jobject obj, jlong arg1, jstring arg2) {
	const char* arg2native = env->GetStringUTFChars(arg2, 0);

	instance.getNumOfHighlights((NVGSDK_HANDLE*)arg1, arg2native);
}

