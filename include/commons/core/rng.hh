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


/*
    My fastest non cryptographic pseudorandom number generator
    Found via testing to be:

    ~16x faster than libc- rand
    ~10x faster than std::mt19937
    ~8x faster than using the clock cycle register
    ~2x faster than wyhash
*/
struct FastPRNG
{

    [[gnu::always_inline]]
    inline static u32 getU32()
    {
        /*
            movl    _state, %eax
            movzwl  %ax, %ecx
            crc32l  %ecx, %eax
            movl    %eax, _state
        */
        _state = __builtin_ia32_crc32si(_state, _state & 0x0000ffff);
        return _state;
    }

private:
    static u32 _state;
};

/*
    My fastest non deterministic and non cryptographic random number generator
*/
struct ND_PRNG
{
    inline static u32 getU32()
    {
        /*
            leaq    (%rip), %rax
            crc32l  _state(%rip), %eax
            movl    %eax, _state(%rip)
            rdtsc
            crc32l  _state(%rip), %eax
            retq
        */
        // Since the function is inlined, RIP will be different for each place in the code this function is called
        // This adds some nondeterminism, but not complete nondeterminism
        __UINT64_TYPE__ rip;
        asm volatile("lea (%%rip), %0" : "=r"(rip));
        _state = __builtin_ia32_crc32si(static_cast<u32>(rip), _state);
        // Further nondeterminism is added by the clock cycle register
        return __builtin_ia32_crc32si(static_cast<u32>(__builtin_readcyclecounter()), _state);
    }

private:
    static u32 _state;
};

}  // namespace cm
#endif
