// #include <commons/core.hh>
// #include <commons/system/Profiler.hh>
// #include <commons/datastructs/string.hh>
// #include <commons/system/io/print.hh>


// namespace cm {

// void Assert_::operator()(bool c, char const* msg, unsigned len, SourceLocation src) const {
//     return this->operator()(c, String(msg, len), src);
// }

// void Assert_::failed(String const& msg, SourceLocation src) {
// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wdeprecated-declarations"

// auto& stream = System::getStandardErrStream();
// stream.directWriteBytes("ASSERTION FAILED: ", sizeof("ASSERTION FAILED: "))
//     .directWriteBytes(msg.cstr(), msg.sizeBytes())
//     .directWriteBytes("\nat ", sizeof("\nat "))
//     .directWriteBytes(src.function(), __builtin_strlen(src.function()))
//     .directWriteBytes(" in ", sizeof(" in "))
//     .directWriteBytes(src.file(), __builtin_strlen(src.file()));

// // cm::Profiler::debugLog();
// __builtin_abort();

// #pragma clang diagnostic pop
// }

// }  // namespace cm
