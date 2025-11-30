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

#include <commons/system.hh>


UNSAFE_BEGIN;
void cm::OutStream::_print(StringRef const& format, ArrayRef<ConstRefWrapper<Printable>> const& objects)
{
    auto fmtIter = format.begin();
    auto objIter = objects.begin();

    while (fmtIter.isNotEnd() && *fmtIter != '`') {
        char ch = *fmtIter;
        this->writeBytes(&ch, sizeof(char));
        ++fmtIter;
    }
    while (objIter.isNotEnd()) {
        Assert(fmtIter != format.end() && *fmtIter == '`', "More arguments than specified in format string");

        String str;
        (*objIter)->output(str);
        this->writeBytes(str.cstr(), str.sizeBytes());

        ++objIter;
        ++fmtIter;

        while (fmtIter.isNotEnd() && *fmtIter != '`') {
            char ch = *fmtIter;
            this->writeBytes(&ch, sizeof(char));
            ++fmtIter;
        }
    }
}
UNSAFE_END;
