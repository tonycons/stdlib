#include <commons/godbolt.hh>

using namespace cm;

///
/// Test functionality of Optional<T>
///
inline void testOptional()
{
    stdout->println("\nTESTING Optional<T>");
    // Test 1: dangling assignment warning
#ifdef TEST_THAT_WARNINGS_ARE_SHOWN
    {
        int j = 1;
        Optional<int&> k = j;
        {
            int z;
            k = z;  // Should warn here
        }
    }
#endif
    {
        Optional<int> k = 1;
        {
            Optional<int> j = 2;
            k = j;  // This should not warn
        }
    }
    struct TestOD
    {
        TestOD(int) { stdout->print(" constructor "); };
        ~TestOD() { stdout->print(" destructor "); }
        TestOD(TestOD const&) { stdout->print(" copy constructor "); }
        void operator=(TestOD const&) { stdout->print(" copy assignment "); }
    };
    usize t = 0;
    {
        Optional<StringRef> a = None;
        stdout->println("\t(`) Expect \"false\" : `", t++, a.hasValue());
        stdout->println("\t(`) Expect \"None\" : `", t++, a);
        stdout->println("\t(`) Expect \"Default\" : `", t++, a.valueOr("Default"));
        a = "Hello";
        stdout->println("\t(`) Expect \"true\" : `", t++, a.hasValue());
        stdout->println("\t(`) Expect \"Hello\" : `", t++, a);

        Optional<TestOD> b = None;
        stdout->print("\t(`) Expect \"constructor\" : ", t++);
        b = 1;
        stdout->print("\n\t(`) Expect \"destructor\" : ", t++);
    }
    stdout->println("\nFINISHED TESTING Optional<T>");
}
