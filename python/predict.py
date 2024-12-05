import time
import pickle
import firebase_admin
from firebase_admin import credentials, db
import numpy as np

# Firebase initialization
cred = credentials.Certificate("config.json")  # Replace with your Firebase key file
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-glove-f2b81-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Replace with your Firebase URL
})

# Firebase reference for incoming data
input_ref = db.reference("devices/3c6FsvRdvIaObUmMh4byYeBbAIU2/reading")  # Replace with your path
# Firebase reference for predictions
output_ref = db.reference("devices/3c6FsvRdvIaObUmMh4byYeBbAIU2/prediction")  # Replace with your path

# Load the trained model
with open('sign_recognition_model.pkl', 'rb') as model_file:
    model = pickle.load(model_file)

# Load the label encoder
with open('label_encoder.pkl', 'rb') as encoder_file:
    label_encoder = pickle.load(encoder_file)

def preprocess_data(data):
    """
    Preprocess the input data for prediction.
    Converts values to float and reshapes them for model input.
    """
    features = [
        float(data.get('little', 0)),
        float(data.get('ring', 0)),
        float(data.get('middle', 0)),
        float(data.get('pointer', 0)),
        float(data.get('thumb', 0)),
        float(data.get('accel_x', 0)),
        float(data.get('accel_y', 0)),
        float(data.get('accel_z', 0))
    ]
    return np.array(features).reshape(1, -1)

def predict_gesture(data):
    """
    Predicts the gesture using the trained model and label encoder.
    """
    # Preprocess data for prediction
    processed_data = preprocess_data(data)
    prediction = model.predict(processed_data)
    predicted_label = label_encoder.inverse_transform(prediction)[0]
    return predicted_label

def on_data_change(event):
    """
    Callback function triggered when data changes in Firebase.
    """
    data = event.data
    if data:
        print("Incoming data:", data)
        try:
            # Predict the gesture
            prediction = predict_gesture(data)
            print(f"Predicted Gesture: {prediction}")
            
            # Push the prediction to Firebase
            output_ref.set({"gesture": prediction})
            print("Prediction pushed to Firebase.")
        except Exception as e:
            print("Error during prediction:", e)

# Attach the listener to the input node
input_ref.listen(on_data_change)

print('Listening for real-time data and predicting gestures...')
# Keep the script running
while True:
    time.sleep(1)
