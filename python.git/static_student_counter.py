space_in_a_classroom = 20.0
teachers = 50
students_here = 900
students_per_teacher = students_here / teachers
students_per_classroom = students_here / space_in_a_classroom
people_per_room = students_per_classroom + 1
total = students_here + teachers

print "There are", students_here, "students here." 
print "There\'s", teachers , "here." 
print "That\'s" , total, "people today."
print "We have", space_in_a_classroom, "spaces per clasroom."
print "We have to put", students_per_classroom, "students in each room."
print "That\'s a total of" , people_per_room, "people per room." 