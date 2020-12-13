using System;
using System.Collections.Generic;
using ZOOM_SDK_DOTNET_WRAP;

namespace zoom_sdk_demo
{
    class ChatbotController : IDisposable
    {
        private bool disposedValue;
        private IMeetingChatControllerDotNetWrap _chatController;
        private List<Message> _messages;
        private int _round;
        private string _userName;
        private bool _inProgress;

        public ChatbotController(IMeetingChatControllerDotNetWrap chatController, string userName)
        {
            _messages = new List<Message>();
            _round = 1;
            _inProgress = false;
            _userName = userName;
            _chatController = chatController;
            _chatController.Add_CB_onChatMsgNotifcation(onChatMsgNotifcation);
        }

        private void onChatMsgNotifcation(IChatMsgInfoDotNetWrap chatMsg)
        {
            var timestamp = chatMsg.GetTimeStamp();
            var sender = chatMsg.GetSenderDisplayName();
            var content = chatMsg.GetContent();
            if (sender != _userName)
            {
                switch (content.ToLower())
                {
                    case "new":
                        NewInitiative();
                        break;
                    case "done":
                        CompleteInitiative();
                        break;
                    default:
                        AddMessage(timestamp, sender, content);
                        break;
                }
            }
        }

        private void NewInitiative()
        {
            if (_inProgress)
            {
                SendMessageEveryone("Initiative round already in progress.");
            }
            else
            {
                _inProgress = true;
                SendMessageEveryone("Initiative round " + _round + " is now starting.");
            }
        }

        private void CompleteInitiative()
        {
            if (_inProgress)
            {
                _messages.Sort();
                SendMessageEveryone("Initiative round " + _round + " results:");
                foreach (var message in _messages)
                {
                    SendMessageEveryone(message.Sender + ": " + message.Roll);
                }
                _messages.Clear();
                _round++;
                _inProgress = false;
            }
            else
            {
                SendMessageEveryone("Initiative round is not currently in progress");
            }
        }

        private void AddMessage(DateTime? timestamp, string sender, string content)
        {
            if (_inProgress)
            {
                try
                {
                    var roll = int.Parse(content);
                    var message = new Message
                    {
                        Timestamp = timestamp,
                        Sender = sender,
                        Roll = roll,
                    };
                    _messages.Add(message);
                }
                catch
                {
                    _chatController.SendChatTo(0, "'" + sender + "': your message '" + content + "' was invalid");
                }
            }
        }

        private void SendMessageEveryone(string message)
        {
            _chatController.SendChatTo(0, message);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }

    class Message : IComparable<Message>
    {
        public DateTime? Timestamp { get; set; }
        public string Sender { get; set; }
        public int Roll { get; set; }

        public int CompareTo(Message other)
        {
            return other.Roll.CompareTo(Roll);
        }
    }
}