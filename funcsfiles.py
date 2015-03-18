from sys import argv #import argv

script, input_file = argv #take these from argv input

def print_all(f): #define function, print everything in file
    print f.read() #print what you've read from the file

def rewind(f): #define function with var f, rewind to first byte of file
    f.seek(0) #seek first byte of file

def print_a_line(line_count, f): #define function with line_count and f vars
    print line_count, f.readline() #print the number of the line and what's on that line

current_file = open(input_file) #open input file in argv

print "First let's print the whole file:\n" #say this

print_all(current_file) #print everything in current_file

print "Now let's rewind." #say this

rewind(current_file) #go back to beginning first byte of current_file

print "Let's print three lines:" #say this

current_line = 1 #current_line var needed, setting to 1
print_a_line(current_line, current_file) #print line 1 of the current file

current_line = current_line + 1 #adding +1 for the next line
print_a_line(current_line, current_file) #print the next line of current file

current_line = current_line + 1 #same as above
print_a_line(current_line, current_file)#same as above