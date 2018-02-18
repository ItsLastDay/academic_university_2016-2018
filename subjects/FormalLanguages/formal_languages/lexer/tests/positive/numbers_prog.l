// Numbers in various forms; arithmetic and logical ops.
read x;
read y;

if (x > -10.0) && x < 1e5*   500
then write -.5
else write 0.000

if (y >= 15) && (y <= 60) || x    
    > -100.500e700
then write (x != y);
else write (x == y)


write 0
write -0
write 0.011005e+5
write 1e-9
write .1


write x + y;
write x+y;
write x+y123+4;
write x -     y
write x-y
write    x  *  y
write x*y
write y / x // comment
write y/x// comment
write y % x % 6
write y%x%6


write (((x + 1) * 5) - 6123) / 9;
