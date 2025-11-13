#ifdef _WIN32

#include <Commons/core.hh>
#include <Commons/system/Profiler.hh>
#include "WindowsNT.hh"  // IWYU pragma: keep
#include "WinStandardStreams.hh"
#include "Win32Error.hh"  // IWYU pragma: keep

// idgaf about warnings in this file

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-identifier"
#pragma GCC diagnostic ignored "-Weverything"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      VARIABLES
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static SRWLOCK s_heapLock;
static int argc;
static char* argv[64];
static void (*atexit_entries[64])();
static int num_atexit_entries;
static cm::Win32StandardOutStream* _win32_stdout;
static cm::Win32StandardErrStream* _win32_stderr;
static UINT8 _win32_stdout_data[sizeof(cm::Win32StandardOutStream)];
static UINT8 _win32_stderr_data[sizeof(cm::Win32StandardErrStream)];

// fluff: Just random symbols and functions clang expects to exist on windows.

/*
__fltused implies you are using or have at least declared some floats or doubles. The compiler
injects this 'useless' symbol to cause a floating support .obj to get loaded from the crt.
*/
extern "C" int _fltused = 0;
extern "C" __UINT32_TYPE__ _tls_index = 0;

#if __cpp_threadsafe_static_init
extern "C" int _Init_global_epoch = -__INT_MAX__;
extern "C" thread_local int _Init_thread_epoch = -__INT_MAX__;
extern "C" SRWLOCK _Init_thread_guard{};
constexpr int eUninitialized = 0;
constexpr int eBeingInitialized = -1;
#endif

int main(int, char**);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      HELPERS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
///----------------------------------------------------------------------------------------------------------------------
/// edit 5/8/2025
///
//
static void initArgcArgv()
{
    bool flag = false;
    char* c1 = ::GetCommandLineA();

    argv[0] = c1;
    argc = 1;

    while (*c1) {
        if (*c1 == '\"') {
            flag = !flag;
        } else if (*c1 == ' ' && flag == 0) {
            if (*(c1 + 1)) {
                argv[argc] = c1 + 1;
                argc++;
            }
            *c1 = '\0';
        }
        c1++;
    }
}

///----------------------------------------------------------------------------------------------------------------------
/// edit 2024
///
//
extern "C" int atexit(void (*callback)())
{
    // this->atexit_guard.lock();
    bool full = num_atexit_entries >= 64;
    if (full)
        goto end;

    for (int i = 0; i < num_atexit_entries; i++) {
        if (atexit_entries[i] == callback)
            goto end;
    }
    atexit_entries[num_atexit_entries++] = callback;
end:
    // this->atexit_guard.unlock();
    return full;
    // return 1;
}

///----------------------------------------------------------------------------------------------------------------------
/// edit 2024
///
//
extern "C" [[noreturn]]
void exit(int code)
{
    for (int i = 0; i < num_atexit_entries; i++) {
        if (atexit_entries[i])
            atexit_entries[i]();
    }

    ExitProcess(code);
    while (true)
        ;
}


//
///----------------------------------------------------------------------------------------------------------------------
/// edit 5/8/2025
///
///  * This is the "real" entry point, where the CRT would initialize
///
//
$ExternC $NoReturn $DLLExport [[gnu::force_align_arg_pointer]]
void mainCRTStartup()
{

    // Initialze the heap and other variables
    InitializeSRWLock(&s_heapLock);
    num_atexit_entries = 0;
    _win32_stdout = new (_win32_stdout_data) cm::Win32StandardOutStream();
    _win32_stderr = new (_win32_stderr_data) cm::Win32StandardErrStream();

    // Initialize the command line
    initArgcArgv();
#if __cpp_threadsafe_static_init
    ::InitializeSRWLock(&_Init_thread_guard);
#endif
    cm::Profiler::init();

    __try
    {
        cm::Profiler::push(cm::SourceLocation(0, 0, "main", "main"));
        int exitCode = main(argc, argv);
        cm::Profiler::pop();
        exit(exitCode);
    }
    __except (EXCEPTION_CONTINUE_SEARCH)
    {
        cm::Profiler::printStackTrace();
        cm::panic("Exception not handled by __C_specific_handler\n", "", {});
    }
}


