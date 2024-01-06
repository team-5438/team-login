# Login
This is a simple tool to allow people to "login".

The program needs a csv file to read from/output to. This can be found at the
top of login.c as a macro. The default value is "timesheet.csv", and should
contain something like this:
```csv
id#,name,total days in,day in...
1234,John Doe,0
```
where every new line contains a new person.

The program takes user input from stdin, (by default it rejects any chars as we
have numerical id numbers). Every column past the days in is filled with the
date and time that the person punched in.
## BUILDING
To build you just need a c compiler, and make. Then you can run `make` to
build to the local directory. To install the program system wide run
`sudo make install`.
## TODO
1. parse info from csv to calculate averages per day
