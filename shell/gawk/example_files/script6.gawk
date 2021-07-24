{
total = 0
for (i = 1; i <= NF; ++i) {
  total += $i
}
avg = total / NF
print "average is:", total, "/", NF, "=", avg
}
