#pragma once

#include "SqlFwd.h"     // E_SqlServerType
//------------------------------------------------
#include "bux/GLR.h"    // bux::GLR::*
#include <list>         // std::list<>
#include <optional>     // std::optional<>
#include <variant>      // std::variant<>
#include <vector>       // std::vector<>

using bux::GLR::C_LexPtr;

//
//      Constants
//
enum
{
    // Co-Flags of C_CreateTable & C_CreateView
    CTA_CREATE_OR_REPLACE       = 1<<0,
    CTA_TEMPORARY               = 1<<1,
    CTA_CREATE_IF_NOT_EXISTS    = 1<<2,
    CTA_DEFAULT_CHARSET         = 1<<3,
    CTA_DEFAULT_COLLATE         = 1<<4,
    // Flags of <NumType>
    DTA_UNSIGNED                = 1<<0,
    DTA_ZEROFILL                = 1<<1,
    // Flags of C_Select
    // --- Embeded enum begin
    SELATTR_ALL                 = 1,
    SELATTR_DISTINCT            = 2,
    SELATTR_DISTINCTROW         = 3,
    SELATTR_DISTINCT_MASK       = 3,    // 2 bits
    // --- Embeded enum end
    SELATTR_HIGH_PRIORITY       = 1<<2,
    SELATTR_STRAIGHT_JOIN       = 1<<3,
    SELATTR_SQL_SMALL_RESULT    = 1<<4,
    SELATTR_SQL_BIG_RESULT      = 1<<5,
    SELATTR_SQL_BUFFER_RESULT   = 1<<6,
    // --- Embeded enum begin
    SELATTR_SQL_CACHE           = 1<<7,
    SELATTR_SQL_NO_CACHE        = 2<<7,
    SELATTR_SQL_CACHE_MASK      = 3<<7,
    // --- Embeded enum end
    SELATTR_SQL_CALC_FOUND_ROWS = 1<<9
};

//
//      Types
//
typedef std::vector<std::string> C_StrQ;
typedef std::function<std::string(const std::string &s)> F_Str2Str;

struct I_SqlCmd: bux::I_LexAttr
{
    virtual C_StrQ to(E_SqlServerType type, F_Str2Str t2s) const = 0;
};
typedef std::shared_ptr<const I_SqlCmd> C_SqlCmdPtr;

struct I_SqlClause: bux::I_LexAttr
{
    virtual std::string to(E_SqlServerType type, F_Str2Str t2s) const = 0;
};
typedef std::shared_ptr<const I_SqlClause> C_SqlClausePtr;
typedef std::vector<C_SqlClausePtr> C_SqlClauseQ;

class C_TableName
{
public:

    // Nonvirtuals
    C_TableName(C_LexPtr &tbl_name);
    std::string dbPrefix() const;
    std::string bareName() const;
    std::string str(F_Str2Str t2s) const;
    bool prefixed() const   { return !m_dbName.empty(); }

private:

    // Data
    std::string         m_dbName, m_tblName;
};

class C_LocaleMixin
{
public:

    // Nonvirtuals
    auto &charset() const   { return m_charset; }
    bool charset(const std::string &s);
    auto &collate() const   { return m_collate; }
    bool collate(const std::string &s);

protected:

    // Data
    std::string             m_charset, m_collate;
};

class C_FlagsMixin
{
public:

    // Nonvirtuals
    C_FlagsMixin(): m_flags(0) {}
    void enable(int flags) { m_flags |= flags; }

protected:

    // Data
    int                     m_flags;
};

struct C_IndexColumn
{
    std::string             m_name;
    int                     m_length;
    const char              *m_ad;  // ASC / DESC

    C_IndexColumn(const std::string &name, int length = -1, const char *ad = nullptr);
};
typedef std::vector<C_IndexColumn> C_IndexColumns;

enum E_MatchCond
{
    MATCH_NONE,
    MATCH_FULL,
    MATCH_PARTIAL,
    MATCH_SIMPLE
};

enum E_EventAction
{
    EA_NONE,
    EA_RESTRICT,
    EA_CASCADE,
    EA_SET_NULL,
    EA_NO_ACTION
};

