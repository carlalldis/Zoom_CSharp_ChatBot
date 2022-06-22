using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ZOOM_SDK_DOTNET_WRAP;

namespace Zoom_CSharp_ChatBot
{
    /// <summary>
    /// Controls the state of initiative rounds and user communication
    /// </summary>
    class ChatBotController
    {
        private bool _enablePending = false;
        private bool _enabled = false;
        private IMeetingChatControllerDotNetWrap _chatController; // The chat controller for the zoom meeting
        private List<Message> _messages; // A list of initiative values for the round (clears every round)
        private Dictionary<string, List<int>> _tally; // A list of initiative values per person for the session
        private int _round; // The number of this round
        private string _userName; // The username of the bot (to mitigate self-replies)
        private bool _inProgress; // Determines if there is a round in progress

        public ChatBotController(IMeetingChatControllerDotNetWrap chatController, string userName)
        {
            _messages = new List<Message>();
            _tally = new Dictionary<string, List<int>>();
            _round = 1; // Start at round 1
            _inProgress = false; // Start with a round not in progress
            _userName = userName;
            _chatController = chatController;
            _chatController.Add_CB_onChatMsgNotifcation(onChatMsgNotification); // Add event handler for messages
        }

        internal async Task EnableAsync()
        {
            if (!_enablePending && !_enabled)
            {
                _enablePending = true;
                await Task.Delay(5000);
                SendMessageEveryone("I am now initialized!");
                Help();
                _enabled = true;
                _enablePending = false;
            }
        }

        internal void Disable()
        {
            if (_enabled)
                _enabled = false;
        }

        private void Help()
        {
            SendMessageEveryone("The following commands are available:" +
                "\r\n\t'new'\t\t: Start a new round" +
                "\r\n\t'done'\t\t: Finish the current round" +
                "\r\n\t'undo'\t\t: Go back to the previous round" +
                "\r\n\t'restart'\t: Remove the current rolls" +
                "\r\n\t'tally'\t\t: Show the totals of all rolls" +
                "\r\n\t'help'\t\t: Show this message again");
        }

        /// <summary>
        /// When a chat message is recevied, follow logic to determine course of action (new round, add value to round, or end round)
        /// </summary>
        /// <param name="chatMsg"></param>
        private void onChatMsgNotification(IChatMsgInfoDotNetWrap chatMsg)
        {
            if (!_enabled)
                return;
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
                    case "undo":
                        UndoCompleteInitiative();
                        break;
                    case "restart":
                        RestartInitiative();
                        break;
                    case "tally":
                        TallyInitiative();
                        break;
                    case "help":
                        Help();
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
                _messages.Clear();
                _inProgress = true;
                SendMessageEveryone("Initiative round " + _round + " is now starting.");
            }
        }

        /// <summary>
        /// Restart the current round, if someone screwed up
        /// </summary>
        private void RestartInitiative()
        {
            if (_inProgress)
            {
                _inProgress = false;
                NewInitiative();
            }
            else
            {
                SendMessageEveryone("Initiative round is not in progress. You cannot 'restart'. Please type 'undo' to go back to the previous round.");
            }
        }

        /// <summary>
        /// Undo an initiative completion
        /// </summary>
        private void UndoCompleteInitiative()
        {
            if (_inProgress || _round == 1)
            {
                SendMessageEveryone("Initiative round already in progress. You cannot 'undo'. Please type 'restart' to restart the round.");
            }
            else
            {
                _inProgress = true;
                _round--;
                SendMessageEveryone("Initiative round " + _round + " is now in starting...again.");
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
                var resultMessageList = new List<string>();
                resultMessageList.Add("Initiative round " + _round + " results:");
                resultMessageList.Add("");
                foreach (var message in _messages)
                {
                    resultMessageList.Add(message.Sender + ": " + message.Roll);
                }
                var resultMessage = string.Join("\n", resultMessageList);
                SendMessageEveryone(resultMessage);
                _round++;
                _inProgress = false;
            }
            else
            {
                SendMessageEveryone("Initiative round is not currently in progress");
            }
        }

        private void AddTally(string Sender, int Roll)
        {
            if (_tally.ContainsKey(Sender))
            {
                _tally[Sender].Add(Roll);
            }
            else
            {
                var newList = new List<int>();
                newList.Add(Roll);
                _tally.Add(Sender, newList);
            }
        }

        private void TallyInitiative()
        {
            var tallyTotals = new List<Tally>();
            foreach (var sender in _tally.Keys)
            {
                var rolls = _tally[sender];
                var newTally = new Tally()
                {
                    Sender = sender,
                    Rolls = _tally[sender],
                    Total = rolls.Sum()
                };
                tallyTotals.Add(newTally);
            }
            tallyTotals.Sort();
            var resultTallyList = new List<string>();
            resultTallyList.Add("Initiative tally:");
            resultTallyList.Add("");
            foreach (var tally in tallyTotals)
            {
                resultTallyList.Add(tally.Sender + ": " + tally.Total + " (" + string.Join(", ", tally.Rolls) + ")");
            }
            var resultTally = string.Join("\n", resultTallyList);
            SendMessageEveryone(resultTally);
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
                    AddTally(message.Sender, message.Roll);
                }
                catch
                {
                    _chatController.SendChatMsgTo("'" + sender + "': your message '" + content + "' was invalid", 0, ChatMessageType.SDKChatMessageType_To_All);
                }
            }
        }

        /// <summary>
        /// Send a message to everyone in the meeting
        /// </summary>
        /// <param name="message"></param>
        private void SendMessageEveryone(string message)
        {
            var err = _chatController.SendChatMsgTo(message, 0, ChatMessageType.SDKChatMessageType_To_All);
            switch (err)
            {
                case SDKError.SDKERR_SUCCESS:
                    break;
                case SDKError.SDKERR_TOO_FREQUENT_CALL:
                    // Retry once
                    Task.Delay(100).Wait();
                    _chatController.SendChatMsgTo(message, 0, ChatMessageType.SDKChatMessageType_To_All);
                    break;
                default:
                    throw new InvalidOperationException(err.ToString());
            }
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

    class Tally : IComparable<Tally>
    {
        public string Sender { get; set; }
        public int Total { get; set; }
        public List<int> Rolls { get; set; }

        public int CompareTo(Tally other)
        {
            return other.Total.CompareTo(Total);
        }
    }
}