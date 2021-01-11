#ifndef ParseSQLH
#define ParseSQLH

#include "SqlObjs.h"    // I_SqlCmd, F_Str2Str, C_ClauseAsCmd

//
//      Types
//
class C_SqlTree
{
public:

    // Nonvirtuals
    C_SqlTree(const std::string &sql);
    C_SqlTree(const C_SqlTree&) = delete;
    C_SqlTree &operator=(const C_SqlTree&) = delete;
    void operator=(const std::string &sql);
    //-------------------------------------------------
    auto &at(size_t i)  const { return m_cmds.at(i); }
    auto begin()        const { return m_cmds.begin(); }
    auto end()          const { return m_cmds.end(); }
    bool empty()        const { return m_cmds.empty(); }
    auto size()         const { return m_cmds.size(); }

private:

    // Data
    std::vector<C_SqlCmdPtr>    m_cmds;
};

//
//      Implement member templates
//
template<class T>
T *sql_cast(const C_SqlCmdPtr &ptr)
{
    if (T *t = dynamic_cast<T*>(ptr.get()))
        return t;
    if (auto cmd = dynamic_cast<const C_ClauseAsCmd*>(ptr.get()))
    {
        if (T *t = *cmd)
            return t;
    }
    return {};
}

#endif // ParseSQLH
