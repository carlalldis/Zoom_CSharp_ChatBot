/*!
* \file meeting_breakout_rooms_interface2.h
* \brief Meeting Service Breakout Room Interface
* Valid for both ZOOM style and user custom interface mode.
*
*	//////////////////////////// Creator ////////////////////////////
*	1. Main Functions:
*		1) create|delete|rename BO
*		2) assign|remove user to BO
*       3) set BO option
*	2. Remarks:
*       1) These editing can only be done before BO is started
*
*	//////////////////////////// Admin ////////////////////////////
*   1. Main Functions:
*		1) after BO is started, assign new user to BO,
*		2) after BO is started, switch user from BO-A to BO-B
*       3) stop BO
*		4) start BO
*
*	//////////////////////////// Assistant ////////////////////////////
*	1. Main Functions:
*		1) join BO with BO id
*		2) leave BO
*
*   //////////////////////////// Attendee ////////////////////////////
*   1. Main Functions:
*		1) join BO
*       2) leave BO
*       3) request help
*
*	//////////////////////////// DataHelper ////////////////////////////
*	1. Main Functions:
*		1) get unassigned user list
*		2) get BO list
*       3) get BO object
*
*
*	host in master conference     : creator + admin + assistant + dataHelper
*	host in BO conference         : admin + assistant + dataHelper
*	CoHost in master conference   : [attendee] or [creator + admin + assistant + dataHelper]
*	CoHost in BO conference       : [attendee] or [admin + assistant + dataHelper]
*	attendee in master conference : attendee + [assistant + dataHelper]
*   attendee in BO conference     : attendee + [assistant + dataHelper]
*   
*   Import Remarks: 
*   1. attendee in master conference/attendee in BO conference
*	   1) if BOOption.IsParticipantCanChooseBO is true, attendee has objects:  [attendee + assistant + dataHelper]
*      2) if BOOption.IsParticipantCanChooseBO is false, attendee has object:  [attendee]
*   2. CoHost in master conference
*	   1) if CoHost is desktop client, and host is desktop client, the CoHost has objects: [creator + admin + assistant + dataHelper]
*      2) if CoHost is desktop client, and host is mobile client, the CoHost has object: [attendee]
*      3) if CoHost is mobile client, the CoHost has object: [attendee]
*/

#ifndef _MEETING_BREAKOUT_ROOMS_INTERFACE2_H_
#define _MEETING_BREAKOUT_ROOMS_INTERFACE2_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

typedef enum
{
	BO_CTRL_USER_STATUS_UNASSIGNED			= 1, ///<User is in main conference, not assigned to BO
	BO_CTRL_USER_STATUS_ASSIGNED_NOT_JOIN   = 2, ///<User is assigned to BO, but not join
	BO_CTRL_USER_STATUS_IN_BO				= 3, ///<User is already in BO
	BO_CTRL_USER_STATUS_UNKNOWN             = 4, ///<Unknown status
}BO_CTRL_USER_STATUS;

typedef enum
{
	ATTENDEE_REQUEST_FOR_HELP_RESULT_IDLE,	            ///<host receive the help request and there is no other one currently requesting for help
	ATTENDEE_REQUEST_FOR_HELP_RESULT_BUSY,	            ///<host is handling other's request with the request dialog
	ATTENDEE_REQUEST_FOR_HELP_RESULT_IGNORE,	        ///<host click "later" button or close the request dialog directly
	ATTENDEE_REQUEST_FOR_HELP_RESULT_HOST_ALREADY_IN_BO	///<host already in your BO meeting
}ATTENDEE_REQUEST_FOR_HELP_RESULT;

/// \brief BO interface.
///
class IBOMeeting
{
public:
	virtual ~IBOMeeting() {}

	/// \brief Get BO ID.
	virtual const wchar_t* GetBOID() = 0;

	/// \brief Get BO name.
	virtual const wchar_t* GetBOName() = 0;

	/// \brief Get user ID list in the BO.
	/// \return If the function succeeds, the return value is a pointer to IList object. For more details, see \link IList \endlink,
	virtual IList<const wchar_t*>* GetBOUserList() = 0;
};

