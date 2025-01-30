import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, accuracy_score
from sklearn.preprocessing import LabelEncoder
import pickle

# Load data from the CSV file
csv_file = 'flex_accel_data.csv'  #change the name of csv file
data = pd.read_csv(csv_file)

# Check for missing values
if data.isnull().values.any():
    print("Warning: Missing values found. Fill or remove them before training.")
    data = data.dropna()  # Alternatively, you can fill missing values if needed

# Separate features and target
X = data[['little', 'ring', 'middle', 'pointer', 'thumb', 'accel_x', 'accel_y', 'accel_z']]
y = data['result']

# Encode target labels to integers
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42)

# Train a Random Forest Classifier
print("Training the model...")
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Evaluate the model
print("Evaluating the model...")
y_pred = model.predict(X_test)
print("Classification Report:")
print(classification_report(y_test, y_pred, target_names=label_encoder.classes_))
print("Accuracy:", accuracy_score(y_test, y_pred))

# Save the model to a file
model_filename = 'new_model.pkl'
with open(model_filename, 'wb') as file:
    pickle.dump(model, file)
print(f"Model saved to {model_filename}")

# Save the label encoder to a file
encoder_filename = 'label_new_encoder.pkl'
with open(encoder_filename, 'wb') as file:
    pickle.dump(label_encoder, file)
print(f"Label encoder saved to {encoder_filename}")
