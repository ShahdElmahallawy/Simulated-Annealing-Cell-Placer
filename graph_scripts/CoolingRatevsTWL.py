import matplotlib.pyplot as plt

def plot_twl_vs_cooling_rate(cooling_rates, file_names):
    plt.figure(figsize=(10, 6))

    final_twls = []
    for rate, file_name in zip(cooling_rates, file_names):
        with open(file_name, 'r') as file:
            twls = [float(line.split()[1]) for line in file]
        final_twls.append(twls[-1])  # Assuming the last TWL value is the final one

    plt.plot(cooling_rates, final_twls, marker='o')
    
    plt.xlabel('Cooling Rate')
    plt.ylabel('Final Total Wire Length (TWL)')
    plt.title('Final TWL vs. Cooling Rate')
    plt.grid(True)
    plt.savefig('./graphs/CoolingRatevsTWL/d0.png')
    plt.show()

cooling_rates = [0.75, 0.80, 0.85, 0.90, 0.95]
file_names = [f'./log/d0_annealing_log_{rate}.txt' for rate in cooling_rates]
plot_twl_vs_cooling_rate(cooling_rates, file_names)
