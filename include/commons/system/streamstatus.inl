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
/// Bitmask representing the error states of the stream.
///
enum [[clang::flag_enum]] StreamStatus : u64 {
    STATUS_OK = 0,
    STATUS_FAILED_INIT = (1 << 0),
    STATUS_FAILED_WRITE = (1 << 1),
    STATUS_FAILED_FLUSH = (1 << 2),
    STATUS_NOT_ALL_BYTES_FLUSHED = (1 << 3),

    // The last operation failed because the stream is invalid
    STATUS_ERR_INVALID = (1 << 4),

    // The last operation was interrupted by a signal before it could complete.
    STATUS_ERR_INTERRUPT = (1 << 5),

    // The last operation failed due to an I/O error.
    STATUS_ERR_IO = (1 << 6),

    // The last operation failed due to an unknown error.
    STATUS_ERR_UNKNOWN = (1 << 7),

    STATUS_ERR_ACCESS = (1 << 8),
    STATUS_ERR_BUSY = (1 << 9),
    STATUS_ERR_QUOTA = (1 << 10),

    STATUS_ERR_ALREADY_EXISTS = (1 << 11),

    // The operation failed because the type of the stream might not support that operation.
    STATUS_ERR_UNSUPPORTED = (1 << 12),

    STATUS_ERR_LOOP = (1 << 13),
    STATUS_ERR_LIMIT = (1 << 14),

    STATUS_ERR_NOT_FOUND = (1 << 15),
    STATUS_ERR_MEMORY = (1 << 16),


    // The last operation failed for any reason
    STATUS_ANY_ERR = 0xffff'ffff'ffff'fffe,
};

}  // namespace cm

#endif
