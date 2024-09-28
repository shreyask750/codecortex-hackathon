import sys
import pickle
import numpy as np

# Load the model from the pickle file
model = pickle.load(open('model.pkl', 'rb'))

# Get the input values from the command line arguments
ambient_temperature = float(sys.argv[1])
relative_humidity = float(sys.argv[2])
wet_bulb_temperature = float(sys.argv[3])
chilled_water_supply_temperature = float(sys.argv[4])
chilled_water_return_temperature = float(sys.argv[5])
plant_efficiency = float(sys.argv[6])
total_chiller_power = float(sys.argv[7])
flow = float(sys.argv[8])

# Create an input array for prediction
input_data = np.array([[flow, chilled_water_supply_temperature, chilled_water_return_temperature,
                        plant_efficiency, ambient_temperature, total_chiller_power,
                        relative_humidity, wet_bulb_temperature]])

# Make the prediction
prediction = model.predict(input_data)

# Print the prediction (this will be captured by Node.js)
print(prediction[0])
