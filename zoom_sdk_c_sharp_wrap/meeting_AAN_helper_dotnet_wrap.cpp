#include "stdafx.h"
#include "meeting_AAN_helper_dotnet_wrap.h"
#include "zoom_sdk_dotnet_wrap_util.h"
#include "wrap/sdk_wrap.h"
namespace ZOOM_SDK_DOTNET_WRAP {

	SDKError CMeetingAANControllerDotNetWrap::ShowAANPanel(unsigned int x, unsigned int y)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingAANController().ShowAANPanel(x, y);
	}
	SDKError CMeetingAANControllerDotNetWrap::HideAANPanel()
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingAANController().HideAANPanel();
	}
}