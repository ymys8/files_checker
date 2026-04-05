#include "arg_parser.h"
#include "file_reader.h"
#include "client.h"

#include <iostream>

namespace
{
    const std::string SERVER_HOST = "127.0.0.1";
}

int main(int argc, char *argv[])
{
    try
    {
        CmdArgs args = ArgParser::parse(argc, argv);

        std::string content = FileReader::read(args.filePath);

        TcpClient client;
        client.connectServer(SERVER_HOST, args.port);
        client.sendText(content);

        std::string response = client.receive();
        std::cout << response;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}