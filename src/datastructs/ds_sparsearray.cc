#include <Commons/datastructs/SparseArray.hh>


namespace cm {


void SparseArrayBase::clear(void (*dtor)(void*))
{
    void** ptr = _data;

    auto visitor = [&](this auto const& visitor_, unsigned depth) -> void {
        if (depth == 0) {
            for (int k = 0; k < 256; k++) {
                if (ptr[k]) {
                    dtor(ptr);
                    __builtin_free(ptr);
                }
            }
        } else {
            for (int k = 0; k < 256; k++) {
                if (ptr[k] != nullptr) {
                    ptr = reinterpret_cast<void**>(ptr[k]);
                    visitor_(depth - 1);
                    delete[] ptr;
                }
            }
        }
    };
    visitor(8);
}


}  // namespace cm