////////////////////////////////////////// IBOCreator //////////////////////////////////////////
/// \brief BO creator callback handler.
///
class IBOCreatorEvent 
{
public:
	virtual ~IBOCreatorEvent() {}

	/// \brief If CreateBO successfully, you will receive the event. Make sure you receive the event before start bo.
	/// \param strBOID, to indicate which bo has been created successfully.
	virtual void onBOCreateSuccess(const wchar_t* strBOID) = 0;
};

/// \brief enum for BO stop countdown
enum BO_STOP_COUNTDOWN
{
	BO_STOP_NOT_COUNTDOWN,
	BO_STOP_COUNTDOWN_SECONDS_10,
	BO_STOP_COUNTDOWN_SECONDS_15,
	BO_STOP_COUNTDOWN_SECONDS_30,
	BO_STOP_COUNTDOWN_SECONDS_60,
	BO_STOP_COUNTDOWN_SECONDS_120,
};

/// \brief BO option.
struct BOOption
{
	BO_STOP_COUNTDOWN countdown_seconds; ///<Set the countdown after closing breakout room.
	bool IsParticipantCanChooseBO;       ///<Enable/Disable that participant can choose breakout room.
	bool IsParticipantCanReturnToMainSessionAtAnyTime; ///<Enable/Disable that participant can return to main session at any time.
	bool IsAutoMoveAllAssignedParticipantsEnabled;     ///<Enable/Disable that auto move all assigned participants to breakout room.
	bool IsBOTimerEnabled;         ///<true: it's timer BO false: not timer BO
	bool IsTimerAutoStopBOEnabled; ///<true: if time is up, will stop BO auto. false: don't auto stop.
	unsigned int nTimerDuration;   ///<seconds of BO timer duration, NOTE: when nTimerDuration is 0, it means that the BO duration is 30*60 seconds.

	BOOption()
	{
		countdown_seconds = BO_STOP_COUNTDOWN_SECONDS_60;
		IsParticipantCanChooseBO = false;
		IsParticipantCanReturnToMainSessionAtAnyTime = true;
		IsAutoMoveAllAssignedParticipantsEnabled = false;
		IsBOTimerEnabled = false;
		IsTimerAutoStopBOEnabled = false;
		nTimerDuration = 0;
	}
};

/// \brief Batch Creater BO helper interface.
///
class IBatchCreateBOHelper
{
public:
	/// \brief Prepare to batch create BO rooms.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum
	/// \remarks If the function succeeds, all the created BO rooms will be removed. And the prepared list you added by calling \link AddNewBoToList \endlink will be clear.
	virtual SDKError CreateBOTransactionBegin() = 0;

	/// \brief Add the BO name to a prepared list.
	/// \param strNewBOName, the BO name you want to create.
	/// \return true if the BO name is added to the prepared list successfully.
	/// \remarks The max number of the prepared list is 50. The max length of the BO room name is 32.
	/// \Remark CreateBOTransactionBegin() must be called before this function is called. Otherwise false will be returned.
	virtual bool AddNewBoToList(const wchar_t* strNewBOName) = 0;

	/// \brief Batch create BO rooms according to the prepare list.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum
	/// \Remarks CreateBOTransactionBegin() must be called before this function is called. Otherwise SDKErr_WRONG_USAGE will be returned.
	virtual SDKError CreateBoTransactionCommit() = 0;
};

/// \brief BO creator interface.
///
class IBOCreator
{
public:

	virtual void SetEvent(IBOCreatorEvent* pEvent) = 0;

	/// \brief Create a BO.
	/// \param strBOName, the BO name.
	/// \return if success the return value is BO ID, otherwise NULL.
	virtual const wchar_t* CreateBO(const wchar_t* strBOName) = 0;
	
	/// \brief Update BO name.
	/// \param strBOID, is the BO ID.
	/// \param strNewBOName, is the new BO name.
	/// \return if success the return value is true, otherwise false.
	virtual bool UpdateBOName(const wchar_t* strBOID, const wchar_t* strNewBOName) = 0; 
	
