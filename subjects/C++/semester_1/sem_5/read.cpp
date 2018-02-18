#include <iostream>
#include <cstdio>

using namespace std;

void read(FILE* f, uint64_t &val)
{
    cout << "Read uint64_t" << endl;
    fread(static_cast<void *>(&val), sizeof(val), 1, f);
}

void read(FILE* f, double &val)
{
    cout << "Read double" << endl;
    fread(static_cast<void *>(&val), sizeof(val), 1, f);
}

void read(FILE* f, int &val)
{
    cout << "Read int" << endl;
    fread(static_cast<void *>(&val), sizeof(val), 1, f);
}

void read(FILE* f, const char* (&val))
{
    cout << "Read const char*" << endl;
    fread(static_cast<void *>(&val), sizeof(val), 1, f);
}

void write(FILE* f, uint64_t val)
{
    cout << "Write uint64_t" << endl;
    fwrite(static_cast<void *>(&val), sizeof(val), 1, f);
}

void write(FILE* f, double val)
{
    cout << "Write double" << endl;
    fwrite(static_cast<void *>(&val), sizeof(val), 1, f);
}

void write(FILE* f, int val)
{
    cout << "Write int" << endl;
    fwrite(static_cast<void *>(&val), sizeof(val), 1, f);
}

void write(FILE* f, const char* val)
{
    cout << "Write const char*" << endl;
    fwrite(static_cast<void *>(&val), sizeof(val), 1, f);
}

int main()
{
    uint64_t v1 = 1ll << 60;
    double v2 = 1e300;
    int v3 = 321;
    const char* v4 = reinterpret_cast<const char *>(15);

    FILE* f = fopen("test.txt", "w");
    write(f, v1);
    write(f, v2);
    write(f, v3);
    write(f, v4);
    fclose(f);

    cout << v1 << " " << v2 << " " << v3 << " " << reinterpret_cast<size_t>(v4) << endl;

    v1 = 0; v2 = 0; v3 = 0; v4 = 0;
    f = fopen("test.txt", "r");
    read(f, v1);
    read(f, v2);
    read(f, v3);
    read(f, v4);
    fclose(f);

    cout << v1 << " " << v2 << " " << v3 << " " << reinterpret_cast<size_t>(v4) << endl;
    return 0;
}
