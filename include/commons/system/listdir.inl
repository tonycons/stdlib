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

struct DirectoryListGenerator
{
    StringRef path;

    struct Iterator : public NonCopyable
    {
        isize fd;
        u8 buf[1024];
        isize nread;
        isize bpos;
        StringRef curr;
        StreamStatus status;

        Iterator();
        Iterator(StringRef const& path);
        ~Iterator();
        bool operator==(Iterator const& other) const;
        Iterator& operator++();

        inline StringRef const& operator*() const { return curr; }
    };

    Iterator begin() const { return Iterator(path); }
    Iterator end() const { return Iterator(); }
};


}  // namespace cm
#endif
