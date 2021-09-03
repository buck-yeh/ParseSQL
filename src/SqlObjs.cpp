#include "SqlObjs.h"
//----------------------------------------------------------
#include "bux/LogStream.h"  // HRTN()
#include "bux/XException.h" // LOGIC_ERROR(), RUNTIME_ERROR()
#include <cstring>          // strcmp()
#ifdef _WIN32
//    #include <string.h>     // _stricmp()
//    #define strcasecmp(a,b) _stricmp(a,b)
#else
    #include <strings.h>    // strcasecmp()
#endif
#include <algorithm>        // std::sort()

namespace {

//
//      In-Module Types
//
typedef std::vector<C_ColumnBasePtr> C_ColumnList;
typedef std::vector<C_ConstraintPtr> C_ConstraintSet;

//
//      In-Module Functions
//
auto dbgStr(const C_StrQ &sqls)
{
    std::string ret;
    int line{};
    for (auto &i: sqls)
        ret += fmt::format("\n{}: {}", ++line, i);

    return ret;
}

auto actionName(E_EventAction id)
{
    const char *ret = nullptr;
    switch (id)
    {
    case EA_RESTRICT:   ret = "RESTRICT";   break;
    case EA_CASCADE:    ret = "CASCADE";    break;
    case EA_SET_NULL:   ret = "SET NULL";   break;
    case EA_NO_ACTION:  ret = "NO ACTION";  break;
    case EA_NONE:       break;
    }
    return ret;
}

auto toStr(const C_IndexColumns &cols, E_SqlServerType type)
{
    std::string sql;
    bool first = true;
    for (auto &i: cols)
    {
        sql += first? "(": ", ";
        sql += i.m_name;
        if (i.m_length >= 0 && type == SST_MYSQL)
            sql += '(' + std::to_string(i.m_length) + ')';
        if (i.m_ad)
            sql.append(1,' ').append(i.m_ad);

        first = false;
    }
    sql += ')';
    return sql;
}

std::string dataType2str(const C_LexPtr &dt, E_SqlServerType type, F_Str2Str t2s)
{
    if (auto phrase = dynamic_cast<const I_SqlClause*>(dt.get()))
        return phrase->to(type, t2s);

    return typeName(bux::unlex<E_DataType>(dt));
}

bool isTimestampName(const std::string &name)
{
    return !strcasecmp(name.c_str(), "CURRENT_TIMESTAMP") ||
           !strcasecmp(name.c_str(), "NOW") ||
           !strcasecmp(name.c_str(), "LOCALTIME") ||
           !strcasecmp(name.c_str(), "LOCALTIMESTAMP");
}

std::string expr2str(const C_LexPtr &expr, E_SqlServerType type, F_Str2Str t2s)
{
    if (auto phrase = dynamic_cast<const I_SqlClause*>(expr.get()))
        return phrase->to(type,t2s);
    else if (auto iLex = dynamic_cast<const bux::C_IntegerLex*>(expr.get()))
        return iLex->str();
    else if (auto s = bux::tryUnlex<std::string>(expr))
        return '\'' + *s + '\'';
    else if (auto t = bux::tryUnlex<E_DataType>(expr))
        return typeName(*t);
    else if (auto exprs = bux::tryUnlex<C_LexList>(expr))
    {
        std::string ret;
        for (auto &i: *exprs)
        {
            ret += ret.empty()? "(": ", ";
            ret += expr2str(i, type, t2s);
        }
        return ret += ')';
    }

    // Neither a phrase(id, null, function, ...) nor an integer nor a string
    LOGIC_ERROR("{} not expression type", HRTN(*expr));
}

template<class T>
void assignLex(std::shared_ptr<const T> &lval, C_LexPtr &rval, const char *errorSuffix)
{
    lval = std::dynamic_pointer_cast<const T>(rval);
    if (!lval)
        LOGIC_ERROR("{}{}", HRTN(*rval), errorSuffix);
}

bool hasAutoInc(const C_ColumnBaseDef &col)
{
    if (auto def = dynamic_cast<const C_ColumnDef*>(&col))
        return def->hasAutoInc();

    return false;
}

} // namespace

//
//      Functions
//
std::string bareID(const std::string &id)
{
    switch (id.front())
    {
    case '\'':
    case '`':
        // Assume id.front() == id.back() for performance
        return id.substr(1, id.size()-2);
    default:
        return id;
    }
}

#define _glueDT_(x) DT_##x
#define DT_CASE(x) case _glueDT_(x): ret = #x; break;
const char *typeName(E_DataType type)
{
    const char *ret;
    switch (type)
    {
    DT_CASE(BOOLEAN)
    DT_CASE(BIT)
    DT_CASE(TINYBLOB)
    DT_CASE(MEDIUMBLOB)
    DT_CASE(LONGBLOB)
    DT_CASE(BLOB)
    DT_CASE(BINARY)
    DT_CASE(VARBINARY)
    DT_CASE(TINYINT)
    DT_CASE(SMALLINT)
    DT_CASE(MEDIUMINT)
    DT_CASE(BIGINT)
    DT_CASE(INT)
    DT_CASE(INTEGER)
    DT_CASE(DECIMAL)
    DT_CASE(FLOAT)
    DT_CASE(DOUBLE)
    case DT_DOUBLE_PRECISION:   ret = "DOUBLE PRECISION"; break;
    DT_CASE(REAL)
    DT_CASE(TINYTEXT)
    DT_CASE(MEDIUMTEXT)
    DT_CASE(LONGTEXT)
    DT_CASE(CHAR)
    DT_CASE(NCHAR)
    DT_CASE(TEXT)
    DT_CASE(VARCHAR)
    DT_CASE(VARCHAR2)
    DT_CASE(NVARCHAR)
    DT_CASE(DATE)
    DT_CASE(TIME)
    DT_CASE(DATETIME)
    DT_CASE(TIMESTAMP)
    DT_CASE(YEAR)
    DT_CASE(POINT)
    DT_CASE(LINESTRING)
    DT_CASE(POLYGON)
    DT_CASE(MULTIPOINT)
    DT_CASE(MULTILINESTRING)
    DT_CASE(MULTIPOLYGON)
    DT_CASE(GEOMETRYCOLLECTION)
    DT_CASE(GEOMETRY)
    default:
        LOGIC_ERROR("Unknown E_DataType {}", int(type));
    }
    return ret;
}

