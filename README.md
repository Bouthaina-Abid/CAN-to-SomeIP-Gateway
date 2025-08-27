# CAN-to-SOME/IP Gateway

This project demonstrates a **CAN-to-SOME/IP Gateway** using SIL Kit and vsomeip.  
It allows CAN messages from SIL Kit demos to be sent and routed to SOME/IP clients through the gateway.

---

## 🚗 **For the CAN Application**

### **Step 1 — Build the CAN Demo**
```bash
    cd sil-kit/Demos/communication/Can/
    mkdir build
    cd build
    cmake ..
    make
```

---

## 🌐 **For the SOME/IP Gateway**

### **Step 1 — Build the Gateway**
```bash
    cd vsomeip/silkit_someip_gateway/
    mkdir build
    cd build
    cmake ..
    make
```

---

## 🛠 **Run SIL Kit Services**

### **Step 1 — Start the SIL Kit Registry**
```bash
    cd install/bin/
    ./sil-kit-registry
```

### **Step 2 — Start the System Controller**
```bash
    cd install/bin/
    ./sil-kit-system-controller CanWriter Gateway
```

---

## 🚦 **Run the CAN Demo**

### **Step 1 — Start the CAN Writer**
```bash
    cd sil-kit/Demos/communication/Can/build
    ./CanWriterDemo
```

---

## 🌉 **Run the SOME/IP Gateway**

### **Step 1 — Start the Gateway**
```bash
    cd vsomeip/silkit_someip_gateway/build
    VSOMEIP_CONFIGURATION=../config/vsomeip-config.json VSOMEIP_APPLICATION_NAME=GatewayNotifier ./GatewayNotifier
```

### **Step 2 — Start the SOME/IP Client**
```bash
    cd vsomeip/silkit_someip_gateway/build
    VSOMEIP_CONFIGURATION=../config/someip_client.json VSOMEIP_APPLICATION_NAME=someip_client ./someip_client
```

---

## ⚠️ **Notes**

- Always **start the SIL Kit registry and system controller first**.  

