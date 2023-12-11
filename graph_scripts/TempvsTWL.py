import matplotlib.pyplot as plt

def plot_graph(file_name, title):
    temps, wire_lengths = [], []
    
    with open(file_name, 'r') as file:
        for line in file:
            temp, length = map(float, line.split())
            temps.append(temp)
            wire_lengths.append(length)
    plt.plot(temps, wire_lengths, marker='o')
    plt.xlim(max(temps), min(temps))
    plt.title(title)
    plt.xlabel('Temperature')
    plt.ylabel('Total Wire Length')
    plt.savefig('./graphs/TempvsTWL/d0.png')
    plt.show()
# Example usage
plot_graph('./log/d0_annealing_log_0.95.txt', 'TWL vs Temp')
