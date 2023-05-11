#include "stdafx.h"
#include "meeting_video_dotnet_wrap.h"
#include "zoom_sdk_dotnet_wrap_util.h"
#include "wrap/sdk_wrap.h"
namespace ZOOM_SDK_DOTNET_WRAP {
	
	//translate event
	class MeetingVideoControllerEventHanlder
	{
	public:
		static MeetingVideoControllerEventHanlder& GetInst()
		{
			static MeetingVideoControllerEventHanlder inst;
			return inst;
		}
		void onUserVideoStatusChange(unsigned int userId, ZOOM_SDK_NAMESPACE::VideoStatus status)
		{
			if (CMeetingVideoControllerDotNetWrap::Instance)
				CMeetingVideoControllerDotNetWrap::Instance->ProcUserVideoStatusChange(userId, (VideoStatus)status);
		}

		void onSpotlightedUserListChangeNotification(ZOOM_SDK_NAMESPACE::IList<unsigned int>* plstSpotlightedUserID)
		{
			if (CMeetingVideoControllerDotNetWrap::Instance)
				CMeetingVideoControllerDotNetWrap::Instance->ProcSpotlightedUserListChangeNotification(Convert(plstSpotlightedUserID));
		}
	private:
		MeetingVideoControllerEventHanlder() {}
	};
	//

	void CMeetingVideoControllerDotNetWrap::BindEvent()
	{
		ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().GetMeetingVideoController().m_cbonUserVideoStatusChange =
			std::bind(&MeetingVideoControllerEventHanlder::onUserVideoStatusChange,
				&MeetingVideoControllerEventHanlder::GetInst(), std::placeholders::_1, std::placeholders::_2);

		ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().GetMeetingVideoController().m_cbonSpotlightedUserListChangeNotification =
			std::bind(&MeetingVideoControllerEventHanlder::onSpotlightedUserListChangeNotification,
				&MeetingVideoControllerEventHanlder::GetInst(), std::placeholders::_1);
	}

	void CMeetingVideoControllerDotNetWrap::ProcUserVideoStatusChange(unsigned int userId, VideoStatus status)
	{
		event_onUserVideoStatusChange(userId, status);
	}

	void CMeetingVideoControllerDotNetWrap::ProcSpotlightedUserListChangeNotification(array<unsigned int>^ lstSpotlightedUserID)
	{
		event_onSpotlightVideoChangeNotification(lstSpotlightedUserID);
	}

	SDKError CMeetingVideoControllerDotNetWrap::MuteVideo()
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().MuteVideo();
	}

	SDKError CMeetingVideoControllerDotNetWrap::UnmuteVideo()
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().UnmuteVideo();
	}

	SDKError CMeetingVideoControllerDotNetWrap::PinVideoToFirstView(unsigned int userid)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().PinVideoToFirstView(userid);
	}
	SDKError CMeetingVideoControllerDotNetWrap::UnPinVideoFromFirstView(unsigned int userid)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().UnPinVideoFromFirstView(userid);
	}
	SDKError CMeetingVideoControllerDotNetWrap::SpotlightVideo(unsigned int userid)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().SpotlightVideo(userid);
	}
	SDKError CMeetingVideoControllerDotNetWrap::UnSpotlightVideo(unsigned int userid)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().UnSpotlightVideo(userid);
	}
	SDKError CMeetingVideoControllerDotNetWrap::HideOrShowNoVideoUserOnVideoWall(bool bHide)
	{
		return (SDKError)ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().
			GetMeetingVideoController().HideOrShowNoVideoUserOnVideoWall(bHide);
	}
}