import json
import time
import requests
from bs4 import BeautifulSoup
from pathlib import Path
check = 0
checker = dict()
old_check = dict()
upcheck0 = 0
upcheck1 = 0
upcheck2 = 0
upcheck3 = 0
upcheck4 = 0
upcheck5 = 0
upcheck6 = 0
upcheck7 = 0
upcheck8 = 0
upcheck9 = 0
rawcheck = 0
checkpath = Path("done.json")
recipe_path = Path("Recipes")
pathlist = ['https://satisfactory.wiki.gg/wiki/Manufacturer','https://satisfactory.wiki.gg/wiki/Assembler', 'https://satisfactory.wiki.gg/wiki/Foundry',
            'https://satisfactory.wiki.gg/wiki/Constructor','https://satisfactory.wiki.gg/wiki/Smelter', 'https://satisfactory.wiki.gg/wiki/Refinery',
            'https://satisfactory.wiki.gg/wiki/Blender', 'https://satisfactory.wiki.gg/wiki/Particle_Accelerator','https://satisfactory.wiki.gg/wiki/Quantum_Encoder']
checker["0"] = None
checker["1"] = None
checker["2"] = None
checker["3"] = None
checker["4"] = None
checker["5"] = None
checker["6"] = None
checker["7"] = None
checker["8"] = None
old_check["0"] = None
old_check["1"] = None
old_check["2"] = None
old_check["3"] = None
old_check["4"] = None
old_check["5"] = None
old_check["6"] = None
old_check["7"] = None
old_check["8"] = None
machinelist = ['Manufacturer', 'Assembler', 'Foundry', 'Constructor', 'Smelter', 'Refinery', 'Blender', 'Particle Accelerator', 'Quantum Encoder']
counter = 0
while True:
    try:
        for row in pathlist:
            if checker[str(counter)] is None:
                pagecheck = requests.get(row)
                findcheck = BeautifulSoup(pagecheck.content, 'html.parser')
                toe = findcheck.find(attrs={'id': "footer-info-lastmod"}).text
                print(toe)
                time.sleep(1)
                checker[str(counter)] = toe
            counter = counter + 1
        break
    except:
        counter = 0
        print("Connection Severed. Trying again. . .")
        time.sleep(3)
        continue
if checkpath.is_file():
    with open(checkpath, 'r') as f:
        old_check = json.load(f)
        rawcheck = 1
