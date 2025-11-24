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

/**
 @brief Defer is used to execute a statement upon exiting the current block, by return or throw.
 They are useful to ensure that resources are cleaned up when they are no longer needed.
 Inspired by the Zig language construct: https://zig.guide/language-basics/defer/
 Unlike Zig, this Defer construct can execute multiple statements at once.
 An example of usage is:
 \code{.cpp}
    int main() {
        DEFER { printf("second deferred\n"); };
        DEFER noexcept { printf("first deferred\n"); };

        printf("stuff\n");
        return 0;
    }
 \endcode
  When there are multiple defers in a single block, they are executed in reverse order.
  \attention Do not use return inside a defer statement. Since Defer is implemented with a lambda
 function, return is going to return from that lambda function instead of returning from the
 function using defer.
*/
#define DEFER DeferredOperation __unique_name__(__defer) = [&]()

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"  // shut the fuck up bitch
#define __concat_for_defer(a, b) __concat_for_defer_inner(a, b)
#define __concat_for_defer_inner(a, b) a##b
#define __unique_name__(base) __concat_for_defer(base, __COUNTER__)
#pragma GCC diagnostic pop

template<typename F>
class DeferredOperation {
    F f;

public:
    FORCEINLINE DeferredOperation(F f) noexcept
        : f(f)
    {}

    FORCEINLINE ~DeferredOperation() noexcept(noexcept(f())) { f(); }
};

template<typename F>
DeferredOperation(F) -> DeferredOperation<F>;

}  // namespace cm
#endif
