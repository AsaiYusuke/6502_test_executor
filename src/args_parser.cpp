#include "args_parser.h"
#include "exception/parse_abort.h"

args_parser::args_parser(int argc, char *argv[])
{
    args::ArgumentParser parser("6502 test executor");
    args::HelpFlag help(parser, "HELP", "Show this help menu.", {'h', "help"});
    program_file_path = new args::ValueFlag<string>(parser, "PROGRAM", "Specify the path of the program image file to be tested.", {'p', "program"});
    debug_file_path = new args::ValueFlag<string>(parser, "DEBUG", "Specify the path of the debug information file used for testing.", {'d', "debug"});
    test_file_path = new args::ValueFlag<string>(parser, "TEST", "(REQUIRED)\nSpecify the path of the test scinario file.", {'t', "test"}, args::Options::Required);

    test_timeout = new args::ValueFlag<int>(parser, "TIMEOUT", "Specify the timeout period before the test becomes an error.", {"timeout"}, 10000);

    quiet = new args::Flag(parser, "quiet", "Do not show any output.", {'q', "quiet"});
    quiet_ok = new args::Flag(parser, "quiet OK", "Do not show the successful output.", {"quiet-ok"});
    quiet_fail = new args::Flag(parser, "quiet FAIL", "Do not show the failed output.", {"quiet-fail"});
    quiet_summary = new args::Flag(parser, "quiet summary", "Do not show the test summary output.", {"quiet-summary"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Help)
    {
        cout << parser;
        throw parse_abort(0);
    }
    catch (const args::ParseError e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        throw parse_abort(1);
    }
    catch (const args::ValidationError e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        throw parse_abort(1);
    }
}

string args_parser::get_program_path()
{
    return args::get(*program_file_path);
}

string args_parser::get_debug_path()
{
    return args::get(*debug_file_path);
}

string args_parser::get_test_path()
{
    return args::get(*test_file_path);
}

int args_parser::get_test_timeout()
{
    return args::get(*test_timeout);
}

bool args_parser::is_quiet()
{
    return *quiet;
}

bool args_parser::is_quiet_success()
{
    return *quiet_ok;
}

bool args_parser::is_quiet_failed()
{
    return *quiet_fail;
}

bool args_parser::is_quiet_summary()
{
    return *quiet_summary;
}
