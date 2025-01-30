import csv
import time
import firebase_admin
from firebase_admin import credentials, db

# Firebase initialization
cred = credentials.Certificate("config.json")  # Replace with your key file
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-glove-f2b81-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Replace with your Firebase URL
})

ref = db.reference("devices/3c6FsvRdvIaObUmMh4byYeBbAIU2/reading")  # Replace with the correct Firebase path

# Define the CSV file name and header row
csv_file = 'new.csv'
header = ['little', 'ring', 'middle', 'pointer', 'thumb', 'accel_x', 'accel_y', 'accel_z', 'result']

# Initialize CSV file with header if it doesn't exist
try:
    with open(csv_file, mode='x', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(header)
except FileExistsError:
    pass

# Listen to the database changes
def on_data_change(event):
    data = event.data
    if data:
        # Extract data from Firebase
        little = data.get('little', 'N/A')
        ring = data.get('ring', 'N/A')
        middle = data.get('middle', 'N/A')
        pointer = data.get('pointer', 'N/A')
        thumb = data.get('thumb', 'N/A')
        accel_x = data.get('accel_x', 'N/A')
        accel_y = data.get('accel_y', 'N/A')
        accel_z = data.get('accel_z', 'N/A')
        
        # Add a hardcoded result value as a string
        result = "A"  # Replace "space" with the specific label you want to assign
        
        # Create a row with the data
        row = [little, ring, middle, pointer, thumb, accel_x, accel_y, accel_z, result]
        
        # Save the row to the CSV file
        if all(value != 'N/A' for value in row):  # Ensure all values are valid
            with open(csv_file, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow(row)
            print('Data saved to CSV:', row)

# Attach the listener
ref.listen(on_data_change)

print('Listening for data changes...')
# Keep the script running to listen for changes
while True:
    time.sleep(1)
