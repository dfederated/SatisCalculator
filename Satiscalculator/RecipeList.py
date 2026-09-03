import os
import json
from pathlib import Path

recipePath = Path("Recipes")
filepath = recipePath / "RecipeList.json"
foundrypath = recipePath / "foundry.json"
constructorpath = recipePath / "constructor.json"
assemblerpath = recipePath / "assembler.json"
manufacturerpath = recipePath / "manufacturer.json"
refinerypath = recipePath / "refinery.json"
blenderpath = recipePath / "blender.json"
smelterpath = recipePath / "smelter.json"
particlepath = recipePath / "particles.json"
quantumpath = recipePath / "quantum.json"
empty = dict()
empty['0'] = 0
recipes = os.listdir(recipePath)
if "RecipeList.json" in recipes:
    recipes.remove("RecipeList.json")
if "foundry.json" in recipes:
    recipes.remove("foundry.json")
if "constructor.json" in recipes:
    recipes.remove("constructor.json")
if "assembler.json" in recipes:
    recipes.remove("assembler.json")
if "manufacturer.json" in recipes:
    recipes.remove("manufacturer.json")
if "refinery.json" in recipes:
    recipes.remove("refinery.json")
if "blender.json" in recipes:
    recipes.remove("blender.json")
if "smelter.json" in recipes:
    recipes.remove("smelter.json")
if "particles.json" in recipes:
    recipes.remove("particles.json")
if "quantum.json" in recipes:
    recipes.remove("quantum.json")
rawcheck = dict()
constructorDict = dict()
assemblerDict = dict()
manufacturerDict = dict()
refineryDict = dict()
blenderDict = dict()
smelterDict = dict()
particleDict = dict()
quantumDict = dict()
foundryDict = dict()
counter = 0
foundrycount = 0
constructorcount = 0
assemblercount = 0
manufacturercount = 0
refinerycount = 0
blendercount = 0
smeltercount = 0
particlecount = 0
quantumcount = 0
raw = dict()
raw['ID'] = None
if len(recipes) != 0:
    for r in recipes:
        rawcheck = recipePath / recipes[counter]
        with open(rawcheck, 'r') as read:
            raw = json.load(read)
        if raw['ID'] == 'Foundry':
            foundryDict[foundrycount] = r.replace('.json', '')
            foundrycount += 1
            raw['ID'] = None
        with open(foundrypath, 'w') as write:
            json.dump(foundryDict, write)
        if raw['ID'] == 'Constructor':
            constructorDict[constructorcount] = r.replace('.json', '')
            constructorcount += 1
            raw['ID'] = None
        with open(constructorpath, 'w') as write:
            json.dump(constructorDict, write)
        if raw['ID'] == 'Assembler':
            assemblerDict[assemblercount] = r.replace('.json', '')
            assemblercount += 1
            raw['ID'] = None
        with open(assemblerpath, 'w') as write:
            json.dump(assemblerDict, write)
        if raw['ID'] == 'Manufacturer':
            manufacturerDict[manufacturercount] = r.replace('.json', '')
            manufacturercount += 1
            raw['ID'] = None
            with open(manufacturerpath, 'w') as write:
                json.dump(manufacturerDict, write)
        if raw['ID'] == 'Refinery':
            refineryDict[refinerycount] = r.replace('.json', '')
            refinerycount += 1
            raw['ID'] = None
            with open(refinerypath, 'w') as write:
                json.dump(refineryDict, write)
        if raw['ID'] == 'Blender':
            blenderDict[blendercount] = r.replace('.json', '')
            blendercount += 1
            raw['ID'] = None
            with open(blenderpath, 'w') as write:
                json.dump(blenderDict, write)
        if raw['ID'] == 'Smelter':
            smelterDict[smeltercount] = r.replace('.json', '')
            smeltercount += 1
            raw['ID'] = None
            with open(smelterpath, 'w') as write:
                json.dump(smelterDict, write)
        if raw['ID'] == 'Particle':
            particleDict[particlecount] = r.replace('.json', '')
            particlecount += 1
            raw['ID'] = None
            with open(particlepath, 'w') as write:
                json.dump(particleDict, write)
        if raw['ID'] == 'Quantum':
            quantumDict[quantumcount] = r.replace('.json', '')
            quantumcount += 1
            raw['ID'] = None
            with open(quantumpath, 'w') as write:
                json.dump(quantumDict, write)
        counter = counter + 1
    if len(recipes) == 0:
        with open(filepath, 'w') as write:
            json.dump(empty, write)
