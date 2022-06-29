using Concentus.Oggfile;
using Concentus.Structs;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Zoom_CSharp_ChatBot.Speech
{
    internal class OggDecoderStream : Stream, IDisposable
    {
        private readonly MemoryStream _pcmStream = new();
        public OggDecoderStream(byte[] data)
        {
            using var dataStream = new MemoryStream(data);
            DecodeOggStream(dataStream);
        }
        public OggDecoderStream(Stream data)
        {
            DecodeOggStream(data);
        }

        private void DecodeOggStream(Stream dataStream)
        {
            var decoder = new OpusDecoder(48000, 1);
            var oggReader = new OpusOggReadStream(decoder, dataStream);
            while (oggReader.HasNextPacket)
            {
                var packet = oggReader.DecodeNextPacket();
                for (int i = 0; i < packet.Length; i++)
                {
                    var oggbytes = BitConverter.GetBytes(packet[i]);
                    _pcmStream.Write(oggbytes, 0, oggbytes.Length);
                }
            }
            _pcmStream.Position = 0;
        }

        public override bool CanRead => _pcmStream.CanRead;

        public override bool CanSeek => _pcmStream.CanSeek;

        public override bool CanWrite => _pcmStream.CanWrite;

        public override long Length => _pcmStream.Length;

        public override long Position { get => _pcmStream.Position; set => _pcmStream.Position = value; }

        public override void Flush() => _pcmStream.Flush();

        public override int Read(byte[] buffer, int offset, int count) =>
            _pcmStream.Read(buffer, offset, count);


        public override long Seek(long offset, SeekOrigin origin) =>
            _pcmStream.Seek(offset, origin);

        public override void SetLength(long value) =>
            _pcmStream.SetLength(value);

        public override void Write(byte[] buffer, int offset, int count) =>
            _pcmStream.Write(buffer, offset, count);

        protected override void Dispose(bool disposing)
        {
            _pcmStream.Close();
            base.Dispose(disposing);
        }
    }
}
