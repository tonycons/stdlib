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
#include <pthread.h>


namespace cm {

struct Mutex
{
    Mutex()
    {
        pthread_mutexattr_init(&_attr);
        pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &_attr);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_mutexattr_destroy(&_attr);
    }

    void lock() { pthread_mutex_lock(&_mutex); }
    void unlock() { pthread_mutex_unlock(&_mutex); }

private:
    pthread_mutex_t _mutex{};
    pthread_mutexattr_t _attr{};
};

}  // namespace cm
