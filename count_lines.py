import os;

farr = os.listdir(".");

lines_t = 0
empty_lines_t = 0
comment_lines_t = 0

for f in farr:
	lines = 0
	empty_lines = 0
	comment_lines = 0
	if (os.path.isfile(f) and f != "count_lines.py"):
		fin = open(f, "r")
		
		line = fin.readline()
		while line:
			if line.strip() == "":
				empty_lines += 1
			elif line.strip()[0:2] == "//":
				comment_lines += 1
			else:
				lines += 1
			line = fin.readline()
		
		print(f + " has " + str(lines) + " lines, " + str(comment_lines) + " comment-only lines, and " + str(empty_lines) + " empty lines.")
		lines_t += lines
		empty_lines_t += empty_lines
		comment_lines_t += comment_lines
		continue
		
		fin.close()

print("This library has " + str(lines_t) + " lines of code, " + str(comment_lines_t) + " comment-only lines, and " + str(empty_lines_t) + " empty lines for a total of " + str(lines_t + comment_lines_t + empty_lines_t) + " lines.")
