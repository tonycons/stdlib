// #include <Commons/gc/GC.hh>


// namespace cm {


// bool GCAllocation::invalid() const
// {
//     return ptr != nullptr;
// }

// void* GCAllocation::data()
// {
//     return ptr;
// }

// void const* GCAllocation::data() const
// {
//     return ptr;
// }

// size_t GCAllocation::size() const
// {
//     if (GCObject* item = GC.getObject(this->ptr); item != nullptr)
//         return item->size;

// return 0;
// }

// GCObjectFlags GCAllocation::getFlags() const
// {
//     if (GCObject* item = GC.getObject(this->ptr); item != nullptr)
//         return item->flags;

// return 0;
// }

// void GCAllocation::setFlags(GCObjectFlags flags)
// {
//     if (GCObject* item = GC.getObject(this->ptr); item != nullptr)
//         item->flags = flags;
//     else
//         ;
// }

// decltype(GCObject::dtor) GCAllocation::getDestructor() const
// {
//     if (GCObject* item = GC.getObject(this->ptr); item != nullptr)
//         return item->dtor;

// return nullptr;
// }

// void GCAllocation::setDestructor(void (*dtor)(void*))
// {
//     if (GCObject* item = GC.getObject(this->ptr); item != nullptr)
//         item->dtor = dtor;
//     else
//         ;
// }

// }  // namespace cm
