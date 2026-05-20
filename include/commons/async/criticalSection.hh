/*
   Copyright 2026 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once
#include <commons/async/mutex.hh>

namespace cm {

///
/// Used when guarding a critical section to make sure you don't forget to unlock your mutex!
/// Upon creation given a mutex, it locks the mutex. When this object is destroyed, the mutex is unlocked.
///
struct CriticalSection
{
    explicit CriticalSection(Mutex& mutex)
        : _mutex(&mutex)
    {
        _mutex->lock();
    }

    ~CriticalSection()
    {
        [[assume(_mutex != nullptr)]];
        _mutex->unlock();
    }

private:
    Mutex* _mutex;
};

}  // namespace cm
