using System;
using System.ComponentModel; // CancelEventArgs
using System.Threading.Tasks;
using System.Windows;
using ZOOM_SDK_DOTNET_WRAP;

namespace Zoom_CSharp_ChatBot
{
    /// <summary>
    /// Interaction logic for start_join_meeting.xaml
    /// </summary>
    public partial class StartJoinMeeting : Window
    {
        private ChatBotController? _chatBotController;
        private AudioBotController? _audioBotController;
        public StartJoinMeeting()
        {
            InitializeComponent();
        }

        public async void OnMeetingStatusChanged(MeetingStatus status, int iResult)
        {
            switch (status)
            {
                case MeetingStatus.MEETING_STATUS_INMEETING:
                    {
                        EnableAudioBotController();
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

        private void EnableAudioBotController()
        {
            if (_audioBotController == null)
            {
                var meeting = CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap();
                var settings = CZoomSDKeDotNetWrap.Instance.GetSettingServiceWrap();
                var audioController = meeting.GetMeetingAudioController();
                var audioSettings = settings.GetAudioSettings();
                _audioBotController = new AudioBotController(audioController, audioSettings);
            }
            _audioBotController.Enable();
        }

        public void OnUserJoin(Array lstUserID)
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
        public void OnUserLeft(Array lstUserID)
        {
            //todo
        }
        public void OnHostChangeNotification(UInt32 userId)
        {
            //todo
        }
        public void OnLowOrRaiseHandStatusChanged(bool bLow, UInt32 userid)
        {
            //todo
        }
        public void OnUserNameChanged(UInt32 userId, string userName)
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
            CZoomSDKeDotNetWrap.Instance.GetMeetingServiceWrap().
                GetMeetingParticipantsController().Add_CB_onUserNameChanged(OnUserNameChanged);
        }

        private void ButtonJoinApiClick(object sender, RoutedEventArgs e)
        {
            RegisterCallBack();
            JoinParam param = new JoinParam
            {
                userType = SDKUserType.SDK_UT_WITHOUT_LOGIN
            };
            JoinParam4WithoutLogin join_api_param = new ZOOM_SDK_DOTNET_WRAP.JoinParam4WithoutLogin();
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

        void Wnd_Closing(object sender, CancelEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