class C_ReferenceDef: public I_SqlClause
{
public:

    // Nonvirtuals
    C_ReferenceDef(C_LexPtr &tbl_name, const C_IndexColumns &cols);
    void match(E_MatchCond cond)        { m_match = cond; }
    void onDelete(E_EventAction action) { m_onDelete = action; }
    void onUpdate(E_EventAction action) { m_onUpdate = action; }
    auto &tableName() const             { return m_tableName; }
    auto &columns()                     { return m_cols; }
    auto &columns() const               { return m_cols; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_TableName             m_tableName;
    C_IndexColumns          m_cols;
    E_MatchCond             m_match;
    E_EventAction           m_onDelete, m_onUpdate;
};

class C_ReferenceMixin
{
public:

    // Nonvirtuals
    void references(C_LexPtr &refDef);
    auto referenced() const { return m_refDef.get(); }

protected:

    // Data
    std::shared_ptr<const C_ReferenceDef> m_refDef;
};

enum E_PrimaryUniqueAttr
{
    PUA_NONE,
    PUA_PRIMARY_KEY,
    PUA_UNIQUE_KEY,
    //-------------------------
    PUA_MAX = 255   // to suppress warnings from implementation details of the generated parser
};

enum E_ColFormat
{
    COLUMN_FORMAT_NONE,
    COLUMN_FORMAT_FIXED,
    COLUMN_FORMAT_DYNAMIC,
    COLUMN_FORMAT_DEFAULT
};

enum E_DataType
{
    DT_BOOLEAN,
    DT_BIT,
    DT_TINYBLOB,
    DT_MEDIUMBLOB,
    DT_LONGBLOB,
    DT_BLOB,
    DT_BINARY,
    DT_VARBINARY,
    DT_TINYINT,
    DT_SMALLINT,
    DT_MEDIUMINT,
    DT_BIGINT,
    DT_INT,
    DT_INTEGER,
    DT_DECIMAL,
    DT_FLOAT,
    DT_DOUBLE,
    DT_DOUBLE_PRECISION,
    DT_REAL,
    DT_TINYTEXT,
    DT_MEDIUMTEXT,
    DT_LONGTEXT,
    DT_CHAR,
    DT_NCHAR,
    DT_TEXT,
    DT_VARCHAR,
    DT_VARCHAR2,
    DT_NVARCHAR,
    DT_DATE,
    DT_TIME,
    DT_DATETIME,
    DT_TIMESTAMP,
    DT_YEAR,
    DT_POINT,
    DT_LINESTRING,
    DT_POLYGON,
    DT_MULTIPOINT,
    DT_MULTILINESTRING,
    DT_MULTIPOLYGON,
    DT_GEOMETRYCOLLECTION,
    DT_GEOMETRY
};

class C_ColumnBaseDef: public I_SqlClause
{
public:

    // Nonvirtuals
    C_ColumnBaseDef(const std::string &name, C_LexPtr &type);
    void comment(const std::string &rem)    { m_comment = rem; }
    auto getTypeLocale() const              { return dynamic_cast<const C_LocaleMixin*>(m_type.get()); }
    auto &name() const                      { return m_colName; }
    std::optional<E_DataType> getShallowType() const;
    std::string typeStr(E_SqlServerType type, F_Str2Str t2s) const;

protected:

    // Data
    const std::string       m_colName;
    std::string             m_comment;
    C_LexPtr                m_type;
};
typedef std::shared_ptr<const C_ColumnBaseDef> C_ColumnBasePtr;

enum E_ColDefOrder
{
    CDO_DEFAULT_FIRST,
    CDO_ON_UPDATE_FIRST
};

class C_ColumnDefault: public I_SqlClause
{
public:

    // Nonvirtuals
    C_ColumnDefault(E_ColDefOrder order, C_LexPtr &first, C_LexPtr &second);
    auto first() const  { return m_first.get(); }
    auto second() const { return m_second.get(); }
    auto order() const  { return m_order; }
    bool timestampOnly() const;

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_first, m_second;
    E_ColDefOrder           m_order;
};

class C_ColumnDef: public C_ColumnBaseDef, public C_ReferenceMixin
{
public:

