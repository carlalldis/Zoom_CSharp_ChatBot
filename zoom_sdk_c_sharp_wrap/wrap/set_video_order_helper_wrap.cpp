#include "set_video_order_helper_wrap.h"
#include "./meeting_service_components_wrap/meeting_video_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
ISetVideoOrderHelper* InitISetVideoOrderHelperFunc(IMeetingVideoControllerWrap* pOwner)
{
	if (pOwner && pOwner->GetSDKObj())
	{
		ZOOM_SDK_NAMESPACE::ISetVideoOrderHelper* pObj = pOwner->GetSDKObj()->GetSetVideoOrderHelper();
		return pObj;
	}

	return NULL;
}

void UninitISetVideoOrderHelperFunc(ISetVideoOrderHelper* obj)
{
	
}

//virtual SDKError SetVideoOrderTransactionBegin() = 0;
IMPL_FUNC_0(ISetVideoOrderHelper, SetVideoOrderTransactionBegin, SDKError, SDKERR_UNINITIALIZE)
//virtual bool AddVideoToOrder(unsigned int userId, unsigned int position) = 0;
IMPL_FUNC_2(ISetVideoOrderHelper, AddVideoToOrder, bool, unsigned int, userId, unsigned int, position, false)
//virtual SDKError SetVideoOrderTransactionCommit() = 0;
IMPL_FUNC_0(ISetVideoOrderHelper, SetVideoOrderTransactionCommit, SDKError, SDKERR_UNINITIALIZE)

END_ZOOM_SDK_NAMESPACE