bool isTimestamp(const bux::I_LexAttr *lex)
{
    if (auto id = dynamic_cast<const C_Id*>(lex))
    {
        if (id->list().size() == 1 &&
            isTimestampName(id->list().front()))
            return true;
    }
    else if (auto fn = dynamic_cast<const C_FunctionCall*>(lex))
    {
        if (isTimestampName(fn->name()))
            return true;
    }
    return false;
}

void partitionCreateDefs(const C_SqlClauseQ &srcDefs, C_ColumnList &cols, C_ConstraintSet &constraints)
{
    for (auto &i: srcDefs)
    {
        if (auto a = std::dynamic_pointer_cast<const C_ColumnBaseDef>(i))
            cols.emplace_back(a);
        else if (auto b = std::dynamic_pointer_cast<const C_ConstraintDef>(i))
            constraints.emplace_back(b);
        /*
        else
            Any other cases (currently only C_CheckDef) are silently ignored
        */
    }
}

//
//      Implement Classes
//
C_TableName::C_TableName(C_LexPtr &tbl_name)
{
    auto &src = dynamic_cast<const C_Id&>(*tbl_name).list();
    switch (auto n = src.size())
    {
    case 1:
        m_tblName = src.front();
        break;
    case 2:
        m_dbName = src.at(0);
        m_tblName = src.at(1);
        break;
    default:
        LOGIC_ERROR("Table name consisting of {} identifiers", n);
    }
}

std::string C_TableName::dbPrefix() const
{
    if (!m_dbName.empty())
        return m_dbName + '.';

    return {};
}

std::string C_TableName::bareName() const
{
    return bareID(m_tblName);
}

std::string C_TableName::str(F_Str2Str t2s) const
{
    const auto pfx = dbPrefix();
    if (pfx.empty() && t2s)
        switch (const auto c = m_tblName.front())
        {
        case '\'':
        case '`':
            // Assume id.front() == id.back() for performance
            return c + t2s(m_tblName.substr(1, m_tblName.size()-2)) + c;
        default:
            return t2s(m_tblName);
        }
    return pfx + m_tblName;
}

bool C_LocaleMixin::charset(const std::string &s)
{
    if (m_charset.empty() || !strcasecmp(m_charset.c_str(), s.c_str()))
    {
        m_charset = s;
        return true;
    }
    return false;
}

bool C_LocaleMixin::collate(const std::string &s)
{
    if (m_collate.empty() || !strcasecmp(m_collate.c_str(), s.c_str()))
    {
        m_collate = s;
        return true;
    }
    return false;
}

C_CreateTable::C_CreateTable(C_LexPtr &tbl_name, const C_SqlClauseQ &defs):
    m_tableName(tbl_name),
    m_defs(defs)
{
}

bool C_CreateTable::autoInc(int lastUsed)
{
    if (!m_autoInc || *m_autoInc == lastUsed)
    {
        m_autoInc = lastUsed;
        return true;
    }
    return false;
}

void C_CreateTable::comment(const std::string &rem)
{
    if (!m_comment.empty())
        m_comment += "; ";

    m_comment += rem;
}

bool C_CreateTable::engine(const std::string &id)
{
    if (m_engine.empty() || !strcasecmp(m_engine.c_str(), id.c_str()))
    {
        m_engine = id;
        return true;
    }
    return false;
}

std::string C_AlterItem::str(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret;
    switch (m_type)
    {
    case AIT_TABLE_OPTION:
        ret = std::get<std::string>(m_alteree);
        break;
    case AIT_DROP_INDEX:
        ret = "DROP INDEX " + std::get<std::string>(m_alteree);
        break;
    case AIT_DROP_PRIMARY_KEY:
        ret = "DROP PRIMARY KEY";
        break;
    case AIT_DROP_FOREIGN_KEY:
        ret = "DROP FOREIGN KEY " + std::get<std::string>(m_alteree);
        break;
    case AIT_DROP_COLUMN:
        ret = "DROP COLUMN " + std::get<std::string>(m_alteree);
        break;
    case AIT_MODIFY_COLUMN:
        ret = "MODIFY COLUMN " + std::get<C_ColumnBasePtr>(m_alteree)->to(type, t2s);
    AddPos:
        if (!m_pos.empty())
            ret.append(1,' ').append(m_pos);
        break;
    case AIT_ADD_COLUMN:
        ret = "ADD COLUMN " + std::get<C_ColumnBasePtr>(m_alteree)->to(type, t2s);
        goto AddPos;
    case AIT_ADD_CONSTRAINT:
        ret = "ADD " + std::get<C_ConstraintPtr>(m_alteree)->to(type, t2s);
        break;
    default:
        LOGIC_ERROR("Unknown type {}", int(m_type));
    }
    return ret;
}

void C_CreateTable::removeForeignKeys()
{
    for (auto i = m_defs.begin(); i != m_defs.end();)
    {
        if (auto foreign = dynamic_cast<const C_ReferenceMixin*>(i->get()))
        {
            if (dynamic_cast<const C_ConstraintDef*>(i->get()) && foreign->referenced())
                // Remove the foreign key constraint
            {
                i = m_defs.erase(i);
                continue;
            }

            // Should be C_ColumnDef case: Strip foreign key phrase
            C_LexPtr _;
            const_cast<C_ReferenceMixin*>(foreign)->references(_);
        }
        ++i;
    }
}

void C_CreateTable::removeColumns(std::function<bool(const std::string &)> match)
{
    for (auto i = m_defs.begin(); i != m_defs.end();)
    {
        if (auto col = dynamic_cast<const C_ColumnBaseDef*>(i->get()))
        {
            if (match(bareID(col->name())))
            {
                i = m_defs.erase(i);
                continue;
            }
        }
        else if (auto key = dynamic_cast<const C_ConstraintDef*>(i->get()))
        {
            // Collect indices of  erased columns
            std::vector<std::ptrdiff_t> eraseInd;
            auto &cols = key->columns();
            for (size_t j = cols.size(); j--;)
                if (match(bareID(cols[j].m_name)))
                    eraseInd.emplace_back(j);

            // Remove the whole key if all columns are gone
            if (eraseInd.size() == cols.size())
            {
                i = m_defs.erase(i);
                continue;
            }

            // Remove matched columns, and associated columns if it is foreign key
            auto &vcols = const_cast<C_IndexColumns&>(cols);
            if (auto ref = key->referenced())
            {
                auto &assocols = const_cast<C_ReferenceDef*>(ref)->columns();
                for (auto j: eraseInd)
                {
                    vcols.erase(vcols.begin() + j);
                    assocols.erase(assocols.begin() + j);
                }
            }
            else
                for (auto j: eraseInd)
                    vcols.erase(vcols.begin() + j);
        }
        ++i;
    }
}

