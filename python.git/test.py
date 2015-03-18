def getTwoNumbers():
    numberA = raw_input("Enter your first number: ")
    numberB = raw_input("Enter your second number: ")
    return int(numberA), int(numberB)
    print "The numbers were: %d %d" % (numberA, numberB)