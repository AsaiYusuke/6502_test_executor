#include <stdio.h>
#include <map>
#include <sysexits.h>

#include "args_parser.h"
#include "exception/parse_abort.h"
#include "emulation/emulation_devices.h"
#include "test.h"

using namespace std;

#define EX_TEST_FAILED 200

int main(int argc, char *argv[])
{
    try
    {
        bool result =
            test(new args_parser(argc, argv))
                .execute();

        return result ? EX_OK : EX_TEST_FAILED;
    }
    catch (parse_abort &e)
    {
        return e.get_return_value();
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        return EX_SOFTWARE;
    }
}
