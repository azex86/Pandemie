from ast import Tuple
import subprocess
import threading
import collections
import os




print(f"os = {os.name}")
CORE_PATH = "./Pandemie.bin" if os.name == "posix" else '../x64/Release/Pandemie.exe'




class Stop_Exception(Exception):
	pass

def get_line()->Tuple(dict,str):
	"""
	Return a line of data received from the C++ core
	"""

	line = input().strip()
	if line == "" or line =="exit":
		raise Stop_Exception("Core has exited")

	#print(f"sortie du core : {line}")
	#line is formated as key=value;key=value;....
	try:
		new_data_line = {}
		data = line.split(";")
		for d in data:
			if d=='':
				continue
			key,value = d.split("=")
			new_data_line[key] = value
		return (new_data_line,line)
	except Exception as e:
		print(f"erreur : {e}")

