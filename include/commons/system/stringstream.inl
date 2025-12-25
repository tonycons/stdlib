/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once
#ifdef __inline_sys_header__

namespace cm {

///
/// Defines a stream that just writes to some string
///
struct StringStream : public IOutStream<StringStream>
{
    StringStream() = delete;
    StringStream(String& to)
        : _to(to)
    {}

    inline StringStream& writeBytes(void const* data, usize sizeBytes)
    {
        _to.insert(_to.length(), String(static_cast<char const*>(data), sizeBytes));
        return *this;
    }

    inline StringStream& flush() { return *this; }

private:
    String& _to;
};

}  // namespace cm
#endif
