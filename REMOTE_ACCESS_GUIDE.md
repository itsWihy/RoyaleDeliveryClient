# Remote Access with Tailscale

This document explains how to securely connect to the Royale Delivery Server from anywhere in the world using Tailscale, avoiding the need for port forwarding or exposing your home IP.

## 1. How it Works
Tailscale creates a secure, private mesh network (a "Tailnet") between your devices. Even when you are away from home, your laptop/phone and the Raspberry Pi will act as if they are on the same local network.

*   **Server (Pi):** Already configured with Tailscale.
*   **Pi's Tailscale IP:** `100.97.85.5`
*   **Client (Your Laptop/Phone):** You must install and log in to Tailscale to "see" the Pi.

## 2. Setup Instructions

### For a New Device (Fresh Install)
If you are opening a new laptop and want to connect:
1.  **Install Tailscale:** Download from [tailscale.com/download](https://tailscale.com/download) and log in with your account.
2.  **Ensure Tailscale is 'Active':** You must see the Pi (`urieldonttouch`) in your device list as "Online".
3.  **Copy the Client Folder:** Copy the built `RoyaleDeliveryClient` folder (or just the executable) to your new laptop.
4.  **Run the App:** It is already hardcoded to look for the Pi at `100.97.85.5`.

### Step A: Install Tailscale on your Client
1.  **Download:** Go to [tailscale.com/download](https://tailscale.com/download) and install the app for your OS (Windows, macOS, Linux, iOS, or Android).
2.  **Login:** Sign in using the **same account** you used for the Raspberry Pi.
3.  **Verify:** Once logged in, you should see `urieldonttouch` (or your Pi's hostname) in the Tailscale list of devices.

### Step B: Configure the Royale Client
The client is currently configured to use the Pi's Tailscale IP. If you need to verify or change it:

1.  Open `RoyaleDeliveryClient/headers/globals.h`.
2.  Ensure `PI_ADDRESS` is set to the Tailscale IP:
    ```cpp
    #define PI_ADDRESS "100.97.85.5"
    ```

## 3. Usage
1.  **Turn on Tailscale** on your computer.
2.  **Run the Royale Delivery Client.**
3.  The client will connect to `100.97.85.5`. Tailscale will securely route this traffic to your Pi at home, no matter what network you are actually on (Coffee shop, LTE, etc.).

## 4. How to Verify Connection
If it's not working on your new laptop, follow these steps to verify:

1.  **Ping Test:** Open a terminal and run `ping 100.97.85.5`. If you get responses, your Tailscale tunnel is working.
2.  **Port Test:** Run `nc -zv 100.97.85.5 5004` (Linux/Mac) or `Test-NetConnection 100.97.85.5 -Port 5004` (Windows PowerShell).
3.  **Client Check:** If the app says "Couldn't reach server", check that your laptop hasn't "Slept" the Tailscale connection.

## 5. Why this is safer
*   **No Port Forwarding:** You don't have to open port 5004 on your router.
*   **Encryption:** All traffic between your laptop and the Pi is end-to-end encrypted.
*   **Invisible:** Your server is not visible to the public internet; only your authenticated Tailscale devices can see it.