machinecounter = 0
while True:
    if old_check == checker:
        print("No Update Needed.")
        break
    try:
        if upcheck0 == 0:
            if old_check["0"] != checker["0"]:
                recipe_path = Path('Recipes')
                recipe_path.mkdir(parents=True, exist_ok=True)
                #Manufacturers
                #Setting up Dictionary
                i =dict()
                i['ID'] = 'Manufacturer'
                i['Ingredient1'] = None
                i['Ingredient2'] = None
                i['Ingredient3'] = None
                i['Ingredient4'] = None
                i['Product'] = None
                i['Input1'] = 0
                i['Input2'] = 0
                i['Input3'] = 0
                i['Input4'] = 0
                i['Output'] = 0
                i['Wattage'] = 0
                i['json'] = '.json'
                o =dict()
                o['ID'] = 'Manufacturer'
                o['Ingredient1'] = None
                o['Ingredient2'] = None
                o['Ingredient3'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Input2'] = 0
                o['Input3'] = 0
                o['Output'] = 0
                o['Wattage'] = 0
                o['json'] = '.json'

                #Grabbing Webpage
                print('Connecting to Manufacturer webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Manufacturer')
                soup = BeautifulSoup(page.content, 'html.parser')
                item4c = soup.find_all(attrs={'data-items-count': '4'})
                item3c = soup.find_all(attrs={'data-items-count': '3'})
                wattage = soup.select('section.pi-item:nth-child(6) > div:nth-child(2) > div:nth-child(2)')
                wt = wattage[0]
                wt = str(wt)
                watts = ''.join(c for c in wt if c.isdigit())
                watts = int(watts)
                i['Wattage'] = watts
                o['Wattage'] = watts

                #Counters
                in1 = 0
                in2 = 0
                count = len(item4c)
                count2 = len(item3c)
                print('Parsing Data...\n. . . . . . . . . .\n')
                #Products with 4 inputs
                while True:
                    #Getting Product and output
                    p = item4c[in1]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    try:
                        t = item4c[in1]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.find_next(class_="item-minute")
                        t = t.find_next(class_="item-num")
                        t_num = t.string
                        t_num = float(t_num)
                    except:
                        print("ERROR")
                        t = item4c[in1]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.find_next(class_="item-minute")
                        t = t.find_next(class_="item-num")
                        t_num = t.string
                        t_num = float(t_num)
                    #Getting Names of Ingredients
                    items_n = item4c[in1]
                    items_n = items_n.find(class_='item-name')
                    ing1 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing2 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing3 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing4 = items_n.string

                    #Getting Input Times
                    items_t = item4c[in1]
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    inum1 = float(inum1)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum2 = items_t.string
                    inum2 = float(inum2)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum3 = items_t.string
                    inum3 = float(inum3)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum4 = items_t.string
                    inum4 = float(inum4)

                    #Adding items to Dictionary
                    i['Ingredient1'] = ing1
                    i['Input1'] = inum1
                    i['Ingredient2'] = ing2
                    i['Input2'] = inum2
                    i['Ingredient3'] = ing3
                    i['Input3'] = inum3
                    i['Ingredient4'] = ing4
                    i['Input4'] = inum4
                    i['Product'] = p
                    i['Output'] = t_num
                    i['fn'] = i.get('Product') + i.get('json')
                    #Write to file
                    filepath = recipe_path / i['fn']
                    with open(filepath, 'w') as write:
                        i.pop('fn', None)
                        i.pop('json', None)
                        json.dump(i, write)
                    #Moving to next item
                    in1 = in1 + 1
                    if in1 != count: i['json'] = '.json'
                    if in1 == count : break

                #Getting Items with 3 Ingredients
                while True:
                    # Getting Product and output
                    p = item3c[in2]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    try:
                        t = item3c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.find_next(class_="item-minute")
                        t = t.find_next(class_="item-num")
                        t_num = t.string
                        t_num = float(t_num)
                    except:
                        print("ERROR")
                        t = item3c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.contents
                        t = t[0]
                        t = str(t)
                        t_num = ''.join(c for c in t if c.isdigit())
                        t_num = float(t_num)

                    # Getting Names of Ingredients
                    items_n = item3c[in2]
                    items_n = items_n.find(class_='item-name')
                    ing1 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing2 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing3 = items_n.string

                    # Getting Input Times
                    items_t = item3c[in2]
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    inum1 = float(inum1)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum2 = items_t.string
                    inum2 = float(inum2)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum3 = items_t.string
                    inum3 = float(inum3)

                    # Adding items to Dictionary
                    o['Ingredient1'] = ing1
                    o['Input1'] = inum1
                    o['Ingredient2'] = ing2
                    o['Input2'] = inum2
                    o['Ingredient3'] = ing3
                    o['Input3'] = inum3
                    o['Product'] = p
                    o['Output'] = t_num
                    o['fn'] = o.get('Product') + o.get('json')

                    # Write to file
                    filepath = recipe_path / o['fn']
                    with open(filepath, 'w') as write:
                        o.pop('fn', None)
                        o.pop('json', None)
                        json.dump(o, write)
                    # Moving to next item
                    in2 = in2 + 1
                    if in2 != count2: o['json'] = '.json'
                    if in2 == count2: break
                print('Manufacturers Successfully Obtained.\n')
                upcheck0 = 1
        if upcheck1 == 0:
            if old_check["1"] != checker["1"]:
                machinecounter = 1
                #Assemblers
                #Setting up Dictionary
                o =dict()
                o['ID'] = 'Assembler'
                o['Ingredient1'] = None
                o['Ingredient2'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Input2'] = 0
                o['Output'] = 0
                o['Wattage'] = 0
                o['json'] = '.json'

                #Grabbing Webpage
                print('Connecting to Assembler webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Assembler')
                soup = BeautifulSoup(page.content, 'html.parser')
                item2c = soup.find_all(attrs={'data-items-count': '2'})
                wattage = soup.select('section.pi-item:nth-child(6) > div:nth-child(2) > div:nth-child(2)')
                wt = wattage[0]
                wt = str(wt)
                watts = ''.join(c for c in wt if c.isdigit())
                o['Wattage'] = int(watts)

                #Counters
                in2 = 0
                count2 = len(item2c)
                print('Parsing Data...\n. . . . . . . . . .\n')
                #Getting Items with 2 Ingredients
                while True:
                    # Getting Product and output
                    p = item2c[in2]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    try:
                        t = item2c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.find_next(class_="item-minute")
                        t = t.find_next(class_="item-num")
                        t_num = t.string
                        t_num = float(t_num)
                    except:
                        print("ERROR")
                        t = item2c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.contents
                        t = t[0]
                        t = str(t)
                        t_num = ''.join(c for c in t if c.isdigit())
                        t_num = float(t_num)

                    # Getting Names of Ingredients
                    items_n = item2c[in2]
                    items_n = items_n.find(class_='item-name')
                    ing1 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing2 = items_n.string

                    # Getting Input Times
                    items_t = item2c[in2]
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    inum1 = float(inum1)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum2 = items_t.string
                    inum2 = float(inum2)

                    # Adding items to Dictionary
                    o['Ingredient1'] = ing1
                    o['Input1'] = inum1
                    o['Ingredient2'] = ing2
                    o['Input2'] = inum2
                    o['Product'] = p
                    o['Output'] = t_num
                    o['fn'] = o.get('Product') + o.get('json')
                    if o['Ingredient2'] is None:
                        o['Ingredient2'] = 'Screws'
                    # Write to file
                    filepath = recipe_path / o['fn']
                    with open(filepath, 'w') as write:
                        o.pop('fn', None)
                        o.pop('json', None)
                        json.dump(o, write)
                    # Moving to next item
                    in2 = in2 + 1
                    if in2 != count2: o['json'] = '.json'
                    if in2 == count2: break
                print('Assemblers Successfully Obtained.\n')
                upcheck1 = 1
        if upcheck2 == 0:
            if old_check["2"] != checker["2"]:
                machinecounter = 2
                #Foundery's
                #Setting up Dictionary
                o =dict()
                o['ID'] = 'Foundry'
                o['Ingredient1'] = None
                o['Ingredient2'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Input2'] = 0
                o['Output'] = 0
                o['Wattage'] = 0
                o['json'] = '.json'
                #Grabbing Webpage
                print('Connecting to Foundry webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Foundry')
                soup = BeautifulSoup(page.content, 'html.parser')
                item2c = soup.find_all(attrs={'data-items-count': '2'})
                wattage = soup.select('section.pi-item:nth-child(6) > div:nth-child(2) > div:nth-child(2)')
                wt = wattage[0]
                wt = str(wt)
                watts = ''.join(c for c in wt if c.isdigit())
                o['Wattage'] = int(watts)

                #Counters
                in2 = 0
                count2 = len(item2c)
                print('Parsing Data...\n. . . . . . . . . .\n')
                #Getting Items with 2 Ingredients
                while True:
                    # Getting Product and output
                    p = item2c[in2]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    try:
                        t = item2c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.find_next(class_="item-minute")
                        t = t.find_next(class_="item-num")
                        t_num = t.string
                        t_num = float(t_num)
                    except:
                        print("ERROR")
                        t = item2c[in2]
                        t = t.parent
                        t = t.parent
                        t = t.find('td')
                        t = t.find_next('td')
                        t = t.find_next('td')
                        t = t.contents
                        t = t[0]
                        t = str(t)
                        t_num = ''.join(c for c in t if c.isdigit())
                        t_num = float(t_num)

                    # Getting Names of Ingredients
                    items_n = item2c[in2]
                    items_n = items_n.find(class_='item-name')
                    ing1 = items_n.string
                    items_n = items_n.find_next(class_='item-name')
                    ing2 = items_n.string

                    # Getting Input Times
                    items_t = item2c[in2]
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    inum1 = float(inum1)

                    items_t = items_t.find_next(class_='item-minute')
                    items_t = items_t.find_next(class_='item-num')
                    inum2 = items_t.string
                    inum2 = float(inum2)

                    # Adding items to Dictionary
                    o['Ingredient1'] = ing1
                    o['Input1'] = inum1
                    o['Ingredient2'] = ing2
                    o['Input2'] = inum2
                    o['Product'] = p
                    o['Output'] = t_num
                    o['fn'] = o.get('Product') + o.get('json')

                    # Write to file
                    filepath = recipe_path / o['fn']
                    with open(filepath, 'w') as write:
                        o.pop('fn', None)
                        o.pop('json', None)
                        json.dump(o, write)
                    # Moving to next item
                    in2 = in2 + 1
                    if in2 != count2: o['json'] = '.json'
                    if in2 == count2: break
                print("Foundry's Successfully Obtained.\n")
                upcheck2 = 1
        if upcheck3 == 0:
            if old_check["3"] != checker["3"]:
                machinecounter = 3
                #Constructors
                #Setting up Dictionary
                o =dict()
                o['ID'] = 'Constructor'
                o['Ingredient1'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Output'] = 0
                o['Wattage'] = 0
                o['json'] = '.json'

                #Grabbing Webpage
                print('Connecting to Constructor webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Constructor')
                soup = BeautifulSoup(page.content, 'html.parser')
                item2c = soup.find_all(attrs={'data-items-count': '1'})
                wattage = soup.select('section.pi-item:nth-child(6) > div:nth-child(2) > div:nth-child(2)')
                wt = wattage[0]
                wt = str(wt)
                watts = ''.join(c for c in wt if c.isdigit())
                o['Wattage'] = int(watts)

                #Counters
                in2 = 0
                count2 = len(item2c)
                print('Parsing Data...\n. . . . . . . . . .\n')
                #Getting Items with 1 Ingredients
                while True:
                    # Getting Product and output
                    p = item2c[in2]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    t = item2c[in2]
                    t = t.parent
                    t = t.parent
                    t = t.find('td')
                    t = t.find_next('td')
                    t = t.find_next('td')
                    t = t.find_next(class_="item-minute")
                    t = t.find_next(class_="item-num")
                    t_num = t.string
                    if t_num == '1,500':
                        t_num = 1500
                    t_num = float(t_num)

                    # Getting Names of Ingredients
                    items_n = item2c[in2 - 1]
                    items_n = items_n.find(class_='recipe-item')
                    items_n = items_n.find_next(class_='item-name')
                    ing1 = items_n.string

                    # Getting Input Times
                    items_t = item2c[in2]
                    items_t = items_t.parent
                    items_t = items_t.parent
                    items_t = items_t.find('td')
                    items_t = items_t.find_next('td')
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    if inum1 == '1,500':
                        inum1 = 1500
                    inum1 = float(inum1)

                    # Adding items to Dictionary
                    o['Ingredient1'] = ing1
                    o['Input1'] = inum1
                    o['Product'] = p
                    o['Output'] = t_num
                    o['fn'] = o.get('Product') + o.get('json')

                    # Write to file
                    filepath = recipe_path / o['fn']
                    with open(filepath, 'w') as write:
                        o.pop('fn', None)
                        o.pop('json', None)
                        json.dump(o, write)
                    # Moving to next item
                    in2 = in2 + 1
                    if in2 != count2: o['json'] = '.json'
                    if in2 == count2: break
                print("Constructor's Successfully Obtained.\n")
                upcheck3 = 1
        if upcheck4 == 0:
            if old_check["4"] != checker["4"]:
                machinecounter = 4
                #Smelters
                #Setting up Dictionary
                o =dict()
                o['ID'] = 'Smelter'
                o['Ingredient1'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Output'] = 0
                o['Wattage'] = 0
                o['json'] = '.json'

                #Grabbing Webpage
                print('Connecting to Smelter webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Smelter')
                soup = BeautifulSoup(page.content, 'html.parser')
                item2c = soup.find_all(attrs={'data-items-count': '1'})
                wattage = soup.select('section.pi-item:nth-child(6) > div:nth-child(2) > div:nth-child(2)')
                wt = wattage[0]
                watts = wt.string
                cleanwatts = ''.join(c for c in watts if c.isdigit())
                o['Wattage'] = int(cleanwatts)

                #Counters
                in2 = 0
                count2 = len(item2c)
                print('Parsing Data...\n. . . . . . . . . .\n')
                #Getting Items with 1 Ingredients
                while True:
                    # Getting Product and output
                    p = item2c[in2]
                    p = p.parent
                    p = p.parent
                    p = p.find('td')
                    p = p.contents
                    p = p[0]
                    t = item2c[in2]
                    t = t.parent
                    t = t.parent
                    t = t.find('td')
                    t = t.find_next('td')
                    t = t.find_next('td')
                    t = t.find_next(class_="item-minute")
                    t = t.find_next(class_="item-num")
                    t_num = t.string
                    t_num = float(t_num)

                    # Getting Names of Ingredients
                    items_n = item2c[in2 - 1]
                    items_n = items_n.find(class_='recipe-item')
                    items_n = items_n.find_next(class_='item-name')
                    ing1 = items_n.string

                    # Getting Input Times
                    items_t = item2c[in2]
                    items_t = items_t.parent
                    items_t = items_t.parent
                    items_t = items_t.find('td')
                    items_t = items_t.find_next('td')
                    items_t = items_t.find(class_='item-minute')
                    items_t = items_t.find_next(class_="item-num")
                    inum1 = items_t.string
                    if inum1 == '1,500':
                        inum1 = 1500
                    inum1 = float(inum1)

                    # Adding items to Dictionary
                    o['Ingredient1'] = ing1
                    o['Input1'] = inum1
                    o['Product'] = p
                    o['Output'] = t_num
                    o['fn'] = o.get('Product') + o.get('json')

                    # Write to file
                    filepath = recipe_path / o['fn']
                    with open(filepath, 'w') as write:
                        o.pop('fn', None)
                        o.pop('json', None)
                        json.dump(o, write)
                    # Moving to next item
                    in2 = in2 + 1
                    if in2 != count2: o['json'] = '.json'
                    if in2 == count2: break
                print("Smelter's Successfully Obtained.\n")
                upcheck4 = 1
        if upcheck5 == 0:
            if old_check["5"] != checker["5"]:
                machinecounter = 5
                #Refinery
                #Setting up Dictionaries
                i = dict()
                i['ID'] = 'Refinery'
                i['Ingredient1'] = None
                i['Ingredient2'] = None
                i['Product'] = None
                i['Product2'] = None
                i['Input1'] = 0
                i['Input2'] = 0
                i['Output'] = 0
                i['Output2'] = 0
                i['Wattage'] = 30
                i['json'] = '.json'
                o = dict()
                o['ID'] = 'Refinery'
                o['Ingredient1'] = None
                o['Ingredient2'] = None
                o['Product'] = None
                o['Input1'] = 0
                o['Input2'] = 0
                o['Output'] = 0
                o['Wattage'] = 30
                o['json'] = '.json'
                r = dict()
                r['ID'] = 'Refinery'
                r['Ingredient1'] = None
                r['Product'] = None
                r['Product2'] = None
                r['Input1'] = 0
                r['Output'] = 0
                r['Output2'] = 0
                r['Wattage'] = 30
                r['json'] = '.json'
                e = dict()
                e['ID'] = 'Refinery'
                e['Ingredient1'] = None
                e['Product'] = None
                e['Input1'] = 0
                e['Output'] = 0
                e['Wattage'] = 30
                e['json'] = '.json'

                #Grabbing Webpage
                print('Connecting to Refinery webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Refinery')
                soup = BeautifulSoup(page.content, 'html.parser')
                item1c = soup.find('tbody')
                item1c = item1c.find_all('tr')

                #Counters
                count1 = len(item1c) - 1
                i1 = 0
                check_o2 = 0
                check_i2 = 0
                while True:
                    #Grabbing Products + Ingredients\Times
                    p = item1c[i1 + 1]
                    p2 = p.find_next('td')          #Setting Second Product
                    p2 = p2.find_next('td')
                    p2 = p2.find_next('td')
                    p2 = p2.find_next('td')
                    p2 = p2.find(attrs={'data-items-count': '2'})
                    p = p.find_next('td')
                    in1 = p.find_next('td')                             #Setting First Ingredient
                    in2 = in1.find(attrs={'data-items-count': '2'})       #Setting Second Ingredient
                    t1 = p.find_next('td')
                    t1 = t1.find_next('td')
                    t1 = t1.find_next('td')
                    t1 = t1.find_next(class_='item-minute')
                    t1 = t1.find_next(class_='item-num')
                    t1_num = t1.string
                    t1_num = float(t1_num)
                    p = p.contents
                    p = p[0]
                    if p2 is not None:
                        p2 = p2.find_next('div')
                        p2 = p2.find_next('div')
                        p2 = p2.find_next(class_='item-name')
                        t2 = p2.find_next(class_='item-minute')
                        t2 = t2.find_next(class_='item-num')
                        p2 = p2.contents
                        p2 = p2[0]
                        t2_num = t2.string

                        t2_num = float(t2_num)
                        check_o2 = 1
                    in1 = in1.find_next(class_='item-name')
                    in1_t = in1.find_next(class_='item-minute')
                    in1_t = in1_t.find_next(class_='item-num')
                    in1 = in1.contents
                    in1 = in1[0]
                    in1_tnum = in1_t.string
                    in1_tnum = float(in1_tnum)
                    in1_t = in1_tnum
                    if in2 is not None:
                        in2 = in2.find_next(class_='recipe-item')
                        in2 = in2.find_next(class_='recipe-item')
                        in2 = in2.find_next(class_='item-name')
                        in2_t = in2.find_next(class_='item-minute')
                        in2_t = in2_t.find_next(class_='item-num')
                        in2 = in2.contents
                        in2 = in2[0]
                        in2_tnum = in2_t.string
                        in2_tnum = float(in2_tnum)
                        in2_t = in2_tnum
                        check_i2 = 1
                    if check_o2 == 1 and check_i2 == 1:
                        i['Product'] = p
                        i['Product2'] = p2
                        i['Output'] = t1_num
                        i['Output2'] = t2_num
                        i['Ingredient1'] = in1
                        i['Ingredient2'] = in2
                        i['Input1'] = in1_t
                        i['Input2'] = in2_t
                        i['fn'] = i.get('Product') + i.get('json')
                        filepath = recipe_path / i['fn']
                        with open(filepath, 'w') as write:
                            i.pop('fn', None)
                            i.pop('json', None)
                            json.dump(i, write)
                    elif check_o2 == 1 and check_i2 == 0:
                        r['Product'] = p
                        r['Product2'] = p2
                        r['Output'] = t1_num
                        r['Output2'] = t2_num
                        r['Ingredient1'] = in1
                        r['Input1'] = in1_t
                        r['fn'] = r.get('Product') + r.get('json')
                        filepath = recipe_path / r['fn']
                        with open(filepath, 'w') as write1:
                            r.pop('fn', None)
                            r.pop('json', None)
                            json.dump(r, write1)
                    elif check_o2 == 0 and check_i2 == 1:
                        o['Product'] = p
                        o['Output'] = t1_num
                        o['Ingredient1'] = in1
                        o['Ingredient2'] = in2
                        o['Input1'] = in1_t
                        o['Input2'] = in2_t
                        o['fn'] = o.get('Product') + o.get('json')
                        filepath = recipe_path / o['fn']
                        with open(filepath, 'w') as write2:
                            o.pop('fn', None)
                            o.pop('json', None)
                            json.dump(o, write2)
                    elif check_o2 == 0 and check_i2 == 0:
                        e['Product'] = p
                        e['Output'] = t1_num
                        e['Ingredient1'] = in1
                        e['Input1'] = in1_t
                        e['fn'] = e.get('Product') + e.get('json')
                        filepath = recipe_path / e['fn']
                        with open(filepath, 'w') as write3:
                            e.pop('fn', None)
                            e.pop('json', None)
                            json.dump(e, write3)
                        # Moving to next item
                    i1 = i1 + 1
                    if i1 != count1: i['json'] = '.json'
                    if i1 != count1: o['json'] = '.json'
                    if i1 != count1: r['json'] = '.json'
                    if i1 != count1: e['json'] = '.json'
                    check_o2 = 0
                    check_i2 = 0
                    if i1 == count1: break
                print("Refinery's Successfully Added...\n")
                upcheck5 = 1
        if upcheck6 == 0:
            if old_check["6"] != checker["6"]:
                machinecounter = 6
                print('Connecting to Blender webpage. . . .\n\n. . . . . . . . . .\n')
                in3out2 = dict()
                in3out2['ID'] = 'Blender'
                in3out2['Product'] = None
                in3out2['Product2'] = None
                in3out2['Ingredient1']= None
                in3out2['Ingredient2'] = None
                in3out2['Ingredient3'] = None
                in3out2['Input1'] = None
                in3out2['Input2'] = None
                in3out2['Input3'] = None
                in3out2['Output'] = None
                in3out2['Output2'] = None
                in3out2['Wattage'] = 75
                in3out1 = dict()
                in3out1['ID'] = 'Blender'
                in3out1['Product'] = None
                in3out1['Ingredient1'] = None
                in3out1['Ingredient2'] = None
                in3out1['Ingredient3'] = None
                in3out1['Input1'] = None
                in3out1['Input2'] = None
                in3out1['Input3'] = None
                in3out1['Output'] = None
                in3out1['Wattage'] = 75
                in4out1 = dict()
                in4out1['ID'] = 'Blender'
                in4out1['Product'] = None
                in4out1['Ingredient1'] = None
                in4out1['Ingredient2'] = None
                in4out1['Ingredient3'] = None
                in4out1['Ingredient4'] = None
                in4out1['Input1'] = None
                in4out1['Input2'] = None
                in4out1['Input3'] = None
                in4out1['Input4'] = None
                in4out1['Output'] = None
                in4out1['Wattage'] = 75
                in4out2 = dict()
                in4out2['ID'] = 'Blender'
                in4out2['Product'] = None
                in4out2['Product2'] = None
                in4out2['Ingredient1'] = None
                in4out2['Ingredient2'] = None
                in4out2['Ingredient3'] = None
                in4out2['Ingredient4'] = None
                in4out2['Input1'] = None
                in4out2['Input2'] = None
                in4out2['Input3'] = None
                in4out2['Input4'] = None
                in4out2['Output'] = None
                in4out2['Output2'] = None
                in4out2['Wattage'] = 75
                in2out1 = dict()
                in2out1['ID'] = 'Blender'
                in2out1['Product'] = None
                in2out1['Ingredient1'] = None
                in2out1['Ingredient2'] = None
                in2out1['Input1'] = None
                in2out1['Input2'] = None
                in2out1['Output'] = None
                in2out1['Wattage'] = 75
                in2out2 = dict()
                in2out2['ID'] = 'Blender'
                in2out2['Product'] = None
                in2out2['Product2'] = None
                in2out2['Ingredient1'] = None
                in2out2['Ingredient2'] = None
                in2out2['Input1'] = None
                in2out2['Input2'] = None
                in2out2['Output'] = None
                in2out2['Output2'] = None
                in2out2['Wattage'] = 75
                page = requests.get('https://satisfactory.wiki.gg/wiki/Blender')
                soup = BeautifulSoup(page.content, 'html.parser')
                find = soup.find('tbody')
                find = find.find_all('tr')

                count1 = len(find) - 1
                count2 = 0
                while True:
                    item = find[count2 + 1]

                    product = item.find_next('td').text
                    product = product.replace('Alternate', '')
                    input1 = item.find_next('td')
                    input1 = input1.find_next('td')
                    incheck2 = input1.find(attrs={'data-items-count': '2'})
                    incheck3 = input1.find(attrs={'data-items-count': '3'})
                    incheck4 = input1.find(attrs={'data-items-count': '4'})
                    if incheck2 is not None:
                        output = input1.find_next('td')
                        output = output.find_next('td')
                        outcheck2 = output.find(attrs={'data-items-count': '2'})
                        outcheck1 = output.find(attrs={'data-items-count': '1'})
                        if outcheck2 is not None:
                            ing = input1.find_next(class_='item-name')
                            in2out2['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in2out2['Ingredient2'] = ing.text
                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out2['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out2['Input2'] = t1

                            out = output.find_next(class_='item-name')
                            in2out2['Product'] = out.text
                            out = out.find_next(class_='item-name')
                            in2out2['Product2'] = out.text
                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out2['Output'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out2['Output2'] = t1
                            in2out2['fn'] = product + '.json'
                            filepath = recipe_path / in2out2['fn']
                            with open(filepath, 'w') as write:
                                in2out2.pop('fn', None)
                                json.dump(in2out2, write)
                        if outcheck1 is not None:
                            ing = input1.find_next(class_='item-name')
                            in2out1['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in2out1['Ingredient2'] = ing.text
                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out1['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out1['Input2'] = t1

                            out = output.find_next(class_='item-name')
                            in2out1['Product'] = out.text
                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in2out1['Output'] = t1
                            in2out1['fn'] = product + '.json'
                            filepath = recipe_path / in2out1['fn']
                            with open(filepath, 'w') as write:
                                in2out1.pop('fn', None)
                                json.dump(in2out1, write)
                    if incheck3 is not None:
                        output = input1.find_next('td')
                        output = output.find_next('td')
                        outcheck2 = output.find(attrs={'data-items-count': '2'})
                        outcheck1 = output.find(attrs={'data-items-count': '1'})
                        if outcheck2 is not None:
                            ing = input1.find_next(class_='item-name')
                            in3out2['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in3out2['Ingredient2'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in3out2['Ingredient3'] = ing.text

                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out2['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out2['Input2'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out2['Input3'] = t1

                            out = output.find_next(class_='item-name')
                            in3out2['Product'] = out.text
                            out = out.find_next(class_='item-name')
                            in3out2['Product2'] = out.text

                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out2['Output'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out2['Output2'] = t1
                            in3out2['fn'] = product + '.json'
                            filepath = recipe_path / in3out2['fn']
                            with open(filepath, 'w') as write:
                                in3out2.pop('fn', None)
                                json.dump(in3out2, write)
                        if outcheck1 is not None:
                            ing = input1.find_next(class_='item-name')
                            in3out1['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in3out1['Ingredient2'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in3out1['Ingredient3'] = ing.text

                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out1['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out1['Input2'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out1['Input3'] = t1

                            out = output.find_next(class_='item-name')
                            in3out1['Product'] = out.text
                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in3out1['Output'] = t1
                            in3out1['fn'] = product + '.json'
                            filepath = recipe_path / in3out1['fn']
                            with open(filepath, 'w') as write:
                                in3out1.pop('fn', None)
                                json.dump(in3out1, write)
                    if incheck4 is not None:
                        output = input1.find_next('td')
                        output = output.find_next('td')
                        outcheck2 = output.find(attrs={'data-items-count': '2'})
                        outcheck1 = output.find(attrs={'data-items-count': '1'})
                        if outcheck2 is not None:
                            ing = input1.find_next(class_='item-name')
                            in4out2['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out2['Ingredient2'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out2['Ingredient3'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out2['Ingredient4'] = ing.text

                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Input2'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Input3'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Input4'] = t1

                            out = output.find_next(class_='item-name')
                            in4out2['Product'] = out.text
                            out = out.find_next(class_='item-name')
                            in4out2['Product2'] = out.text
                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Output'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out2['Output2'] = t1
                            in4out2['fn'] = product + '.json'
                            filepath = recipe_path / in4out2['fn']
                            with open(filepath, 'w') as write:
                                in4out2.pop('fn', None)
                                json.dump(in4out2, write)
                        if outcheck1 is not None:
                            ing = input1.find_next(class_='item-name')
                            in4out1['Ingredient1'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out1['Ingredient2'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out1['Ingredient3'] = ing.text
                            ing = ing.find_next(class_='item-name')
                            in4out1['Ingredient4'] = ing.text
                            t = input1.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out1['Input1'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out1['Input2'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out1['Input3'] = t1
                            t = t.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out1['Input4'] = t1
                            out = output.find_next(class_='item-name')
                            in4out1['Product'] = out.text
                            t = output.find_next(class_='item-minute')
                            t1 = t.text
                            t1 = ''.join(c for c in t1 if c.isdigit())
                            t1 = int(t1)
                            in4out1['Output'] = t1
                            in4out1['fn'] = product + '.json'
                            filepath = recipe_path / in4out1['fn']
                            with open(filepath, 'w') as write:
                                in4out1.pop('fn', None)
                                json.dump(in4out1, write)
                    incheck2 = None
                    incheck3 = None
                    incheck4 = None
                    outcheck1 = None
                    outcheck2 = None
                    outcheck3 = None
                    count2 = count2 + 1
                    if count2 == count1:
                        break
                print("Blender's Successfully Added...\n")
                upcheck6 = 1
        if rawcheck == 0:
            #Raw Materials
            print('Adding Raw Materials. . .')
            r = dict()
            r['Product'] = 'Alien Protein'
            r['ID'] = 'Raw'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Leaves'
            r['ID'] = 'Raw'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Mycelia'
            r['ID'] = 'Raw'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Iron Ore'
            r['ID'] = 'Raw'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Wood'
            r['ID'] = 'Raw'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Caterium Ore'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Copper Ore'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Limestone'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Coal'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Raw Quartz'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Sulfur'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Uranium'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Bauxite'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'SAM'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Crude Oil'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Water'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Nitrogen Gas'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            r['Product'] = 'Biomass'
            r['Raw'] = None
            r['Output'] = 1
            r['json'] = '.json'
            r['fn'] = r.get('Product') + r.get('json')
            filepath = recipe_path / r['fn']
            with open(filepath, 'w') as write:
                r.pop('fn')
                r.pop('json')
                json.dump(r, write)
            print("\nRaw Materials Added.")
            rawcheck = 1
        if upcheck7 == 0:
            if old_check["7"] != checker["7"]:
                machinecounter = 7
                # Grabbing Webpage
                print('\nConnecting to Particle webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Particle_Accelerator')
                soup = BeautifulSoup(page.content, 'html.parser')
                item1c = soup.find('tbody')
                item1c = item1c.find_all('tr')
                counter = 1
                check1 = False
                check2 = False
                check3 = False
                dict1 = dict()
                dict2 = dict()
                dict3 = dict()
                dict1["ID"] = "Particle"
                dict2["ID"] = "Particle"
                dict3["ID"] = "Particle"
                dict1["Wattage"] = 1000
                dict2["Wattage"] = 1000
                dict3["Wattage"] = 1000
                while True:
                    try:
                        item = item1c[counter]
                    except IndexError:
                        break

                    product = item.find_all(attrs={'data-items-count': '3'})
                    if len(product) > 0:
                        check3 = True
                    if not product:
                        product = item.find_all(attrs={'data-items-count': '2'})
                        if len(product) > 0:
                            check2 = True
                        if not product:
                            product = item.find_all(attrs={'data-items-count': '1'})
                            check1 = True
                    if check1:
                        item1 = product[0].a  # Ingredient 1
                        item1num = product[0].find_next(class_="item-minute").text  # Input 1
                        item1num = item1num.replace('m3', '')
                        item1num = ''.join(c for c in item1num if c.isdigit())
                        productnum = product[0].find_next(class_="item-minute")  # Output
                        productnum = productnum.find_next(class_="item-minute").text
                        productnum = ''.join(c for c in productnum if c.isdigit())
                        product = product[0].a  # Product
                        item1 = item1.find_next(class_='item-name').text
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name').text
                        dict1["Ingredient1"] = item1
                        dict1['Product'] = product
                        dict1['Input1'] = item1num
                        dict1['Output'] = productnum
                        filepath = Path("Recipes")
                        product = product + '.json'
                        filepath = filepath / product
                        with open(filepath, 'w') as write:  # Writing Recipe
                            json.dump(dict1, write)
                        counter = counter + 1
                        check1 = False
                    if check2:
                        item1 = product[0].a
                        item1num = product[0].find_next(class_="item-minute").text  # Input 1
                        item1num = item1num.replace('m3', '')
                        item1num = ''.join(c for c in item1num if c.isdigit())
                        item2 = product[0].a
                        item2num = product[0].find_next(class_="item-minute")  # Input 2
                        item2num = item2num.find_next(class_="item-minute").text
                        item2num = item2num.replace('m3', '')
                        item2num = ''.join(c for c in item2num if c.isdigit())
                        productnum = product[0].find_next(class_="item-minute")  # Output
                        proudctnum = productnum.find_next(class_="item-minute")
                        productnum = productnum.find_next(class_="item-minute").text
                        productnum = ''.join(c for c in productnum if c.isdigit())
                        product = product[0].a
                        item1 = item1.find_next(class_='item-name').text
                        item2 = item2.find_next(class_='item-name')
                        item2 = item2.find_next(class_='item-name').text
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name').text
                        dict2['Ingredient1'] = item1
                        dict2['Ingredient2'] = item2
                        dict2['Prodcut'] = product
                        dict2['Input1'] = item1num
                        dict2['Input2'] = item2num
                        dict2['Output'] = productnum
                        filepath = Path("Recipes")
                        product = product + '.json'
                        filepath = filepath / product
                        with open(filepath, 'w') as write:
                            json.dump(dict2, write)
                        counter = counter + 1
                        check2 = False
                    if check3:
                        item1 = product[0].a
                        item1num = product[0].find_next(class_="item-minute").text  # Input 1
                        item1num = item1num.replace('m3', '')
                        item1num = ''.join(c for c in item1num if c.isdigit())
                        item2 = product[0].a
                        item2num = product[0].find_next(class_="item-minute")  # Input 2
                        item2num = item2num.find_next(class_="item-minute").text
                        item2num = item2num.replace('m3', '')
                        item2num = ''.join(c for c in item2num if c.isdigit())
                        item3 = product[0].a
                        item3num = product[0].find_next(class_="item-minute")  # Input 3
                        item3num = item3num.find_next(class_="item-minute")
                        item3num = item3num.find_next(class_="item-minute").text
                        item3num = item3num.replace('m3', '')
                        item3num = ''.join(c for c in item3num if c.isdigit())
                        productnum = product[0].find_next(class_="item-minute")  # Output
                        productnum = productnum.find_next(class_="item-minute")
                        productnum = productnum.find_next(class_="item-minute")
                        productnum = productnum.find_next(class_="item-minute").text
                        productnum = productnum.replace('m3', '')
                        productnum = ''.join(c for c in productnum if c.isdigit())
                        product = product[0].a
                        item1 = item1.find_next(class_='item-name').text
                        item2 = item2.find_next(class_='item-name')
                        item2 = item2.find_next(class_='item-name').text
                        item3 = item3.find_next(class_='item-name')
                        item3 = item3.find_next(class_='item-name')
                        item3 = product.find_next(class_='item-name').text
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name')
                        product = product.find_next(class_='item-name').text
                        dict3['Ingredient1'] = item1
                        dict3['Ingredient2'] = item2
                        dict3['Ingredient3'] = item3
                        dict3['Input1'] = item1num
                        dict3['Input2'] = item2num
                        dict3['Input3'] = item3num
                        dict3['Product'] = product
                        dict3['Output'] = productnum
                        filepath = Path("Recipes")
                        product = product + '.json'
                        filepath = filepath / product
                        with open(filepath, 'w') as write:
                            json.dump(dict3, write)
                        counter = counter + 1
                        check3 = False
                print('Particles Successfully Obtained.\n')
                upcheck7 = 1
        if upcheck8 == 0:
            if old_check["8"] != checker["8"]:
                machinecounter = 8
                print('Connecting to Quantum Encoder webpage. . . .\n\n. . . . . . . . . .\n')
                page = requests.get('https://satisfactory.wiki.gg/wiki/Quantum_Encoder')
                soup = BeautifulSoup(page.content, 'html.parser')
                item1c = soup.find('tbody')
                item1c = item1c.find_all('tr')
                counter = 1
                dict1 = dict()
                dict1["ID"] = 'Quantum'
                dict1["Wattage"] = 2000
                while True:
                    try:
                        item = item1c[counter]
                    except IndexError:
                        break
                    product = item.find_next(attrs={'data-items-count': '2'})
                    outputs = product.find_all(class_='item-num')
                    product = product.find_all(class_='item-name')
                    prod1 = product[0].text
                    prod2 = product[1].text
                    out1 = outputs[1].text
                    out2 = outputs[3].text
                    ingredients = item.find_next(attrs={'data-items-count': '4'})
                    inputs = ingredients.find_all(class_='item-num')
                    ingredients = ingredients.find_all(class_='item-name')
                    ing1 = ingredients[0].text
                    ing2 = ingredients[1].text
                    ing3 = ingredients[2].text
                    ing4 = ingredients[3].text
                    input1 = inputs[1].text
                    input2 = inputs[3].text
                    input3 = inputs[5].text
                    input4 = inputs[7].text
                    dict1["Product"] = prod1
                    dict1["Product2"] = prod2
                    dict1["Output"] = out1
                    dict1["Output2"] = out2
                    dict1["Ingredient1"] = ing1
                    dict1["Ingredient2"] = ing2
                    dict1["Ingredient3"] = ing3
                    dict1["Ingredient4"] = ing4
                    dict1["Input1"] = input1
                    dict1["Input2"] = input2
                    dict1["Input3"] = input3
                    dict1["Input4"] = input4
                    filepath = Path('Recipes')
                    if prod1 == "Power Shard":
                        prod1 = "Synthetic " + prod1
                    name = prod1 + '.json'
                    filepath = filepath / name
                    with open(filepath, 'w') as write:
                        json.dump(dict1, write)
                    counter = counter + 1
                print('Quantum Encoder Successfully Obtained.\n')
                upcheck8 = 1
        print("\n\nAll Recipes Successfully Added.")
        with open (checkpath, 'w') as write:
            json.dump(checker, write)
        break
    except:
        print("Connection Severed. Trying Again(",machinelist[machinecounter],"). . . .\n")
        time.sleep(3)
        continue