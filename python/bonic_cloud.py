from firebase_admin import credentials, initialize_app, db

def init():
    # Set the credentials to access Firebase
    
    try:
        cred = credentials.Certificate('config.json')
        initialize_app(cred, {
            'databaseURL': 'https://ai-based-smart-energy-meter-default-rtdb.firebaseio.com'
        })
        print("Bonic Cloud initialized successfully")
    except Exception as e:
        print(f"Bonic Cloud initializing Error: {e}")


def get_ref():
   return db.reference("/devices/i6v29xWLkNNXWfGjta1jh3z336j2/predictions/")


def get_data_ref():
    return db.reference("/devices/i6v29xWLkNNXWfGjta1jh3z336j2/predictions_result")

def reset():
    return db.reference("/devices/i6v29xWLkNNXWfGjta1jh3z336j2/data")

def reading():
    return db.reference("/devices/i6v29xWLkNNXWfGjta1jh3z336j2/reading")
# def get_data_ref():
#     return db.reference("/predictions/")
# def get_ref():
#    return db.reference("/predictions/")