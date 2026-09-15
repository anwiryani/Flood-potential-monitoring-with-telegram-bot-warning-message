Flood potential monitoring with telegram bot warning message

<img width="221" height="372" alt="image" src="https://github.com/user-attachments/assets/7ac9437b-43a0-43ab-9c8e-1d2233498ed9" />

The system laverage ultrasonic sensor to detecting river water level and five period moving average to avoid misclassification of the state due to momentary data spike. 
There are three states in the system (aman, siaga, and awas) And four tasks, which are: 
1. Sensing (collecting ultrasonic data)
2. Moving average (calculate average value of the data in window range)
3. State alert (determine the state based on the moving average result)
4. Send message (send warning message to telegram bot if the "AWAS" state detected)
