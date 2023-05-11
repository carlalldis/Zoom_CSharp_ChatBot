#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingChatController* InitIMeetingChatControllerFunc(IMeetingChatCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingChatControllerFunc(IMeetingChatController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingChatController, IMeetingChatCtrlEvent)
NORMAL_CLASS(IMeetingChatController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingChatController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingChatCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual SDKError SendChatMsgTo(wchar_t* content, unsigned int receiver, SDKChatMessageType type) = 0;
DEFINE_FUNC_3(SendChatMsgTo, SDKError, wchar_t*, content, unsigned int, receiver, SDKChatMessageType, type)
//virtual const ChatStatus* GetChatStatus() = 0;
DEFINE_FUNC_0(GetChatStatus, const ChatStatus*)
//virtual bool IsMeetingChatLegalNoticeAvailable() = 0;
DEFINE_FUNC_0(IsMeetingChatLegalNoticeAvailable, bool)
//virtual const wchar_t* getChatLegalNoticesPrompt() = 0;
DEFINE_FUNC_0(getChatLegalNoticesPrompt, const wchar_t*)
//virtual const wchar_t* getChatLegalNoticesExplained() = 0;
DEFINE_FUNC_0(getChatLegalNoticesExplained, const wchar_t*)
//virtual SDKError SetParticipantsChatPriviledge(SDKChatPriviledge priviledge) = 0;
DEFINE_FUNC_1(SetParticipantsChatPriviledge, SDKError, SDKChatPriviledge, priviledge)
//virtual bool IsChatMessageCanBeDeleted(const wchar_t* msgID) = 0;
DEFINE_FUNC_1(IsChatMessageCanBeDeleted, bool, const wchar_t*, msgID)
//virtual SDKError DeleteChatMessage(const wchar_t* msgID) = 0;
DEFINE_FUNC_1(DeleteChatMessage, SDKError, const wchar_t*, msgID)
//virtual IList<const wchar_t*>* GetAllChatMessageID() = 0;
DEFINE_FUNC_0(GetAllChatMessageID, IList<const wchar_t*>*)
 
//virtual void onChatMsgNotifcation(IChatMsgInfo* chatMsg, const wchar_t* content = NULL) = 0;
CallBack_FUNC_2(onChatMsgNotifcation, IChatMsgInfo*, chatMsg, const wchar_t*, content)
//virtual void onChatStautsChangedNotification(ChatStatus* status_) = 0;
CallBack_FUNC_1(onChatStautsChangedNotification, ChatStatus*, status_)
//virtual void onChatMsgDeleteNotification(const wchar_t* msgID, SDKChatMessageDeleteType deleteBy) = 0;
CallBack_FUNC_2(onChatMsgDeleteNotification, const wchar_t*, msgID, SDKChatMessageDeleteType, deleteBy)
//virtual void onShareMeetingChatStatusChanged(bool isStart) = 0;
CallBack_FUNC_1(onShareMeetingChatStatusChanged, bool, isStart)
END_CLASS_DEFINE(IMeetingChatController)
END_ZOOM_SDK_NAMESPACE