#pragma once
using namespace System;
#include "zoom_sdk_dotnet_wrap_def.h"
namespace ZOOM_SDK_DOTNET_WRAP {

	public interface class IMeetingAANControllerDotNetWrap
	{
	public:
		SDKError ShowAANPanel(unsigned int x, unsigned int y);
		SDKError HideAANPanel();
	};

	private ref class CMeetingAANControllerDotNetWrap sealed : public IMeetingAANControllerDotNetWrap
	{
	public:
		static property CMeetingAANControllerDotNetWrap^ Instance
		{
			CMeetingAANControllerDotNetWrap^ get() { return m_Instance; }
		}

		virtual SDKError ShowAANPanel(unsigned int x, unsigned int y);
		virtual SDKError HideAANPanel();

	private:
		static CMeetingAANControllerDotNetWrap^ m_Instance = gcnew CMeetingAANControllerDotNetWrap;
	};
}