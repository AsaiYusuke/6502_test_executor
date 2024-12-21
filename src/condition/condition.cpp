#include "condition/condition.hpp"

#include "condition/condition_register_a_x_y.hpp"
#include "condition/condition_register_status_flag.hpp"
#include "condition/condition_register_pc.hpp"
#include "condition/condition_memory.hpp"
#include "condition/condition_stack.hpp"
#include "condition/condition_interrupt.hpp"
#include "condition/condition_mocked_proc.hpp"
#include "condition/condition_timeout.hpp"
#include "condition/condition_mocked_value.hpp"
#include "util/value_convert.hpp"

using namespace std;

condition::condition(emulation_devices *_device, json condition, json target)
    : condition::condition(_device, condition)
{
    register_pc_def = new condition_register_pc(_device, target);
}

condition::condition(emulation_devices *_device, json condition_json)
{
    device = _device;
    if (condition_json["register"].is_object())
        for (auto &register_element : condition_json["register"].items())
            switch (register_name_type_map[register_element.key()])
            {
            case register_type::P:
                for (auto &status_element : register_element.value().items())
                    status_flag_defs.push_back(
                        new condition_register_status_flag(
                            device,
                            status_element.key(),
                            status_element.value()));
                break;
            default:
                register_defs.push_back(
                    new condition_register_a_x_y(
                        device,
                        register_element.key(),
                        register_element.value()));
                break;
            }

    if (condition_json["memory"].is_array())
        for (auto &memory_def : condition_json["memory"])
            memory_defs.push_back(
                new condition_memory(
                    device,
                    memory_def));

    stack_def = new condition_stack(device, condition_json["stack"]);

    if (condition_json["interrupt"].is_array())
        for (auto &interrupt_def : condition_json["interrupt"])
            interrupt_defs.push_back(
                new condition_interrupt(
                    device,
                    interrupt_def));

    if (condition_json["mockedProc"].is_array())
        for (auto &mocked_proc_def : condition_json["mockedProc"])
            mocked_proc_defs.push_back(
                new condition_mocked_proc(
                    device,
                    mocked_proc_def));

    timeout_def = new condition_timeout(device, condition_json["timeout"]);
}

emulation_devices *condition::get_device() const
{
    return device;
}

vector<condition_register_a_x_y *> condition::get_register_defs() const
{
    return register_defs;
}

vector<condition_register_status_flag *> condition::get_status_flag_defs() const
{
    return status_flag_defs;
}

vector<condition_memory *> condition::get_memory_defs() const
{
    return memory_defs;
}

condition_register_pc *condition::get_register_pc_def() const
{
    return register_pc_def;
}

condition_stack *condition::get_stack_def() const
{
    return stack_def;
}

vector<condition_interrupt *> condition::get_interrupt_defs() const
{
    return interrupt_defs;
}

vector<condition_mocked_proc *> condition::get_mocked_proc_defs() const
{
    return mocked_proc_defs;
}

condition_timeout *condition::get_timeout_def() const
{
    return timeout_def;
}