C_AlterItems C_CreateTable::compare(const C_CreateTable &other, E_SqlServerType serverType,
    std::function<void(E_SpecialColumnChange chageType, const std::string &colName)> changed) const
{
    C_AlterItems ret;
    std::string s;

    // ALTER SPECS 1/7: Update table options
    if (m_engine != other.m_engine)
        ret.emplace_back(AIT_TABLE_OPTION, "ENGINE=" + other.m_engine);

    bool collationChanged = false;
    if ((CTA_DEFAULT_CHARSET & m_flags) != (CTA_DEFAULT_CHARSET & other.m_flags) ||
        m_charset != other.m_charset ||
        (m_collate != other.m_collate && other.m_collate.empty()))
    {
        s.clear();
        if (CTA_DEFAULT_CHARSET & other.m_flags)
            s = "DEFAULT ";
        ret.emplace_back(AIT_TABLE_OPTION, s + "CHARSET=" + other.m_charset);
        collationChanged = true;
    }
    if ((CTA_DEFAULT_COLLATE & m_flags) != (CTA_DEFAULT_COLLATE & other.m_flags) ||
        (m_collate != other.m_collate && !other.m_collate.empty()))
    {
        s.clear();
        if (CTA_DEFAULT_COLLATE & other.m_flags)
            s = "DEFAULT ";
        ret.emplace_back(AIT_TABLE_OPTION, s + "COLLATE=" + other.m_collate);
        collationChanged = true;
    }
    if (m_comment != other.m_comment)
        ret.emplace_back(AIT_TABLE_OPTION, "COMMENT='" + other.m_comment + '\'');
    if (m_autoInc != other.m_autoInc)
        ret.emplace_back(AIT_TABLE_OPTION, "AUTO_INCREMENT=" + std::to_string(other.m_autoInc? *other.m_autoInc: 1));

    // Finding constraints-to-be-dropped first
    C_ColumnList            myCols, otherCols;
    C_ConstraintSet         myConstraints, otherConstraints;
    partitionCreateDefs(m_defs, myCols, myConstraints);
    partitionCreateDefs(other.m_defs, otherCols, otherConstraints);

    for (auto i = myConstraints.begin(); i != myConstraints.end();)
    {
        const auto iStr = (**i).to(serverType,{});
        for (auto j = otherConstraints.begin(); j != otherConstraints.end(); ++j)
            if (!strcasecmp(iStr.c_str(), (**j).to(serverType,{}).c_str()))
            {
                i = myConstraints.erase(i);
                otherConstraints.erase(j);
                goto Next_myConstraint;
            }
        ++i;
    Next_myConstraint:;
    }

    // ALTER SPECS 2/7: Drop constraints
    for (auto i: myConstraints)
        switch (auto type = i->kind())
        {
        case SCK_INDEX:
        case SCK_FULLTEXT:
        case SCK_SPATIAL:
        case SCK_UNIQUE_KEY:
            s = i->indexName();
            if (!s.empty())
                ret.emplace_back(AIT_DROP_INDEX, s);
            else
                RUNTIME_ERROR(
                    "Dropping noname index (of type {})\n"
                    "{}\n"
                    "\tv.s.\n"
                    "{}",
                    int(type), dbgStr(to(serverType,{})), dbgStr(other.to(serverType,{})));
            break;
        case SCK_PRIMARY_KEY:
            ret.emplace_back(AIT_DROP_PRIMARY_KEY, i);
            break;
        case SCK_FOREIGN_KEY:
            s = i->constraintName();
            if (!s.empty())
                ret.emplace_back(AIT_DROP_FOREIGN_KEY, s);
            else
                RUNTIME_ERROR("Dropping noname foreign key");
            break;
        default:
            LOGIC_ERROR("Unknown constraint type {}", int(type));
        }

    // ALTER SPECS 3/7: Drop columns (while comparing columns)
    std::vector<std::pair<size_t,size_t>> colMap;
    for (size_t i = 0; i < myCols.size(); ++i)
    {
        const auto myColName = myCols[i]->name();
        for (size_t j = 0; j < otherCols.size(); ++j)
            if (!strcasecmp(bareID(myColName).c_str(), bareID(otherCols[j]->name()).c_str()))
            {
                colMap.emplace_back(i, j);
                goto Next_myCol;
            }
        ret.emplace_back(AIT_DROP_COLUMN, myColName);
    Next_myCol:;
    }

    // ALTER SPECS 4/7: Modify columns
    std::sort(colMap.begin(), colMap.end(), [](const std::pair<size_t,size_t> &a, const std::pair<size_t,size_t> &b) {
        return a.second < b.second;
    });
    auto iMap = colMap.begin();
    size_t advance = 0;
    while (iMap != colMap.end() && advance == iMap->first && advance == iMap->second)
    {
        const auto mcol = myCols[advance];
        const auto ocol = otherCols[advance];
        auto dataTypeChanged = false;
        bool same = false;
        if (strcasecmp(mcol->typeStr(serverType,{}).c_str(), ocol->typeStr(serverType,{}).c_str()))
            dataTypeChanged = true;
        else if (!strcasecmp(mcol->to(serverType,{}).c_str(), ocol->to(serverType,{}).c_str()))
        {
            same = true;
            if (collationChanged)
            {
                if (auto locale = ocol->getTypeLocale())
                {
                    if (locale->charset().empty() &&
                        locale->collate().empty())
                    {
                        same = false;
                        dataTypeChanged = true;
                    }
                }
            }
        }
        if (!same)
        {
            ret.emplace_back(AIT_MODIFY_COLUMN, ocol);
            if (!hasAutoInc(*mcol) && hasAutoInc(*ocol))
                ret.back().m_addAutoInc = true;
            if (dataTypeChanged)
                changed(SCC_DATA_TYPE, ocol->name());
        }
        ++iMap;
        ++advance;
    }

    // ALTER SPECS 5/7: Add or Reposition (& Modify) columns
    for (; iMap != colMap.end(); ++iMap, ++advance)
    {
        for (; advance < iMap->second; ++advance)
        {
            ret.emplace_back(AIT_ADD_COLUMN, otherCols[advance], advance? " AFTER "+otherCols[advance-1]->name(): " FIRST");
            if (hasAutoInc(*otherCols[advance]))
                ret.back().m_addAutoInc = true;
        }
        const auto ocol = otherCols[advance];
        auto dataTypeChanged = false;
        if (strcasecmp(myCols[iMap->first]->typeStr(serverType,{}).c_str(), ocol->typeStr(serverType,{}).c_str()))
            dataTypeChanged = true;
        if (collationChanged)
        {
            if (auto locale = ocol->getTypeLocale())
            {
                if (locale->charset().empty() &&
                    locale->collate().empty())
                    dataTypeChanged = true;
            }
        }

        ret.emplace_back(AIT_MODIFY_COLUMN, ocol, advance? " AFTER "+otherCols[advance-1]->name(): " FIRST");
        if (!hasAutoInc(*myCols[iMap->first]) && hasAutoInc(*ocol))
            ret.back().m_addAutoInc = true;
        if (dataTypeChanged)
            changed(SCC_DATA_TYPE, ocol->name());
    }

    // ALTER SPECS 6/7: Append new columns
    for (advance = colMap.empty()? 0: colMap.back().second+1; advance < otherCols.size(); ++advance)
    {
        ret.emplace_back(AIT_ADD_COLUMN, otherCols[advance]);
        if (hasAutoInc(*otherCols[advance]))
            ret.back().m_addAutoInc = true;
    }

    // ALTER SPECS 7/7: Add new constraints
    for (auto &i: otherConstraints)
        ret.emplace_back(AIT_ADD_CONSTRAINT, i);

    return ret;
}

