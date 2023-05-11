#include "meeting_closedcaption_ctrl_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IClosedCaptionController* InitIClosedCaptionControllerFunc(IClosedCaptionControllerEvent* pEvent, IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IClosedCaptionController* pObj = pOwner->GetSDKObj()->GetMeetingClosedCaptionController();
		if (pObj)
		{
			pObj->SetEvent(pEvent);
		}
		return pObj;
	}

	return NULL;
}

void UninitIClosedCaptionControllerFunc(IClosedCaptionController* obj)
{
	if (obj)
	{
		obj->SetEvent(NULL);
	}
}

//virtual bool IsMeetingSupportCC() = 0;
IMPL_FUNC_0(IClosedCaptionController, IsMeetingSupportCC, bool, false)
//virtual bool CanAssignOthersToSendCC() = 0;
IMPL_FUNC_0(IClosedCaptionController, CanAssignOthersToSendCC, bool, false)
//virtual SDKError AssignCCPriviledge(unsigned int userid, bool bAssigned) = 0;
IMPL_FUNC_2(IClosedCaptionController, AssignCCPriviledge, SDKError, unsigned int, userid, bool, bAssigned, SDKERR_UNINITIALIZE)
//virtual bool CanBeAssignedToSendCC(unsigned int userid) = 0;
IMPL_FUNC_1(IClosedCaptionController, CanBeAssignedToSendCC, bool, unsigned int, userid, false)
//virtual bool CanSendClosedCaption() = 0;
IMPL_FUNC_0(IClosedCaptionController, CanSendClosedCaption, bool, false)
//virtual SDKError SendClosedCaption(const wchar_t* ccMsg) = 0;
IMPL_FUNC_1(IClosedCaptionController, SendClosedCaption, SDKError, const wchar_t*, ccMsg, SDKERR_UNINITIALIZE)
//virtual bool IsSaveCCEnabled() = 0;
IMPL_FUNC_0(IClosedCaptionController, IsSaveCCEnabled, bool, false)
//virtual SDKError SaveCCHistory() = 0;
IMPL_FUNC_0(IClosedCaptionController, SaveCCHistory, SDKError, SDKERR_UNINITIALIZE)
//virtual const wchar_t* GetClosedCaptionHistorySavedPath() = 0;
IMPL_FUNC_0(IClosedCaptionController, GetClosedCaptionHistorySavedPath, const wchar_t*, NULL)
//virtual const wchar_t* GetClosedCaptionUrlFor3rdParty() = 0;
IMPL_FUNC_0(IClosedCaptionController, GetClosedCaptionUrlFor3rdParty, const wchar_t*, NULL)
//virtual bool IsLiveTranscriptLegalNoticeAvailable() = 0;
IMPL_FUNC_0(IClosedCaptionController, IsLiveTranscriptLegalNoticeAvailable, bool, false)
//virtual const wchar_t* getLiveTranscriptChatLegalNoticesPrompt() = 0;
IMPL_FUNC_0(IClosedCaptionController, getLiveTranscriptLegalNoticesPrompt, const wchar_t*, NULL)
//virtual const wchar_t* getLiveTranscriptLegalNoticesExplained() = 0;
IMPL_FUNC_0(IClosedCaptionController, getLiveTranscriptLegalNoticesExplained, const wchar_t*, NULL)
//virtual bool IsLiveTranscriptionFeatureEnabled() = 0;
IMPL_FUNC_0(IClosedCaptionController, IsLiveTranscriptionFeatureEnabled, bool, false)
//virtual SDKLiveTranscriptionStatus GetLiveTranscriptionStatus() = 0;
IMPL_FUNC_0(IClosedCaptionController, GetLiveTranscriptionStatus, SDKLiveTranscriptionStatus, SDK_LiveTranscription_Status_Stop)
//virtual bool CanStartLiveTranscription() = 0;
IMPL_FUNC_0(IClosedCaptionController, CanStartLiveTranscription, bool, false)
//virtual SDKError StartLiveTranscription() = 0;
IMPL_FUNC_0(IClosedCaptionController, StartLiveTranscription, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError StopLiveTranscription() = 0;
IMPL_FUNC_0(IClosedCaptionController, StopLiveTranscription, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError EnableRequestLiveTranscription(bool bEnable) = 0;
IMPL_FUNC_1(IClosedCaptionController, EnableRequestLiveTranscription, SDKError, bool, bEnable, SDKERR_UNINITIALIZE)
//virtual bool IsRequestToStartLiveTranscriptionEnalbed() = 0;
IMPL_FUNC_0(IClosedCaptionController, IsRequestToStartLiveTranscriptionEnalbed, bool, false)
//virtual SDKError RequestToStartLiveTranscription(bool bRequestAnonymous) = 0;
IMPL_FUNC_1(IClosedCaptionController, RequestToStartLiveTranscription, SDKError, bool, bRequestAnonymous, SDKERR_UNINITIALIZE)
END_ZOOM_SDK_NAMESPACE