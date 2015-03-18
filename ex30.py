people = raw_input('How many people: ')
cars = raw_input('How many cars: ')
trucks = raw_input ('How many trucks: ')

if cars > people:
    print "We should take the cars"
elif cars < people: 
    print "We should not take the cars"
else:
    print "We can't decide"

if trucks > cars:
    print "That's too many trucks."
elif trucks < cars: 
    print "Maybe we could take the trucks."
else:
    print "We still can't decide."

if people > trucks:
    print "Alright, let's just take the trucks."
else:
    print "Let's stay home then."

if people == cars:
    print "We've got a car for every person"
elif people != cars:
    print "Horata ne sa kolkoto kolite"
else:
    print "Opravqi se"

if cars > people or trucks < cars:
    print "Kolite sa poveche ot horata ili ot kamionite"
else: 
    print "Ne znam brat"