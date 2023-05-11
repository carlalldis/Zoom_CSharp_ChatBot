#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingWaitingRoomController* InitIMeetingWaitingRoomControllerFunc(IMeetingWaitingRoomEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingWaitingRoomControllerFunc(IMeetingWaitingRoomController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingWaitingRoomController, IMeetingWaitingRoomEvent)
NORMAL_CLASS(IMeetingWaitingRoomController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingWaitingRoomController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingWaitingRoomEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
//virtual bool IsSupportWaitingRoom() = 0;
DEFINE_FUNC_0(IsSupportWaitingRoom, bool)
//virtual bool IsWaitingRoomOnEntryFlagOn() = 0;
DEFINE_FUNC_0(IsWaitingRoomOnEntryFlagOn, bool)
//virtual SDKError EnableWaitingRoomOnEntry(bool bEnable) = 0;
DEFINE_FUNC_1(EnableWaitingRoomOnEntry, SDKError, bool, bEnable)
//virtual IList<unsigned int >* GetWaitingRoomLst() = 0;
DEFINE_FUNC_0(GetWaitingRoomLst, IList<unsigned int >*)
//virtual IUserInfo* GetWaitingRoomUserInfoByID(unsigned int userid) = 0;
DEFINE_FUNC_1(GetWaitingRoomUserInfoByID, IUserInfo*, unsigned int, userid)
//virtual SDKError AdmitToMeeting(unsigned int userid) = 0;
DEFINE_FUNC_1(AdmitToMeeting, SDKError, unsigned int, userid)
//virtual SDKError PutInWaitingRoom(unsigned int userid) = 0;
DEFINE_FUNC_1(PutInWaitingRoom, SDKError, unsigned int, userid)
//virtual bool IsAudioEnabledInWaitingRoom() = 0;
DEFINE_FUNC_0(IsAudioEnabledInWaitingRoom, bool)
//virtual bool IsVideoEnabledInWaitingRoom() = 0;
DEFINE_FUNC_0(IsVideoEnabledInWaitingRoom, bool)

//virtual void onWatingRoomUserJoin(unsigned int userID) = 0;
CallBack_FUNC_1(onWatingRoomUserJoin, unsigned int, userID)
//virtual void onWatingRoomUserLeft(unsigned int userID) = 0;
CallBack_FUNC_1(onWatingRoomUserLeft, unsigned int, userID)
//virtual void onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn) = 0;
CallBack_FUNC_1(onWaitingRoomPresetAudioStatusChanged, bool, bAudioCanTurnOn)
//virtual void onWaitingRoomPresetVideoStatusChanged( bool bVideoCanTurnOn) = 0;
CallBack_FUNC_1(onWaitingRoomPresetVideoStatusChanged, bool, bVideoCanTurnOn)
//virtual void onCustomWaitingRoomDataUpdated(CustomWaitingRoomData& bData, IWaitingRoomDataDownloadHandler* bHandler) = 0;
CallBack_FUNC_2(onCustomWaitingRoomDataUpdated, CustomWaitingRoomData&, bData, IWaitingRoomDataDownloadHandler*, bHandler)
//virtual void onWaitingRoomUserNameChanged(unsigned int userID, const wchar_t* userName) = 0;
CallBack_FUNC_2(onWaitingRoomUserNameChanged, unsigned int, userID, const wchar_t*, userName)
END_CLASS_DEFINE(IMeetingWaitingRoomController)
END_ZOOM_SDK_NAMESPACE