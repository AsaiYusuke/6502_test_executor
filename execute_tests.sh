#/bin/bash
if [ -z $1 ]; then
    echo "usage: $0 <executable.nes> <symbols table> <test directory>"
    exit 1
fi

RC=0
script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
for test_file in `ls $3/*.test`; do
    msg=$($script_dir/6502_tester $1 $2 $test_file 2>&1 >/dev/null)
    if [[ $? -eq 0 ]]; then
       echo -e "  \e[32m[PASS]\e[0m    $(basename $test_file .test)"
    else
       echo -e "  \e[31m[FAIL]\e[0m    $(basename $test_file .test)"
       while read -r line; do
           echo "                $line"
           done <<< "$msg"
       RC=1
    fi
done
exit $RC
