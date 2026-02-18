#include <commons/system.hh>
#include <commons/datastructs.hh>
#include <commons/startup.hh>
// #define TEST_THAT_WARNINGS_ARE_SHOWN 0
// #include "testoptional.cc"


using namespace cm;


void tryPrintFileContents(StringRef const& path)
{
    if (auto fs = FileInStream(path, FileOpenMode::Read); fs.ok()) {
        if (auto const rd = fs.readAllAsString(); rd.isErr()) {
            stdout.println("Error reading `: ` (`)", path, rd.errRef().codeName, rd.errRef().extMessage);
        } else {
            stdout.println(rd.okRef());
        }
    } else {
        stdout.println("Error opening `: ` (`)", path, fs.status().codeName, fs.status().extMessage);
    }
}

constexpr Union<int, float> a1 = 1;


int main()
{
    tryPrintFileContents(".ninja_log");
    tryPrintFileContents("setup");
    // auto s = FixedMap("hello", "!", "bob", "ugh", "apple", ":C", "apple", ":^\\");
    //
    // stdout.println(s["hello"]);
    // stdout.println(s["bob"]);
    // stdout.println(s["apple"]);
    //
    // Union<double, int> const h = true;
    // h.match([](int) { stdout.println("this is an int"); }, [](double) { stdout.println("this is a double"); });
    //
    // DelegateUnion<CFunction<void()>, double, int> q = 1.0;
    // q.store<int>(CFunction<void()>([] { stdout.println("int!"); }));
    // q.store<double>(CFunction<void()>([] { stdout.println("double!"); }));
    //
    // stdout.println(q.load().hasValue());
    // q = 1.0;
    // stdout.println(q.load().hasValue());
}


///
/// An example of using arrays and ranges
/// https://en.wikipedia.org/wiki/Levenshtein_distance#Iterative_with_full_matrix
///
// int levenshteinDistance(StringRef s1, StringRef s2)
// {
//     auto m = int(s1.length() + 1);
//     auto n = int(s2.length() + 1);

// auto f = [&](int i, int j, auto& a) {
//     return min(a(i - 1, j) + 1, a(i, j - 1) + 1, a(i - 1, j - 1) + (s1[i - 1] != s2[j - 1]));
// };

// return Array2D<int>(m, n)
//     .set(Range(0, m), 0, Functions::identity<0>)  // For each Nth row, set the value of the first column to N
//     .set(0, Range(0, n), Functions::identity<1>)  // For the first row, set the value of each Nth column to N
//     .set(Range(1, m), Range(1, n), f)             // Then apply a function to the values in [1..m][1..nx`]
//     (m - 1, n - 1);                               // final result
// }

/*
int main()
{
    stdout->println(
        "` ` ` `",  //
        levenshteinDistance("Hello", "hoLle"), levenshteinDistance("Hello", "heLlo"),
        levenshteinDistance("Hello", "Gello"), levenshteinDistance("kitten", "sitting"));

    // simple file access

    if (auto bob = FileOutStream("bob.txt"); bob->ok()) {
        bob->println("Hello from Bob!");
        stdout->println("bob exists. Check bob for hello!");
    } else {
        stdout->println("Can't open bob!");
    }

    // Comparators

    int k1[] = {1, 2};
    int k2[] = {2, 2};
    int k3[] = {1, 2, 3, 4, 5};

    ArrayRef<int> k = k1;

    stdout->println("should be 0: `", k.compareTimesafe(k1));
    stdout->println("should be -1: `", k.compareTimesafe(k2));
    stdout->println("should be -1: `", k.compareTimesafe(k3));
    stdout->println(k.mean());
    stdout->println(true);


    testOptional();


    FixedQueue<int, 15> queue = {1, 2, 3, 4, 5};
    // queue.outputString(queue, [](char) {});

    k.outputString(k, [](char) {});

    stdout->println(queue);

    // for (auto dir : Filter(ArrayRef{"a", "b", "c"}, StartsWith<'a'>)) {
    //     stdout->println(dir);
    // }



    return 0;
}
*/
