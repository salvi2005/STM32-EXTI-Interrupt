# 🧠 MPU6050 & MPU9250 Deep Dive (IMU for Drone)

This module explains **MPU6050 (6DOF)** and **MPU9250 (9DOF)** sensors used in drones for orientation and stabilization.

This is a complete beginner → advanced guide including:

• Sensor theory  
• Working principle  
• Registers  
• I2C communication  
• Angle calculation  
• Sensor fusion  
• Drone application  

---

# 📌 What is IMU?

IMU = Inertial Measurement Unit

It measures motion and orientation using sensors.

---

# 🔥 MPU6050 vs MPU9250

| Feature | MPU6050 | MPU9250 |
|--------|--------|--------|
Accelerometer | ✅ | ✅ |
Gyroscope | ✅ | ✅ |
Magnetometer | ❌ | ✅ |
DOF | 6 | 9 |
Yaw accuracy | Low | High |

---

# ⚙️ Sensors Inside IMU

## 1️⃣ Accelerometer

Measures:

Gravity + acceleration

Used for:

• Tilt detection  
• Roll & Pitch angle  

Example:

If device is tilted → X/Y axis values change  

---

## 2️⃣ Gyroscope

Measures:

Angular velocity (deg/sec)

Used for:

• Rotation detection  
• Smooth motion  

Important:

Gyro gives speed → angle obtained by integration  

---

## 3️⃣ Magnetometer (Only MPU9250)

Measures:

Earth magnetic field

Used for:

• Direction (Yaw)  
• Compass  

---

# 📊 Sensor Understanding

| Sensor | What it tells |
|-------|--------------|
Accelerometer | Tilt |
Gyroscope | Rotation speed |
Magnetometer | Direction |

---

# 🔌 Communication (I2C)

MPU uses I2C protocol.

Pins:

SDA → Data  
SCL → Clock  

---

# 📍 I2C Addresses

MPU6050 / MPU9250:

0x68  

Magnetometer (AK8963):

0x0C  

---

# 📘 Important Registers

## MPU Registers

| Register | Address | Function |
|--------|--------|--------|
PWR_MGMT_1 | 0x6B | Wake sensor |
ACCEL_XOUT_H | 0x3B | Acc data |
GYRO_XOUT_H | 0x43 | Gyro data |

---

## Magnetometer Registers

| Register | Address |
|--------|--------|
HXL | 0x03 |
CNTL1 | 0x0A |

---

# ⚡ Working Flow

IMU Data Flow:

Raw sensor data  
↓  
I2C read  
↓  
Convert to physical values  
↓  
Calculate angles  
↓  
Apply filter  
↓  
Final orientation  

---

# 📥 Raw Data Reading

Accelerometer:

```c
accX = I2C_Read16(0x68, 0x3B);
accY = I2C_Read16(0x68, 0x3D);
accZ = I2C_Read16(0x68, 0x3F);
```

Gyroscope:

```c
gyroX = I2C_Read16(0x68, 0x43);
gyroY = I2C_Read16(0x68, 0x45);
gyroZ = I2C_Read16(0x68, 0x47);
```

Magnetometer:

```c
magX = I2C_Read16(0x0C, 0x03);
magY = I2C_Read16(0x0C, 0x05);
```

---

# 🔄 Conversion Formula

Accelerometer:

Acc = raw / 16384.0  

Gyroscope:

Gyro = raw / 131.0  

---

# 📐 Angle Calculation

## Roll

```c
roll = atan2(accY, accZ) * 57.3;
```

## Pitch

```c
pitch = atan2(-accX, sqrt(accY*accY + accZ*accZ)) * 57.3;
```

## Yaw (MPU9250)

```c
yaw = atan2(magY, magX) * 57.3;
```

---

# 🔥 Problem with Sensors

Accelerometer → noisy  
Gyroscope → drift  

---

# 🧠 Solution: Sensor Fusion

Combine both sensors:

Short term → Gyro  
Long term → Accelerometer  

---

# ⚡ Complementary Filter

```c
angle = 0.98*(angle + gyro*dt) + 0.02*acc_angle;
```

---

# 🧩 Full Concept

Accelerometer → stable but noisy  
Gyroscope → smooth but drift  

Combine → stable + smooth  

---

# 🚁 Drone Application

Drone control flow:

IMU sensor  
↓  
Angle calculation  
↓  
PID controller  
↓  
Motor mixing  
↓  
PWM output  
↓  
Motors  

---

# ⚠️ Important Notes

• dt must be correct  
• Calibration required  
• Magnetometer affected by noise  
• Sensor mounting must be stable  

---

# 📊 Final Output

After processing:

roll → tilt  
pitch → tilt  
yaw → direction  

---

# 🧠 Summary

MPU6050:

Roll + Pitch  

MPU9250:

Roll + Pitch + Yaw  

---

# 🚀 Next Step

Next module:

PID Controller  

This is the core of drone stabilization.
