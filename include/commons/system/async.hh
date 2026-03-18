#pragma once

#include "../datastructs.hh"


#include <pthread.h>


namespace cm {

struct Mutex
{
    Mutex() { pthread_mutex_init(&_mutex, &_attr); }
    ~Mutex() { pthread_mutex_destroy(&_mutex); }

    void lock() { pthread_mutex_lock(&_mutex); }
    void unlock() { pthread_mutex_unlock(&_mutex); }

private:
    pthread_mutex_t _mutex{};
    pthread_mutexattr_t _attr{};
};

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


template<typename T>
struct AddConstReferenceIfNotVoid
{
    using Type = T const&;
};

template<>
struct AddConstReferenceIfNotVoid<void>
{
    using Type = void;
};

///
/// Represents a single thread of execution. begin execution immediately upon construction of the associated thread
/// object (pending any OS scheduling delays), starting at the top-level function provided as a constructor argument.
///
template<typename ReturnType>
struct Thread : NonCopyable, NonMovable
{
private:
    Optional<SelectType<IsSame<ReturnType, void>, ReturnType, int>> _result{};
    IFunction<ReturnType()>* _func{};
    pthread_t _tid{};
    pthread_attr_t _attr{};
    bool _joined{};
    DLList<IFunction<void()> const*> _callbacks{};

public:
    explicit Thread(auto&& func)
    {
        _func = new Closure<sizeof(func), ReturnType()>(func);
        pthread_attr_init(&_attr);
        pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setguardsize(&_attr, LibraryConfig::DEFAULT_THREAD_GUARD_SIZE);
        pthread_attr_setstacksize(&_attr, LibraryConfig::DEFAULT_THREAD_STACK_SIZE);

        pthread_create(
            &_tid, &_attr,
            [](void* arg) -> void* {
                auto self = static_cast<Thread*>(arg);
                if constexpr (!IsSame<ReturnType, void>) {
                    self->_result = (*(self->_func))();
                } else {
                    (*(self->_func))();
                }
                return nullptr;
            },
            this);
    }

    ~Thread()
    {
        // Make sure the thread finishes
        await();
        pthread_attr_destroy(&_attr);
        delete _func;
    }

    AddConstReferenceIfNotVoid<ReturnType>::Type await()
    {
        if (!_joined) {
            pthread_join(_tid, nullptr);
            _joined = true;
            // Call and then reset the callbacks
            _callbacks.forEach([](IFunction<void()> const* f) {
                (*f)();
                delete f;
            });
            _callbacks.clear();
        }
        if constexpr (!IsSame<ReturnType, void>) {
            return _result.ref();
        } else {
            return;
        }
    }

    Thread& callback(auto&& func)
    {
        _callbacks.end().insert(new Closure<sizeof(func), void()>(func));
        return *this;
    }
};

template<typename Func>
Thread(Func&& func) -> Thread<typename FunctionTraits<Func>::ReturnType>;

//
// template<typename T>
// class Promise {
// private:
//     Thread<T>* _thread{};
//     Optional<T> _result{};
//
// public:
//     Promise(auto&& func)
//         : _thread(new Thread<T>(func))
//     {}
//
//     ~Promise()
//     {
//         //;
//     }
//
//     T const& await() { return _thread->await(); }
// };
// auto async(auto&& func) { return Promise<typename FunctionTraits<decltype(func)>::ReturnType>(func); }


}  // namespace cm
