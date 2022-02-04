#include <stdio.h>
#include <map>

#include "args_parser.h"
#include "exception/parse_abort.h"
#include "emulation/emulation_devices.h"
#include "test.h"

using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        args_parser args{argc, argv};
        test test_def = test(&args);
        bool result = test_def.execute();
        if (!result)
            return 1;
    }
    catch (parse_abort &e)
    {
        return e.get_return_value();
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
