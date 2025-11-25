#include <commons/core.hh>
#include <commons/system.hh>
#include "commons/datastructs/Range.hh"


using namespace cm;


///
/// An example of using arrays and ranges
/// https://en.wikipedia.org/wiki/Levenshtein_distance#Iterative_with_full_matrix
///
int levenshteinDistance(StringValue s1, StringValue s2)
{
    auto m = int(s1.length() + 1);
    auto n = int(s2.length() + 1);

    auto f = [&](int i, int j, auto& a) {
        return min(a(i - 1, j) + 1, a(i, j - 1) + 1, a(i - 1, j - 1) + (s1[i - 1] != s2[j - 1]));
    };

    return Array2D<int>(m, n)
        .set(Range(0, m), 0, Functions::identity<0>)  // For each Nth row, set the value of the first column to N
        .set(0, Range(0, n), Functions::identity<1>)  // For the first row, set the value of each Nth column to N
        .set(Range(1, m), Range(1, n), f)             // Then apply a function to the values in [1..m][1..nx`]
        (m - 1, n - 1);                               // final result
}


int main()
{
    // Example 1
    auto ld = levenshteinDistance;
    stdout->println("`", {ld("Hello", "hoLle"), ld("Hello", "heLlo"), ld("Hello", "Gello"), ld("kitten", "sitting")});


    // Example 2: simple file access

    if (auto bob = FileOutStream("bob.txt"); bob->ok()) {
        bob->println("Hello from Bob!");
        stdout->println("bob exists. Check bob for hello!");
    } else {
        stdout->println("Can't open bob!");
    }

    return 0;
}
