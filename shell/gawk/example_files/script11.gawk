function myprint(a, b, c) {
  printf "%10s %d %.5f\n", a, b, c 
}
{
  myprint($1, $2, $3)
}
