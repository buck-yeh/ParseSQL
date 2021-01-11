#include "ParseSQL.h"
#include "Parser.h"         // C_SQLParser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_SQLScanner
//----------------------------------------------------------
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream

C_SqlTree::C_SqlTree(const std::string &sql)
{
    operator=(sql);
}

void C_SqlTree::operator=(const std::string &sql)
{
    C_SQLParser                         parser;
    bux::C_ScreenerNo<TID_LEX_Spaces>   preparser(parser);
    C_SQLScanner                        scanner(preparser);
    bux::C_IMemStream                   in(sql);
    try
    {
        bux::scanFile("", in, scanner);
    }
    catch (const std::exception &e)
    {
        RUNTIME_ERROR("{}: {}\n\"{}\"", HRTN(e), e.what(), sql);
    }
    parser.eachAccepted([this,&sql](auto &lex) {
        if (auto p = std::dynamic_pointer_cast<const I_SqlCmd>(lex))
            m_cmds.emplace_back(p);
        else if (lex)
            RUNTIME_ERROR("Unexpected type {}\n\"{}\"", HRTN(*lex), sql);
        else
            RUNTIME_ERROR("Null result");
    });
}
