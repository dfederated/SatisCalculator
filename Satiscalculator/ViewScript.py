import os
import json
import pyodbc
from pathlib import Path


with open('login.txt', 'r') as f:
    login = f.readlines()
    login = login[0]
cnxn = pyodbc.connect(login)
cursor = cnxn.cursor()

tempPath = Path("viewtemp.json")

with open(tempPath) as tempFile:
    tempData = json.load(tempFile)
try:
    cursor.execute("SELECT ID, Product, SUM(Machines) AS Machines, SUM(Wattage) AS Wattage,SUM(Output) AS Output FROM " + tempData["Table"] + " GROUP BY ID, Product HAVING COUNT(*) = 1;")
except:
    input("ERROR")
getcheck = dict()
getdict = dict()
final = []
counter = 0
for row in cursor.fetchall():
    getcheck['ID'] = row[0]
    getdict['Count'] = counter
    getdict['Product'] = row[1]
    getdict['Machines'] = row[2]
    getdict['Wattage'] = row[3]
    getdict['Output'] = row[4]
    if getcheck['ID'] == 'BYPRODUCT':
        getdict['ID'] = getcheck['ID']
        del getdict['Machines']
        del getdict['Wattage']
        del getdict['ID']
    if counter == cursor.rowcount - 1:
        getdict['Count'] = "LAST"
    final.append(getdict)
    getcheck = dict()
    getdict = dict()
    counter += 1
if os.path.exists("currentview.json"):
    os.remove("currentview.json")
with open("currentview.json", 'w') as tempFile:
    json.dump(final, tempFile)