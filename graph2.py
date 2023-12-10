# import matplotlib.pyplot as plt

# def plot_twl(cooling_rates, file_names):
#     plt.figure(figsize=(10, 6))

#     for rate, file_name in zip(cooling_rates, file_names):
#         temperatures, twls = [], []
#         with open(file_name, 'r') as file:
#             for line in file:
#                 parts = line.split()
#                 temperatures.append(float(parts[0]))
#                 twls.append(float(parts[1]))
        
#         plt.plot(temperatures, twls, label=f'Cooling Rate {rate}')
#         plt.xlim(max(temperatures), min(temperatures))

#     plt.xlabel('Cooling Rate')
#     plt.ylabel('Total Wire Length (TWL)')
#     plt.title('TWL vs. Temperature for Different Cooling Rates')
#     plt.legend()
#     plt.grid(True)
#     plt.show()

# cooling_rates = [0.75, 0.80, 0.85, 0.90, 0.95]
# file_names = [f'out/t1_annealing_log_{rate}.txt' for rate in cooling_rates]
# plot_twl(cooling_rates, file_names)
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
    plt.show()

cooling_rates = [0.75, 0.80, 0.85, 0.90, 0.95]
file_names = [f'out\\t3_annealing_log_{rate}.txt' for rate in cooling_rates]
plot_twl_vs_cooling_rate(cooling_rates, file_names)
