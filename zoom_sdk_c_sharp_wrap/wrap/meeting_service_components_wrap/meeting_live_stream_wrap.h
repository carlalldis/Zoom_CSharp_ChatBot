#pragma once
#include "common_include.h"

BEGIN_ZOOM_SDK_NAMESPACE

class IMeetingLiveStreamItemImpl : public IMeetingLiveStreamItem
{
public:
	IMeetingLiveStreamItemImpl(const wchar_t* liveStreamURL, const wchar_t* liveStreamURLDescription);
	virtual ~IMeetingLiveStreamItemImpl() {};

	virtual void SetLiveStreamURL(const wchar_t* liveStreamURL);
	virtual const wchar_t* GetLiveStreamURL();
	virtual void SetLiveStreamURLDescription(const wchar_t* liveStreamURLDescription);
	virtual const wchar_t* GetLiveStreamURLDescription();

private:
	std::wstring _liveStreamURL;
	std::wstring _liveStreamURLDescription;
};


class IMeetingServiceWrap;
IMeetingLiveStreamController* InitIMeetingLiveStreamControllerFunc(IMeetingLiveStreamCtrlEvent* pEvent, IMeetingServiceWrap* pOwner);
void UninitIMeetingLiveStreamControllerFunc(IMeetingLiveStreamController* obj);
BEGIN_CLASS_DEFINE_WITHCALLBACK(IMeetingLiveStreamController, IMeetingLiveStreamCtrlEvent)
NORMAL_CLASS(IMeetingLiveStreamController)
INIT_UNINIT_WITHEVENT_AND_OWNSERVICE(IMeetingLiveStreamController, IMeetingServiceWrap)
virtual SDKError SetEvent(IMeetingLiveStreamCtrlEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}
// virtual SDKError CanStartLiveStream() = 0;
DEFINE_FUNC_0(CanStartLiveStream, SDKError)
// virtual SDKError StartLiveStream(IMeetingLiveStreamItem* item_) = 0;
DEFINE_FUNC_1(StartLiveStream, SDKError, IMeetingLiveStreamItem*, item_)
// virtual SDKError StartLiveStreamWithSteamingURL(const wchar_t* streamingURL, const wchar_t* streamingKey, const wchar_t* broadcastURL) = 0;
DEFINE_FUNC_3(StartLiveStreamWithSteamingURL, SDKError, const wchar_t*, streamingURL, const wchar_t*, streamingKey, const wchar_t*, broadcastURL)
// virtual SDKError StopLiveStream() = 0;
DEFINE_FUNC_0(StopLiveStream, SDKError)
// virtual IList<IMeetingLiveStreamItem* >* GetSupportLiveStreamURL() = 0;
DEFINE_FUNC_0(GetSupportLiveStreamURL, IList<IMeetingLiveStreamItem*>*)
// virtual LiveStreamStatus GetCurrentLiveStreamStatus() = 0;
DEFINE_FUNC_0(GetCurrentLiveStreamStatus, LiveStreamStatus)
//virtual SDKError StartRawLiveStream(const wchar_t* broadcastURL) = 0;
DEFINE_FUNC_1(StartRawLiveStream, SDKError, const wchar_t*, broadcastURL)
//virtual SDKError StopRawLiveStream() = 0;
DEFINE_FUNC_0(StopRawLiveStream, SDKError)
//virtual bool IsRawLiveStreamSupported() = 0;
DEFINE_FUNC_0(IsRawLiveStreamSupported, bool)
//virtual SDKError CanStartRawLiveStream() = 0;
DEFINE_FUNC_0(CanStartRawLiveStream, SDKError)
//virtual SDKError RequestRawLiveStream(const wchar_t* broadcastURL) = 0;
DEFINE_FUNC_1(RequestRawLiveStream, SDKError, const wchar_t*, broadcastURL)
//virtual SDKError RemoveRawLiveStreamPrivilege(unsigned int userid) = 0;
DEFINE_FUNC_1(RemoveRawLiveStreamPrivilege, SDKError, unsigned int, userid)
//virtual IList<RawLiveStreamInfo>* GetRawLiveStreamingInfoList() = 0;
DEFINE_FUNC_0(GetRawLiveStreamingInfoList, IList<RawLiveStreamInfo>*)
//virtual IList<unsigned int>* GetRawLiveStreamPrivilegeUserList() = 0;
DEFINE_FUNC_0(GetRawLiveStreamPrivilegeUserList, IList<unsigned int>*)

// virtual void onLiveStreamStatusChange(LiveStreamStatus status) = 0;
CallBack_FUNC_1(onLiveStreamStatusChange, LiveStreamStatus, status)
//virtual void onRawLiveStreamPrivilegeChanged(bool bHasPrivilege) = 0;
CallBack_FUNC_1(onRawLiveStreamPrivilegeChanged, bool, bHasPrivilege)
//virtual void onRawLiveStreamPrivilegeRequestTimeout() = 0;
CallBack_FUNC_0(onRawLiveStreamPrivilegeRequestTimeout)
//virtual void onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege) = 0;
CallBack_FUNC_2(onUserRawLiveStreamPrivilegeChanged, unsigned int, userid, bool, bHasPrivilege)
//virtual void onRawLiveStreamPrivilegeRequested(IRequestRawLiveStreamPrivilegeHandler* handler) = 0;
CallBack_FUNC_1(onRawLiveStreamPrivilegeRequested, IRequestRawLiveStreamPrivilegeHandler*, handler)
//virtual void onUserRawLiveStreamingStatusChanged(IList<RawLiveStreamInfo>* liveStreamList) = 0;
CallBack_FUNC_1(onUserRawLiveStreamingStatusChanged, IList<RawLiveStreamInfo>*, liveStreamList)

END_CLASS_DEFINE(IMeetingLiveStreamController)
END_ZOOM_SDK_NAMESPACE
