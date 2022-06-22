using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ZOOM_SDK_DOTNET_WRAP;

namespace Zoom_CSharp_ChatBot
{
    /// <summary>
    /// Controls audio in the session
    /// </summary>
    class AudioBotController
    {
        private bool _enablePending = false;
        private bool _enabled = false;
        private readonly IMeetingAudioControllerDotNetWrap _audioController; // The chat controller for the zoom meeting
        private readonly IAudioSettingContextDotNetWrap _audioSettings;

        public AudioBotController(IMeetingAudioControllerDotNetWrap audioController, IAudioSettingContextDotNetWrap audioSettings)
        {
            _audioController = audioController;
            _audioSettings = audioSettings;
        }

        internal void Enable()
        {
            if (!_enablePending && !_enabled)
            {
                _enablePending = true;
                _audioController.JoinVoip();
                _enabled = true;
                _enablePending = false;
            }
        }

        internal void Disable()
        {
            if (_enabled)
                _enabled = false;
        }
    }
}