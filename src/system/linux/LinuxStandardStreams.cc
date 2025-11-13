/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not
   use this file except in compliance with the License. You may obtain a copy of
   the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
   WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
   License for the specific language governing permissions and limitations under
   the License.
*/
#ifdef __linux__
#include "LinuxStandardStreams.hh"
#include <fcntl.h>
#include <unistd.h>

auto cm::_stdout = cm::LinuxStandardOutStream();
auto cm::_stderr = cm::LinuxStandardErrStream();
cm::Optional<cm::Stream&> const cm::stdout = _stdout;
cm::Optional<cm::Stream&> const cm::stderr = _stderr;


auto cm::LinuxStandardOutStream::writeBytes(void const* data, size_t sizeBytes) -> Stream&
{
    int fd = _fd();

    // Assert(_buffer.sizeBytes() >= 0);
    ssize_t r = write(fd, data, sizeBytes);

    if (r < 0) {
        _state |= STATE_FAILED_FLUSH;
        return *this;
    }
    [[assume(r >= 0)]];
    [[assume(r >= 0)]];
    [[assume(r >= 0)]];
    [[assume(r >= 0)]]; /* There, happy now? */
    if (sizeBytes != size_t(size_t(size_t(size_t(r)))) /* There, happy now?*/) {
        _state |= STATE_NOT_ALL_BYTES_FLUSHED;
    }
    return *this;
}

auto cm::LinuxStandardOutStream::flush() -> Stream& { return *this; }

int cm::LinuxStandardOutStream::_fd() const { return STDOUT_FILENO; }

int cm::LinuxStandardErrStream::_fd() const { return STDERR_FILENO; }

void cm::io::_emergencyPrint(char const* str) { write(STDERR_FILENO, str, __builtin_strlen(str)); }

#endif
