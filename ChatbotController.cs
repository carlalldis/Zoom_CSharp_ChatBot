using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Zoom_CSharp_ChatBot.Music;
using Zoom_CSharp_ChatBot.Speech;
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
        private readonly IMeetingChatControllerDotNetWrap _chatController; // The chat controller for the zoom meeting
        private readonly List<Message> _messages; // A list of initiative values for the round (clears every round)
        private readonly Dictionary<string, List<int>> _tally; // A list of initiative values per person for the session
        private int _round; // The number of this round
        private readonly string _userName; // The username of the bot (to mitigate self-replies)
        private bool _inProgress; // Determines if there is a round in progress
        private VoiceController? _voiceController;
        private MusicController? _musicController;
        private List<string> _facts = new();

        private const string STARTUP_MESSAGE = "DND Bot initialized!";

        public ChatBotController(IMeetingChatControllerDotNetWrap chatController, string userName)
        {
            _messages = new List<Message>();
            _tally = new Dictionary<string, List<int>>();
            _round = 1; // Start at round 1
            _inProgress = false; // Start with a round not in progress
            _userName = userName;
            _chatController = chatController;
            _chatController.Add_CB_onChatMsgNotifcation(OnChatMsgNotification); // Add event handler for messages
        }

        internal async Task EnableAsync()
        {
            if (!_enablePending && !_enabled)
            {
                _enablePending = true;
                _facts = (await File.ReadAllLinesAsync("DndFacts.txt")).ToList();
                await Task.Delay(5000);
                try
                {
                    _voiceController = new VoiceController();
                }
                catch (Exception ex)
                {
                    SendTextMessage($"Voice failed to initialize: {ex.Message}");
                }
                try
                {
                    _musicController = new MusicController();
                }
                catch (Exception ex)
                {
                    SendTextMessage($"Music failed to initialize: {ex.Message}");
                }
                SendTextMessage(STARTUP_MESSAGE);
                await SendVoiceMessageAsync(STARTUP_MESSAGE, SpeakingStyle.friendly);
                await NewFactMessage();
                Help();
                _enabled = true;
                _enablePending = false;
            }
        }

        private async Task NewFactMessage()
        {
            if (_facts.Count == 0)
            {
                SendTextMessage("Sorry, no more facts");
                await SendVoiceMessageAsync("Sorry, no more facts", SpeakingStyle.sad);
            }
            else
            {
                var random = new Random();
                int index = random.Next(_facts.Count);
                var chosenFact = _facts[index];
                _facts.RemoveAt(index);
                SendTextMessage($"DnD Fact: {chosenFact}");
                await SendVoiceMessageAsync($"Here's a DND Fact: {chosenFact}", SpeakingStyle.cheerful);
            }
        }

        private async Task SendVoiceMessageAsync(string message, SpeakingStyle style)
        {
            if (_voiceController is not null)
            {
                try
                {
                    await _voiceController.SpeakAsync(message, style);
                }
                catch (Exception ex)
                {
                    SendTextMessage($"Voice send failed: {ex.Message}");
                }
            }
        }

        internal void Disable()
        {
            if (_enabled)
                _enabled = false;
        }

        private void Help()
        {
            SendTextMessage("The following commands are available:" +
                "\r\n\t'new'\t\t: Start a new round" +
                "\r\n\t'done'\t\t: Finish the current round" +
                "\r\n\t'undo'\t\t: Go back to the previous round" +
                "\r\n\t'restart'\t: Remove the current rolls" +
                "\r\n\t'tally'\t\t: Show the totals of all rolls" +
                "\r\n\t'fact'\t\t: Read an interesting DnD fact" +
                "\r\n\t'speak <style> <text>'\t\t: Make me say a sentence" +
                "\r\n\t'play <file>'\t\t: Play some music" +
                "\r\n\t'stop'\t\t: Stop the music" +
                "\r\n\t'help'\t\t: Show this message again");
        }

        /// <summary>
        /// When a chat message is recevied, follow logic to determine course of action (new round, add value to round, or end round)
        /// </summary>
        /// <param name="chatMsg"></param>
        private void OnChatMsgNotification(IChatMsgInfoDotNetWrap chatMsg)
        {
            try
            {
                if (!_enabled)
                    return;
                var timestamp = chatMsg.GetTimeStamp() ?? DateTime.Now;
                var sender = chatMsg.GetSenderDisplayName();
                var content = chatMsg.GetContent();
                if (sender != _userName)
                {
                    switch (content.ToLower().Split(' ')[0])
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
                        case "fact":
                            _ = NewFactMessage();
                            break;
                        case "speak":
                            ParseSpeakMessage(content);
                            break;
                        case "play":
                            PlayMusic(content);
                            break;
                        case "stop":
                            StopMusic();
                            break;
                      case "volume":
                            SetMusicVolume(content);
                            break;
                        case "listmusic":
                            ListMusic();
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
            catch (Exception ex)
            {
                SendTextMessage(ex.Message);
            }
        }

        private void StopMusic()
        {
            _musicController?.Stop();
        }

        private void PlayMusic(string content)
        {
            var musicName = content[5..];
            _musicController?.Play(musicName);
        }

        private void ListMusic()
        {
            var files = MusicController.GetAvailableFiles();
            var filesString = string.Join(", ", files);
            SendTextMessage($"Available music: {filesString}");
        }

        private void SetMusicVolume(string content)
        {
            var vol = int.Parse(content.Replace("volume ",""));
            _musicController?.SetVolume(vol);
        }

        private void ParseSpeakMessage(string content)
        {
            var contentArray = content.Split(' ');
            if (contentArray.Length < 3)
            {
                SendTextMessage("Please specify both <style> and <string>");
                return;
            }
            var styleString = contentArray[1];
            var styleParseSuccess = Enum.TryParse(styleString, out SpeakingStyle style);
            if (!styleParseSuccess)
            {
                var availableStyles = string.Join(", ", Enum.GetNames<SpeakingStyle>());
                SendTextMessage($"Speaking style '{styleString}' was invalid. Available styles are: {availableStyles}");
            }
            var speakString = string.Join(" ", contentArray.Skip(2));
            _ = SendVoiceMessageAsync(speakString, style);
        }

        /// <summary>
        /// Start a new round, unless there is one in progress
        /// </summary>
        private void NewInitiative()
        {
            if (_inProgress)
            {
                SendTextMessage("Initiative round already in progress.");
            }
            else
            {
                _messages.Clear();
                _inProgress = true;
                SendTextMessage($"Initiative round {_round} is now starting.");
                _ = SendVoiceMessageAsync($"Round {_round} started", SpeakingStyle.excited);
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
                SendTextMessage("Initiative round is not in progress. You cannot 'restart'. Please type 'undo' to go back to the previous round.");
            }
        }

        /// <summary>
        /// Undo an initiative completion
        /// </summary>
        private void UndoCompleteInitiative()
        {
            if (_inProgress || _round == 1)
            {
                SendTextMessage("Initiative round already in progress. You cannot 'undo'. Please type 'restart' to restart the round.");
            }
            else
            {
                _inProgress = true;
                _round--;
                SendTextMessage("Initiative round " + _round + " is now in progress again. Enter new rolls.");
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
                var resultMessageList = new List<string>
                {
                    "Initiative round " + _round + " results:",
                    ""
                };
                foreach (var message in _messages)
                {
                    resultMessageList.Add(message.Sender + ": " + message.Roll);
                }
                var resultMessage = string.Join("\n", resultMessageList);
                SendTextMessage(resultMessage);
                if (_messages.Count != 0)
                {
                    var winner = _messages[0].Sender;
                    _ = SendVoiceMessageAsync($"{winner} wins", SpeakingStyle.whispering);
                }
                _round++;
                _inProgress = false;
            }
            else
            {
                SendTextMessage("Initiative round is not currently in progress");
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
                var newList = new List<int>
                {
                    Roll
                };
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
            var resultTallyList = new List<string>
            {
                "Initiative tally:",
                ""
            };
            foreach (var tally in tallyTotals)
            {
                resultTallyList.Add(tally.Sender + ": " + tally.Total + " (" + string.Join(", ", tally.Rolls!) + ")");
            }
            var resultTally = string.Join("\n", resultTallyList);
            SendTextMessage(resultTally);
        }

        /// <summary>
        /// Add a new value to the round in progress
        /// </summary>
        /// <param name="timestamp"></param>
        /// <param name="sender"></param>
        /// <param name="content"></param>
        private void AddMessage(DateTime timestamp, string sender, string content)
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
        private void SendTextMessage(string message)
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
        public DateTime? Timestamp { get; init; }
        public string? Sender { get; init; }
        public int Roll { get; init; }

        public int CompareTo(Message? other) => other?.Roll.CompareTo(Roll) ?? 0;
    }

    class Tally : IComparable<Tally>
    {
        public string? Sender { get; init; }
        public int Total { get; init; }
        public List<int>? Rolls { get; init; }
        public int CompareTo(Tally? other) => other?.Total.CompareTo(Total) ?? 0;
    }
}