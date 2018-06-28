#include <fstream>
#include <cstring>
#include <string>
using namespace std;
void FindBorder(const char str[], size_t &idx1, size_t &idx2)
{
    while (idx1 < idx2 && isspace(str[idx1++])) continue;
    while (idx1 < idx2 && isspace(str[--idx2])) continue;
}
int CompareFile(const char path1[], const char path2[])
{
    ifstream fin1(path1), fin2(path2);
    string str1, str2;
    while (getline(fin1, str1) && getline(fin2, str2))
    {
        size_t idx11 = 0, idx12 = str1.length(), idx21 = 0, idx22 = str2.length();
        FindBorder(str1.c_str(), idx11, idx12);
        FindBorder(str2.c_str(), idx21, idx22);
        if ((idx12 - idx11 != idx22 - idx21) || (memcmp(str1.c_str() + idx11, str2.c_str() + idx21, idx12 - idx11) != 0)) return -1;
    }
    ifstream &rest = fin1.eof() ? fin2 : fin1;
    if (rest.eof()) return 0;
    for (int ch = rest.get(); !rest.eof(); rest.get())
        if (!isspace(ch)) return -1;
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc < 3) return fprintf(stderr, "Usage: %s file1 file2\n", argv[0]), -1;
    return CompareFile(argv[1], argv[2]) == 0 ? puts("Accepted"), 0 : puts("Wrong Answer"), -1;
}