	/// \brief Remove a BO.
	/// \param strBOID, is the BO ID.
	/// \return if success the return value is true, otherwise false.
	virtual bool RemoveBO(const wchar_t* strBOID) = 0;
	
	/// \brief Assign a user to a BO.
	/// \param strUserID, is the user ID.
	/// \param strBOID, is the BO ID.
	/// \return if success the return value is true, otherwise false.
	virtual bool AssignUserToBO(const wchar_t* strUserID, const wchar_t* strBOID) = 0;
	
	/// \brief Remove some user from a BO.
	/// \param strUserID, is the user ID.
	/// \param strBOID, is the BO ID.
	/// \return if success the return value is true, otherwise false.
	virtual bool RemoveUserFromBO(const wchar_t* strUserID, const wchar_t* strBOID) = 0;									

	/// \brief Set BO option.
	/// \param option, the option that you want to set.
	/// \return if success the return value is true, otherwise false.
	virtual bool SetBOOption(const BOOption& option) = 0;
	
	/// \brief Get BO option
	/// \param option, Get the current bo option through this parameter.
	/// \return if success the return value is true, otherwise false.
	virtual bool GetBOOption(BOOption& option) = 0;

	/// \brief Get the Batch create bo controller.
	/// \return If the function succeeds, the return value is a pointer to IBatchCreateBOHelper. Otherwise returns NULL.
	virtual IBatchCreateBOHelper* GetBatchCreateBOHelper() = 0;
};

////////////////////////////////////////// IBOAdmin //////////////////////////////////////////
enum BOControllerError
{
	BOControllerError_NULL_POINTER = 0,
	BOControllerError_WRONG_CURRENT_STATUS,
	BOControllerError_TOKEN_NOT_READY,
	BOControllerError_NO_PRIVILEGE,
	BOControllerError_BO_LIST_IS_UPLOADING,
	BOControllerError_UPLOAD_FAIL,
	BOControllerError_NO_ONE_HAS_BEEN_ASSIGNED,
	BOControllerError_UNKNOWN = 100
};

/// \brief BO admin callback handler.
///
class IBOAdminEvent
{
public:

	virtual ~IBOAdminEvent() {}

	/// \brief when someone send the request help, notify it.
	/// \param strUserID, is the user ID which send the request help.
	virtual void onHelpRequestReceived(const wchar_t* strUserID) = 0;

	/// \brief when StartBO fail, you will receive the event.
	/// \param errCode, identify the specific error code for trouble shooting.
	virtual void onStartBOError(BOControllerError errCode) = 0;

	/// \brief if it's timer BO, after start BO, you will receive the event. 
	/// \param [remaining] remaining time, [isTimesUpNotice] true: when time is up, auto stop BO. false: don't auto stop BO. 
	virtual void onBOEndTimerUpdated(int remaining, bool isTimesUpNotice) = 0;
};

/// \brief BO admin interface.
///
class IBOAdmin
{
public:
	/// \brief start BO.
	/// \return true indicates success, otherwise fail.
	virtual bool StartBO() = 0;

	/// \brief stop BO.
	/// \return true indicates success, otherwise fail.
	virtual bool StopBO() = 0;
	
	/// \brief To set a unassigned user to a BO, when BO is started.
	/// \return true indicates success, otherwise fail.
	virtual bool AssignNewUserToRunningBO(const wchar_t* strUserID, const wchar_t* strBOID) = 0;
	
	/// \brief To Switch user to other BO, when BO is started.
	/// \return true indicates success, otherwise fail.
	virtual bool SwitchAssignedUserToRunningBO(const wchar_t* strUserID, const wchar_t* strBOID) = 0;
	
	/// \brief Determine if can start BO.
	/// \return true indicates can, otherwise can not.
	virtual bool CanStartBO() = 0;
	
	/// \brief Set admin callback handler.
	/// \param pEvent, A pointer to the IBOAdminEvent. For more details, see \link IBOAdminEvent \endlink.
	virtual void SetEvent(IBOAdminEvent* pEvent) = 0;
	
	/// \brief To join the BO which request help is from.
	/// \return true indicates success, otherwise fail.
	virtual bool JoinBOByUserRequest(const wchar_t* strUserID) = 0;
	
