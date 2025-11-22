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
#ifdef __inline_core_header__


namespace cm {

void _checksumStore(void const*, __UINT32_TYPE__);
__UINT32_TYPE__ _checksumGet(void const*);
void _checksumRemove(void const*);

/**
 * @brief Attempts to detect memory corruption/tampering of a struct's member variables.
 * The way this works is at the start of every function call, it will check if the checksum of a struct's data is equal
 * to a stored checksum. (The stored checksum is stored externally)
 * If it is not equal, the program will crash. Otherwise, the function will continue to execute,
 * and upon return will update the stored checksum to that of its data.
 * So, if there is any modification of its data without the checksum being updated, it has a high chance of being
 * detected.
 * @tparam Derived The class to use checksum protection for.
 */
template<class Derived>
class ChecksumProtection {
public:
    ~ChecksumProtection() { _checksumRemove(this); }

protected:
    ChecksumProtection() { checksumUpdate(); }

    void checksumValidate() const
    {
        auto cg = _checksumGet(this);
        if (cg != 0) {
            Assert(cg == computeChecksum(), ASMS_DATA_CORRUPTION2);
        }
    }
    void checksumUpdate() const { _checksumStore(this, computeChecksum()); }

    struct ValidateAndUpdateChecksum
    {
        ValidateAndUpdateChecksum(ChecksumProtection const* cc)
            : cc(cc)
        {
            cc->checksumValidate();
        }
        ~ValidateAndUpdateChecksum() { cc->checksumUpdate(); }
        ChecksumProtection const* cc;
    };

private:
    u32 computeChecksum() const { return Hash<Crc32>::hashData(this, SizeOf<Derived>); }
};

#define expand_the_macro_to_the_moon(x, y) ValidateAndUpdateChecksum x##y(this)
#define CHECKSUM_PROTECT_expand_the_macro(x, y) expand_the_macro_to_the_moon(x, y)
#define CHECKSUM_PROTECTED CHECKSUM_PROTECT_expand_the_macro(_, __COUNTER__)
}  // namespace cm

#endif
