# Testing Data Collection (50-Trial)

These two Arduino Uno sketches let you measure:

- **True-positive rate** (TP / 50)  
- **False-positive rate** (FP / 50)  
- **Per-trial latency** (ms)

did testng entirely via the Serial Monitor.

---

## Prerequisites

- Two **Arduino Uno** boards  
- USB connection to your computer  
- **Arduino IDE** (set Serial Monitor to 115200 baud)

---

## Sketches

- **Transmitter**:  
  `Test data collection files/Transmitter/Transmitter.ino`  
  Logs `SEND,⟨timestamp⟩,⟨payload⟩` and modulates the LEDs.

- **Receiver**:  
  `Test data collection files/UNODecoder/UNODecoder.ino`  
  Logs `RECV,⟨timestamp⟩,⟨decoded⟩`, then prints  
  `pattern matched` / `no pattern match`, and after 50 runs prints:
  ```
  SUMMARY,Trials=50,TP=xx,FP=yy
  ```

---

##  Procedure

1. **Upload** each sketch to its Uno.  
2. **Open** a *single* Serial Monitor at **115200 baud**.  
3. **Run** the trial: the sketches will automatically execute 50 transmissions.  
4. **Observe** lines like:
   ```
   SEND,0,101011
   RECV,7,101011
   pattern matched
   …
   SUMMARY,Trials=50,TP=48,FP=2
   ```
5. **Stop** logging when you see the final `SUMMARY` line.

---

##  Data Extraction

- **Latency** for each trial = `RECV_time – SEND_time`  
- **True-positive rate** = `TP / 50`  
- **False-positive rate** = `FP / 50`

Copy your entire Serial log into a spreadsheet to compute & plot these metrics for your Results section.
