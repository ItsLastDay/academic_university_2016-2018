#include <vector>

#include "my_ostream_con.h"
#include "my_ostream_file.h"
#include "my_ostream.h"
#include "my_ostream_combo.h"

void foo(my_ostream& obj)
{
    obj << "foo called\n";
}

int main()
{
    my_ostream* console_out = new my_ostream_con;
    my_ostream* file_out = new my_ostream_file("output.txt");
    std::vector<my_ostream*> streams = {console_out, file_out};

    (*console_out) << "Hello, world! " << 12 << " " << 34.5 << "\n";
    foo(*console_out);

    for (auto stream_ptr: streams)
    {
        (*stream_ptr) << "This is " << 42 << " a test " << 123.45 
            << " 56er" << 234 << 1;
    }

    my_ostream_combo combo_out(streams);
    combo_out << "Ccccombo output" << 324 << 45.6 << "sdf\n";

    foo(combo_out);
    
    delete console_out;
    delete file_out;

    return 0;
}
