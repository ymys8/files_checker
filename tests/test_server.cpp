#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "arg_parser.h"
#include "config_reader.h"
#include "pattern_matcher.h"

namespace fs = std::filesystem;

///  Вспомогательный класс для создания временных файлов
class TmpFile
{
public:
    /// Конструктор
    explicit TmpFile(const std::string &content, const std::string &name = "tmp_test_config.json")
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

class PatternMatcherTest : public ::testing::Test {};

TEST_F(PatternMatcherTest, NoMatches)
{
    PatternMatcher matcher({"virus", "malware"});
    auto result = matcher.match("this is a clean file");

    EXPECT_TRUE(result.empty());
}

TEST_F(PatternMatcherTest, SinglePatternSingleMatch)
{
    PatternMatcher matcher({"virus"});
    auto result = matcher.match("this file has a virus inside");

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result["virus"], 1);
}

TEST_F(PatternMatcherTest, SinglePatternMultipleMatches)
{
    PatternMatcher matcher({"virus"});
    auto result = matcher.match("virus at start and virus at end virus");

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result["virus"], 3);
}

TEST_F(PatternMatcherTest, MultiplePatternsInText)
{
    PatternMatcher matcher({"virus", "malware", "trojan"});
    auto result = matcher.match("found virus and malware but no tr0jan");

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result["virus"], 1);
    EXPECT_EQ(result["malware"], 1);
}

TEST_F(PatternMatcherTest, EmptyText)
{
    PatternMatcher matcher({"virus"});
    auto result = matcher.match("");

    EXPECT_TRUE(result.empty());
}

TEST_F(PatternMatcherTest, NoPatterns)
{
    PatternMatcher matcher({});
    auto result = matcher.match("anything");

    EXPECT_TRUE(result.empty());
}

TEST_F(PatternMatcherTest, AdjacentMatches)
{
    PatternMatcher matcher({"ab"});
    auto result = matcher.match("ababab");

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result["ab"], 3);
}

class ConfigReaderTest : public ::testing::Test {};

TEST_F(ConfigReaderTest, ValidConfig)
{
    TmpFile tmp(R"({"patterns": ["virus", "malware", "trojan"]})");
    auto patterns = ConfigReader::read(tmp.path());

    ASSERT_EQ(patterns.size(), 3);
    EXPECT_EQ(patterns[0], "virus");
    EXPECT_EQ(patterns[1], "malware");
    EXPECT_EQ(patterns[2], "trojan");
}

TEST_F(ConfigReaderTest, SinglePattern)
{
    TmpFile tmp(R"({"patterns": ["single"]})");
    auto patterns = ConfigReader::read(tmp.path());

    ASSERT_EQ(patterns.size(), 1);
    EXPECT_EQ(patterns[0], "single");
}

TEST_F(ConfigReaderTest, EmptyPatternsList)
{
    TmpFile tmp(R"({"patterns": []})");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, MissingPatternsKey)
{
    TmpFile tmp(R"({"other_key": "value"})");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, PatternsNotArray)
{
    TmpFile tmp(R"({"patterns": "not_array"})");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, PatternNotString)
{
    TmpFile tmp(R"({"patterns": [123]})");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, EmptyPatternString)
{
    TmpFile tmp(R"({"patterns": [""]})");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, InvalidJson)
{
    TmpFile tmp("not a json at all {{{");

    EXPECT_THROW(ConfigReader::read(tmp.path()), std::runtime_error);
}

TEST_F(ConfigReaderTest, NonExistentFile)
{
    EXPECT_THROW(ConfigReader::read("/tmp/nonexistent_config_xyz.json"), std::runtime_error);
}

class ServerArgParserTest : public ::testing::Test
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

TEST_F(ServerArgParserTest, ValidArgs)
{
    TmpFile tmp(R"({"patterns": ["test"]})", "tmp_srv_config.json");
    std::vector<std::string> args = {"server", "-c", tmp.path(), "-p", "9090"};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.configPath, tmp.path());
    EXPECT_EQ(result.port, 9090);
}

TEST_F(ServerArgParserTest, ValidArgsReversedOrder)
{
    TmpFile tmp(R"({"patterns": ["test"]})", "tmp_srv_config2.json");
    std::vector<std::string> args = {"server", "-p", "4040", "-c", tmp.path()};
    auto argv = makeArgv(args);

    CmdArgs result = ArgParser::parse(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(result.configPath, tmp.path());
    EXPECT_EQ(result.port, 4040);
}

TEST_F(ServerArgParserTest, MissingConfigPath)
{
    std::vector<std::string> args = {"server", "-p", "8080"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ServerArgParserTest, MissingPort)
{
    TmpFile tmp(R"({})", "tmp_srv_config3.json");
    std::vector<std::string> args = {"server", "-c", tmp.path()};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ServerArgParserTest, InvalidPort)
{
    TmpFile tmp(R"({})", "tmp_srv_config4.json");
    std::vector<std::string> args = {"server", "-c", tmp.path(), "-p", "xyz"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ServerArgParserTest, PortOutOfRange)
{
    TmpFile tmp(R"({})", "tmp_srv_config5.json");
    std::vector<std::string> args = {"server", "-c", tmp.path(), "-p", "99999"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}

TEST_F(ServerArgParserTest, NonExistentConfig)
{
    std::vector<std::string> args = {"server", "-c", "/tmp/no_such_config.json", "-p", "8080"};
    auto argv = makeArgv(args);

    EXPECT_THROW(ArgParser::parse(static_cast<int>(args.size()), argv.data()), std::invalid_argument);
}
