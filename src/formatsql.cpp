#include "Parser.h"         // C_SQLParser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_SQLScanner
#include "SqlObjs.h"        // I_SqlCmd
//------------------------------------------------
#include "bux/MemIn.h"      // bux::C_IMemStream
#include "bux/StrUtil.h"    // HRTN()
#include <fstream>          // std::ifstream
#include <iostream>         // std::cout

enum
{
    MAIN_SUCCESS            = 0,
    MAIN_ARG_ERROR,
    MAIN_OPEN_FILE_ERROR,
    MAIN_NOT_ACCEPTED,
    MAIN_NULL_RESULT,
    MAIN_CAUGHT
};

void printSqlCommands(const std::vector<std::string> &sqls)
{
    int ind = 0;
    for (auto &i: sqls)
        std::cout <<(++ind) <<": " <<i <<";\n";
}

int parseSQL(const char *filename, std::istream &in)
{
    C_SQLParser                         parser;
    bux::C_ScreenerNo<TID_LEX_Spaces>   screener(parser);
    C_SQLScanner                        scanner(screener);
    bux::scanFile(filename, in, scanner);

    size_t ind{};
    parser.eachAccepted([&ind](auto &p){
        if (auto sql = dynamic_cast<const I_SqlCmd*>(p.get()))
        {
            ++ind;
            std::cout <<"MySQL["<<ind<<"]:\n";
            printSqlCommands(sql->to(SST_MYSQL, {}));
            //------------------------------------------------
            std::cout <<"\n"
                        "SQLite["<<ind<<"]:\n";
            printSqlCommands(sql->to(SST_SQLITE, {}));
        }
        else
            std::cout <<"Null SQL["<<ind<<"]\n";
    });
    if (!ind)
    {
        std::cout <<"Not accepted on EOF\n";
        return MAIN_NOT_ACCEPTED;
    }
    return MAIN_SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        // Help
    {
        std::cout <<
            "\n"
            "parsesql <SqlCmd>\n"
            "parsesql @ <FileContainingSqlCmd>\n"
            "\n"
            "If <SqlCmd> starts with a '@', the rest chars denote a file name from which the input SQL will be read.\n";
        return MAIN_ARG_ERROR;
    }

    try
    {
        if (argv[1][0] == '@')
            // Read from file
        {
            auto file = argv[1][1]? argv[1]+1: argv[2];
            std::ifstream  in(file);
            if (!in)
            {
                std::cout <<"Fail to read \"" <<file <<"\"\n";
                return MAIN_OPEN_FILE_ERROR;
            }
            return parseSQL(file, in);
        }
        else
        {
            bux::C_IMemStream  in(argv[1]);
            return parseSQL("ARG1", in);
        }
    }
    catch (const std::exception &t)
    {
        std::cout <<HRTN(t) <<": " <<t.what() <<'\n';
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        std::cout <<"Unknown exception\n";
        return MAIN_CAUGHT;
    }
}
