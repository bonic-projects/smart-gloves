import time
import firebase_admin
from firebase_admin import credentials, db

# Firebase initialization
cred = credentials.Certificate("config.json")  # Replace with your Firebase key file
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-glove-f2b81-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Replace with your Firebase URL
})

# Firebase reference for predictions
output_ref = db.reference("devices/3c6FsvRdvIaObUmMh4byYeBbAIU2/prediction")  # Replace with your path

# Simulated text and space gesture
text_to_send = "H O W space A R E space Y O U space W H A T space I S space Y O U R space N A M E space".split()  # Includes "space" as a gesture

def mimic_ml_model():
    """
    Mimics the behavior of an ML model by sending characters and space gestures one by one to Firebase.
    """
    while True:
        for char in text_to_send:
            print(f"Sending gesture: {char}")
            output_ref.set({"gesture": char})
            time.sleep(3)  # Wait 3 seconds between each gesture

# Start the simulation
if __name__ == "__main__":
    mimic_ml_model()
