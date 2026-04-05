#include <cstddef>

const int MAX_PATTERNS = 64;
const int MAX_PATTERN_LEN = 128;

const char* FIFO_PATH = "/tmp/hometask.fifo";

///
struct PatternStatistic
{
    char name[MAX_PATTERN_LEN]; ///<
    size_t matchCount;          ///<
};

///
struct SharedStatistic
{
    size_t checkedFilesCount;                ///<
    size_t patternCount;                     ///<
    PatternStatistic patterns[MAX_PATTERNS]; ///<
};