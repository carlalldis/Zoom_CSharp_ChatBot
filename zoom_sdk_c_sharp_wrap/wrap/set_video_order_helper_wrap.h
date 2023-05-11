#pragma once
#include "common_include.h"
BEGIN_ZOOM_SDK_NAMESPACE
class IMeetingVideoControllerWrap;
ISetVideoOrderHelper* InitISetVideoOrderHelperFunc(IMeetingVideoControllerWrap* pOwner);
void UninitISetVideoOrderHelperFunc(ISetVideoOrderHelper* obj);
BEGIN_CLASS_DEFINE(ISetVideoOrderHelper)
NORMAL_CLASS(ISetVideoOrderHelper)
INIT_UNINIT_OWNSERVICE(ISetVideoOrderHelper, IMeetingVideoControllerWrap)

//virtual SDKError SetVideoOrderTransactionBegin() = 0;
DEFINE_FUNC_0(SetVideoOrderTransactionBegin, SDKError)
//virtual bool AddVideoToOrder(unsigned int userId, unsigned int position) = 0;
DEFINE_FUNC_2(AddVideoToOrder, bool, unsigned int, userId, unsigned int, position)
//virtual SDKError SetVideoOrderTransactionCommit() = 0;
DEFINE_FUNC_0(SetVideoOrderTransactionCommit, SDKError)

END_CLASS_DEFINE(ISetVideoOrderHelper)
END_ZOOM_SDK_NAMESPACE
