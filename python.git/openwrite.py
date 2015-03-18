from sys import argv
script, filename = argv

print "We're going to erase %r. " % filename
print "Cancel with Control-C."
print "If you do want that, hit RETURN."

raw_input("?")

print "Opening the file..."
target = open(filename, 'w')

print "Truncating the file!"
target.truncate()

print "Please enter content to insert:"

line1 = raw_input("Line 1: ")
line2 = raw_input("Line 2: ")
line3 = raw_input("Line 3: ")

print "I'm going to write these to the file. Please confirm by hitting enter"

raw_input("> ")

target.write(line1)
target.write("\n")
target.write(line2)
target.write("\n")
target.write(line3)
target.write("\n")

print "Closing file!"
target.close()