    // Nonvirtuals - Ctor
    C_ColumnDef(const std::string &name, C_LexPtr &type);

    // Nonvirtuals - Getters
    bool hasAutoInc() const { return m_autoInc; }
    auto getDefault() const { return m_default.get(); }
    auto pua() const        { return m_pua; }
    auto &lastError() const { return m_errMsg; }

    // Nonvirtuals - Setters
    void autoInc(bool yes = true)               { m_autoInc = yes; }
    void setColumFormat(E_ColFormat format)     { m_colFormat = format; }
    bool setDefault(C_LexPtr &expr);
    void setKeyType(E_PrimaryUniqueAttr type)   { m_pua = type; }
    void setNullity(bool notNull)               { m_notNull = notNull; }

    // Implement C_ColumnBaseDef
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    std::shared_ptr<const C_ColumnDefault> m_default;
    std::string             m_errMsg;
    int                     m_notNull   {-1};   // -1: default, 0: null, 1: not null
    bool                    m_autoInc   {false};
    E_PrimaryUniqueAttr     m_pua       {PUA_NONE};
    E_ColFormat             m_colFormat {COLUMN_FORMAT_NONE};
};

enum E_GenColumnMode
{
    GCM_VIRTUAL,
    GCM_PERSISTENT
};

class C_GenColumnDef: public C_ColumnBaseDef
{
public:

    // Nonvirtuals
    C_GenColumnDef(const std::string &name, C_LexPtr &type, C_LexPtr &expr, E_GenColumnMode mode);
    void unique() { m_unique = true; }

    // Implement C_ColumnBaseDef
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_expr;
    const E_GenColumnMode   m_mode;
    bool                    m_unique;
};

class C_CheckDef: public I_SqlClause
{
public:

    // Nonvirtuals
    C_CheckDef(C_LexPtr &expr): m_expr(expr) {}

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_expr;
};

enum E_SqlConstraintKind
{
    SCK_INDEX,
    SCK_FULLTEXT,
    SCK_SPATIAL,
    SCK_PRIMARY_KEY,
    SCK_UNIQUE_KEY,
    SCK_FOREIGN_KEY
};

enum E_InexUsing
{
    USING_BTREE,
    USING_HASH,
    USING_RTREE
};

class C_ConstraintDef: public I_SqlClause, public C_ReferenceMixin
{
public:

    // Nonvirtuals
    C_ConstraintDef(E_SqlConstraintKind kind, const std::string *id, const C_IndexColumns &cols);
    auto &columns()                 { return m_cols; }
    auto &columns() const           { return m_cols; }
    auto &constraintName() const    { return m_constraintName; }
    auto kind() const               { return m_kind; }
    auto &indexName() const         { return m_id; }
    //------------------------------
    void constraintName(const std::string &s)   { m_constraintName = s; }
    bool indexType(E_InexUsing type);
    bool keyBlockSize(int i);
    bool parserName(const std::string &name);

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    std::string                 m_id, m_constraintName;
    C_IndexColumns              m_cols;
    E_SqlConstraintKind         m_kind;
    //-------------------------------------------------
    std::optional<E_InexUsing>  m_indexUsing;
    std::optional<int>          m_keyBlockSize;
    std::string                 m_parserName;

    // Nonvirtuals
    std::string constraintHeader() const;
};
typedef std::shared_ptr<const C_ConstraintDef> C_ConstraintPtr;

enum E_AlterItemType
{
    AIT_TABLE_OPTION,
    AIT_DROP_INDEX,
    AIT_DROP_PRIMARY_KEY,
    AIT_DROP_FOREIGN_KEY,
    AIT_DROP_COLUMN,
    AIT_MODIFY_COLUMN,
    AIT_ADD_COLUMN,
    AIT_ADD_CONSTRAINT
};

struct C_AlterItem
{
    // Data
    E_AlterItemType         m_type;
    bool                    m_addAutoInc{false};
    std::string             m_pos;
    std::variant<std::string, C_ConstraintPtr, C_ColumnBasePtr> m_alteree;

