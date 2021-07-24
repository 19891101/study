BEGIN{
pi = 3.1415926
printf "    sin(30): %.4f\n", sin(pi / 6)
printf "    cos(30): %.4f\n", cos(pi / 6)
printf "     exp(2): %.4f\n", exp(2)
printf "    int(pi): %.4f\n", int(pi)
printf "log(exp(2)): %.4f\n", log(exp(2))
printf "     rand(): %.4f\n", rand()
printf "     rand(): %.4f\n", rand()
printf "    sqrt(9): %.4f\n", sqrt(9)
printf "   and(5,6): %d\n", and(5, 6)
printf "    or(5,6): %d\n", or(5, 6)
printf "   xor(5,6): %d\n", xor(5, 6)
printf "lshift(5,2): %d\n", lshift(5,2)
printf "rshift(5,2): %d\n", rshift(5,2)
}
