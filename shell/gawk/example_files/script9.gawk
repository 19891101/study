BEGIN{
s="11, abc, 11, pqr"
printf "          gensub: %s\n", gensub("11", "22", 1, s)
printf "          gensub: %s\n", gensub("11", "22", 2, s)
printf "          gensub: %s\n", gensub("11", "22", "g", s)
printf "          gensub: %s\n", gensub("a[a-z]{2}", "xxx", "g", s)
printf "gsub-replace-num: %d\n", gsub("11", "22", s)
printf " replace-by-gsub: %s\n", s
s="11, abc, 11, pqr"
printf "           index: %d\n", index(s, "abc")
printf "           index: %d\n", index(s, "xxx")
printf "          length: %d\n", length(s)
printf "           match: %d\n", match("xx11,11", "11")
printf "           match: %d\n", match("xx11,11", "11", a)
for (x in a) {
  print x, " -> ", a[x]
}
printf "           split: %d\n", split("xx11,11,1323", b, ",")
for (x in b) {
  print x, " -> ", b[x]
}
printf "         sprintf: %s\n", sprintf("format data: [%s], %d", s, 123)
printf "          substr: %s\n", substr(s, 5, 3)
printf "         toupper: %s\n", toupper("abc")
printf "         tolower: %s\n", tolower("abcDEF")
}
