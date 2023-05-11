#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingAANController* InitIMeetingAANControllerFunc(IMeetingServiceWrap* pOwner);
BEGIN_CLASS_DEFINE(IMeetingAANController)
NORMAL_CLASS(IMeetingAANController)
void Init_Wrap(IMeetingServiceWrap * pOwner) { m_obj = InitIMeetingAANControllerFunc(pOwner); }
void Uninit_Wrap(){ m_obj=NULL;};

//virtual SDKError ShowAANPanel(unsigned int x, unsigned int y) = 0;
DEFINE_FUNC_2(ShowAANPanel, SDKError, unsigned int, x, unsigned int, y)
//virtual SDKError HideAANPanel() = 0;
DEFINE_FUNC_0(HideAANPanel, SDKError)

END_CLASS_DEFINE(IMeetingAANController)
END_ZOOM_SDK_NAMESPACE