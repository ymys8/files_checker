#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <cstdio>

#include "arg_parser.h"
#include "file_reader.h"

namespace fs = std::filesystem;

///  Вспомогательный класс для создания временных файлов
class TmpFile
{
public:
    /// Конструктор
    explicit TmpFile(const std::string &content = "", const std::string &name = "tmp_test_file.txt")
        : filepath(fs::temp_directory_path() / name)
    {
        std::ofstream f(filepath);
        f << content;
        f.close();
    }

    /// Деструктор
    ~TmpFile()
    {
        fs::remove(filepath);
    }

    /// Получить путь до файла
    std::string path() const { return filepath.string(); }

private:
    fs::path filepath;
};

class FileReaderTest : public testing::Test {};

TEST_F(FileReaderTest, ReadExistingFile)
{
    TmpFile tmp("hello world");
    std::string content = FileReader::read(tmp.path());
    EXPECT_EQ(content, "hello world");
}

TEST_F(FileReaderTest, ReadEmptyFile)
{
    TmpFile tmp("");
    std::string content = FileReader::read(tmp.path());
    EXPECT_TRUE(content.empty());
}

TEST_F(FileReaderTest, ReadMultilineFile)
{
    std::string content = "line1\nline2\nline3";
    TmpFile tmp(content);
    std::string fileContent = FileReader::read(tmp.path());
    EXPECT_EQ(fileContent, content);
}

TEST_F(FileReaderTest, ThrowOnNonExistentFile)
{
    EXPECT_THROW(FileReader::read("/tmp/nonexistent_test_file_xyz.txt"), std::runtime_error);
}

class ClientArgParserTest : public testing::Test
{
protected:
    void SetUp() override
    {
        optind = 1;
    }
};

static std::vector<char *> makeArgv(std::vector<std::string> &args)
{
    std::vector<char *> argv;
    for (auto &a : args)
    {
        argv.push_back(a.data());
    }
    argv.push_back(nullptr);
    return argv;
}

TEST_F(ClientArgParserTest, ValidArgs)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "8080"};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.filePath, tmp.path());
    EXPECT_EQ(result.port, 8080);
}

TEST_F(ClientArgParserTest, ValidArgsReversedOrder)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-p", "9090", "-f", tmp.path()};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.filePath, tmp.path());
    EXPECT_EQ(result.port, 9090);
}

TEST_F(ClientArgParserTest, MissingFilePath)
{
    std::vector<std::string> args = {"client", "-p", "8080"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, MissingPort)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path()};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, InvalidPortNotANumber)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "abc"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, PortZero)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "0"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, PortTooLarge)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "70000"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, NonExistentFile)
{
    std::vector<std::string> args = {"client", "-f", "/tmp/nonexistent_xyz_test.txt", "-p", "8080"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ClientArgParserTest, MinPort)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "1"};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.port, 1);
}

TEST_F(ClientArgParserTest, MaxPort)
{
    TmpFile tmp("data");
    std::vector<std::string> args = {"client", "-f", tmp.path(), "-p", "65535"};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.port, 65535);
}
