#include <iostream>
#include "arg_parser.h"
#include "config_reader.h"
#include "server.h"

int main(int argc, char *argv[])
{
    try
    {
        CmdArgs args = ArgParser::parse(argc, argv);
        std::vector<std::string> patterns = ConfigReader::read(args.configPath);
        TcpServer server{args.port, patterns};
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    }

    return 0;
}