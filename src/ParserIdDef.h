// This file is fully generated by running parsergen v1.6.1
// with grammer definition file "grammar.txt"

#ifndef ParserIdDef_H
#define ParserIdDef_H

#include <bux/LexBase.h>    // bux::TOKENGEN_LB

// Token Id Constants
enum
{
    TID_LEX_Error                   = bux::TOKENGEN_LB+0,	// $Error
    TID_LEX_Id                      = bux::TOKENGEN_LB+1,	// $Id
    TID_LEX_Num                     = bux::TOKENGEN_LB+2,	// $Num
    TID_LEX_Spaces                  = bux::TOKENGEN_LB+3,	// $Spaces
    TID_LEX_String                  = bux::TOKENGEN_LB+4,	// $String
    TID_KEYWORD_ACTION              = bux::TOKENGEN_LB+5,	// "ACTION"
    TID_KEYWORD_ALGORITHM           = bux::TOKENGEN_LB+6,	// "ALGORITHM"
    TID_KEYWORD_ALL                 = bux::TOKENGEN_LB+7,	// "ALL"
    TID_KEYWORD_ALWAYS              = bux::TOKENGEN_LB+8,	// "ALWAYS"
    TID_KEYWORD_AND                 = bux::TOKENGEN_LB+9,	// "AND"
    TID_KEYWORD_AS                  = bux::TOKENGEN_LB+10,	// "AS"
    TID_KEYWORD_ASC                 = bux::TOKENGEN_LB+11,	// "ASC"
    TID_KEYWORD_AUTOINCREMENT       = bux::TOKENGEN_LB+12,	// "AUTOINCREMENT"
    TID_KEYWORD_AUTO_INCREMENT      = bux::TOKENGEN_LB+13,	// "AUTO_INCREMENT"
    TID_KEYWORD_BETWEEN             = bux::TOKENGEN_LB+14,	// "BETWEEN"
    TID_KEYWORD_BIGINT              = bux::TOKENGEN_LB+15,	// "BIGINT"
    TID_KEYWORD_BINARY              = bux::TOKENGEN_LB+16,	// "BINARY"
    TID_KEYWORD_BIT                 = bux::TOKENGEN_LB+17,	// "BIT"
    TID_KEYWORD_BLOB                = bux::TOKENGEN_LB+18,	// "BLOB"
    TID_KEYWORD_BOOL                = bux::TOKENGEN_LB+19,	// "BOOL"
    TID_KEYWORD_BOOLEAN             = bux::TOKENGEN_LB+20,	// "BOOLEAN"
    TID_KEYWORD_BTREE               = bux::TOKENGEN_LB+21,	// "BTREE"
    TID_KEYWORD_BY                  = bux::TOKENGEN_LB+22,	// "BY"
    TID_KEYWORD_BYTE                = bux::TOKENGEN_LB+23,	// "BYTE"
    TID_KEYWORD_CASCADE             = bux::TOKENGEN_LB+24,	// "CASCADE"
    TID_KEYWORD_CASCADED            = bux::TOKENGEN_LB+25,	// "CASCADED"
    TID_KEYWORD_CASE                = bux::TOKENGEN_LB+26,	// "CASE"
    TID_KEYWORD_CHAR                = bux::TOKENGEN_LB+27,	// "CHAR"
    TID_KEYWORD_CHARACTER           = bux::TOKENGEN_LB+28,	// "CHARACTER"
    TID_KEYWORD_CHARSET             = bux::TOKENGEN_LB+29,	// "CHARSET"
    TID_KEYWORD_CHECK               = bux::TOKENGEN_LB+30,	// "CHECK"
    TID_KEYWORD_COLLATE             = bux::TOKENGEN_LB+31,	// "COLLATE"
    TID_KEYWORD_COLUMN_FORMAT       = bux::TOKENGEN_LB+32,	// "COLUMN_FORMAT"
    TID_KEYWORD_COMMENT             = bux::TOKENGEN_LB+33,	// "COMMENT"
    TID_KEYWORD_CONSTRAINT          = bux::TOKENGEN_LB+34,	// "CONSTRAINT"
    TID_KEYWORD_CREATE              = bux::TOKENGEN_LB+35,	// "CREATE"
    TID_KEYWORD_CROSS               = bux::TOKENGEN_LB+36,	// "CROSS"
    TID_KEYWORD_CURRENT_USER        = bux::TOKENGEN_LB+37,	// "CURRENT_USER"
    TID_KEYWORD_DATABASE            = bux::TOKENGEN_LB+38,	// "DATABASE"
    TID_KEYWORD_DATE                = bux::TOKENGEN_LB+39,	// "DATE"
    TID_KEYWORD_DATETIME            = bux::TOKENGEN_LB+40,	// "DATETIME"
    TID_KEYWORD_DEC                 = bux::TOKENGEN_LB+41,	// "DEC"
    TID_KEYWORD_DECIMAL             = bux::TOKENGEN_LB+42,	// "DECIMAL"
    TID_KEYWORD_DEFAULT             = bux::TOKENGEN_LB+43,	// "DEFAULT"
    TID_KEYWORD_DEFINER             = bux::TOKENGEN_LB+44,	// "DEFINER"
    TID_KEYWORD_DELETE              = bux::TOKENGEN_LB+45,	// "DELETE"
    TID_KEYWORD_DESC                = bux::TOKENGEN_LB+46,	// "DESC"
    TID_KEYWORD_DISTINCT            = bux::TOKENGEN_LB+47,	// "DISTINCT"
    TID_KEYWORD_DISTINCTROW         = bux::TOKENGEN_LB+48,	// "DISTINCTROW"
    TID_KEYWORD_DIV                 = bux::TOKENGEN_LB+49,	// "DIV"
    TID_KEYWORD_DOUBLE              = bux::TOKENGEN_LB+50,	// "DOUBLE"
    TID_KEYWORD_DYNAMIC             = bux::TOKENGEN_LB+51,	// "DYNAMIC"
    TID_KEYWORD_ELSE                = bux::TOKENGEN_LB+52,	// "ELSE"
    TID_KEYWORD_END                 = bux::TOKENGEN_LB+53,	// "END"
    TID_KEYWORD_ENGINE              = bux::TOKENGEN_LB+54,	// "ENGINE"
    TID_KEYWORD_ENUM                = bux::TOKENGEN_LB+55,	// "ENUM"
    TID_KEYWORD_ESCAPE              = bux::TOKENGEN_LB+56,	// "ESCAPE"
    TID_KEYWORD_EXISTS              = bux::TOKENGEN_LB+57,	// "EXISTS"
    TID_KEYWORD_FIXED               = bux::TOKENGEN_LB+58,	// "FIXED"
    TID_KEYWORD_FLOAT               = bux::TOKENGEN_LB+59,	// "FLOAT"
    TID_KEYWORD_FOR                 = bux::TOKENGEN_LB+60,	// "FOR"
    TID_KEYWORD_FORCE               = bux::TOKENGEN_LB+61,	// "FORCE"
    TID_KEYWORD_FOREIGN             = bux::TOKENGEN_LB+62,	// "FOREIGN"
    TID_KEYWORD_FROM                = bux::TOKENGEN_LB+63,	// "FROM"
    TID_KEYWORD_FULL                = bux::TOKENGEN_LB+64,	// "FULL"
    TID_KEYWORD_FULLTEXT            = bux::TOKENGEN_LB+65,	// "FULLTEXT"
    TID_KEYWORD_GENERATED           = bux::TOKENGEN_LB+66,	// "GENERATED"
    TID_KEYWORD_GEOMETRY            = bux::TOKENGEN_LB+67,	// "GEOMETRY"
    TID_KEYWORD_GEOMETRYCOLLECTION  = bux::TOKENGEN_LB+68,	// "GEOMETRYCOLLECTION"
    TID_KEYWORD_GLOB                = bux::TOKENGEN_LB+69,	// "GLOB"
    TID_KEYWORD_GROUP               = bux::TOKENGEN_LB+70,	// "GROUP"
    TID_KEYWORD_HASH                = bux::TOKENGEN_LB+71,	// "HASH"
    TID_KEYWORD_HAVING              = bux::TOKENGEN_LB+72,	// "HAVING"
    TID_KEYWORD_HIGH_PRIORITY       = bux::TOKENGEN_LB+73,	// "HIGH_PRIORITY"
    TID_KEYWORD_IF                  = bux::TOKENGEN_LB+74,	// "IF"
    TID_KEYWORD_IGNORE              = bux::TOKENGEN_LB+75,	// "IGNORE"
    TID_KEYWORD_IN                  = bux::TOKENGEN_LB+76,	// "IN"
    TID_KEYWORD_INDEX               = bux::TOKENGEN_LB+77,	// "INDEX"
    TID_KEYWORD_INNER               = bux::TOKENGEN_LB+78,	// "INNER"
    TID_KEYWORD_INSERT              = bux::TOKENGEN_LB+79,	// "INSERT"
    TID_KEYWORD_INT                 = bux::TOKENGEN_LB+80,	// "INT"
    TID_KEYWORD_INTEGER             = bux::TOKENGEN_LB+81,	// "INTEGER"
    TID_KEYWORD_INTO                = bux::TOKENGEN_LB+82,	// "INTO"
    TID_KEYWORD_INVOKER             = bux::TOKENGEN_LB+83,	// "INVOKER"
    TID_KEYWORD_IS                  = bux::TOKENGEN_LB+84,	// "IS"
    TID_KEYWORD_JOIN                = bux::TOKENGEN_LB+85,	// "JOIN"
    TID_KEYWORD_KEY                 = bux::TOKENGEN_LB+86,	// "KEY"
    TID_KEYWORD_KEY_BLOCK_SIZE      = bux::TOKENGEN_LB+87,	// "KEY_BLOCK_SIZE"
    TID_KEYWORD_LEFT                = bux::TOKENGEN_LB+88,	// "LEFT"
    TID_KEYWORD_LIKE                = bux::TOKENGEN_LB+89,	// "LIKE"
    TID_KEYWORD_LIMIT               = bux::TOKENGEN_LB+90,	// "LIMIT"
    TID_KEYWORD_LINESTRING          = bux::TOKENGEN_LB+91,	// "LINESTRING"
    TID_KEYWORD_LOCAL               = bux::TOKENGEN_LB+92,	// "LOCAL"
    TID_KEYWORD_LOCK                = bux::TOKENGEN_LB+93,	// "LOCK"
    TID_KEYWORD_LONGBLOB            = bux::TOKENGEN_LB+94,	// "LONGBLOB"
    TID_KEYWORD_LONGTEXT            = bux::TOKENGEN_LB+95,	// "LONGTEXT"
    TID_KEYWORD_MATCH               = bux::TOKENGEN_LB+96,	// "MATCH"
    TID_KEYWORD_MEDIUMBLOB          = bux::TOKENGEN_LB+97,	// "MEDIUMBLOB"
    TID_KEYWORD_MEDIUMINT           = bux::TOKENGEN_LB+98,	// "MEDIUMINT"
    TID_KEYWORD_MEDIUMTEXT          = bux::TOKENGEN_LB+99,	// "MEDIUMTEXT"
    TID_KEYWORD_MERGE               = bux::TOKENGEN_LB+100,	// "MERGE"
    TID_KEYWORD_MOD                 = bux::TOKENGEN_LB+101,	// "MOD"
    TID_KEYWORD_MODE                = bux::TOKENGEN_LB+102,	// "MODE"
    TID_KEYWORD_MULTILINESTRING     = bux::TOKENGEN_LB+103,	// "MULTILINESTRING"
    TID_KEYWORD_MULTIPOINT          = bux::TOKENGEN_LB+104,	// "MULTIPOINT"
    TID_KEYWORD_MULTIPOLYGON        = bux::TOKENGEN_LB+105,	// "MULTIPOLYGON"
    TID_KEYWORD_NATIONAL            = bux::TOKENGEN_LB+106,	// "NATIONAL"
    TID_KEYWORD_NATURAL             = bux::TOKENGEN_LB+107,	// "NATURAL"
    TID_KEYWORD_NCHAR               = bux::TOKENGEN_LB+108,	// "NCHAR"
    TID_KEYWORD_NO                  = bux::TOKENGEN_LB+109,	// "NO"
    TID_KEYWORD_NOT                 = bux::TOKENGEN_LB+110,	// "NOT"
    TID_KEYWORD_NULL                = bux::TOKENGEN_LB+111,	// "NULL"
    TID_KEYWORD_NUMERIC             = bux::TOKENGEN_LB+112,	// "NUMERIC"
    TID_KEYWORD_NVARCHAR            = bux::TOKENGEN_LB+113,	// "NVARCHAR"
    TID_KEYWORD_OFFSET              = bux::TOKENGEN_LB+114,	// "OFFSET"
    TID_KEYWORD_ON                  = bux::TOKENGEN_LB+115,	// "ON"
    TID_KEYWORD_OPTION              = bux::TOKENGEN_LB+116,	// "OPTION"
    TID_KEYWORD_OR                  = bux::TOKENGEN_LB+117,	// "OR"
    TID_KEYWORD_ORDER               = bux::TOKENGEN_LB+118,	// "ORDER"
    TID_KEYWORD_OUTER               = bux::TOKENGEN_LB+119,	// "OUTER"
    TID_KEYWORD_PARSER              = bux::TOKENGEN_LB+120,	// "PARSER"
    TID_KEYWORD_PARTIAL             = bux::TOKENGEN_LB+121,	// "PARTIAL"
    TID_KEYWORD_PARTITION           = bux::TOKENGEN_LB+122,	// "PARTITION"
    TID_KEYWORD_PERSISTENT          = bux::TOKENGEN_LB+123,	// "PERSISTENT"
    TID_KEYWORD_POINT               = bux::TOKENGEN_LB+124,	// "POINT"
    TID_KEYWORD_POLYGON             = bux::TOKENGEN_LB+125,	// "POLYGON"
    TID_KEYWORD_PRECISION           = bux::TOKENGEN_LB+126,	// "PRECISION"
    TID_KEYWORD_PRIMARY             = bux::TOKENGEN_LB+127,	// "PRIMARY"
    TID_KEYWORD_PROCEDURE           = bux::TOKENGEN_LB+128,	// "PROCEDURE"
    TID_KEYWORD_REAL                = bux::TOKENGEN_LB+129,	// "REAL"
    TID_KEYWORD_REFERENCES          = bux::TOKENGEN_LB+130,	// "REFERENCES"
    TID_KEYWORD_REGEXP              = bux::TOKENGEN_LB+131,	// "REGEXP"
    TID_KEYWORD_REPLACE             = bux::TOKENGEN_LB+132,	// "REPLACE"
    TID_KEYWORD_RESTRICT            = bux::TOKENGEN_LB+133,	// "RESTRICT"
    TID_KEYWORD_RIGHT               = bux::TOKENGEN_LB+134,	// "RIGHT"
    TID_KEYWORD_RLIKE               = bux::TOKENGEN_LB+135,	// "RLIKE"
    TID_KEYWORD_ROLLUP              = bux::TOKENGEN_LB+136,	// "ROLLUP"
    TID_KEYWORD_RTREE               = bux::TOKENGEN_LB+137,	// "RTREE"
    TID_KEYWORD_SCHEMA              = bux::TOKENGEN_LB+138,	// "SCHEMA"
    TID_KEYWORD_SECURITY            = bux::TOKENGEN_LB+139,	// "SECURITY"
    TID_KEYWORD_SELECT              = bux::TOKENGEN_LB+140,	// "SELECT"
    TID_KEYWORD_SET                 = bux::TOKENGEN_LB+141,	// "SET"
    TID_KEYWORD_SHARE               = bux::TOKENGEN_LB+142,	// "SHARE"
    TID_KEYWORD_SIMPLE              = bux::TOKENGEN_LB+143,	// "SIMPLE"
    TID_KEYWORD_SMALLINT            = bux::TOKENGEN_LB+144,	// "SMALLINT"
    TID_KEYWORD_SPATIAL             = bux::TOKENGEN_LB+145,	// "SPATIAL"
    TID_KEYWORD_SQL                 = bux::TOKENGEN_LB+146,	// "SQL"
    TID_KEYWORD_SQL_BIG_RESULT      = bux::TOKENGEN_LB+147,	// "SQL_BIG_RESULT"
    TID_KEYWORD_SQL_BUFFER_RESULT   = bux::TOKENGEN_LB+148,	// "SQL_BUFFER_RESULT"
    TID_KEYWORD_SQL_CACHE           = bux::TOKENGEN_LB+149,	// "SQL_CACHE"
    TID_KEYWORD_SQL_CALC_FOUND_ROWS = bux::TOKENGEN_LB+150,	// "SQL_CALC_FOUND_ROWS"
    TID_KEYWORD_SQL_NO_CACHE        = bux::TOKENGEN_LB+151,	// "SQL_NO_CACHE"
    TID_KEYWORD_SQL_SMALL_RESULT    = bux::TOKENGEN_LB+152,	// "SQL_SMALL_RESULT"
    TID_KEYWORD_STORAGE             = bux::TOKENGEN_LB+153,	// "STORAGE"
    TID_KEYWORD_STRAIGHT_JOIN       = bux::TOKENGEN_LB+154,	// "STRAIGHT_JOIN"
    TID_KEYWORD_TABLE               = bux::TOKENGEN_LB+155,	// "TABLE"
    TID_KEYWORD_TEMPORARY           = bux::TOKENGEN_LB+156,	// "TEMPORARY"
    TID_KEYWORD_TEMPTABLE           = bux::TOKENGEN_LB+157,	// "TEMPTABLE"
    TID_KEYWORD_TEXT                = bux::TOKENGEN_LB+158,	// "TEXT"
    TID_KEYWORD_THEN                = bux::TOKENGEN_LB+159,	// "THEN"
    TID_KEYWORD_TIME                = bux::TOKENGEN_LB+160,	// "TIME"
    TID_KEYWORD_TIMESTAMP           = bux::TOKENGEN_LB+161,	// "TIMESTAMP"
    TID_KEYWORD_TINYBLOB            = bux::TOKENGEN_LB+162,	// "TINYBLOB"
    TID_KEYWORD_TINYINT             = bux::TOKENGEN_LB+163,	// "TINYINT"
    TID_KEYWORD_TINYTEXT            = bux::TOKENGEN_LB+164,	// "TINYTEXT"
    TID_KEYWORD_UNDEFINED           = bux::TOKENGEN_LB+165,	// "UNDEFINED"
    TID_KEYWORD_UNIQUE              = bux::TOKENGEN_LB+166,	// "UNIQUE"
    TID_KEYWORD_UNSIGNED            = bux::TOKENGEN_LB+167,	// "UNSIGNED"
    TID_KEYWORD_UPDATE              = bux::TOKENGEN_LB+168,	// "UPDATE"
    TID_KEYWORD_USE                 = bux::TOKENGEN_LB+169,	// "USE"
    TID_KEYWORD_USING               = bux::TOKENGEN_LB+170,	// "USING"
    TID_KEYWORD_VARBINARY           = bux::TOKENGEN_LB+171,	// "VARBINARY"
    TID_KEYWORD_VARCHAR             = bux::TOKENGEN_LB+172,	// "VARCHAR"
    TID_KEYWORD_VARCHAR2            = bux::TOKENGEN_LB+173,	// "VARCHAR2"
    TID_KEYWORD_VARYING             = bux::TOKENGEN_LB+174,	// "VARYING"
    TID_KEYWORD_VIEW                = bux::TOKENGEN_LB+175,	// "VIEW"
    TID_KEYWORD_VIRTUAL             = bux::TOKENGEN_LB+176,	// "VIRTUAL"
    TID_KEYWORD_WHEN                = bux::TOKENGEN_LB+177,	// "WHEN"
    TID_KEYWORD_WHERE               = bux::TOKENGEN_LB+178,	// "WHERE"
    TID_KEYWORD_WITH                = bux::TOKENGEN_LB+179,	// "WITH"
    TID_KEYWORD_XOR                 = bux::TOKENGEN_LB+180,	// "XOR"
    TID_KEYWORD_YEAR                = bux::TOKENGEN_LB+181,	// "YEAR"
    TID_KEYWORD_ZEROFILL            = bux::TOKENGEN_LB+182,	// "ZEROFILL"
    TID_LITERAL_213d                = bux::TOKENGEN_LB+183,	// "!="
    TID_LITERAL_2626                = bux::TOKENGEN_LB+184,	// "&&"
    TID_LITERAL_2727                = bux::TOKENGEN_LB+185,	// "\'\'"
    TID_LITERAL_3c3c                = bux::TOKENGEN_LB+186,	// "<<"
    TID_LITERAL_3c3d                = bux::TOKENGEN_LB+187,	// "<="
    TID_LITERAL_3c3d3e              = bux::TOKENGEN_LB+188,	// "<=>"
    TID_LITERAL_3c3e                = bux::TOKENGEN_LB+189,	// "<>"
    TID_LITERAL_3d3d                = bux::TOKENGEN_LB+190,	// "=="
    TID_LITERAL_3e3d                = bux::TOKENGEN_LB+191,	// ">="
    TID_LITERAL_3e3e                = bux::TOKENGEN_LB+192,	// ">>"
    TID_LITERAL_602a60              = bux::TOKENGEN_LB+193,	// "`*`"
    TID_LITERAL_7c7c                = bux::TOKENGEN_LB+194,	// "||"
    TID_UB_
};

#endif // ParserIdDef_H
