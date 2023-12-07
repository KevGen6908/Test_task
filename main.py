import matplotlib.pyplot as plt

data = []
with open('/home/kevgen/CLionProjects/Test_task/cmake-build-debug/simulation_result.txt', 'r') as file:
    for line in file:
        channel_error_prob, error_rate = map(float, line.split())
        data.append((channel_error_prob, error_rate))

channel_error_probabilities, error_rates = zip(*data)
plt.plot(error_rates, channel_error_probabilities)
plt.title('The number of successes \n versus the probability of an error in the channel')
plt.xlabel('The probability of an error in the channel')
plt.ylabel('the probability of an error in the output')
plt.grid(True)
plt.show()
