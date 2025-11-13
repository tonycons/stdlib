#if __STDC_HOSTED__

#pragma GCC diagnostic ignored "-Weverything"

#include "WinStandardStreams.hh"
#include "WindowsNT.hh"


static u8 s_StandardOutStream[sizeof(cm::Win32StandardOutStream)];
static u8 s_StandardErrStream[sizeof(cm::Win32StandardErrStream)];
extern int appMain(int argc, char** argv);

///
///
///
auto cm::System::getStandardOutStream() -> Stream& { return *PtrWin32StandardOutStream(s_StandardOutStream); }

///
///
///
auto cm::System::getStandardErrStream() -> Stream& { return *PtrWin32StandardErrStream(s_StandardErrStream); }

///
///
///
int main(int argc, char** argv)
{
    using namespace cm;
    new (s_StandardOutStream) Win32StandardOutStream();
    new (s_StandardErrStream) Win32StandardErrStream();

    int code = appMain(argc, argv);

    PtrWin32StandardOutStream(s_StandardOutStream)->~Win32StandardOutStream();
    PtrWin32StandardErrStream(s_StandardErrStream)->~Win32StandardErrStream();
    return code;
}

///
///
///
void cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    DWORD _;
    HANDLE out = GetStdHandle(STD_ERROR_HANDLE);

    // Red color begin

    WriteFile(out, "\x1B[31m", sizeof("\x1B[31m"), &_, nullptr);
    WriteFile(out, message, DWORD(__builtin_strlen(message)), &_, nullptr);

    // Print source location and reason

    if (src.function() || src.file()) {
        WriteFile(out, " in ", 4, &_, nullptr);
    }
    if (src.function() != nullptr) {
        WriteFile(out, src.function(), DWORD(__builtin_strlen(src.function())), &_, nullptr);
    }
    if (src.file() != nullptr) {
        WriteFile(out, " at \"", 5, &_, nullptr);
        WriteFile(out, src.file(), DWORD(__builtin_strlen(src.file())), &_, nullptr);
        WriteFile(out, "\"", 1, &_, nullptr);
    }
    if (reason != nullptr) {
        WriteFile(out, ".\n\treason: ", 11, &_, nullptr);
        WriteFile(out, reason, DWORD(__builtin_strlen(reason)), &_, nullptr);
    }

    Profiler::printStackTrace();

    // Red color end

    WriteFile(out, "\x1B[0m", sizeof("\x1B[0m"), &_, nullptr);

    if constexpr (ATLAS_USE_WINDOWS_NT) {
        // Use the fastest, most brutal termination by calling NtTerminateProcess directly.
        NtTerminateProcess(nullptr, -1);
        NtTerminateProcess((void*)-1, -1);
    } else {
        ExitProcess(-1);
        while (true)
            ;
    }
}


#endif