void C_CreateTable::eachDef(std::function<void(const C_SqlClausePtr &def)> apply) const
{
    for (auto &i: m_defs)
        apply(i);
}

C_StrQ C_CreateTable::getKeyColumnNames(E_PrimaryUniqueAttr keyType) const
{
    for (auto &i: m_defs)
        if (auto constraint = dynamic_cast<const C_ConstraintDef*>(i.get()))
        {
            bool found = false;
            switch (constraint->kind())
            {
            case SCK_PRIMARY_KEY:
                if (keyType == PUA_PRIMARY_KEY)
                    found = true;

                break;
            case SCK_UNIQUE_KEY:
                if (keyType == PUA_UNIQUE_KEY)
                    found = true;

                break;
            default:;
            }
            if (found)
            {
                C_StrQ ret;
                for (auto &col: constraint->columns())
                    ret.emplace_back(col.m_name);

                return ret;
            }
        }
        else if (auto col = dynamic_cast<const C_ColumnDef*>(i.get()))
        {
            if (col->pua() == keyType)
                return {col->name()};
        }
    return {};
}

C_StrQ C_CreateTable::to(E_SqlServerType type, F_Str2Str t2s) const
{
    C_StrQ lines;
    std::string sql ="CREATE";
    if (CTA_CREATE_OR_REPLACE & m_flags)
    {
        if (SST_SQLITE != type)
            sql += " OR REPLACE";
        else
            // Prepend DROP TABLE instead
            lines.emplace_back("DROP TABLE IF EXISTS " + m_tableName.str(t2s));
    }
    if (CTA_TEMPORARY & m_flags)
        sql += " TEMPORARY";

    sql += " TABLE ";
    if (CTA_CREATE_IF_NOT_EXISTS & m_flags)
        sql += "IF NOT EXISTS ";

    sql += m_tableName.str(t2s);

    std::vector<const C_ConstraintDef*> indices;
    sql += toNewTable(type, &indices);
    lines.emplace_back(sql);

    size_t count = 0;
    for (auto i: indices)
        // Currently for SQLite exclusively
    {
        sql = "CREATE INDEX ";
        if (CTA_CREATE_IF_NOT_EXISTS & m_flags)
            sql += "IF NOT EXISTS ";

        auto indName = i->indexName();
        if (indName.empty())
            indName = m_tableName.bareName() + "_key" + std::to_string(++count);

        sql.append(m_tableName.dbPrefix()).append(indName).append(" ON ").append(m_tableName.str(t2s))
           .append(toStr(i->columns(), type));
        lines.emplace_back(sql);
    }
    return lines;
}

std::string C_CreateTable::toNewTable(E_SqlServerType type, std::vector<const C_ConstraintDef*> *indices) const
{
    std::string sql;
    C_SqlClauseQ spared;
    for (auto i = m_defs.begin(), end = m_defs.end(); i != end; ++i)
    {
        bool found = false;
        for (auto &j: spared)
            if (*i == j)
                found = true;

        if (found)
            continue;

        if (SST_SQLITE == type)
        {
            if (auto col = dynamic_cast<const C_ColumnDef*>(i->get()))
            {
                if (col->hasAutoInc() && col->pua() == PUA_NONE)
                    for (auto j = i; ++j != end;)
                        if (auto constraint = dynamic_cast<const C_ConstraintDef*>(j->get()))
                            if (constraint->kind() == SCK_PRIMARY_KEY)
                            {
                                auto &cols = constraint->columns();
                                if (cols.size() == 1 && cols.front().m_name == col->name())
                                {
                                    spared.emplace_back(*j);
                                    break;
                                }
                            }
            }
            else if (auto constraint = dynamic_cast<const C_ConstraintDef*>(i->get()))
                switch (constraint->kind())
                {
                case SCK_INDEX:
                    if (indices)
                        // to be appended as CREATE INDEX instead
                        indices->emplace_back(constraint);
                    [[fallthrough]];
                case SCK_FULLTEXT:
                case SCK_SPATIAL:
                    continue; // skip this one
                default:;
                }
        }
        const auto def = (*i)->to(type,{});
        if (!def.empty())
        {
            sql += sql.empty()? " (\n\t": ",\n\t";
            sql += def;
        }
    }
    sql += "\n)";

    if (SST_MYSQL == type)
        // After ')' -- Past create definitions
    {
        if (!m_engine.empty())
            sql += " ENGINE=" + m_engine;
        if (!m_charset.empty())
        {
            if (CTA_DEFAULT_CHARSET & m_flags)
                sql += " DEFAULT";

            sql += " CHARSET=" + m_charset;
        }
        if (m_autoInc)
            sql += " AUTO_INCREMENT=" + std::to_string(*m_autoInc);
        if (!m_collate.empty())
        {
            if (CTA_DEFAULT_COLLATE & m_flags)
                sql += " DEFAULT";

            sql += " COLLATE=" + m_collate;
        }
        if (!m_comment.empty())
            sql += " COMMENT='" + m_comment + '\'';
    }
    return sql;
}

