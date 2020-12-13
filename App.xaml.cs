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
        start_join_meeting start_meeting_wnd = new start_join_meeting();

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
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onAuthenticationReturn(onAuthenticationReturn);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLoginRet(onLoginRet);
                    CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().Add_CB_onLogout(onLogout);
                    AuthParam authParam = new AuthParam
                    {
                        appKey = ConfigurationManager.AppSettings.Get("appKey"),
                        appSecret = ConfigurationManager.AppSettings.Get("appSecret")
                };
                    var sdkError = ZOOM_SDK_DOTNET_WRAP.CZoomSDKeDotNetWrap.Instance.GetAuthServiceWrap().SDKAuth(authParam);
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
        private void Application_Exit(object sender, ExitEventArgs e)
        {
            //clean up sdk
            ZOOM_SDK_DOTNET_WRAP.CZoomSDKeDotNetWrap.Instance.CleanUp();
        }

        //callback
        public void onAuthenticationReturn(AuthResult ret)
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
        public void onLoginRet(LOGINSTATUS ret, IAccountInfo pAccountInfo)
        {
            //MessageBox.Show("Login Status: " + ret.ToString());
        }
        public void onLogout()
        {
            //MessageBox.Show("Logged Out");
        }
    }
}
