#pragma once

#include "args.hxx"

#include "enum/test_result_type.hpp"

using namespace std;

class args_parser
{
private:
    args::ValueFlag<string> *debug_file_path;
    args::ValueFlag<string> *test_file_path;

    args::ValueFlag<string> *coverage_file_path;
    args::ValueFlag<string> *coverage_segment_names;

    args::ValueFlag<string> *test_id;

    args::ValueFlag<uint64_t> *test_timeout;

    args::Flag *quiet;
    args::Flag *quiet_ok;
    args::Flag *quiet_fail;
    args::Flag *quiet_summary;

public:
    args_parser(int argc, char *argv[]);
    string get_debug_path();
    string get_test_path();
    string get_coverage_path();
    string get_coverage_segment_names();
    string get_test_id();
    uint64_t get_test_timeout();
    bool is_quiet();
    bool is_quiet_type(test_result_type type);
    bool is_quiet_summary();
};
