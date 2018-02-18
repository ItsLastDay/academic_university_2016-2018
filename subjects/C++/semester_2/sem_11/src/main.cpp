#include <cstdio>
#include <cassert>
#include <vector>
#include <utility>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "myassert.h"
#include "mylog.h"
#include "instructions.h"


const char* instruction_name(InstructionCode code)
{
#define POPULATE_NAME(n, d, s) names[INST_##n] = #n,
    static const char* names[INST_LAST] = {
        FOR_INSTRUCTIONS(POPULATE_NAME)
    };
#undef POPULATE_NAME
    return names[code];
}

const char* instruction_description(InstructionCode code)
{
#define POPULATE_DESC(n, d, s) descriptions[INST_##n] = #d,
    static const char* descriptions[INST_LAST] = {
        FOR_INSTRUCTIONS(POPULATE_DESC)
    };
#undef POPULATE_DESC
    return descriptions[code];
}

size_t instruction_arg_count(InstructionCode code)
{
#define POPULATE_CNT(n, d, s) arg_counts[INST_##n] = s,
    static size_t arg_counts[INST_LAST] = {
        FOR_INSTRUCTIONS(POPULATE_CNT)
    };
#undef POPULATE_CNT
    return arg_counts[code];
}

std::vector<unsigned char> gen_rnd_instrs(const size_t instr_cnt)
{
    std::vector<unsigned char> instrs;

    for (size_t i = 0; i < instr_cnt; i++)
    {
        InstructionCode code = static_cast<InstructionCode>(rand() % INST_LAST);
        size_t arg_count = instruction_arg_count(code);
        instrs.push_back(code);
        for (size_t j = 1; j < arg_count; j++)
        {
            instrs.push_back(rand() % (std::numeric_limits<unsigned char>::max() + 1));
        }
    }

    return instrs;
}

void log_instrs(const std::vector<unsigned char> &instrs)
{
    size_t instr_idx = 0;
    size_t pos = 0;
    while (pos < instrs.size())
    {
        InstructionCode code = static_cast<InstructionCode>(instrs[pos]);
        ++pos;
        size_t num_args = instruction_arg_count(code);
        uint64_t args_encoded = 0;
        for (size_t i = 0; i + 1 < num_args; i++)
        {
            args_encoded |= (1ull << (i * 8)) * instrs[pos];
            ++pos;
        }
        LOG(INFO, "%s(%lu) 0x%016lx", instruction_name(code), instr_idx, args_encoded);
        ++instr_idx;
    }
}

int main()
{
    std::srand(std::time(0));

    // Part 1
    bool assertion1_evaluated = false;
    myassert(assertion1_evaluated = true);
    assert((CONFIG_DEBUG && assertion1_evaluated)
            || (!CONFIG_DEBUG && !assertion1_evaluated));

    // Part 2
    LOG(INFO, "==============================");
    LOG(DEBUG, "%d %d %d %s", 1, 2, 3, "debug");
    LOG(INFO, "%d %d %s", 1, 2, "info");
    LOG(WARN, "%d %s", 1, "warning");
    LOG(ERROR, "%s", "error!");

    // Part 3
    LOG(INFO, "==============================");
    LOG(INFO, "%s %s", instruction_name(INST_DADD),
            instruction_description(INST_DADD));
    LOG(INFO, "%s %s", instruction_name(INST_IPRINT),
            instruction_description(INST_IPRINT));

    // Part 4
    auto inst_stream = gen_rnd_instrs(20);
    log_instrs(inst_stream);

    // Part 5
    LOG(INFO, "==============================");
    std::vector<unsigned char> super_code = {
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_ILOAD, 0, 0, 0xdd, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_IPRINT
    }; // 0xff + 0xdd0000 + 0xff = 0xdd01fe
    log_instrs(super_code);
    /*
    exec_instrs(super_code);
    */
    return 0;
}
