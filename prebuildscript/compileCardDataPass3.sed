#$!s/enum \(\w\+\) : int32_t$/SELECT * FROM \1 UNION/
#$s/enum \(\w\+\) : int32_t$/SELECT * FROM \1/;
s/^enum \(\w\+\)/SELECT * FROM \1/
$!s/ : int32_t$/ UNION/
$s/ : int32_t$/;/
