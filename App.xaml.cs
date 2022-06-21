using System.Configuration;
using System.Windows;
using ZOOM_SDK_DOTNET_WRAP;

namespace Zoom_CSharp_ChatBot
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        readonly StartJoinMeeting start_meeting_wnd = new StartJoinMeeting();

        private void ApplicationStartup(object sender, StartupEventArgs e)
        {
            //init sdk
            {
                InitParam param = new InitParam
                {
                    sdk_dll_path = ".\\zoom_sdk_dotnet_wrap\\zSDK.dll",
                    web_domain = "https://zoom.us"
                };
                SDKError err = CZoomSDKeDotNetWrap.Instance.Initialize(param);
                if (SDKError.SDKERR_SUCCESS == err)
                {
                    //register callbacks
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onAuthenticationReturn(OnAuthenticationReturn);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLoginRet(OnLoginRet);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLogout(OnLogout);
                    AuthParam authParam = new AuthParam
                    {
                        appKey = ConfigurationManager.AppSettings.Get("appKey"),
                        appSecret = ConfigurationManager.AppSettings.Get("appSecret")
                    };
                    var sdkError = CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().SDKAuth(authParam);
                    if (sdkError != SDKError.SDKERR_SUCCESS)
                    {
                        MessageBox.Show("Failed to connect: " + sdkError.ToString());
                        Current.Shutdown();
                    }
                }
                else
                {
                    MessageBox.Show("Failed to initialise: " + err.ToString());
                    Current.Shutdown();
                }
            }
        }

        private void ApplicationExit(object sender, ExitEventArgs e)
        {
            //clean up sdk
            CZoomSDKeDotNetWrap.Instance.CleanUp();
        }

        //callback
        public void OnAuthenticationReturn(AuthResult ret)
        {
            if (AuthResult.AUTHRET_SUCCESS == ret)
            {
                start_meeting_wnd.Show();
            }
            else
            {
                MessageBox.Show("Failed to authenticate: " + ret.ToString());
                Current.Shutdown();
            }
        }
        private void OnLoginRet(LOGINSTATUS ret, IAccountInfo pAccountInfo, LOGINFAILREASON reason)
        {
            if (ret == LOGINSTATUS.LOGIN_FAILED)
                MessageBox.Show($"Login Status: {ret}\r\nAccount Info: {pAccountInfo}\r\nFail Reason: {reason}");
        }
        public void OnLogout()
        {
            MessageBox.Show("Logged Out");
        }
    }
}