std::string C_CheckDef::to(E_SqlServerType type, F_Str2Str t2s) const
{
    return "CHECK (" + expr2str(m_expr, type, t2s) + ')';
}

C_ColumnBaseDef::C_ColumnBaseDef(const std::string &name, C_LexPtr &type):
    m_colName(name),
    m_type(type)
{
}

std::optional<E_DataType> C_ColumnBaseDef::getShallowType() const
{
    E_DataType type;
    if (auto dt = dynamic_cast<const C_DataTypeLex*>(m_type.get()))
        type = dt->type();
    else if (auto t = bux::tryUnlex<E_DataType>(m_type))
        type = *t;
    else
        return {};

    return type;
}

std::string C_ColumnBaseDef::typeStr(E_SqlServerType type, F_Str2Str t2s) const
{
    return expr2str(m_type, type, t2s);
}

C_ColumnDefault::C_ColumnDefault(E_ColDefOrder order, C_LexPtr &first, C_LexPtr &second):
    m_first(first),
    m_second(second),
    m_order(order)
{
}

std::string C_ColumnDefault::to(E_SqlServerType type, F_Str2Str t2s) const
{
    const std::string s1st = expr2str(m_first, type, t2s);
    std::string ret, s2nd;
    if (m_second)
        s2nd = expr2str(m_second, type, t2s);

    switch (m_order)
    {
    case CDO_DEFAULT_FIRST:
        ret = "DEFAULT " + s1st;
        if (m_second && SST_SQLITE != type)
            ret += " ON UPDATE " + s2nd;
        break;
    case CDO_ON_UPDATE_FIRST:
        if (SST_SQLITE != type)
        {
            ret = "ON UPDATE " + s1st;
            if (m_second)
                ret += " DEFAULT " + s2nd;
        }
        else
        {
            if (m_second)
                ret += "DEFAULT " + s2nd;
        }
        break;
    default:
        LOGIC_ERROR("Unkonow order {}", int(m_order));
    }
    return ret;
}

bool C_ColumnDefault::timestampOnly() const
{
    return CDO_DEFAULT_FIRST != m_order || m_second;
}

C_ColumnDef::C_ColumnDef(const std::string &name, C_LexPtr &type):
    C_ColumnBaseDef(name, type)
{
}

bool C_ColumnDef::setDefault(C_LexPtr &expr)
{
    /* KNOWLEDGE SOURCE:
       ON UPDATE clause syntax dealt here is not mentioned within CREATE TABLE syntax pages.
       https://dev.mysql.com/doc/refman/5.7/en/timestamp-initialization.html
       https://mariadb.com/kb/en/mariadb/timestamp/
    */
    if (auto def = std::dynamic_pointer_cast<const C_ColumnDefault>(expr))
    {
        if (def->timestampOnly())
        {
            if (auto type = getShallowType())
                switch (*type)
                {
                case DT_DATETIME:
                case DT_TIMESTAMP:
                    break; // It's ok
                default:
                    m_errMsg = "ON UPDATE clause is alllowed only for DATETIME and TIMESTAMP";
                    return false; // Syntax error
                }
        }
        m_default = def;
        return true;
    }
    m_errMsg = "Not C_ColumnDefault";
    return false;
}

std::string C_ColumnDef::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string sql = m_colName;
    if (m_autoInc && SST_SQLITE == type)
    {
        sql += " INTEGER PRIMARY KEY AUTOINCREMENT";
        if (m_default)
            sql.append(1, ' ').append(m_default->to(type, t2s));
    }
    else
    {
        sql += ' ' + dataType2str(m_type, type, t2s);
        if (m_notNull > 0)
            sql += " NOT NULL";
        else if (!m_notNull && type != SST_SQLITE)
            sql += " NULL";

        if (m_default)
            sql.append(1, ' ').append(m_default->to(type, t2s));
        if (m_autoInc)
            sql += " AUTO_INCREMENT";

        switch (m_pua)
        {
        case PUA_PRIMARY_KEY:
            sql += " PRIMARY KEY";
            break;
        case PUA_UNIQUE_KEY:
            sql += " UNIQUE";
            break;
        default:;
        }
    }

    if (!m_comment.empty() && type != SST_SQLITE)
        sql += " COMMENT '" + m_comment + '\'';

    if (type == SST_MYSQL)
        switch (m_colFormat)
        {
        case COLUMN_FORMAT_FIXED:
            sql += " COLUMN_FORMAT FIXED";
            break;
        case COLUMN_FORMAT_DYNAMIC:
            sql += " COLUMN_FORMAT DYNAMIC";
            break;
        case COLUMN_FORMAT_DEFAULT:
            sql += " COLUMN_FORMAT DEFAULT";
            break;
        default:;
        }

    if (m_refDef)
        sql += ' ' + m_refDef->to(type, t2s);

    return sql;
}

C_GenColumnDef::C_GenColumnDef(const std::string &name, C_LexPtr &type, C_LexPtr &expr, E_GenColumnMode mode):
    C_ColumnBaseDef(name, type),
    m_expr(expr),
    m_mode(mode),
    m_unique(false)
{
}

std::string C_GenColumnDef::to(E_SqlServerType type, F_Str2Str t2s) const
{
    if (SST_SQLITE == type)
        return {};

    std::string sql = m_colName + ' ' + dataType2str(m_type, type, t2s) + " AS (" + expr2str(m_expr, type, t2s);
    switch (m_mode)
    {
    case GCM_VIRTUAL:       sql +=  ") VIRTUAL";       break;
    case GCM_PERSISTENT:    sql +=  ") PERSISTENT";    break;
    default:
        LOGIC_ERROR("Unexpected mode {}", int(m_mode));
    }
    if (m_unique)
        sql += " UNIQUE";
    if (!m_comment.empty())
        sql += " COMMENT '" + m_comment + '\'';
    return sql;
}

