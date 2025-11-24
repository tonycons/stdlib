// /*
//    Copyright 2025 Anthony A. Constantinescu.

// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.
// */

// #pragma once
// #include <commons/core.hh>
// #include <commons/core.hh>
// #include <commons/core.hh>
// #include <commons/core.hh>


// namespace cm {

// /**
//  * @brief Class which implements an expandable memory segment, for use by Array with a non-fixed size
//  */
// struct ArrayBuffer final : NonCopyable, protected ChecksumProtection<ArrayBuffer>
// {
// public:
//     using DefaultConstructor = CFunction<void(void*)>;
//     using CopyConstructor = CFunction<void(void*, void const*)>;

// ArrayBuffer();
// ~ArrayBuffer();

// /**
//  * @brief Removes all objects from the buffer.
//  * @param objClass The class of the objects.
//  */
// void clear(Class const& objClass);

// /**
//  * @brief Reserve space for objects.
//  * @param numObjects The number of additional objects to reserve space for.
//  * @param objClass The class of the objects.
//  */
// void reserve(u32 numObjects, Class const& objClass);

// /**
//  * @brief Appends elements to the buffer.
//  * @param pObjects Pointer to an array of objects to append. If null, default (empty) objects are appended.
//  * @param numObjects The number of objects to append.
//  * @param objClass The class of the objects.
//  * @return A pointer to the first object appended.
//  */
// void* append(void const* pObjects, u32 numObjects, Class const& objClass);

// /**
//  * @brief Erases a series of objects from the buffer.
//  * @param objectIndex The index of the first object to erase.
//  * @param numObjects The number of objects to erase.
//  * @param objClass The class of the objects.
//  */
// void erase(u32 objectIndex, u32 numObjects, Class const& objClass);

// /**
//  * @brief Returns the size bytes of the total number of elements in the buffer.
//  */
// inline u32 sizeBytes() const { return this->_size; }

// /**
//  * @brief Returns the capacity: How many elements can be stored without resizing the internal buffer.
//  */
// inline u32 capacityBytes() const { return this->_capacity; }

// /**
//  * @brief Returns a pointer to the internal buffer.
//  * @note Not safe to hold on to this pointer if objects are being appended to the buffer.
//  */
// inline u8* buf() const { return this->_buffer; }

// private:
//     u8* _buffer;
//     u32 _size;
//     u32 _capacity;
//     u8 _initial_buffer[16];
//     bool _dtor_called = false;
//     // i32 expand_called_counter = 0;

// void _reallocate();
// void _deallocate();
// void _tryExpand(u32 reqAdditionalSizeBytes);
// void _tryShrinking();
// };

// // static_assert(sizeof(ArrayBuffer) == 32);

// }  // namespace cm
