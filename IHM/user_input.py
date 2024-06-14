import threading
import launch_core
def start_user_input(data_to_core:dict):
	th = threading.Thread(target=user_input,args=(data_to_core,),daemon=True)
	th.start()
    
def user_input(data_to_core:dict):
	while True:
		line = input(">>")
		if line == "exit":
			break
		else:
			try:
				key,value = line.split("=")
				data_to_core[key] = value
				launch_core.update_core()
			except BaseException as e:
				print(e)
				break
	return
