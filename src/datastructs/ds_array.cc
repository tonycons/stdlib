// /*
//    Copyright 2024 Anthony A. Constantinescu.

// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.
// */

// #include <commons/datastructs/array.hh>
//
// #include <commons/system/Profiler.hh>

// namespace cm {

// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/9/2025
// ///
// //
// ArrayBuffer::ArrayBuffer()
//     : _buffer(_initial_buffer), _size(0), _capacity(16), _initial_buffer{}, _dtor_called(false)
// {}

// ArrayBuffer::~ArrayBuffer()
// {
//
//     CM_CHECKSUM_PROTECTED;

// Assert(!_dtor_called, ASMS_BUG);
// _deallocate();
// }

// //
// ///----------------------------------------------------------------------------------------------------------------------
// ///
// //
// [[gnu::noinline]]
// void ArrayBuffer::clear(Class const& objClass) UNSAFE({
//
//     CM_CHECKSUM_PROTECTED;

// Assert(0 <= objClass.sizeBytes && objClass.sizeBytes <= 65536);

// if (objClass.destructor != nullptr) {
//     for (u32 i = 0; i < _size; i += objClass.sizeBytes) {
//         objClass.destructor(&_buffer[i]);
//     }
// }
// _deallocate();
// _buffer = _initial_buffer;
// _size = 0;
// _capacity = 16;
// __builtin_memset_inline(_initial_buffer, 0, 16);
// });

// //
// ///----------------------------------------------------------------------------------------------------------------------
// ///
// //
// void* ArrayBuffer::append(void const* pObjects, u32 numObjects, Class const& objClass)
// {
//
//     CM_CHECKSUM_PROTECTED;

// Assert(pObjects, "Null object array");
// Assert(0 < numObjects && 0 < objClass.sizeBytes, "Invalid objectSize or numObjects");
// Assert((0 < (objClass.sizeBytes * numObjects)) && ((objClass.sizeBytes * numObjects) < 0x7fffffff), "Too large");

// u32 sizeBytes = objClass.sizeBytes * numObjects;

// _Pragma("GCC diagnostic push") {}
// _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"") {}

// _tryExpand(sizeBytes);
// void* basePtr = &_buffer[_size];

// if (pObjects != nullptr) {
//     if (objClass.copyConstructor) {
//         auto* dstBytePtr = reinterpret_cast<u8*>(basePtr);
//         auto* srcBytePtr = reinterpret_cast<u8 const*>(pObjects);

// for (u32 i = 0; i < numObjects; i++) {
//     objClass.copyConstructor(dstBytePtr, srcBytePtr);
//     srcBytePtr += objClass.sizeBytes;
//     dstBytePtr += objClass.sizeBytes;
//     _size += objClass.sizeBytes;
// }
// } else {
// __builtin_memmove(basePtr, pObjects, size_t(sizeBytes));
// _size += sizeBytes;
// }
// } else {
// if (objClass.defaultConstructor) {
// u8* bytePtr = reinterpret_cast<u8*>(basePtr);

// for (u32 i = 0; i < numObjects; i++) {
//     objClass.defaultConstructor(bytePtr);
//     bytePtr += objClass.sizeBytes;
//     _size += objClass.sizeBytes;
// }
// } else {
// __builtin_memset(basePtr, 0, size_t(sizeBytes));
// _size += sizeBytes;
// }
// }
// _Pragma("GCC diagnostic pop") {}

// return basePtr;
// }

// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/8/2025
// ///
// //
// void ArrayBuffer::erase(u32 objectIndex, u32 numObjects, Class const& objClass) UNSAFE({
//
//     CM_CHECKSUM_PROTECTED;

// Assert(0 <= objectIndex && 0 <= objClass.sizeBytes && 0 <= objectIndex);
// Assert(objClass.sizeBytes * (objectIndex + numObjects) <= 0x7fffffff);
// Assert(static_cast<u32>(objClass.sizeBytes * (objectIndex + numObjects)) <= _size);

// // Index into our byte buffer that we start erasing from.
// auto const byteIndex = objectIndex * objClass.sizeBytes;

// // The number of bytes to erase.
// auto const sizeBytes = numObjects * objClass.sizeBytes;

// // Call the destructor on each object if provided.
// if (objClass.destructor) {
//     u8* objectsBytePtr = reinterpret_cast<u8*>(&_buffer[byteIndex]);
//     for (u32 i = 0; i < numObjects; i++) {
//         objClass.destructor(objectsBytePtr);
//         objectsBytePtr += objClass.sizeBytes;
//     }
// }
// // Then shift the buffer elements to fill the gap
// if ((byteIndex) != _size - 1) {
//     auto bytesToMove = _size - sizeBytes;
//     Assert(bytesToMove >= 0 && bytesToMove <= _size);
//     __builtin_memmove(&_buffer[byteIndex], &_buffer[byteIndex + sizeBytes], size_t(bytesToMove));
// }
// _size -= sizeBytes;
// _tryShrinking();
// });


// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/8/2025
// ///
// //
// void ArrayBuffer::_reallocate()
// {
//
//     CM_CHECKSUM_PROTECTED;

// Assert(_capacity > 0);

// // Allocate a new larger buffer
// auto* newBuffer = new u8[size_t(_capacity)]{0};

// _Pragma("GCC diagnostic push") {}
// _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"") {}

// // Copy the old buffer to the new buffer
// __builtin_memcpy(newBuffer, _buffer, size_t(_size));
// _Pragma("GCC diagnostic pop") {}

// // Delete our old buffer
// _deallocate();

// // Set our buffer to the new buffer
// _buffer = newBuffer;
// }

// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/8/2025
// ///
// //
// void ArrayBuffer::_deallocate() UNSAFE({
//
//     CM_CHECKSUM_PROTECTED;

// Assert(_buffer != nullptr);

// if (_buffer != _initial_buffer  //&& _size > 16
//                                 //
// )
// {
//     delete[] _buffer;
//     _buffer = nullptr;
// }
// });

// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/9/2025
// ///
// //
// void ArrayBuffer::_tryExpand(u32 reqAdditionalSizeBytes) UNSAFE({
//
//     CM_CHECKSUM_PROTECTED;

// Assert(_capacity >= 16, ASMS_DATA_CORRUPTION);

// if (_size + reqAdditionalSizeBytes >= _capacity - 1) {

// // Calculate the new capacity
// auto prevCapacity = _capacity;

// if (_capacity < 256) {
//     _capacity *= 2;
// } else {
//     _capacity += (_capacity << 1u);
// }
// if (_capacity < prevCapacity + reqAdditionalSizeBytes) {
//     _capacity += reqAdditionalSizeBytes;
// }
// Assert(_capacity >= 16, ASMS_BUG);
// _reallocate();
// }
// });

// //
// ///----------------------------------------------------------------------------------------------------------------------
// /// edit 5/9/2025
// ///
// //
// void ArrayBuffer::_tryShrinking()
// {
//
//     CM_CHECKSUM_PROTECTED;

// // Determine if shrinking is necessary
// if (_size > 16 && _size < (_capacity / 4)) {
//     _capacity = max(16u, _capacity / 2);

// if (_capacity <= 16) {  // Go back to using the _initial_buffer if shrunk enough
//     __builtin_memset_inline(_initial_buffer, 0, 16);
//     _deallocate();
//     _buffer = _initial_buffer;
// } else {
//     _reallocate();
// }
// }
// }

// }  // namespace cm
