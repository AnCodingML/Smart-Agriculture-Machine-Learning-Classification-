import pyrebase
from sklearn import tree
import pandas as pd
from sklearn.metrics import classification_report
import time


firebaseConfig = {
  "apiKey": "AIzaSyBMtMFZWNxTR_Oz8gqmdER2kpoORAiatVE",
  "authDomain": "ml-agriculture.firebaseapp.com",
  "databaseURL": "https://ml-agriculture-default-rtdb.firebaseio.com",
  "projectId": "ml-agriculture",
  "storageBucket": "ml-agriculture.appspot.com",
  "messagingSenderId": "248773331346",
  "appId": "1:248773331346:web:6d973d1a11a9b397d7d94b",
  "measurementId": "G-HD878LEXNE"}

firebase=pyrebase.initialize_app(firebaseConfig)
db=firebase.database()

clf = tree.DecisionTreeClassifier()

FileDB = 'data.txt'
Database = pd.read_csv(FileDB, sep=" ", header=0)
print(Database)

#x = Data, y=Target
X = Database[[u'Suhu',u'Kelembaban', u'Tanaman']]
Y = Database.Hasil

# CHALLENGE - ...and train them on our data
clf = clf.fit(X, Y)

def main():
    users=db.child("Monitoring").get()
    prediction = clf.predict([[(users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman'])]])
    print((users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman']))
    if (users.val()['Tanaman']) == 1:
        print("Tanaman Kelapa")
    if (users.val()['Tanaman']) == 2:
        print("Tanaman Bunga")
    print("Keadaan = ",prediction)
    if clf.predict([[(users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman'])]]) == 'Ideal':
        print ("Pompa Mati")
        db.child("Monitoring").update({"Pompa":'0'})
    if clf.predict([[(users.val()['Suhu']),(users.val()['Tanah']),(users.val()['Tanaman'])]]) == 'siram':
        print ("Pompa Nyala")
        db.child("Monitoring").update({"Pompa":'1'})
    #time.sleep(1)
    main()
main()
