import random 
import string 


length = 100
s = "q"
while length > 0 : 
	s += random.choice(string.ascii_letters + string.digits)
	s += " "
	s += '\n'
	length = length - 1
print(s)
