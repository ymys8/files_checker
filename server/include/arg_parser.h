#pragma once

#include <string>
#include <cstdint>

/// 
struct CmdArgs
{
    std::string configPath; ///<
    uint16_t port; ///<  
};

/// 
class ArgParser
{
public:
    /// 
    static CmdArgs parse(int argc, char *argv[]);
};
