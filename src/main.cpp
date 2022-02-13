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
        bool result =
            test(new args_parser(argc, argv))
                .execute();

        return result ? 0 : 1;
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
}
