// #pragma once

// #include "../datastructs.hh"  // IWYU pragma: keep

// #include <pthread.h>

// #include <commons/async/mutex.hh>
// #include <commons/async/criticalSection.hh>

// namespace cm {


// template<typename T>
// struct AddConstReferenceIfNotVoid
// {
//     using Type = T const&;
// };

// template<>
// struct AddConstReferenceIfNotVoid<void>
// {
//     using Type = void;
// };


// ///
// /// Represents a single thread of execution. begin execution immediately upon construction of the associated thread
// /// object (pending any OS scheduling delays), starting at the top-level function provided as a constructor argument.
// ///
// struct Thread : NonCopyable, NonMovable
// {
//     IFunction<void()>* _func;
//     pthread_t _tid{};
//     pthread_attr_t _attr{};
//     bool _joined{};
//     DLList<IFunction<void()> const*> _callbacks{};
//     Arena<u8>* _allocator{};
//     ArrayRef<u8> _stack{};

// ///
// /// Initialize a thread
// /// @param allocator The memory allocator for the thread
// ///
// explicit Thread(Arena<u8>& allocator = mainAllocator<u8>)
// {
//     _allocator = &allocator;
//     _stack = _allocator->allocateArray(LibraryConfig::DEFAULT_THREAD_STACK_SIZE).val();

// pthread_attr_init(&_attr);
// pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
// pthread_attr_setguardsize(&_attr, LibraryConfig::DEFAULT_THREAD_GUARD_SIZE);
// pthread_attr_setstack(&_attr, _stack.data(), _stack.sizeBytes());
// }

// ///
// /// Start execution immediately at a provided entry point.
// /// @param entry The entry point
// /// @param allocator The memory allocator for the thread
// ///
// void start(IsCallableAndReturns<void> auto&& entry)
// {
//     _func = new (_allocator) Closure<sizeof(entry), void()>(entry);
//     auto threadEntryPoint = [](void* arg) -> void* {
//         auto self = static_cast<Thread*>(arg);
//         self->_func->operator()();
//         // self->_func->operator()();
//         return nullptr;
//     };
//     pthread_create(&_tid, &_attr, threadEntryPoint, this);
// }

// ///
// /// Destructor
// ///
// ~Thread()
// {
//     cancel();
//     pthread_attr_destroy(&_attr);
//     _allocator->deallocate(_stack.data());
//     ::operator delete(_func, _allocator);
// }

// ///
// /// Stop whatever the thread is doing
// ///
// void cancel() { pthread_cancel(_tid); }

// ///
// /// Wait for the thread to finish.
// ///
// void join()
// {
//     if (!_joined) {
//         pthread_join(_tid, nullptr);
//         _joined = true;
//         // Call and then reset the callbacks
//         _callbacks.forEach([&](IFunction<void()> const* f) {
//             (*f)();
//             ::operator delete(const_cast<IFunction<void()>*>(f), _allocator);
//         });
//         _callbacks.clear();
//     }
// }


// ///
// /// Sets a callback to be executed when the thread finishes. This callback must be a function taking no arguments
// /// and returning void.
// ///
// Thread& callback(auto&& func)
// {
//     _callbacks.end().insert(new (_allocator) Closure<sizeof(func), void()>(func));
//     return *this;
// }

// ///
// /// Returns the thread's allocator
// ///
// Arena<u8>* getAllocator() { return _allocator; }
// };

// ///
// /// A promise is a subtype of Thread whose specific purpose is to eventually return a value.
// ///
// template<typename ReturnType>
// struct Promise : Thread
// {
//     static_assert(!IsSame<ReturnType, void>, "A Promise cannot return void");
//     Optional<ReturnType> _result{};

// ///
// /// Initialize a promise, starting execution immediately at a provided entry point.
// ///
// explicit Promise(auto&& entry, with_default_allocator(u8))
//     : Thread(allocator)
// {
//     (void)entry;
//     Thread::start([this /*, entry_ = Closure<sizeof(entry), ReturnType()>(entry)*/] -> void {
//         __builtin_printf("Thread started\n");
//         __builtin_printf("this addr %p\n", static_cast<void*>(this));
//         __builtin_printf("Result addr %p\n", static_cast<void*>(&_result));

// asm volatile("" ::: "memory");
// _result = 10; /*entry_();*/
// });
// }

// ///
// /// Waits for the return value of the thread's main function
// ///
// ReturnType await()
// {
//     Thread::join();
//     return _result.ref();
// }
// };

// template<typename Func>
// Promise(Func&& func) -> Promise<typename FunctionTraits<Func>::ReturnType>;


// }  // namespace cm