C_ReferenceDef::C_ReferenceDef(C_LexPtr &tbl_name, const C_IndexColumns &cols):
    m_tableName(tbl_name),
    m_cols(cols),
    m_match(MATCH_NONE),
    m_onDelete(EA_NONE),
    m_onUpdate(EA_NONE)
{
}

std::string C_ReferenceDef::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string sql = "REFERENCES " + m_tableName.str(t2s) + toStr(m_cols, type);
    switch (m_match)
    {
    case MATCH_FULL:    sql += " MATCH FULL";       break;
    case MATCH_PARTIAL: sql += " MATCH PARTIAL";    break;
    case MATCH_SIMPLE:  sql += " MATCH SIMPLE";     break;
    default:;
    }
    if (auto p = actionName(m_onDelete))
        sql.append(" ON DELETE ").append(p);
    if (auto p = actionName(m_onUpdate))
        sql.append(" ON UPDATE ").append(p);

    return sql;
}

void C_ReferenceMixin::references(C_LexPtr &refDef)
{
    if (!refDef)
        m_refDef.reset();
    else
        assignLex(m_refDef, refDef, " not C_ReferenceDef");
}

C_ConstraintDef::C_ConstraintDef(
    E_SqlConstraintKind     kind,
    const std::string       *id,
    const C_IndexColumns    &cols   ):
    m_cols(cols),
    m_kind(kind)
{
    if (id)
        m_id = *id;
}

bool C_ConstraintDef::indexType(E_InexUsing type)
{
    if (!m_indexUsing || *m_indexUsing == type)
    {
        m_indexUsing = type;
        return true;
    }
    return false;
}

bool C_ConstraintDef::keyBlockSize(int i)
{
    if (!m_keyBlockSize || *m_keyBlockSize == i)
    {
        m_keyBlockSize = i;
        return true;
    }
    return false;
}

bool C_ConstraintDef::parserName(const std::string &name)
{
    if (m_parserName.empty() || !strcasecmp(m_parserName.c_str(), name.c_str()))
    {
        m_parserName = name;
        return true;
    }
    return false;
}

std::string C_ConstraintDef::constraintHeader() const
{
    std::string sql;
    if (!m_constraintName.empty())
        sql = "CONSTRAINT " + m_constraintName + " ";

    return sql;
}

std::string C_ConstraintDef::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string sql;
    switch (m_kind)
    {
    case SCK_INDEX: // Non-SQLite is assumed
        sql = "INDEX ";
        break;
    case SCK_FULLTEXT:
        sql = "FULLTEXT ";
        break;
    case SCK_SPATIAL:
        sql = "SPATIAL ";
        break;
    case SCK_PRIMARY_KEY:
        sql = constraintHeader() + "PRIMARY KEY ";
        break;
    case SCK_UNIQUE_KEY:
        sql = constraintHeader() + "UNIQUE ";
        break;
    case SCK_FOREIGN_KEY:
        sql = constraintHeader() + "FOREIGN KEY ";
        break;
    }

    if (SST_SQLITE != type)
    {
        if (!m_id.empty())
            sql += m_id + ' ';
    }
    sql += toStr(m_cols, type);
    if (m_refDef)
        sql += ' ' + m_refDef->to(type,t2s);

    if (SST_SQLITE != type)
    {
        if (m_keyBlockSize)
            sql += " KEY_BLOCK_SIZE = " + std::to_string(*m_keyBlockSize);
        if (m_indexUsing)
            switch (*m_indexUsing)
            {
            case USING_BTREE:   sql += " USING BTREE";  break;
            case USING_HASH:    sql += " USING HASH";   break;
            case USING_RTREE:   sql += " USING RTREE";  break;
            }
        if (!m_parserName.empty())
            sql += " WITH PARSER " + m_parserName;
    }
    return sql;
}


C_DataTypeLex::C_DataTypeLex(E_DataType type, int w1, int w2):
    m_type(type), m_w1(w1), m_w2(w2)
{
}

std::string C_DataTypeLex::to(E_SqlServerType, F_Str2Str) const
{
    std::string ret = typeName(m_type);
    if (m_w1 >= 0)
    {
        ret += '(' + std::to_string(m_w1);
        if (m_w2 >= 0)
            ret += ',' + std::to_string(m_w2);

        ret += ')';
    }
    if (DTA_UNSIGNED & m_flags)
        ret += " UNSIGNED";
    if (DTA_ZEROFILL & m_flags)
        ret += " ZEROFILL";
    return ret;
}

C_LocaleClauseLex::C_LocaleClauseLex(C_LexPtr &innerClause): m_innerClause(innerClause)
{
}

std::string C_LocaleClauseLex::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret = expr2str(m_innerClause, type, t2s);
    if (SST_MYSQL == type)
    {
        if (!m_charset.empty())
            ret += " CHARSET " + m_charset;
        if (!m_collate.empty())
            ret += " COLLATE " + m_collate;
    }
    return ret;
}

C_ListTypeLex::C_ListTypeLex(E_ListType type, const C_StrQ &list):
    m_type(type),
    m_list(list)
{
}

std::string C_ListTypeLex::to(E_SqlServerType, F_Str2Str) const
{
    std::string ret;
    switch (m_type)
    {
    case LT_ENUM:   ret = "ENUM";   break;
    case LT_SET:    ret = "SET";    break;
    }
    bool first = true;
    for (auto &s: m_list)
    {
        ret += first? "('": "', '";
        ret += s;
        first = false;
    }
    ret += "')";
    return ret;
}

C_CreateView::C_CreateView(C_LexPtr &tbl_name, C_LexPtr &select):
    m_tableName(tbl_name),
    m_algo(ALGORITHM_NONE),
    m_security(SQL_SECURITY_NONE),
    m_checkOpt(CHKOPT_NONE)
{
    assignLex(m_select, select, " not C_Select");
}

