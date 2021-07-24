BEGIN{
date = systime()
day = strftime("%A, %b %d, %Y", date)
print day
tp = mktime("2021 03 27 12 58 30")
print tp
print strftime("%A, %b %d, %Y", tp)
}
