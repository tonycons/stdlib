#include <commons/system.hh>

namespace cm {


void OutStream::_print(char const* sFmt, std::initializer_list<ConstRefWrapper<AnyRef>> const& objects) UNSAFE({
    char const* fmtPtr;
    auto iter = objects.begin();

    Assert(sFmt, "sFmt null");

    for (fmtPtr = sFmt; *fmtPtr != '`' && *fmtPtr != '\0'; fmtPtr++) {
        writeBytes(fmtPtr, sizeof(char));
    }
    while (iter != objects.end()) {
        Assert(*fmtPtr == '`', "More arguments than specified in format string");
        String str;
        (*iter)->outputString([&](char c) { str.append(c); });
        iter++;

        writeBytes(str.cstr(), str.sizeBytes());

        fmtPtr++;
        for (; *fmtPtr != '`' && *fmtPtr != '\0'; fmtPtr++) {
            writeBytes(fmtPtr, sizeof(char));
        }
    }
})

}  // namespace cm
