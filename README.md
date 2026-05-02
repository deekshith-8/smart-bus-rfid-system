# Smart Bus Monitoring System 🚍

An IoT-based RFID system using ESP32 to track student boarding and exiting in real-time.

## 🔧 Features
- RFID-based identification
- Real-time Firebase logging
- WiFi-enabled ESP32
- Live status tracking (Boarded / Exited)
- Timestamp logging

## 🛠 Tech Stack
- ESP32
- RC522 RFID Module
- Firebase Realtime Database
- Arduino IDE

## ⚙️ How it works
1. Student taps RFID card
2. ESP32 reads UID
3. Matches student
4. Updates status
5. Sends data to Firebase

## 📡 Output
Data is stored in Firebase with:
- Name
- Status
- Student ID
- Time

## 🚀 Future Improvements
- Mobile app dashboard
- GPS tracking
- Notifications to parents
