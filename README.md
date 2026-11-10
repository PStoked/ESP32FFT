# ESP32FFT
## BUILD INSTRUCTIONS
### Materials:
   1. [ESP32 DEV KIT](https://www.amazon.com/AITRIP-ESP-WROOM-32-Bluetooth-ESP32-DevKitC-32-Development/dp/B0DPS44HCQ/ref=sr_1_10?crid=3EL8Y2SSZDDAE&dib=eyJ2IjoiMSJ9.iYxQzxIXwMUcOxna5mEQ19wzSSJLnEEWVRjGFpx80vgXZUUWAp73fyx0wk9UQ-Em79KRGtz9I3vMD7WiurnGaq6M-woUwGoveiR83bCC7hHV-6zsq5y5C8yz5JHyPRgE-18VAfHA68ze3PppjeusmZl8-LL82Mf5jDhMlwUrPekSR99eDNVp20JH8qUQ6M6qMvU40e-d8Mqvrk_RsW9alTKDuMyz14l4eyLKrgPo8vw.42fpS5M9JfynFxN9QReF93aYvLfrkDSRCmKGFx2DhF4&dib_tag=se&keywords=esp32%2Bdev%2Bkit%2Band%2Bterminal%2Badapter&qid=1794352931&sprefix=esp32%2Bdev%2Bkit%2Band%2Bterminal%2Badapt%2Caps%2C243&sr=8-10&th=1)
   1. [LED strip](https://www.amazon.com/BTF-LIGHTING-Upgraded-Individually-Addressable-Waterproof/dp/B07LG6NSDF/ref=sr_1_15?crid=2PRWO66NX2US&dib=eyJ2IjoiMSJ9.gwiCWeta6gEdd7X_NvtU8AlhUFXuQ1rMVaE8dpcFzSduIrFZKvuX6c4UN_VEi33Cwu_0lY-omwen8DuvXZGpsyIcqEZFH_k0pHMEtw-3DSt--AajM-PzMvJ-sZIfHiHjB4SKnDcEUHUfYDd_b54STOFk9jUjjGBWklTh-WGjJs2EsbX0-0h18tJMBwtwvRp4RHygVqI92aHZLnQgY2NGF3OxhcMCKAW7jN05MstdLsS_CX3nAkLdZZUEqLJlYS1LBt5f-frHRsaw68Kgmu8cVToidu0Z9LyevreVfXAQd6A.DrAcWhSValFf5FUTXHPfQ0ULm1KtmY3EXMEBx2v5uoM&dib_tag=se&keywords=btf%2Blighting%2Bws2815%2Brgb%2Bpixel%2Bled%2Bstrip&qid=1794353422&sprefix=btf%2Blighting%2Bws2815%2Brgb%2Bpixel%2Bled%2Bstri%2Caps%2C235&sr=8-15&th=1)
   1. [Breadboard and Jumper Cables](https://www.amazon.com/BOJACK-Values-Solderless-Breadboard-Flexible/dp/B08Y59P6D1/ref=sr_1_5?crid=IEIJJI8FS6TF&dib=eyJ2IjoiMSJ9.CZ9ckeJXN_0HMKMV6aH0RN8nQivL7zMgGjMJMWE_99IDKANEOt3-MzNVn3fRhxU8DRtzSM0zklBahIQ6ZNVjUmcR6dEdaSyZDOU_u2hMG1Qc8h4HLM9Q6HgS7CwxLHlY9jBRYQr5GtY4B_q-3Yr27JJNzr_9u5Em61cD2os171aJlyDdThBcYbhpGCvrAJGSXiq6vYYIXJQ9ZQxxk0x-TJJToeSPrW_WIN69CYOuSYY.6b6mXC7yfr65AdVq7RWExwHT_6TjEJLShPTHa64MOvo&dib_tag=se&keywords=breadboard&qid=1794353159&sprefix=breadboar%2Caps%2C200&sr=8-5&th=1)
   1. [Microphone (3 pack)](https://www.amazon.com/Qoroos-Omnidirectional-Microphone-Interface-Precision/dp/B0FKFR1WFX/ref=sr_1_1?crid=1QIEAZPO1HC56&dib=eyJ2IjoiMSJ9.0NhihoXHotA1n3i9UG92Jz5ruiMHY6iGJAyoKDjdnsQoGMiXfBv1qgLSRzv49MEnYt9TUoOqg33SvHkla4gLGaxBalp3VmI8EmcbcZ4kKk9sMoDR4pI_qoIaMaj6ncxnvQ4MpX1yQ4HBD4t-pOTCrXpo5QZHgDm6A72PJrlGP_dZIBJtIXabjRpvRJZdiwKWwXJPcF1dX3jaYyQyOLsi1j7FHxthIqh3br5LREx1vhA.e3AtAfJ3YXy3SJKxBXY1sF0PVvg7bcx4thQp6zm_TjQ&dib_tag=se&keywords=esp32%2Bmicrophone&qid=1794353195&sprefix=esp32%2Bmicrophon%2Caps%2C185&sr=8-1&th=1)
   1. [Level shifter (12 pack)](https://www.amazon.com/WWZMDiB-Channels-Converter-Bi-Directional-3-3V-5V/dp/B0CCF6TCSJ/ref=sr_1_2?crid=3JISEGQ31QX39&dib=eyJ2IjoiMSJ9.gLesQ66GmiJntzZu5NyUR6n5KeBuFQ8DLaUm7D5nkAkyACHyWRi2sPkBix9at6n2gsqqU2Or3vIQZYFdNw8kkiIcr0lc8ZwOL1O2k3U3ZXGh5xbtewYxdN1IBkjl2oLEM5BKhoE9_dIkyYtBWpJa5gTLwF2xvu8H5SB0dYs7J-PrU8ag38r-jab26OeEgKlgcKZ-nhZWSQgv3YnjP56GowL_cwpKHEiMiua4z69W3kc.pTMHbigRH8EGc85vvwN2MAM1iqGsTLWGdXrOGdnn_s8&dib_tag=se&keywords=esp32+level+shifter&qid=1794353241&sprefix=esp32+level+shifte%2Caps%2C175&sr=8-2)
   1. [12 Volt power supply](https://www.amazon.com/ALITOVE-Adapter-Converter-100-240V-5-5x2-1mm/dp/B01GEA8PQA/ref=sr_1_3?crid=615G0TB9VND5&dib=eyJ2IjoiMSJ9.rx4A8Vv6A-YD9vQCxLdr9lr0XJQHotLKM1MuGej3tPAII1UuT2hvQSil54FhyG92OqSNWDSOdso75ioaoOOhNElDKqyLhbYzcj0_emBhJXofHjo8u2UimzTLKDUOAqRsM3hrvAtNXqAMqZNguOrFB1qxHn3gxLxG8u-4uLOPORadLDmui8FFNh6tZ4_S46Rfqd29Nd-mkj9o5Vg4e_tGLQItejBCgs8cTsYIQDJqmN4.UvqbTsyKqr2sEyubb2FCI7jQA_ePXgWWIyfMbyYnblU&dib_tag=se&keywords=12%2Bvolt%2Bpower%2Bsupply&qid=1794353278&sprefix=12%2Bvolt%2Bpower%2Bsupply%2Caps%2C199&sr=8-3&th=1)
   1. [Bulk Capacitor](https://www.amazon.com/Multilayer-Monolithic-Capacitor-Assortment-Electronics/dp/B0D2GVNDHY/ref=sr_1_7?crid=U7PRJJDEE1T&dib=eyJ2IjoiMSJ9.CJsxPeBSHUonRJEFHhuS_J9z5NAzwUHjaNkfyL4-_t8o8M-2-ufyz2ZeRSMViH3a2ZkRjBLTySt5Xt7mrqLOcGu9YqAidmi0f7Bv9gS-PjtcNJRzvy3iX_vLpEAUMPeiqaautVsMoILRoIek8dc9GYnAHZUXH-kll_ns_tbj3Wsxn17x5d-Js4eF05V3hGlk66TAcwF0PZR7QB_g00DzsBwW3AW4y1U5svtspEzjc3I._kWUuu2lFXrcjhYZDRUrNWbD4-Qh4Ij-ZCur0wZ-ILE&dib_tag=se&keywords=bulk+capacitor&qid=1794353854&sprefix=bulk+capacito%2Caps%2C201&sr=8-7)

### Instructions
1. Plug the ESP32 into the terminal adapter, making sure the correct pins are going to the correct terminals.
1. Plug the microphone and level shifter into the breadboard so that each pin connects to a set of terminals.
1. Connect the grounds to each other: Using 3 wires, connect, GND on one side of the level shifter to GND on the other, GND on the microphone to either side of that, and L/R on the microphone to anywhere on that.
1. Connect a GND terminal on the ESP32 any of these grounds. They should now all be connected to each other.
1. Connect pin 32 to LV1 on the level shifter.
1. Connect pin 33 to SD on the microphone.
1. Connect pin 25 to WS on the microphone.
1. Connect pin 26 to SCK on the microphone.
1. Using the plug from the power source and the plug with the two extra wires from the LEDs, connect the positive and negative wires on each to each other. Insert the capacitor to bridge the gap and act as a bulk capacitor.
1. Connect HV1 to the two wires that aren't power on the LED plug.
1. Connect ground on the LED plug to a grounded terminal on the breadboard.
1. Flash ESP32FFT.ino to the ESP32 following the instructions below:

Flashing the file to an ESP32 following our tutorial here: for a realtime fourier transform device.

1. Download Arduino IDE: https://www.arduino.cc/en/software/#ide
2. Download ESP32FFT.ino and create a folder in C:\Users\usr\Documents\Arduino called ESP32FFT and place the file you just downloaded in the folder.
3. Double click the file within the folder to launch Arduino IDE.
4. Install the ArduinoFFT and FastLED libraries.
5. Upload the code to the ESP32 
6. Enjoy!
