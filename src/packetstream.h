/*
  Copyright (c) 2011, The Mineserver Project
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the The Mineserver Project nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _PACKETSTREAM_H
#define _PACKETSTREAM_H

#include <vector>
#include <deque>
#include <string>
#include <cstring>
#include <iterator>
#include <stdint.h>
#include <iconv.h>
#include <cstdio>

namespace Mineserver
{
  class PacketStream
  {
    typedef std::vector<uint8_t> ByteBuffer;

  private:
    ByteBuffer m_buffer;
    size_t m_pos;
    bool m_valid;
    iconv_t m_iconvUtfHandler;
    iconv_t m_iconvUcsHandler;

  public:
    PacketStream() : m_buffer(),m_pos(0),m_valid(true)
    {
      m_iconvUcsHandler = iconv_open("UTF8", "UCS-2BE");
      m_iconvUtfHandler = iconv_open("UCS-2BE", "UTF8");
    }

    ~PacketStream()
    {
      iconv_close(m_iconvUcsHandler);
      iconv_close(m_iconvUtfHandler);
    }

    inline void append(const uint8_t* const buffer, const size_t len)
    {
      m_buffer.insert(m_buffer.end(), buffer, buffer + len);
    }

    inline void append(const PacketStream& p)
    {
      m_buffer.insert(m_buffer.end(), p.m_buffer.begin(), p.m_buffer.end());
    }

    inline void remove()
    {
      m_buffer.erase(m_buffer.begin(), m_buffer.begin() + m_pos);
      m_pos = 0;
    }

    inline void reset()
    {
      m_pos = 0;
    }

    inline bool haveData(size_t len)
    {
      return (m_buffer.size() - m_pos) >= len;
    }

    PacketStream& operator<<(bool val);
    PacketStream& operator>>(bool& val);
    PacketStream& operator<<(int8_t val);
    PacketStream& operator>>(int8_t& val);
    PacketStream& operator<<(uint8_t val);
    PacketStream& operator>>(uint8_t& val);
    PacketStream& operator<<(int16_t val);
    PacketStream& operator>>(int16_t& val);
    PacketStream& operator<<(int32_t val);
    PacketStream& operator>>(int32_t& val);
    PacketStream& operator<<(int64_t val);
    PacketStream& operator>>(int64_t& val);
    PacketStream& operator<<(float val);
    PacketStream& operator>>(float& val);
    PacketStream& operator<<(double val);
    PacketStream& operator>>(double& val);
    PacketStream& operator<<(const std::string& str);
    PacketStream& operator>>(std::string& str);
    PacketStream& operator<<(const PacketStream& other);
    PacketStream& operator>>(const PacketStream& other);
  };
};

#endif