C_StrQ C_CreateView::to(E_SqlServerType type, F_Str2Str t2s) const
{
    C_StrQ lines;
    std::string sql ="CREATE ";
    if (CTA_CREATE_OR_REPLACE & m_flags)
    {
        if (SST_SQLITE != type)
            sql += "OR REPLACE ";
        else
            // Prepend DROP VIEW instead
            lines.emplace_back("DROP VIEW IF EXISTS " + m_tableName.str(t2s));
    }
    if (SST_MYSQL == type)
    {
        switch (m_algo)
        {
        case ALGORITHM_UNDEFINED:   sql += "ALGORITHM = UNDEFINED ";    break;
        case ALGORITHM_MERGE:       sql += "ALGORITHM = MERGE ";        break;
        case ALGORITHM_TEMPTABLE:   sql += "ALGORITHM = TEMPTABLE ";    break;
        default:;
        }
        if (m_definer)
        {
            if (auto user = bux::tryUnlex<std::string>(m_definer))
                sql += "DEFINER = '" + *user + "' ";
            else
                // Magic which this module and grammar definitions have agreed on
                sql += "DEFINER = CURRENT_USER ";
        }
        switch (m_security)
        {
        case SQL_SECURITY_DEFINER:  sql += "SQL SECURITY DEFINER "; break;
        case SQL_SECURITY_INVOKER:  sql += "SQL SECURITY INVOKER "; break;
        default:;
        }
    }
    sql += "VIEW ";
    if (CTA_CREATE_IF_NOT_EXISTS & m_flags)
        sql += "IF NOT EXISTS ";

    sql += m_tableName.str(t2s);
    sql += toNewView(type);

    lines.emplace_back(sql);
    return lines;
}

std::string C_CreateView::toNewView(E_SqlServerType type) const
{
    std::string s;
    if (!m_columnNames.empty() && SST_MYSQL == type)
    {
        bool first = true;
        for (auto &i: m_columnNames)
        {
            s.append(first? " (": ", ").append(i);
            first = false;
        }
        s += ")";
    }
    s += " AS " + m_select->to(type,{});
    if (SST_SQLITE != type)
        switch (m_checkOpt)
        {
        case CHKOPT_PLAIN:      s += " WITH CHECK OPTION"; break;
        case CHKOPT_CASCADED:   s += " WITH CASCADED CHECK OPTION"; break;
        case CHKOPT_LOCAL:      s += " WITH LOCAL CHECK OPTION"; break;
        default:;
        }

    return s;
}

std::string C_Null::to(E_SqlServerType, F_Str2Str) const
{
    return "NULL";
}

C_Id::C_Id(const std::string &id, E_IdKind kind): m_kind(kind)
{
    m_ids.emplace_back(id);
}

C_Id::C_Id(const char *id, E_IdKind kind): m_kind(kind)
{
    m_ids.emplace_back(id);
}

std::string C_Id::to(E_SqlServerType type, F_Str2Str t2s) const
{
    if (m_ids.size() == 1)
    {
        if (SST_SQLITE == type && isTimestampName(m_ids.front()))
            return "CURRENT_TIMESTAMP";
        if (m_kind == IK_TABLEWISE && t2s)
            return t2s(m_ids.front());
    }

    std::string ret;
    for (auto &i: m_ids)
    {
        if (!ret.empty())
            ret += '.';

        ret += i;
    }
    return ret;
}

std::string C_Parentheses::to(E_SqlServerType type, F_Str2Str t2s) const
{
    return (m_exists? "EXISTS (": "(") + expr2str(m_expr, type, t2s) + ')';
}

std::string C_UnaryOp::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret = expr2str(m_expr, type, t2s);
    if (m_postfix)
        ret.append(1, ' ').append(m_operator);
    else
    {
        auto prefix = std::string(m_operator);
        if (isalnum(prefix.back()))
            prefix += ' ';

        ret.insert(0, prefix);
    }
    return ret;
}

std::string C_AliasedExpr::to(E_SqlServerType type, F_Str2Str t2s) const
{
    return expr2str(m_expr, type, t2s) + (m_useAS?" AS ":" ") + m_alias;
}

std::string C_BinaryOp::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string op = m_operator;
    if (SST_MYSQL != type)
    {
        if (op == "RLIKE")
            op = "REGEXP";
    }
    std::string ret = expr2str(m_left, type, t2s) + (m_not?" NOT ":" ") + op + ' ' + expr2str(m_right, type, t2s);
    if (m_escape)
        ret += " ESCAPE " + expr2str(m_escape, type, t2s);

    return ret;
}

C_ExprCase::C_ExprCase(C_LexPtr &caseValue, const C_LexPairList &whenList, C_LexPtr &elseExpr):
    m_caseValue(caseValue),
    m_elseExpr(elseExpr),
    m_whens(whenList)
{
}

std::string C_ExprCase::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret = "CASE";
    if (m_caseValue)
        ret += ' ' + expr2str(m_caseValue, type, t2s);

    for (auto &i: m_whens)
        ret += " WHEN " + expr2str(i.first, type, t2s) + " THEN " + expr2str(i.second, type, t2s);

    if (m_elseExpr)
        ret += " ELSE " + expr2str(m_elseExpr, type, t2s);

    return ret + " END";
}

C_ClauseAsCmd::C_ClauseAsCmd(C_LexPtr &clause)
{
    assignLex(m_clause, clause, " not I_SqlClause");
}

C_StrQ C_ClauseAsCmd::to(E_SqlServerType type, F_Str2Str t2s) const
{
    return {m_clause->to(type, t2s)};
}

C_FunctionCall::C_FunctionCall(const std::string &name):
    m_name(name),
    m_distinct(false)
{
}

C_FunctionCall::C_FunctionCall(const std::string &name, const C_LexList &params):
    m_name(name),
    m_params(params),
    m_distinct(false)
{
    if (!m_params.empty() && typeid(*m_params.front()) == typeid(bux::I_LexAttr))
    {
        m_params.pop_front();
        m_distinct = true;
    }
}

std::string C_FunctionCall::to(E_SqlServerType type, F_Str2Str t2s) const
{
    if (SST_SQLITE == type && isTimestampName(m_name))
        return "CURRENT_TIMESTAMP"; // regardless contents of m_params

    std::string ret = m_name;
    if (m_params.empty())
        ret += "()";
    else
    {
        bool first = true;
        for (auto &i: m_params)
        {
            if (first)
                ret += m_distinct? "(DISTINCT ": "(";
            else
                ret += ", ";
            ret += expr2str(i, type, t2s);
            first = false;
        }
        ret += ')';
    }
    return ret;
}

C_QueryPlan::C_QueryPlan(const char *verb, const C_StrQ &ids):
    m_verb(verb),
    m_target(nullptr),
    m_ids(ids)
{
}

std::string C_QueryPlan::to(E_SqlServerType, F_Str2Str) const
{
    std::string sql = m_verb;
    sql += " INDEX ";
    if (m_target)
        sql += m_target;

    bool first = true;
    for (auto &i: m_ids)
    {
        sql += first? " (": ", ";
        sql += i;
        first = false;
    }
    return sql + ')';
}