    // Nonvirtuals
    C_AlterItem(E_AlterItemType type):
        m_type(type) {}
    template<class T>
    C_AlterItem(E_AlterItemType type, T &&t):
        m_type(type), m_alteree(std::forward<T>(t)) {}
    template<class T>
    C_AlterItem(E_AlterItemType type, T &&t, const std::string &pos):
        m_type(type), m_pos(pos), m_alteree(std::forward<T>(t)) {}
    bool hasPos() const { return !m_pos.empty(); }
    std::string str(E_SqlServerType type, F_Str2Str t2s) const;
};
typedef std::vector<C_AlterItem> C_AlterItems;

enum E_SpecialColumnChange
{
    SCC_DATA_TYPE
};

class C_CreateTable: public I_SqlCmd, public C_LocaleMixin, public C_FlagsMixin
{
public:

    // Nonvirtuals
    C_CreateTable(C_LexPtr &tbl_name, const C_SqlClauseQ &defs);
    bool autoInc(int lastUsed);
    void comment(const std::string &rem);
    bool engine(const std::string &id);
    auto tableName(F_Str2Str t2s) const  { return m_tableName.str(t2s); }
    void removeForeignKeys();
    void removeColumns(std::function<bool(const std::string &)> match);
    // -------------------------------------------------------------------
    C_AlterItems compare(const C_CreateTable &other, E_SqlServerType serverType,
        std::function<void(E_SpecialColumnChange chageType, const std::string &colName)> changed) const;
    void eachDef(std::function<void(const C_SqlClausePtr &def)> apply) const;
    C_StrQ getKeyColumnNames(E_PrimaryUniqueAttr keyType) const;
    std::string toNewTable(E_SqlServerType type, std::vector<const C_ConstraintDef*> *indices = nullptr) const;

    // Implement I_SqlCmd
    C_StrQ to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const C_TableName       m_tableName;
    C_SqlClauseQ            m_defs;
    std::string             m_engine, m_comment;
    std::optional<int>      m_autoInc;
};

class C_DataTypeLex: public I_SqlClause, public C_FlagsMixin
{
public:

    // Nonvirtuals
    C_DataTypeLex(E_DataType type, int w1 = -1, int w2 = -1);
    auto type() const { return m_type; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const E_DataType        m_type;
    const int               m_w1, m_w2;
};

class C_LocaleClauseLex: public I_SqlClause, public C_LocaleMixin
{
public:

    // Nonvirtuals
    C_LocaleClauseLex(C_LexPtr &innerClause);

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                const m_innerClause;
};

enum E_ListType
{
    LT_ENUM,
    LT_SET
};

class C_ListTypeLex: public I_SqlClause
{
public:

    // Nonvirtuals
    C_ListTypeLex(E_ListType type, const C_StrQ &list);

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const E_ListType        m_type;
    const C_StrQ            m_list;
};

struct C_Null: I_SqlClause
{
    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;
};

enum E_IdKind
{
    IK_UNDER_TABLE,
    IK_TABLEWISE
};

class C_Id: public I_SqlClause
{
public:

    // Ctors
    C_Id() = delete;
    C_Id(const std::string &id, E_IdKind kind);
    C_Id(const char *id, E_IdKind kind);
    template<class T>
    C_Id(T &&t, E_IdKind kind): m_ids(std::forward<T>(t)), m_kind(kind) {}

    // Nonvirtuals
    auto &list() const { return m_ids; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_StrQ                  m_ids;
    const E_IdKind          m_kind;
};

class C_Parentheses: public I_SqlClause
{
public:

    // Ctors
    C_Parentheses(C_LexPtr &expr): m_expr(expr), m_exists(false) {}
    template<class T>
    C_Parentheses(std::shared_ptr<T> &expr): m_expr(expr), m_exists(false) {}
    auto &expr() const { return m_expr; }

    // Nonvirtuals
    void exists() { m_exists = true; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_expr;
    bool                    m_exists;
};

class C_UnaryOp: public I_SqlClause
{
public:

