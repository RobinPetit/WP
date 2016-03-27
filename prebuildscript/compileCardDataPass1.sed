# comments
s://.*$::g

# trailing spaces
s/^[ \t]*//
s/[ \t]*$//

# enums
s/\s*\(=\s*0\s*\)*,\s*/),(/g
s/^{$/(/
s/^};$/);/
s/(/('/g
s/)/')/g
s/enum \(\w\+\) : int32_t$/CREATE TEMPORARY TABLE \1 (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);\
INSERT INTO \1(name) VALUES/g

#0,/^./{/./!d}
