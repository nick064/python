print "You enter a dark room with two doors. Do you go through door #1, #2 or #3?"

door = raw_input("> ")

if door == "1":
    print "There is a giant bear here eating a cheese cake. What do you do"
    print "1. Take the cake"
    print "2. Scream at the bear"

    bear = raw_input("> ")

    if bear == "1":
	print "You take the cake, the bear chases and eats your face off. Gj."
    elif bear == "2":
	print "You scream at the bear, it gets scared and eats your legs off. Gj."
    else:
	print "Well, doing %s is probably better. Bear runs away." % bear

elif door == "2":
    print "You go through door 2 and see a spider rolling a joint. What do you do?"
    print "1. Hit it with your mighty hammer"
    print "2. Sit around and smoke up"
    print "3. Call the police"

    action = raw_input("> ")

    if action == "1" or action == "3":
	print "The spider gets pissed off and entangles you in the corner. GJ."
    else:
	print "You smoke that shit up and talk to the spider. He's a pretty cool guy, his name is Nafarfori"

elif door == "3":
    print "You go through door three and realize that multiple elif's would be a stupid question"

else:
    print "Well awesome, go ahead and restart this."