    // Ctors
    C_UnaryOp(const char *op, C_LexPtr &expr): m_expr(expr), m_operator(op), m_postfix(false) {}
    C_UnaryOp(C_LexPtr &expr, const char *op): m_expr(expr), m_operator(op), m_postfix(true) {}
    template<class T>
    C_UnaryOp(const char *op, std::shared_ptr<T> &expr): m_expr(expr), m_operator(op), m_postfix(false) {}
    template<class T>
    C_UnaryOp(std::shared_ptr<T> &expr, const char *op): m_expr(expr), m_operator(op), m_postfix(true) {}

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_expr;
    const char              *const m_operator;
    const bool              m_postfix;
};

class C_AliasedExpr: public I_SqlClause
{
public:

    // Ctors
    C_AliasedExpr(C_LexPtr &expr, const std::string &alias, bool useAS): m_expr(expr), m_alias(alias), m_useAS(useAS) {}
    template<class T>
    C_AliasedExpr(std::shared_ptr<T> &expr, const std::string &alias, bool useAS): m_expr(expr), m_alias(alias), m_useAS(useAS) {}
    auto &expr() const { return m_expr; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_expr;
    const std::string       m_alias;
    const bool              m_useAS;
};

class C_BinaryOp: public I_SqlClause
{
public:

    // Ctors
    C_BinaryOp() = delete;
    template<class T1, class T2>
    C_BinaryOp(T1 &&left, const char *op, T2 &&right):
        m_operator(op),
        m_left(std::forward<T1>(left)),
        m_right(std::forward<T2>(right)) {}

    // Nonvirtuals
    template<class T>
    void escape(T &&e)  { m_escape = std::forward<T>(e); }
    void negate()       { m_not = true; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const char              *const m_operator;
    C_LexPtr                m_left, m_right, m_escape;
    bool                    m_not{};
};

typedef std::list<C_LexPtr> C_LexList;

#ifdef __GNUC__
template<class T1, class T2>
struct C_MyPair
{
    T1 first;
    T2 second;

    template<class T3, class T4>
    C_MyPair(T3 &&t3, T4 &&t4): first(t3), second(t4) {}
};
#else
template<class T1, class T2>
using C_MyPair = std::pair<T1, T2>;
#endif
typedef std::list<C_MyPair<C_LexPtr, C_LexPtr>> C_LexPairList;

class C_ExprCase: public I_SqlClause
{
public:

    // Nonvirtuals
    C_ExprCase(C_LexPtr &caseValue, const C_LexPairList &whenList, C_LexPtr &elseExpr);

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_caseValue, m_elseExpr;
    C_LexPairList           m_whens;
};

class C_FunctionCall: public I_SqlClause
{
public:

    // Nonvirtuals
    C_FunctionCall(const std::string &name);
    C_FunctionCall(const std::string &name, const C_LexList &params);
    auto &name() const { return m_name; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const std::string       m_name;
    C_LexList               m_params;
    bool                    m_distinct;
};

class C_QueryPlan: public I_SqlClause
{
public:

    // Nonvirtuals
    C_QueryPlan(const char *verb, const C_StrQ &ids);
    void target(const char *op) { m_target = op; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const char              *const m_verb, *m_target;
    C_StrQ                  m_ids;
};

class C_GroupBy: public I_SqlClause
{
public:

    // Nonvirtuals
    C_GroupBy(const C_LexList &orderList);
    void having(C_LexPtr &cond) { m_havingCond = cond; }
    void withRollUp() { m_withRollUp = true; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexList               m_orderList;
    C_LexPtr                m_havingCond;
    bool                    m_withRollUp;
};

class C_Limit: public I_SqlClause
{
public:

    // Nonvirtuals
    C_Limit(int n): m_n1(n), m_justOne(true) {}
    C_Limit(int n1, int n2, bool offsetKeyword): m_n1(n1), m_n2(n2), m_justOne(false), m_offsetKeyword(offsetKeyword) {}

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const int               m_n1;
    int                     m_n2;
    const bool              m_justOne;
    bool                    m_offsetKeyword;
};

class C_From: public I_SqlClause
{
public:

    // Nonvirtuals - Ctor
    C_From(const C_LexList &tableRefs);

    // Nonvirtuals - Setters
    void groupBy(C_LexPtr &cond);
    void limit(C_LexPtr &i);
    void orderBy(const C_LexList &list)         { m_orderByList = list; }
    void partition(const C_StrQ &ids)           { m_partitionList = ids; }
    void procedure(C_LexPtr &proc);
    void queryPlan(C_LexPtr &plan);
    void selectLockType(const char *lockType)   { m_lockType = lockType; }
    void where(C_LexPtr &cond)                  { m_whereCond = cond; }

