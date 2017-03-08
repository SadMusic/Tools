import os
import sys
import re

inputDir = "C:\\Users\\Administrator\\Downloads\\assets"
app = "C:\\Users\\Administrator\\Desktop\\ConsoleApplication\\Debug\\ConsoleApplication.exe"

def _decrypt(fileName):
	os.system('%s %s %s %s %s' % (app, 'xhyl', 'zxzx', fileName, fileName[0:-1]))

def decrypt(dir):
	for x in os.listdir(dir):
		cur = os.path.join(dir, x)
		if os.path.isdir(cur):
			decrypt(cur)
		elif cur.endswith("luac"):
			_decrypt(cur)

decrypt(inputDir)