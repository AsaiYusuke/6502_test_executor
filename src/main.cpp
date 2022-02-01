#include <stdio.h>
#include <map>

#include "args.hxx"
#include "emulation_devices.h"
#include "test.h"

using namespace std;

int main(int argc, char *argv[])
{
    args::ArgumentParser parser("6502 test executor");
    args::HelpFlag help(parser, "HELP", "Show this help menu.", {'h', "help"});
    args::ValueFlag<string> program_file(parser, "PROGRAM", "The program file path.", {'p', "program"});
    args::ValueFlag<string> symbol_file(parser, "SYMBOL", "The symbol file path.", {'s', "symbol"});
    args::ValueFlag<string> test_file(parser, "TEST", "The test file path.", {'t', "test"});
    args::Flag quiet_ok(parser, "quiet OK", "The quiet OK flag", {"quiet-ok"});
    args::Flag quiet_fail(parser, "quiet FAIL", "The quiet FAIL flag", {"quiet-fail"});
    args::Flag quiet_summary(parser, "quiet summary", "The quiet summary flag", {"quiet-summary"});
    args::Flag quiet(parser, "quiet", "The quiet flag", {'q', "quiet"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Help)
    {
        cout << parser;
        return 0;
    }
    catch (const args::ParseError e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        return 1;
    }
    catch (const args::ValidationError e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        return 1;
    }

    const string program_filename = args::get(program_file);
    const string symbol_filename = args::get(symbol_file);
    const string test_filename = args::get(test_file);

    if (program_filename.empty() || symbol_filename.empty() || test_filename.empty())
    {
        cout << parser;
        return -1;
    }

    try
    {
        emulation_devices emulation = emulation_devices(program_filename, symbol_filename);
        test test_def = test(test_filename, &emulation, quiet_ok, quiet_fail, quiet_summary, quiet);
        bool result = test_def.execute();
        if (!result)
            return 1;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
