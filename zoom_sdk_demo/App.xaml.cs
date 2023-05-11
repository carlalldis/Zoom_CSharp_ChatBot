using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using ZOOM_SDK_DOTNET_WRAP;

namespace zoom_sdk_demo
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        readonly start_join_meeting start_meeting_wnd = new start_join_meeting();
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            //init sdk
            {
                ZOOM_SDK_DOTNET_WRAP.InitParam param = new ZOOM_SDK_DOTNET_WRAP.InitParam();
                param.web_domain = "https://zoom.us";
                ZOOM_SDK_DOTNET_WRAP.SDKError err = ZOOM_SDK_DOTNET_WRAP.CZoomSDKeDotNetWrap.Instance.Initialize(param);
                if (ZOOM_SDK_DOTNET_WRAP.SDKError.SDKERR_SUCCESS == err)
                {
                    //register callbacks
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onAuthenticationReturn(OnAuthenticationReturn);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLoginRet(OnLoginRet);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLogout(OnLogout);
                    AuthParam authParam = new AuthParam
                    {
                        appKey = ConfigurationManager.AppSettings.Get("zoomKey"),
                        appSecret = ConfigurationManager.AppSettings.Get("zoomSecret")
                    };
                    var sdkError = CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().SDKAuth(authParam);
                    if (sdkError != SDKError.SDKERR_SUCCESS)
                    {
                        MessageBox.Show("Failed to connect: " + sdkError.ToString());
                        Current.Shutdown();
                    }
                }
                else//error handle.todo
                {
                    MessageBox.Show("Failed to initialise: " + err.ToString());
                    Current.Shutdown();
                }
            }
        }
        private void Application_Exit(object sender, ExitEventArgs e)
        {
            //clean up sdk
            ZOOM_SDK_DOTNET_WRAP.CZoomSDKeDotNetWrap.Instance.CleanUp();
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