    // Nonvirtuals - Getters
    auto &tables() const    { return m_tableRefs; }
    auto where() const      { return m_whereCond.get(); }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexList               m_tableRefs;
    C_LexList               m_orderByList;
    C_StrQ                  m_partitionList;
    std::shared_ptr<const C_QueryPlan> m_queryPlan;
    std::shared_ptr<const C_GroupBy> m_groupBy;
    std::shared_ptr<const C_Limit> m_limit;
    std::shared_ptr<const C_FunctionCall> m_procedure;
    C_LexPtr                m_whereCond;
    const char              *m_lockType;
};

class C_Select: public I_SqlClause, public C_FlagsMixin
{
public:

    // Nonvirtuals
    C_Select(const C_LexList &columns);
    void from(C_LexPtr &fromPhrase);
    auto from() const { return m_from.get(); }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexList               m_columns;
    std::shared_ptr<const C_From> m_from;
};

class C_OnCond: public I_SqlClause
{
public:

    // Nonvirtuals
    C_OnCond(C_LexPtr &cond): m_cond(cond) {}

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_LexPtr                m_cond;
};

class C_UsingCond: public I_SqlClause
{
public:

    // Nonvirtuals
    C_UsingCond(const C_StrQ &ids): m_ids(ids) {}

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_StrQ                  m_ids;
};

class C_JoinExpr: public I_SqlClause
{
public:

    // Nonvirtuals
    C_JoinExpr(C_LexPtr &left, const std::string &joinOp, C_LexPtr &right, C_LexPtr &joinCond);
    auto &left() const      { return m_left; }
    auto &right() const     { return m_right; }

    // Implement I_SqlClause
    std::string to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_SqlClausePtr          m_left, m_right, m_cond;
    const std::string       m_joinOp;
};

class C_ClauseAsCmd: public I_SqlCmd
{
public:

    // Nonvirtuals
    C_ClauseAsCmd(C_LexPtr &clause);
    template<class T> operator T*() const  { return dynamic_cast<T*>(m_clause.get()); }

    // Implement I_SqlCmd
    C_StrQ to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    C_SqlClausePtr       m_clause;
};

enum E_Algorithm
{
    ALGORITHM_NONE,
    ALGORITHM_UNDEFINED,
    ALGORITHM_MERGE,
    ALGORITHM_TEMPTABLE
};

enum E_SqlSecurity
{
    SQL_SECURITY_NONE,
    SQL_SECURITY_DEFINER,
    SQL_SECURITY_INVOKER
};

enum E_CheckOption
{
    CHKOPT_NONE,
    CHKOPT_PLAIN,
    CHKOPT_CASCADED,
    CHKOPT_LOCAL
};

class C_CreateView: public I_SqlCmd, public C_FlagsMixin
{
public:

    // Nonvirtuals
    C_CreateView(C_LexPtr &tbl_name, C_LexPtr &select);
    void algorithm(E_Algorithm a)           { m_algo = a; }
    void checkOption(E_CheckOption opt)     { m_checkOpt = opt; }
    void columnList(const C_StrQ &names)    { m_columnNames = names; }
    void definer(C_LexPtr &er)              { m_definer = er; }
    void security(E_SqlSecurity ss)         { m_security = ss; }
    auto &select() const                    { return *m_select; }
    std::string toNewView(E_SqlServerType type) const;

    // Implement I_SqlCmd
    C_StrQ to(E_SqlServerType type, F_Str2Str t2s) const override;

private:

    // Data
    const C_TableName           m_tableName;
    std::shared_ptr<const C_Select> m_select;
    C_LexPtr                    m_definer;
    C_StrQ                      m_columnNames;
    E_Algorithm                 m_algo;
    E_SqlSecurity               m_security;
    E_CheckOption               m_checkOpt;
};

//
//      Functions
//
std::string bareID(const std::string &id);
const char *typeName(E_DataType type);
bool isTimestamp(const bux::I_LexAttr *lex);
