# Visitor Notification System

A smart notification system that logs visitor contact information when the homeowner is away. The visitor enters their phone number using a keypad, which is displayed on an LCD and stored for the owner to view later.

---

## 🛠️ Hardware Components
- Arduino UNO
- 4x4 Matrix Keypad
- 16x2 LCD Display
- IR Sensor
- Resistors, Jumper wires, Breadboard

---

## 💡 How It Works
- When the system is active (owner is not at home), any visitor can input their mobile number via the keypad.
- The number is displayed on the LCD and stored temporarily in Arduino memory.
- When the owner returns, they can review the list of visitor numbers.
- Future enhancements include full message input with alphanumeric support.

---
## 📽️ Project Video
> ✅ Project Video

[▶ Watch Video](./Visitor-Notifier_Demo.mp4)

---

## 📽️ Simulation Video
> ✅ Proteus simulation demonstrating keypad input and LCD update.

[▶ Watch Simulation](./Proteus_Simulation/Visitor_Notifier_Proteus_Simulation.mp4)

---

## 💾 Arduino Code
Main logic for keypad input, LCD display handling, and storage.

[📄 View Code](./Arduino_Code/visitor_notifier.ino)

---

## 🔧 Future Improvements
- Alphanumeric keypad to allow message input (e.g., “Parcel left at door”)
- GSM module integration to notify homeowner via SMS
- Optional touchscreen support
---

## 📌 Summary
Built as part of B.Tech ECE mini-projects. This system provides a simple yet effective way to notify homeowners of missed visitors using minimal hardware and logic.

