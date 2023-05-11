#include "meeting_raw_archiving_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingRawArchivingController* InitIMeetingRawArchivingControllerFunc(IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingRawArchivingController* pObj = pOwner->GetSDKObj()->GetMeetingRawArchivingController();
		return pObj;
	}
	return NULL;
}

//virtual SDKError StartRawArchiving() = 0;
IMPL_FUNC_0(IMeetingRawArchivingController, StartRawArchiving, SDKError, SDKERR_UNINITIALIZE)
//virtual SDKError StopRawArchiving() = 0;
IMPL_FUNC_0(IMeetingRawArchivingController, StopRawArchiving, SDKError, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE