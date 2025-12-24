/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system/syscall_linux_x64_numbers.inl
   Description: Functionality for making direct syscalls on Linux
*/

#pragma once
#ifdef __inline_sys_header__

struct linux_dirent64
{
    u64 d_ino;               /* 64-bit inode number */
    i64 d_off;               /* Not an offset; see getdents(2) man page */
    unsigned short d_reclen; /* Size of this dirent */
    char d_name[1024];       /* Filename (null-terminated) */
};

namespace cm {

DirectoryListGenerator::Iterator::Iterator()
    : fd(-1), buf(), nread(0), bpos(0), curr(""), status(STATUS_ERR_INVALID)
{}

DirectoryListGenerator::Iterator::Iterator(StringRef const& path)
    : fd(-1), buf(), nread(0), bpos(0), curr(""), status(STATUS_OK)
{
    // buf = new u8[1024];

    fd = isize(LinuxSyscall(LinuxSyscall.open, u64(path.data()), u64(O_RDONLY | O_DIRECTORY)));
    if (fd < 0) {
        setStatusFromErrno(status, int(-fd));
        fd = -1;
    }
}

DirectoryListGenerator::Iterator::~Iterator()
{
    // if (buf != nullptr) {
    //     delete[] buf;
    // }
}

bool DirectoryListGenerator::Iterator::operator==(Iterator const& other) const
{
    if (fd == -1 && other.fd == -1) {
        return true;
    } else {

        return false;  // return fd == other.fd;  // && curr == other.curr;
    }
}

UNSAFE_BEGIN
DirectoryListGenerator::Iterator& DirectoryListGenerator::Iterator::operator++()
{
    if (bpos < nread) {
        // On this iteration, there are 1 or more linux_dirent64 saved in the buffer for us to read
        using linux_dirent_ptr = struct linux_dirent64*;

        _Pragma("clang diagnostic push");
        _Pragma("clang diagnostic ignored \"-Wcast-align\"");

        auto d = linux_dirent_ptr(&buf[bpos]);
        _Pragma("clang diagnostic pop");
        // auto d_type = *(buf + bpos + d->d_reclen - 1);  // Get file type

        curr = StringRef(d->d_name);
        bpos += d->d_reclen;
        return *this;
    }
    // Otherwise read more linux_dirent64 objects
    nread = isize(LinuxSyscall(LinuxSyscall.getdents, u64(fd), u64(buf), u64(sizeof(buf))));
    bpos = 0;
    if (nread <= 0) {
        // either an error occured or we are done
        LinuxSyscall(LinuxSyscall.close, u64(fd));
        fd = -1;
        if (nread < 0) {
            setStatusFromErrno(status, int(-fd));
        }
    }
    return *this;
}
UNSAFE_END

#endif