	/// \brief To ignore the request help.
	/// \return true indicates success, otherwise fail.
	virtual bool IgnoreUserHelpRequest(const wchar_t* strUserID) = 0;

	/// \brief To send the broadcast message.
	/// \return true indicates success, otherwise fail.
	virtual bool BroadcastMessage(const wchar_t* strMsg) = 0;

	/// \brief Host invite user return to main session, When BO is started and user is in BO.
	/// \return true indicates success, otherwise fail.
	virtual bool InviteBOUserReturnToMainSession(const wchar_t* strUserID) = 0;
};

////////////////////////////////////////// IBOAssistant //////////////////////////////////////////

/// \brief BO assistant interface.
///
class IBOAssistant
{
public:
	/// \brief Join BO by BO ID.
	/// \return true indicates success, otherwise fail.
	virtual bool JoinBO(const wchar_t* strBOID) = 0;
	
	/// \brief leave BO
	/// \return true indicates success, otherwise fail.
	virtual bool LeaveBO() = 0;	
};

////////////////////////////////////////// IBOAttendee //////////////////////////////////////////

/// \brief attendee callback handler.
///
class IBOAttendeeEvent
{
public:

	virtual ~IBOAttendeeEvent() {}

	/// \brief To notify the status of request help.
	/// \param eResult, For more details, see \link ATTENDEE_REQUEST_FOR_HELP_RESULT \endlink.
	virtual void onHelpRequestHandleResultReceived(ATTENDEE_REQUEST_FOR_HELP_RESULT eResult) = 0;

	/// \brief To notify if host has joined the BO.
	virtual void onHostJoinedThisBOMeeting() = 0;

	/// \brief To notify if host has leaved the BO.
	virtual void onHostLeaveThisBOMeeting() = 0;
};
/// \brief attendee interface
///
class IBOAttendee
{
public:
	/// \brief Join BO for attendee which is assigned to a BO.
	/// \return true indicates success, otherwise fail.
	virtual bool JoinBo() = 0;

	/// \brief Leave BO for attendee which is in a BO.
	/// \return true indicates success, otherwise fail.
	virtual bool LeaveBo() = 0;

	/// \brief Get name of the BO that attendee is assigned to.
	virtual const wchar_t* GetBoName() = 0;

	/// \brief Set attendee callback handler.
	/// \param pEvent, A pointer to the IBOAttendeeEvent. For more details, see \link IBOAttendeeEvent \endlink.
	virtual void SetEvent(IBOAttendeeEvent* pEvent) = 0;

	/// \brief Request help for attendee.
	/// \return true indicates success, otherwise fail.
	virtual bool RequestForHelp() = 0;

	/// \brief Determine if host is in the BO which attendee is assigned to.
	/// \return true if host is in, otherwise false.
	virtual bool IsHostInThisBO() = 0;

	/// \brief Determine if participant can return to main session.
	/// \return true if can, otherwise false.
	virtual bool IsCanReturnMainSession() = 0;
};

////////////////////////////////////////// IBOData //////////////////////////////////////////

/// \brief BO data callback handler.
///
class IBODataEvent
{
public:
	virtual ~IBODataEvent() {}

	/// \brief To notify if some BO information is changed(user join/leave BO or BO name is modified)
	/// \param strBOID, the BO ID which information is changed.
	virtual void onBOInfoUpdated(const wchar_t* strBOID) = 0; 
	
	/// \brief To notify if unassigned user join/leave master conference or name is modified.
	/// once you receive the callback, you need call GetUnassginedUserList to update the unassigned user information.
	virtual void onUnAssignedUserUpdated() = 0; 

	/// \brief Host/CoHost both can edit BO, Host edit BO->start BO->stop BO, 
	///        then CoHost edit BO->start BO, you will receive the event, you must update BO list in UI.
 	/// \param strBOID, the BO ID which information is changed.
	virtual void OnBOListInfoUpdated() = 0;
};
/// \brief BO data interface
///
class IBOData
{
public:
	/// \brief Set BO data callback handler.
	/// \param pEvent, A pointer to the IBODataEvent. For more details, see \link IBODataEvent \endlink.
	virtual void SetEvent(IBODataEvent* pEvent) = 0;

