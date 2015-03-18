def add(a, b):
    print "Adding %d + %d" % (a, b)
    return a + b

def subtract(a, b):
    print "Subtracting %d - %d" % (a, b)
    return a - b

def multiply(a, b):
    print "Multiplying %d * %d" % (a, b)
    return a * b
1
def divide(a, b):
    print "Dividing %d / %d" % (a, b)
    return a / b
    
#def weird_func(a, b):
#    print "Adding, this will be printed through a var: %d + %d" % (a, b)
#    added = a + b
#    print "We get: " % added

print "Now lets do some math with just functions!" 

#This below is the fancy method#NOTE This needs reworking of the script, float()
#doesn accept two variables at once

#a, b = float(raw_input('Two numbers with a comma in between them: '))

#And this is the simple method

a = float(raw_input('Number A: '))
b = float(raw_input('Number B: '))

age = add(a, b)
height = subtract(a, b)
weight = multiply(a, b)
iq = divide (a, b)

print "Age: %d\nHeight: %d\nWeight: %d\nIQ: %d\n" % (age, height, weight, iq)

#Extra credit shit

print "Puzzle!" 

wut = add(age, subtract(height, multiply(weight, divide(iq, 2))))

print "That becomes: ", wut, "Can you do it by hand?"