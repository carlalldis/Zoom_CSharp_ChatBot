using System;
using System.ComponentModel; // CancelEventArgs
using System.Threading.Tasks;
using System.Windows;
using ZOOM_SDK_DOTNET_WRAP;

namespace zoom_sdk_demo
{
    /// <summary>
    /// Interaction logic for start_join_meeting.xaml
    /// </summary>
    public partial class start_join_meeting : Window
    {
        private ChatBotController _chatBotController;
        public start_join_meeting()
        {
            InitializeComponent();
        }

        private async void OnMeetingStatusChanged(MeetingStatus status, int iResult)
        {
            switch (status)
            {
                case MeetingStatus.MEETING_STATUS_INMEETING:
                    {
                        await EnableChatBotController();
                    }
                    break;
                case MeetingStatus.MEETING_STATUS_ENDED:
                case MeetingStatus.MEETING_STATUS_FAILED:
                    {
                        _chatBotController?.Disable();
                        Show();
                    }
                    break;
                default:
                    break;
            }
        }

        private async Task EnableChatBotController()
        {
            if (_chatBotController == null)
            {
                var meeting = CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap();
                var chatController = meeting.GetMeetingChatController();
                _chatBotController = new ChatBotController(chatController, textBox_username_api.Text);
            }
            await _chatBotController.EnableAsync();
        }

        private static void OnUserJoin(Array lstUserID)
        {
            if (null == lstUserID)
                return;

            for (int i = lstUserID.GetLowerBound(0); i <= lstUserID.GetUpperBound(0); i++)
            {
                uint userid = (uint)lstUserID.GetValue(i);
                IUserInfoDotNetWrap user = CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                    GetMeetingParticipantsController().GetUserByUserID(userid);
                if (null != user)
                {
                    string name = user.GetUserNameW();
                    Console.Write(name);
                }
            }
        }
        private static void OnUserLeft(Array lstUserID)
        {
            //todo
        }
        private static void OnHostChangeNotification(UInt32 userId)
        {
            //todo
        }
        private static void OnLowOrRaiseHandStatusChanged(bool bLow, UInt32 userid)
        {
            //todo
        }
        private void RegisterCallBack()
        {
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().Add_CB_onMeetingStatusChanged(OnMeetingStatusChanged);
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                GetMeetingParticipantsController().Add_CB_onHostChangeNotification(OnHostChangeNotification);
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                GetMeetingParticipantsController().Add_CB_onLowOrRaiseHandStatusChanged(OnLowOrRaiseHandStatusChanged);
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                GetMeetingParticipantsController().Add_CB_onUserJoin(OnUserJoin);
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                GetMeetingParticipantsController().Add_CB_onUserLeft(OnUserLeft);
        }

        private void ButtonJoinApiClick(object sender, RoutedEventArgs e)
        {
            RegisterCallBack();
            JoinParam param = new JoinParam()
            {
                userType = SDKUserType.SDK_UT_WITHOUT_LOGIN
            };
            JoinParam4WithoutLogin join_api_param = new JoinParam4WithoutLogin();
            var meetingnumber_nospaces = textBox_meetingnumber_api.Text.Replace(" ", "");
            join_api_param.meetingNumber = ulong.Parse(meetingnumber_nospaces);
            join_api_param.userName = textBox_username_api.Text;
            join_api_param.psw = textBox_passcode_api.Text;
            param.withoutloginJoin = join_api_param;

            SDKError err = CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().Join(param);
            if (SDKError.SDKERR_SUCCESS == err)
            {
                Hide();
            }
            else//error handle
            { }
        }

        private void Wnd_Closing(object sender, CancelEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
