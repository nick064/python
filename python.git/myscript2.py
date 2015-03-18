from sys import argv
script, variable3, variable4  = argv
def function1(variable1, variable2):

    print "This line is showing variable 1: %d" % variable1
    print "This line is showing variable 2: %d" % variable2
    print "This line is showing both variable 1 and 2: %d and %d" % (variable1, variable2)

print "The values for the function vars can be given directly:"
function1(300, 400)

print "Or we can define them separately:"
my_variable1 = 500
my_variable2 = 600
function1(my_variable1, my_variable2)

print "We can directly do calculations in the variables:"
function1(7 * 100, 8 * 100)

print "We can also ask the user for the variables:"
function1(int(raw_input("Please enter first var: ")), int(raw_input("Please enter second var: ")))

print "Or we can take them from the arguments! \nVar3: %s \nVar4: %s:" % (variable3, variable4)
