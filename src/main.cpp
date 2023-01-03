#include <stdio.h>
#include <map>
#include <sysexits.h>

#include "args_parser.hpp"
#include "emulation/emulation_devices.hpp"
#include "exception/parse_abort.hpp"
#include "test/test.hpp"
#include "util/constant.hpp"

using namespace std;

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
