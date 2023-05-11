#include "meeting_AAN_helper_wrap.h"
#include "meeting_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
IMeetingAANController* InitIMeetingAANControllerFunc(IMeetingServiceWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::IMeetingAANController* pObj = pOwner->GetSDKObj()->GetMeetingAANController();
		return pObj;
	}
	return NULL;
}

//virtual SDKError ShowAANPanel(unsigned int x, unsigned int y) = 0;
IMPL_FUNC_2(IMeetingAANController, ShowAANPanel, SDKError, unsigned int, x, unsigned int, y, SDKERR_UNINITIALIZE)
//virtual SDKError HideAANPanel() = 0;
IMPL_FUNC_0(IMeetingAANController, HideAANPanel, SDKError, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE