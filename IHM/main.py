import launch_core
import graph
import user_input
b = {}

def main_next():
		while True:
			"""line = input(">>")
			if line == "exit":
				break
			else:
				key,value = line.split("=")
				a[key] = value
			launch_core.update_core()
			print(f"data_to_core = {a}\
				\ndata_from_core = {b}")"""
			try:
				line = launch_core.get_line()
				graph.show_one_plot(line[0])
			except launch_core.Stop_Exception as e:
				print("Stop Exception")
				break

try:
	graph.start_graph(f=main_next)
except KeyboardInterrupt:
	print("KeyboardInterrupt")
