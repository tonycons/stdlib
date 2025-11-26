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

#include <commons/datastructs/Sparsearray.hh>


namespace cm {


void SparseArrayBase::clear(void (*dtor)(void*))
{
    void** ptr = _data;

    auto visitor = [&](this auto const& visitor_, unsigned depth) -> void {
        if (depth == 0) {
            for (int k = 0; k < 256; k++) {
                if (ptr[k]) {
                    dtor(ptr);
                    __builtin_free(ptr);
                }
            }
        } else {
            for (int k = 0; k < 256; k++) {
                if (ptr[k] != nullptr) {
                    ptr = reinterpret_cast<void**>(ptr[k]);
                    visitor_(depth - 1);
                    delete[] ptr;
                }
            }
        }
    };
    visitor(8);
}


}  // namespace cm
