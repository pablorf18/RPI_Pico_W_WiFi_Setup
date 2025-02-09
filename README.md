Example of how to connect to a custom WiFi network the RPI Pico W.
First, set the RPI Pico W in AP mode. Then, access it through its web page (you have to connect to the RPI Pico W wifi network and then access to the 192.168.1.1 ip).
Scan your available networks, choose one and send the password. If is correct, the RPI Pico W will change from AP mode to STA mode and will be connected to the network.

To build the project:
1. Export the pico sdk env variable: export PICO_SDK_PATH=/<your_pico_sdk_path>/pico-sdk -> take care and point to your pico-sdk right folder
2. mkdir build && cd build
3. cmake -DPICO_BOARD=pico_w .. -> you will need internet connection to download the makefsdata script from repo!
4. make