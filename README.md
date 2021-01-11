# Brief
1. Prove that [`parsergen`](https://github.com/buck-yeh/parsergen) can generate SQL parser
2. The result parser only knows CREATE TABLE and SELECT statements in either MySQL or SQLite syntax, nothing else. *The attempt was based on retrieved table schema, SQL database as a whole can do version control. It didn't end well due to performance issues.*
3. [`reparse`](src/reparse) will take more than an hour to generate a GLR parser but then the corresponding scanner will follow in no time.
4. The parser is generated painfully but running very smoothly.