///
/// edit 5/8/2025
///
extern "C" void abort() { cm::panic("aborted", "", {}); }

///
/// edit 5/8/2025
///
extern "C" void _purecall() { cm::panic("purecall", "", {}); }

///
/// edit 5/8/2025
///
auto cm::System::getStandardOutStream() -> Stream& { return *_win32_stdout; }

///
/// edit 5/8/2025
///
auto cm::System::getStandardErrStream() -> Stream& { return *_win32_stderr; }


/*
 Windows structured exceptions handler (SEH)
 (They just had to make it an acronym to sound cool)
 Allows us to respond to hard errors and print debug info, etc.

  _VCRTIMP EXCEPTION_DISPOSITION __cdecl __C_specific_handler !!!!

  AAH! So scary! Lots of underscores and capital letters
  Yes, it has to be named like this.
  Typical Windows API nonsense
*/
extern "C" _VCRTIMP _EXCEPTION_DISPOSITION __cdecl __C_specific_handler(
    _EXCEPTION_RECORD* ExceptionRecord, void* EstablisherFrame, _CONTEXT* ContextRecord,
    _DISPATCHER_CONTEXT* DispatcherContext)
{

    DWORD _ = 0u;
    auto code = ExceptionRecord->ExceptionCode;
    auto out = GetStdHandle(-12);

    constexpr char const m1[] = "\x1B[31mAn exception was raised by the system: ";
    constexpr char const m2[] = "\nCause: ";
    constexpr char const m3[] = "\x1B[0m\n";

    // The extra spaces are necessary to make the strings have same length
    // This makes it easier to print them in a context with limited functions
    // clang-format off
    struct S { char const* _1; char const* _2; };
    S s =
    code == STATUS_ACCESS_VIOLATION ? S{
        "STATUS_ACCESS_VIOLATION      ",
        "Read or write memory in a restricted area               "} :
    code == STATUS_ARRAY_BOUNDS_EXCEEDED ? S{
        "STATUS_ARRAY_BOUNDS_EXCEEDED ",
        "Accessing array element out of bounds                   "} :
    code == STATUS_STACK_OVERFLOW ? S{
        "STATUS_STACK_OVERFLOW        ",
        "Stack overflow                                          "} :
    code == 0xC000008E ? S{
        "STATUS_FLOAT_DIVIDE_BY_ZERO  ",
        "Float division by zero                                  "} :
    code == 0x80000002 ? S{
        "STATUS_DATATYPE_MISALIGNMENT ",
        "A data misalignment error in a load or store instruction"} :
    code == 0xC000001D ? S{
        "STATUS_ILLEGAL_INSTRUCTION   ",
        "Invalid or unsupported hardware instruction             "} :
    code == 0xC0000096 ? S{
        "STATUS_PRIVILEGED_INSTRUCTION",
        "No privilege to execute the hardware instruction        "} :
    code == 0x80000003 ? S{
        "STATUS_BREAKPOINT            ",
        "A breakpoint was triggered                              "} :
    code == 0x80000004 ? S{
        "STATUS_SINGLE_STEP           ",
        "A single step or trace operation was completed          "} :
    code == STATUS_NO_MEMORY ? S{
        "STATUS_NO_MEMORY             ",
        "Not enough memory to satisfy the allocation request     "} :
        S{
        "Unknown                      ",
        "Unknown                                                 "};


    // clang-format on
    WriteFile(out, m1, sizeof(m1), &_, nullptr);
    WriteFile(out, s._1, 29, &_, nullptr);
    WriteFile(out, m2, sizeof(m2), &_, nullptr);
    WriteFile(out, s._2, 55, &_, nullptr);
    ::cm::Profiler::printStackTrace();
    WriteFile(out, m3, sizeof(m3), &_, nullptr);

    return ExceptionContinueSearch;
}
#endif
