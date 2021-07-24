BEGIN {
print "The passwd info:"
FS=":"
}
{
text = "'s home is: "
print $1 text $6
}
END{print "Data process finished"}