C_GroupBy::C_GroupBy(const C_LexList &orderList):
    m_orderList(orderList),
    m_withRollUp(false)
{
}

std::string C_GroupBy::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret;
    for (auto &i: m_orderList)
    {
        ret += ret.empty()? "GROUP BY ": ", ";
        ret += expr2str(i, type, t2s);
    }
    if (m_withRollUp)
        ret += " WITH ROLLUP";
    if (m_havingCond)
        ret += " HAVING " + expr2str(m_havingCond, type, t2s);

    return ret;
}

std::string C_Limit::to(E_SqlServerType, F_Str2Str) const
{
    std::string ret = "LIMIT " + std::to_string(m_n1);
    if (!m_justOne)
        ret += (m_offsetKeyword? " OFFSET ": ", ") + std::to_string(m_n2);

    return ret;
}

C_From::C_From(const C_LexList &tableRefs):
    m_tableRefs(tableRefs),
    m_lockType(nullptr)
{
}

void C_From::groupBy(C_LexPtr &cond)
{
    assignLex(m_groupBy, cond, " not C_GroupBy");
}

void C_From::limit(C_LexPtr &i)
{
    assignLex(m_limit, i, " not C_Limit");
}

void C_From::procedure(C_LexPtr &proc)
{
    assignLex(m_procedure, proc, " not C_FunctionCall");
}

void C_From::queryPlan(C_LexPtr &plan)
{
    assignLex(m_queryPlan, plan, " not C_C_QueryPlan");
}

std::string C_From::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string sql = "FROM ";
    bool first = true;
    for (auto &i: m_tableRefs)
    {
        if (first)
            first = false;
        else
            sql += ", ";

        sql += expr2str(i, type, t2s);
    }
    if (SST_MYSQL == type)
    {
        if (!m_partitionList.empty())
        {
            sql += " PARTITION (";
            first = true;
            for (auto &i: m_partitionList)
            {
                if (first)
                    first = false;
                else
                    sql += ", ";

                sql += i;
            }
            sql += ')';
        }
        if (m_queryPlan)
            sql += ' ' + m_queryPlan->to(type, t2s);
    }
    if (m_whereCond)
        sql += " WHERE " + expr2str(m_whereCond, type, t2s);
    if (m_groupBy)
        sql += ' ' + m_groupBy->to(type, t2s);

    first = true;
    for (auto &i: m_orderByList)
    {
        sql.append(first? " ORDER BY ": ", ").append(expr2str(i, type, t2s));
        first = false;
    }
    if (m_limit)
        sql += ' ' + m_limit->to(type, t2s);

    if (SST_MYSQL == type)
    {
        if (m_procedure)
            sql.append(" PROCEDURE ").append(m_procedure->to(type, t2s));
        if (m_lockType)
            sql.append(1, ' ').append(m_lockType);
    }
    return sql;
}

C_Select::C_Select(const C_LexList &columns):
    m_columns(columns)
{
}

void C_Select::from(C_LexPtr &fromPhrase)
{
    assignLex(m_from, fromPhrase, " not C_From");
}

std::string C_Select::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string sql = "SELECT ";
    switch (m_flags & SELATTR_DISTINCT_MASK)
    {
    case SELATTR_ALL:
        sql += "ALL ";
        break;
    case SELATTR_DISTINCTROW:
        if (SST_MYSQL == type)
        {
            sql += "DISTINCTROW ";
            break;
        }
        [[fallthrough]];
    case SELATTR_DISTINCT:
        sql += "DISTINCT ";
        break;
    default:;
    }
    if (SST_MYSQL == type)
    {
        if (SELATTR_HIGH_PRIORITY & m_flags)
            sql += "HIGH_PRIORITY ";
        if (SELATTR_STRAIGHT_JOIN & m_flags)
            sql += "STRAIGHT_JOIN ";
        if (SELATTR_SQL_SMALL_RESULT & m_flags)
            sql += "SQL_SMALL_RESULT ";
        if (SELATTR_SQL_BIG_RESULT & m_flags)
            sql += "SQL_BIG_RESULT ";
        if (SELATTR_SQL_BUFFER_RESULT & m_flags)
            sql += "SQL_BUFFER_RESULT ";
        switch (m_flags & SELATTR_SQL_CACHE_MASK)
        {
        case SELATTR_SQL_CACHE:
            sql += "SQL_CACHE ";
            break;
        case SELATTR_SQL_NO_CACHE:
            sql += "SQL_NO_CACHE ";
            break;
        default:;
        }
        if (SELATTR_SQL_CALC_FOUND_ROWS & m_flags)
            sql += "SQL_CALC_FOUND_ROWS ";
    }
    bool first = true;
    for (auto &i: m_columns)
    {
        if (first)
            first = false;
        else
            sql += ", ";

        sql += expr2str(i, type, t2s);
    }

    if (m_from)
        sql += ' ' + m_from->to(type, t2s);

    return sql;
}

std::string C_OnCond::to(E_SqlServerType type, F_Str2Str t2s) const
{
    return "ON " + expr2str(m_cond, type, t2s);
}

std::string C_UsingCond::to(E_SqlServerType, F_Str2Str) const
{
    std::string ret;
    for (auto &i: m_ids)
        ret.append(ret.empty()? "USING (": ", ").append(i);

    return ret + ')';
}

C_JoinExpr::C_JoinExpr      (
    C_LexPtr                &left,
    const std::string       &joinOp,
    C_LexPtr                &right,
    C_LexPtr                &joinCond   ): m_joinOp(joinOp)
{
    assignLex(m_left,  left,   " not left operand type of JOIN");
    assignLex(m_right, right,  " not right operand type of JOIN");
    if (joinCond)
        assignLex(m_cond, joinCond, " not ON/USING phrase type of JOIN");
}

std::string C_JoinExpr::to(E_SqlServerType type, F_Str2Str t2s) const
{
    std::string ret = m_left->to(type, t2s) + ' ' + m_joinOp + ' ' + m_right->to(type,t2s);
    if (m_cond)
        ret += ' ' + m_cond->to(type, t2s);

    return ret;
}

C_IndexColumn::C_IndexColumn(const std::string &name, int length, const char *ad):
    m_name(name),
    m_length(length),
    m_ad(ad)
{
}
