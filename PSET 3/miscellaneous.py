reps = []
for i in range(40):
	x = 5^i % 39
	if x in reps:
		print("found rep:",i,x)
	else:
		reps.append(x)
		print(i,x)