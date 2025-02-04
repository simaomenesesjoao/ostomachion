import matplotlib.pyplot as plt
import matplotlib
import sys
import numpy as np
import subprocess
import re
import visualizer2 as vis


def get_cpp_state(indices):
    cpp_program = './interactive_state'  # Path to the compiled C++ program
    arguments = [str(index) for index in indices]

    # Run the C++ program with arguments using subprocess
    result = subprocess.run([cpp_program] + arguments, capture_output=True, text=True)
    states_list = vis.parse_states_string(result.stdout)
    return states_list




fig = plt.figure()
plt.subplots_adjust(wspace=0.1, hspace=0.1)
iteration = 0

def plot_states(states_list):
    global iteration
    iteration += 1
    print(indices)

    if len(states_list) != 1:
        sqrt = int(np.sqrt(len(states_list)))+1
    else:
        sqrt = 1
    print("len states:", len(states_list), "sqrt:", sqrt)
    
    axs = []
    for i in range(sqrt):
        for j in range(sqrt):
            ax = fig.add_subplot(sqrt, sqrt, i*sqrt+j+1)
            ax.axis('off')            
            axs.append(ax)
    
    for ax, state in zip(axs, states_list):
        vis.state_to_ax(state, ax)
        
        
            
    fig.canvas.draw()

    if iteration == 1:
        plt.show()

    
# Define the callback function to be run on click
def on_click(event):
    ax = event.inaxes  # The axis that was clicked
    if ax is not None:        
        subplot_number = ax.get_subplotspec().num1
        print("A", subplot_number)
        indices.append(subplot_number)
        plt.clf()
        states_list = get_cpp_state(indices)
        plot_states(states_list)

fig.canvas.mpl_connect('button_press_event', on_click)

indices = []
states_list = get_cpp_state([])
plot_states(states_list)