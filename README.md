# Smart Agriculture

---

Adalah sistem yang dibuat untuk pertanian berbasis AI dan IOT dimana didalamnya terdapat machine learning untuk menentukan kondisi yang akan diberikan pada tanaman, sensor-sensor yang sebagai inputan untuk machine learning, NodeMcu untuk melakukan komunikasi antara perangkat monitoring dengan perangkat pada lapangan untuk melakukan pemantauan, dan sistem software mulai dari aplikasi hingga sistem pada Firebase yang mengatur kerja alat Smart Agriculture.<br>

---

## Dataset<br>

![Jenis Tanaman](https://github.com/AnCodingML/Smart-Agriculture-Machine-Learning-Using-Firebase/blob/main/Database/image.png?raw=true)<br>
Jenis tanaman yang dilakukan pada percobaan kali ini terdapat 3 jenis tanaman yang hidup pada lingkungan yang berbeda:<br>
- Xerofit<br>
Merupakan jenis tanaman yang hidup pada wilayah kering dan cenderung tandus, kelembapan tanah ideal pada jenis tanaman xerofit adalah 23%-28% serta suhu ideal tanaman ini adalah 16-34 derajat celcius. Salah satu contoh tanaman ini adalah kaktus<br>
- Tropofit<br> Merupakan jenis tanaman yang biasa hidup pada kondisi tanah yang tidak terlalu basah dan tidak terlalu kering dengan kelembapan ideal tanaman ini adalah 60% - 80% dengan suhu ideal 16 C - 28 C. Salah satu contohnya adalah pohon apel
- Saprofit<br> Merupakan salah tumbuhan yang hidup pada keadaan  basah dengan kelembapan tanah ideal 65% - 70% dan suhu ideal 19 C - 25 C. salah satu contoh adalah tanaman jamur.<br>
<br>
Berdasarkan kondisi tersebut dibuat dataset dengan isi fitur:<br>

* Suhu = Suhu lingkungan tumbuhan yang didapat dari sensor
* Kelembapan = Kelembapan tanah yang merupakan input dari sensor
* Tanaman = Merupakan jenis tanaman dengan keterangan sebagai berikut:
<br>

| Jenis    | No |
|:---------|--:|
| Xerofit  | 1 |
| Tropofit | 2 |
| Saprofit | 3 |


* Hasil = Memiliki dua kondisi yaitu ideal dan siram. Jika memiliki kondisi siram maka pompa akan hidup untuk menyiram tumbuhan

---
## Skema Rangkaian NodeMcu <br>

![Skema Rangkaian NodeMcu](https://github.com/AnCodingML/Smart-Agriculture-Machine-Learning-Using-Firebase/blob/main/ESP8266/rangkaian.png)<br>

Pada gambar rangkaian diatas terlihat terdapat dua sensor:
1. DHT 11<br>
Sensor DHT11 terdiri dari elemen penginderaan kelembaban kapasitif dan termistor untuk penginderaan suhu. Untuk mengukur suhu sensor ini menggunakan termistor koefisien Suhu Negatif, yang menyebabkan penurunan nilai resistansi dengan kenaikan suhu. Rentang suhu DHT11 adalah dari 0 hingga 50 derajat Celcius dengan akurasi 2 derajat. Kapasitor penginderaan kelembaban memiliki dua elektroda dengan substrat penahan kelembaban sebagai dielektrik di antara keduanya. Perubahan nilai kapasitansi terjadi dengan perubahan tingkat kelembaban.<br>
Konfigurasi pin:<br>

| Nama     | ket   |
|:---------|------:|
| VCC      | 3VDC  |
| Negative |   GND |
| Sensor   |    D2 |

2. Soil Moisture<br>
Cara kerja sensor kelembapan tanah kapasitif adalah ketika sensor mendeteksi kondisi tanah dalam keadaan kering maka tegangan outputnya akan meningkat, namun saat kondisi tanah terdeteksi basah maka tegangan output sensor akan menurun. Pada intinya, sebuah resistor dan kapasitor bekerja untuk memperkirakan jumlah air di lingkungan sekitar.<br>
Konfigruasi pin:

| Nama     | ket   |
|:---------|------:|
| VCC      | 3VDC  |
| Negative |   GND |
| Analog   |    A0 |

3. Relay<br>
Berfungsi untuk menghidupkan pompa sesuai dengan kondisi yang ditentukan<br>
konfigurasi pin:

| Nama     | ket   |
|:---------|------:|
| VCC      | Vin   |
| Negative |   GND |
| Sensor   |    D5 |
| COM      | VIN   |

4. Pompa<br>
Menyalurkan air kepada tanaman dengan kondisi nyala dan hidup sesuai dengan relay<br>
konfigurasi pin:

| Nama     | ket   |
|:---------|------:|
| VCC      | NO    |
| Negative |   GND |

---

## Firebase <br>

![Firebase](https://user-images.githubusercontent.com/117325158/234297827-0bc00d66-c68d-4c23-9c2d-338759d5351f.png)<br>
Pada firebase dibuat data seperti pada gambar, tulisan pada data tersebut harus disesuaikan dengan tulisan pada APK yang dibuat dan pada NodeMcu

---

## APK Monitoring<br>
Aplikasi dibuat menggunakan MIT APP INVENTOR yang kemudian diinstall pada handphone yang akan digunakan untuk monitoring <br>
![image](https://user-images.githubusercontent.com/117325158/234298629-cf5978a0-d4d7-44e1-a0c5-2842123487ce.png)<br>
Secara diagram blok, pemrogaman seperti pada gambar diatas dengan tanaman dilakukan encoding:
- Xerofit = 1
- Toprofit = 2
- Saprofit = 3

Kemudian untuk tampilan monitoring seperti pada gambar dibawah:<br>
![image](https://github.com/AnCodingML/Smart-Agriculture-Machine-Learning-Using-Firebase/blob/main/Phone/image.png?raw=true)<br>
Kondisi yang dapat disesuaikan adalah jenis tanaman yang ditanam yakni antara Xerifit, Tropofit, Saprofit.

---

## Modeling

Jenis pemodelan yang digunakan adalah Decision Tree Classifier. dengan Hasil sebagai target maka program dapat ditulis sebagai berikut
```
#x = Data, y=Target
X = Database[[u'Suhu',u'Kelembaban', u'Tanaman']]
Y = Database.Hasil
```
dilakukan fitting modeling dengan program <br>
```
clf = tree.DecisionTreeClassifier()
clf = clf.fit(X, Y)
```
Kemudian dengan program looping dibuat kondisi<br>
 ```
    if clf.predict([[(users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman'])]]) == 'Ideal':
        print ("Pompa Mati")
        db.child("Monitoring").update({"Pompa":'0'})
    if clf.predict([[(users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman'])]]) == 'siram':
        print ("Pompa Nyala")
        db.child("Monitoring").update({"Pompa":'1'})
 ```
Berdasarkan program tersebut didapatkan hasil jika output hasil prediksi adalah siram maka pompa akan menyala untuk menyiram tumbuhan sedangkan untuk output ideal maka pompa akan berhenti untuk menyiram tumbuhan. <br>

---

## Kesimpulan<br>

Alat yang dibuat dilakukan uji coba lapangan dan didapatkan ketepatan alat yang dibuat adalah sebesar 100%, cara lain menguji coba alat adalah dengan memberikan kondisi secara manual pada Firebase.<br>
Alat yang dibuat mampu menyiram tanaman dan dapat melakukan monitoring dengan mudah menggunakan smartphone pengguna, alat ini dapat lebih dikembangkan dengan alat pengatur suhu ruangan sehingga dapat memaksimalkan suhu ideal bagi tanaman
