#pragma once
#ifdef _NEED_STDAFX_
#include "StdAfx.h"
#endif
#include <windows.h>
#if (defined JS_WRAP)
#include "zoom_sdk.h"
#include "meeting_service_interface.h"
#include "auth_service_interface.h"
#include "setting_service_interface.h"
#include "direct_share_helper_interface.h"
#include "customized_resource_helper_interface.h"
#include "customized_ui/customized_annotation.h"
#include "customized_ui/customized_local_recording.h"
#include "customized_ui/customized_share_render.h"
#include "customized_ui/customized_ui_mgr.h"
#include "customized_ui/customized_video_container.h"
#include "customized_ui/zoom_customized_ui.h"
#include "meeting_service_components/meeting_annotation_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include "meeting_service_components/meeting_emoji_reaction_interface.h"
#include "meeting_service_components/meeting_chat_interface.h"
#include "meeting_service_components/meeting_configuration_interface.h"
#include "meeting_service_components/meeting_h323_helper_interface.h"
#include "meeting_service_components/meeting_live_stream_interface.h"
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_phone_helper_interface.h"
#include "meeting_service_components/meeting_recording_interface.h"
#include "meeting_service_components/meeting_remote_ctrl_interface.h"
#include "meeting_service_components/meeting_sharing_interface.h"
#include "meeting_service_components/meeting_ui_ctrl_interface.h"
#include "meeting_service_components/meeting_video_interface.h"
#include "meeting_service_components/meeting_waiting_room_interface.h"
#include "meeting_service_components/meeting_webinar_interface.h"
#include "meeting_service_components/meeting_closedcaption_interface.h"
#include "meeting_service_components/meeting_interpretation_interface.h"
#include "meeting_service_components/meeting_aan_interface.h"
#include "meeting_service_components/meeting_raw_archiving_interface.h"
#include "zoom_sdk_ext.h"
#include "zoom_sdk_util_define.h"
#include "network_connection_handler_interface.h"
#include "ui_hook_interface.h"
#include "zoom_sdk_platform.h"

#include "rawdata/zoom_rawdata_api.h"
#include "rawdata/rawdata_video_source_helper_interface.h"
#include "rawdata/rawdata_renderer_interface.h"
#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk_raw_data_def.h"
#include "zoom_sdk_sms_helper_interface.h"
#elif (defined CSHARP_WRAP) || (defined INTEL_UNITE)
#include "zoom_sdk.h"
#include "meeting_service_interface.h"
#include "auth_service_interface.h"
#include "setting_service_interface.h"
#include "direct_share_helper_interface.h"
#include "customized_resource_helper_interface.h"
#include "customized_ui/customized_annotation.h"
#include "customized_ui/customized_local_recording.h"
#include "customized_ui/customized_share_render.h"
#include "customized_ui/customized_ui_mgr.h"
#include "customized_ui/customized_video_container.h"
#include "customized_ui/zoom_customized_ui.h"
#include "meeting_service_components/meeting_annotation_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include "meeting_service_components/meeting_emoji_reaction_interface.h"
#include "meeting_service_components/meeting_chat_interface.h"
#include "meeting_service_components/meeting_configuration_interface.h"
#include "meeting_service_components/meeting_h323_helper_interface.h"
#include "meeting_service_components/meeting_live_stream_interface.h"
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_phone_helper_interface.h"
#include "meeting_service_components/meeting_recording_interface.h"
#include "meeting_service_components/meeting_remote_ctrl_interface.h"
#include "meeting_service_components/meeting_sharing_interface.h"
#include "meeting_service_components/meeting_ui_ctrl_interface.h"
#include "meeting_service_components/meeting_video_interface.h"
#include "meeting_service_components/meeting_waiting_room_interface.h"
#include "meeting_service_components/meeting_webinar_interface.h"
#include "meeting_service_components/meeting_closedcaption_interface.h"
#include "meeting_service_components/meeting_interpretation_interface.h"
#include "meeting_service_components/meeting_aan_interface.h"
#include "meeting_service_components/meeting_raw_archiving_interface.h"
#include "zoom_sdk_ext.h"
#include "zoom_sdk_util_define.h"
#include "network_connection_handler_interface.h"
#include "ui_hook_interface.h"
#include "zoom_sdk_platform.h"

#include "rawdata/zoom_rawdata_api.h"
#include "rawdata/rawdata_video_source_helper_interface.h"
#include "rawdata/rawdata_renderer_interface.h"
#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk_raw_data_def.h"
#include "zoom_sdk_sms_helper_interface.h"
#endif
#include <string>
#include <functional>
#include "macro_define.h"
inline void myOutputDebugString(const char* format, ...)
{
	//return;
	va_list args;
	va_start(args, format);
	char szData[2048] = { 0 };
	vsnprintf(szData, 2048, format, args);  //checked safe

	OutputDebugStringA(szData);
	va_end(args);
}
inline void myOutputDebugStringW(const wchar_t* format, ...)
{
	//return;
	va_list args;
	va_start(args, format);
	wchar_t szData[2048] = { 0 };
	_vsnwprintf_s(szData, 2048, _TRUNCATE, format, args);

	OutputDebugStringW(szData);
	va_end(args);
}

_inline std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, &r[0], len);
	return r;
}

_inline std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

#include "sdk_loader.h"