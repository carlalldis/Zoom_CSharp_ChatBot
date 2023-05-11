#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingServiceWrap;
IMeetingRawArchivingController* InitIMeetingRawArchivingControllerFunc(IMeetingServiceWrap* pOwner);
BEGIN_CLASS_DEFINE(IMeetingRawArchivingController)
NORMAL_CLASS(IMeetingRawArchivingController)
void Init_Wrap(IMeetingServiceWrap * pOwner) { m_obj = InitIMeetingRawArchivingControllerFunc(pOwner); }
void Uninit_Wrap(){ m_obj=NULL;};

//virtual SDKError StartRawArchiving() = 0;
DEFINE_FUNC_0(StartRawArchiving, SDKError)
//virtual SDKError StopRawArchiving() = 0;
DEFINE_FUNC_0(StopRawArchiving, SDKError)

END_CLASS_DEFINE(IMeetingRawArchivingController)
END_ZOOM_SDK_NAMESPACE