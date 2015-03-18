from sys import argv
script, user_name, first, second = argv
prompt = '>> '

print "Hi %s, I'm the %s script " % (user_name, script)
print "I'd like to ask you a few questions, since you set the %s argument." % first

print "Do you like music, %s?" % user_name
likes = raw_input(prompt)

print "Where do you live %s?" % user_name
lives = raw_input(prompt)

print "What operating system are you running? I hear %s sucks." % second
computer = raw_input(prompt)

print """
Alright, so you said %s about liking music.
You live in %s Your operating system is %s!
""" %(likes, lives, computer)