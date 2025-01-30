import cv2

# Replace the below URL with your IP camera's URL or video stream URL.
# Example: "http://192.168.0.100:8080/video"
ip  = "192.168.29.240"
stream_url = "http://"+ ip +":81/stream"

# Open a connection to the video stream
cap = cv2.VideoCapture(stream_url)

if not cap.isOpened():
    print("Error: Unable to open video stream.")
    exit()

print("Press 'q' to quit the video stream.")

while True:
    # Read a frame from the video stream
    ret, frame = cap.read()

    # Check if the frame was successfully read
    if not ret:
        print("Error: Unable to fetch frame.")
        break

    # Display the frame
    cv2.imshow('Video Stream', frame)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the video capture object and close display window
cap.release()
cv2.destroyAllWindows()
