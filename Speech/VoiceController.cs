using Microsoft.CognitiveServices.Speech;
using NAudio.CoreAudioApi;
using NAudio.Wave;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Security;
using System.Threading.Tasks;

namespace Zoom_CSharp_ChatBot.Speech
{
    public class VoiceController : IDisposable
    {
        private readonly string _voiceName;
        private readonly SpeakingStyle _defaultStyle;
        private readonly SpeechSynthesizer _speechSynthesizer;
        private readonly WaveOutCapabilities _device;
        private readonly int _deviceNumber;
        private bool disposedValue;

        private const string SSML_TEMPLATE = @"
<speak version=""1.0"" xmlns=""http://www.w3.org/2001/10/synthesis"" xmlns:mstts=""https://www.w3.org/2001/mstts"" xml:lang=""en-US"">
	<voice name = ""%VOICE%"" >
        <mstts:express-as style=""%STYLE%"">
	        %TEXT%
        </mstts:express-as>
	</voice>
</speak>";

        public VoiceController()
        {
            _voiceName = ConfigurationManager.AppSettings.Get("azureSpeechVoice") ?? throw new ArgumentNullException("azureSpeechVoice", "Missing configuration");
            var defaultStyle = ConfigurationManager.AppSettings.Get("azureSpeechDefaultStyle") ?? throw new ArgumentNullException("azureSpeechDefaultStyle", "Missing configuration");
            _defaultStyle = (SpeakingStyle)Enum.Parse(typeof(SpeakingStyle), defaultStyle);
            var key = ConfigurationManager.AppSettings.Get("azureSpeechKey") ?? throw new ArgumentNullException("azureSpeechKey", "Missing configuration");
            var region = ConfigurationManager.AppSettings.Get("azureSpeechRegion") ?? throw new ArgumentNullException("azureSpeechRegion", "Missing configuration");

            var deviceName = ConfigurationManager.AppSettings.Get("soundOutputDevice") ?? throw new ArgumentNullException("speechOutputDevice", "Missing configuration");

            var devices = new List<WaveOutCapabilities>();
            var deviceCount = WaveOut.DeviceCount;
            for (int i = 0; i < deviceCount; i++)
            {
                var capability = WaveOut.GetCapabilities(i);
                devices.Add(capability);
            }
            try
            {
                var matchedDevices = devices.Where(d => d.ProductName == deviceName);
                _device = matchedDevices.Single();
                _deviceNumber = devices.IndexOf(_device);
            }
            catch (InvalidOperationException ex)
            {
                var availableDevices = devices.Select(d => d.ProductName);
                var availableDevicesJoined = string.Join(", ", availableDevices);
                throw new InvalidOperationException($"Failed to find device. Available devices are: {availableDevicesJoined}", ex);
            }

            var speechConfig = SpeechConfig.FromSubscription(key, region);
            speechConfig.SetSpeechSynthesisOutputFormat(SpeechSynthesisOutputFormat.Ogg48Khz16BitMonoOpus);
            _speechSynthesizer = new SpeechSynthesizer(speechConfig);
        }

        public Task SpeakAsync(string text) => SpeakAsync(text, _defaultStyle);
        public async Task SpeakAsync(string text, SpeakingStyle style)
        {

            try
            {
                var ssml = SSML_TEMPLATE
                    .Replace("%VOICE%", _voiceName)
                    .Replace("%STYLE%", style.ToString())
                    .Replace("%TEXT%", SecurityElement.Escape(text));
                var result = await _speechSynthesizer.SpeakSsmlAsync(ssml);
                if (result.Reason != ResultReason.SynthesizingAudioCompleted)
                    throw new InvalidOperationException($"Voice generation failed with reason: {result.Reason}");
                using var pcmStream = new OggDecoderStream(result.AudioData);
                using var mf = new RawSourceWaveStream(pcmStream, new WaveFormat(48000, 16, 1));
                using var waveOut = new WaveOut()
                {
                    DeviceNumber = _deviceNumber,
                    Volume = 1.0f,
                };
                waveOut.Init(mf);
                waveOut.Play();
                while (waveOut.PlaybackState == PlaybackState.Playing)
                {
                    await Task.Delay(100);
                }
            }
            catch
            {
                throw;
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    _speechSynthesizer.Dispose();
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
