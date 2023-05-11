using NAudio.CoreAudioApi;
using NAudio.Wave;
using System;
using System.Configuration;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using System.Collections.Generic;

namespace Zoom_CSharp_ChatBot.Music
{
    public class MusicController
    {
        private readonly WaveOutCapabilities _device;
        private CancellationTokenSource _cancelTaskSource = new();
        private Task? _currentTask;
        private WaveOut? _waveOut;
        private int _volume = 10;
        private float VolumeFloat => _volume / 100f;
        private readonly int _deviceNumber;

        public MusicController()
        {
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
        }

        public static string[] GetAvailableFiles()
        {
            return Directory.GetFiles("Music").Select(f => f.Replace(".mp3","").Replace("Music\\","")).ToArray();
        }

        public void Play(string name)
        {
            if (_currentTask == null)
            {
                var fileName = $"Music\\{name}.mp3";
                var fileCheck = File.Exists(fileName);
                if (!fileCheck)
                {
                    throw new FileNotFoundException($"Could not find {fileName}");
                }
                _cancelTaskSource = new();
                _currentTask = PlayInternal(name, _cancelTaskSource.Token);
            }
            else
            {
                throw new TaskSchedulerException("Music already playing");
            }
        }

        public void Stop()
        {
            if (!_cancelTaskSource.IsCancellationRequested)
                _cancelTaskSource.Cancel();
            _currentTask = null;
        }

        public void SetVolume(int vol)
        {
            _volume = vol;
            if (_waveOut != null)
            {
                _waveOut.Volume = VolumeFloat;
            } 
        }

        private async Task PlayInternal(string name, CancellationToken cancellationToken)
        {
            using var waveOut = new WaveOut()
            {
                DeviceNumber = _deviceNumber,
                Volume = VolumeFloat,
            };
            _waveOut = waveOut;
            var fileName = $"Music\\{name}.mp3";
            using var mp3Reader = new Mp3FileReader(fileName);
            try
            {
                waveOut.Init(mp3Reader);
                waveOut.Volume = VolumeFloat;
                waveOut.Play();
                waveOut.Volume = VolumeFloat;
                while (waveOut.PlaybackState == PlaybackState.Playing)
                {
                    if (cancellationToken.IsCancellationRequested)
                    {
                        waveOut.Stop();
                        return;
                    }
                    await Task.Delay(100, CancellationToken.None);
                }
                waveOut.Stop();
            }
            catch
            {
                throw;
            }
            finally
            {
                _waveOut = null;
                _currentTask = null;
            }
        }
    }
}