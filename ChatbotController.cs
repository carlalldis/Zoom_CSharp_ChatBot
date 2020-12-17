using System;
using System.Collections.Generic;
using ZOOM_SDK_DOTNET_WRAP;

namespace zoom_sdk_demo
{
    /// <summary>
    /// Controls the state of initiative rounds and user communication
    /// </summary>
    class ChatbotController : IDisposable
    {
        private bool disposedValue;
        private IMeetingChatControllerDotNetWrap _chatController; // The chat controller for the zoom meeting
        private List<Message> _messages; // A list of initiative values for the round (clears every round)
        private int _round; // The number of this round
        private string _userName; // The username of the bot (to mitigate self-replies)
        private bool _inProgress; // Determines if there is a round in progress

        public ChatbotController(IMeetingChatControllerDotNetWrap chatController, string userName)
        {
            _messages = new List<Message>();
            _round = 1; // Start at round 1
            _inProgress = false; // Start with a round not in progress
            _userName = userName;
            _chatController = chatController;
            _chatController.Add_CB_onChatMsgNotifcation(onChatMsgNotifcation); // Add event handler for messages
        }

        /// <summary>
        /// When a chat message is recevied, follow logic to determine course of action (new round, add value to round, or end round)
        /// </summary>
        /// <param name="chatMsg"></param>
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

        /// <summary>
        /// Start a new round, unless there is one in progress
        /// </summary>
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

        /// <summary>
        /// Complete the round, unless there is not one in progress
        /// </summary>
        private void CompleteInitiative()
        {
            if (_inProgress)
            {
                _messages.Sort();
                var resultMessageList = new List<String>();
                resultMessageList.Add("Initiative round " + _round + " results:");
                resultMessageList.Add("");
                foreach (var message in _messages)
                {
                    resultMessageList.Add(message.Sender + ": " + message.Roll);
                }
                var resultMessage = string.Join("\n", resultMessageList);
                SendMessageEveryone(resultMessage);
                _messages.Clear();
                _round++;
                _inProgress = false;
            }
            else
            {
                SendMessageEveryone("Initiative round is not currently in progress");
            }
        }

        /// <summary>
        /// Add a new value to the round in progress
        /// </summary>
        /// <param name="timestamp"></param>
        /// <param name="sender"></param>
        /// <param name="content"></param>
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

        /// <summary>
        /// Send a message to everyone in the meeting
        /// </summary>
        /// <param name="message"></param>
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