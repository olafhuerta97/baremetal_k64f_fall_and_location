# baremetal_k64f_fall_and_location_tx
This was a school project made in Tecnologico de Monterrey had as goal to detect fall and the location of psychiatric patients in a mental health hospital. 
The project uses a kinetis nxp k64f microcontroller with its built in accelerometer for detecting the fall and a neo gps to detect ubication. For sending the information to the receptor a nrf24l01 was used. In the receptor the modules used were a lcd 16x02, output compare of diferent frequencys for making tones and a external i2c memory for saving information.
The code is made 100% in baremetal programming, no libraries were used. 
[More info](https://docs.google.com/presentation/d/1O1mpz2Zpg-PNperZPEnL-2zeOvIShckMjRdAIiNy-4w/edit?usp=sharing)
[More info (Spanish)](https://docs.google.com/presentation/d/1DdGcfKBSRmUkOVxrM-Q6-cPUrZTwif7XFlaJo-RJAoQ/edit?usp=sharing)

This is the transmitter code.