	/// \brief Get the id list of all unassigned users. 
	/// \return If the function succeeds, the return value is a pointer to IList object. For more details, see \link IList \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IList<const wchar_t*>* GetUnassginedUserList() = 0;

	/// \brief Get the id list of all BOs. 
	/// \return If the function succeeds, the return value is a pointer to IList object. For more details, see \link IList \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IList<const wchar_t*>* GetBOMeetingIDList() = 0;
	
	/// \brief Get user name by user ID. 
	/// \return user name
	virtual const wchar_t* GetBOUserName(const wchar_t* strUserID) = 0;
	
	/// \brief Get user status by user ID. 
	/// \return user status, For more details, see \link BO_CTRL_USER_STATUS \endlink,
	virtual BO_CTRL_USER_STATUS GetBOUserStatus(const wchar_t* strUserID) = 0;

	/// \brief Determine if strUserID is myself.
	/// \return true if strUserID is myself, otherwise false.
	virtual bool IsBOUserMyself(const wchar_t* strUserID) = 0;

	/// \brief Get BO object by BO ID.
	/// \return If the function succeeds, the return value is a pointer to IBOMeeting object. For more details, see \link IBOMeeting \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOMeeting* GetBOMeetingByID(const wchar_t* strBOID) = 0;

	/// \brief Get current BO name if you in a BO.
	/// \return BO name
	virtual const wchar_t* GetCurrentBoName() = 0;
};

////////////////////////////////////////// IMeetingBOController //////////////////////////////////////////

/// \brief enum for BO status
enum BO_STATUS
{
	BO_STATUS_INVALID = 0,  ///<invalid
	BO_STATUS_EDIT = 1,	    ///<edit & assign
	BO_STATUS_STARTED = 2,	///<BO is started
	BO_STATUS_STOPPING = 3,	///<stopping BO
	BO_STATUS_ENDED = 4		///<BO is ended
};

/// \brief handler for return to main session.
///
class IReturnToMainSessionHandler
{
public:
	virtual ~IReturnToMainSessionHandler() {}

	/// \brief return to main session.
	/// \return If the call is successful, the return value is true, Otherwise false.
	virtual bool ReturnToMainSession() = 0;

	/// \brief Ignore the return invitation, after call 'Ignore()', please don't use the handler unless you receive the invitation again.
	virtual void Ignore() = 0;
};

/// \brief BO controller callback event handler.
///
class IMeetingBOControllerEvent
{
public:
	virtual ~IMeetingBOControllerEvent() {}

	/// \brief To notify that you has creator right. 
	/// \param pCreatorObj, the pointer of creator object. For more details, see \link IBOCreator \endlink enum.
	virtual void onHasCreatorRightsNotification(IBOCreator* pCreatorObj) = 0;

	/// \brief To notify that you has admin right. 
	/// \param pAdminObj, the pointer of admin object. For more details, see \link IBOAdmin \endlink enum.
	virtual void onHasAdminRightsNotification(IBOAdmin* pAdminObj) = 0;

	/// \brief To notify that you has assistant right. 
	/// \param pAssistantObj, the pointer of assistant object. For more details, see \link IBOAssistant \endlink enum.
	virtual void onHasAssistantRightsNotification(IBOAssistant* pAssistantObj) = 0;

	/// \brief To notify that you has attendee right. 
	/// \param pAttendeeObj, the pointer of attendee object. For more details, see \link IBOAttendee \endlink enum.
	virtual void onHasAttendeeRightsNotification(IBOAttendee* pAttendeeObj) = 0;

	/// \brief To notify that you has data right. 
	/// \param pDataHelperObj, the pointer of data helper object. For more details, see \link IBOData \endlink enum.
	virtual void onHasDataHelperRightsNotification(IBOData* pDataHelperObj) = 0;

	/// \brief To notify that you lost creator right. 
	virtual void onLostCreatorRightsNotification() = 0;
	
