import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the estimates from the output file
data = pd.read_csv('output.txt', header=None)
data.columns = ['Estimate']

# Get the minimum and maximum values from the data
min_value = data['Estimate'].min()
max_value = data['Estimate'].max()

# Define the number of bins; you can adjust this as needed
num_bins = 10  # You can change this to control the granularity of the histogram

# Create dynamic bins based on the data range
bins = np.linspace(min_value, max_value, num_bins)

# Plot histogram
plt.hist(data['Estimate'], bins=bins, edgecolor='black')
plt.title('Histogram of Estimates of C')
plt.xlabel('Estimated Bottleneck Speed (Mbps)')
plt.ylabel('Frequency')
plt.grid(axis='y')

# Set x-ticks to show the bin edges
plt.xticks(np.arange(min_value, max_value + 1, step=(max_value - min_value) / num_bins))

# Save the plot as a file
plt.savefig('histogram.png')
plt.close()  # Close the figure
