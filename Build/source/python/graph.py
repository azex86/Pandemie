import threading
import matplotlib
import matplotlib.pyplot
import matplotlib.animation

ref_size = 0
data:list[list] = []#list of list of value
index_labels = [] #list of key
labels:dict = {} #dict of list of value for each key
ignore_keys = ["time","live","rules[infection_time]","rules[infection_range]","rules[infection_rate]","rules[immunity_time]","wait_time",
               "counter[Died]","rules[quarantine]","rules[confinement]","iteration_time","walls","rules[quarantine_time]"]


x_label = "iterations"
y_label = "value"
#mutex de proctection
verrou = threading.Lock()
change = True



def start_graph(f):       
    th = threading.Thread(target=f,daemon=True)
    th.start()
    start_thread_graph(end=th)
    
def start_thread_graph(end:threading.Thread):
    fig = matplotlib.pyplot.figure()
    graph = fig.add_subplot()
    def update(index):
        global change
        if not change:
            return graph
        
        global verrou, data, index_labels
        if (not data.__len__()>0) or (not data[0].__len__()>0):
            return graph
        verrou.acquire()
        graph.clear()
        

        for label in labels:
            if label != x_label:
                common_length = min(len(labels[x_label]), len(labels[label]))
                graph.plot(labels[x_label][:common_length], labels[label][:common_length],label=label)
        
        graph.set_xlabel(x_label)
        graph.set_ylabel(y_label)
        graph.legend()
        graph.set_xscale("linear")
        verrou.release()
        #Si end est terminé, on arrête le graph
        if not end.is_alive():
			#on arrête l'animation
            animation.event_source.stop()
        return graph
    animation = matplotlib.animation.FuncAnimation(fig,update,cache_frame_data=False,blit=False,interval=1000,repeat=True)
    matplotlib.pyplot.show()
    
def show_one_plot(line:dict):
    global ref_size, data, labels, change, verrou, index_labels
    verrou.acquire()
    
    for (key,val) in line.items():
        if key in ignore_keys:
            continue
        try:labels[key].append(float(val))
        except:
            labels[key] = [0 for _ in range(ref_size)]
            index_labels.append(key)
            data.append(labels[key])
            labels[key].append(float(val))
    ref_size+=1
    change = True
    verrou.release()