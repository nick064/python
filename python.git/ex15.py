from sys import argv
script, filename = argv
txt = open(filename)
print "Here's your file %r: " % filename
print txt.read()

print "Type the filename again:"
file_again = raw_input("> ")
txt_again = open(file_again)
print txt_again.read()
txt.close()
txt_again.close()

print "Would you like to wipe a file? If yes specify, if not escape with CTRL+D"
file_to_wipe = raw_input("WARNING! FILE WILL BE WIPED:")
wipe = open(file_to_wipe, 'w')

print "Wiping the file!"
wipe.truncate()

print "File wiped! Displaying contents below %r " % file_to_wipe
wipe.close()