	/// \brief To notify that you lost admin right. 
	virtual void onLostAdminRightsNotification() = 0;

	/// \brief To notify that you lost assistant right. 
	virtual void onLostAssistantRightsNotification() = 0;

	/// \brief To notify that you lost attendee right. 
	virtual void onLostAttendeeRightsNotification() = 0;

	/// \brief To notify that you lost data right. 
	virtual void onLostDataHelperRightsNotification() = 0;

	/// \brief To notify that you receive a broadcast message. 
	/// \param strMsg, the message content.
	/// \param nSenderID, the SenderID.
	virtual void onNewBroadcastMessageReceived(const wchar_t* strMsg, unsigned int nSenderID) = 0;

	/// \brief When BOOption.countdown_seconds != BO_STOP_NOT_COUNTDOWN, host stop BO and all users receive the event.
	/// \param nSeconds, the countdown seconds.
	virtual void onBOStopCountDown(unsigned int nSeconds) = 0;

	/// \brief When you are in BO, host invite you return to main session, you will receive the event.
	/// \param strName, the host name.
	virtual void onHostInviteReturnToMainSession(const wchar_t* strName, IReturnToMainSessionHandler* handler) = 0;

	/// \brief When host change the BO status, all users receive the event.
	/// \param eStatus, current status of BO.
	virtual void onBOStatusChanged(BO_STATUS eStatus) = 0; 

	/// \brief Whenever the host switches you to another BO while you are assigned but haven't joined the BO, you will receive this event.
	/// \param strNewBOName The new BO name.
	/// \param strNewBOID The new BO ID. If the current user is IBOAttendee, then the 2nd parameter strNewBOID will return NULL.
	virtual void onBOSwitchRequestReceived(const wchar_t* strNewBOName, const wchar_t* strNewBOID) = 0;
};

/// \brief Meeting breakout rooms controller interface
///
class IMeetingBOController
{
public:
	/// \brief Set breakout room callback event handler.
	/// \param event, A pointer to the IMeetingBOControllerEvent. For more details, see \link IMeetingBOControllerEvent \endlink.
	/// \return If the function succeeds, the return value is true. Otherwise false. 
	virtual bool SetEvent(IMeetingBOControllerEvent* event) = 0;

	/// \brief Get the pointer of BO creator object. 
	/// \return If the function succeeds, the return value is a pointer to IBOCreator object. For more details, see \link IBOCreator \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOCreator*    GetBOCreatorHelper() = 0;

	/// \brief Get the pointer of BO administrator object. 
	/// \return If the function succeeds, the return value is a pointer to IBOAdmin object. For more details, see \link IBOAdmin \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOAdmin*      GetBOAdminHelper() = 0;

	/// \brief Get the pointer of BO assistant object. 
	/// \return If the function succeeds, the return value is a pointer to IBOAssistant object. For more details, see \link IBOAssistant \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOAssistant*  GetBOAssistantHelper() = 0;

	/// \brief Get the pointer of BO attendee object. 
	/// \return If the function succeeds, the return value is a pointer to IBOAttendee object. For more details, see \link IBOAttendee \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOAttendee*   GetBOAttedeeHelper() = 0;

	/// \brief Get the pointer of BO data object. 
	/// \return If the function succeeds, the return value is a pointer to IBOData object. For more details, see \link IBOData \endlink,
	///Otherwise failed, the return value is NULL.
	virtual IBOData*	   GetBODataHelper() = 0;

	/// \brief Determine if the BO is started or not.
	/// \return true indicates that host has started the BO, otherwise not.
	virtual bool IsBOStarted() = 0;
	
	/// \brief Determine if the BO feature is enabled in current meeting.
	/// \return true indicates that BO feature is enabled in current meeting.
	virtual bool IsBOEnabled() = 0;

	/// \brief Determine if myself is in BO meeting.
	/// \return true indicates that i am in a BO meeting.
	virtual bool IsInBOMeeting() = 0;

	/// \brief Get current BO status
	/// \return The return value is a enum for bo status. For more details, see \link BO_STATUS \endlink.
	virtual BO_STATUS GetBOStatus() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif