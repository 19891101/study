{
total = 0
i = 1
while (i <= NF) {
  total += $i
  ++i
}
avg = total / NF
print "average is:", total, "/", NF, "=", avg
}
