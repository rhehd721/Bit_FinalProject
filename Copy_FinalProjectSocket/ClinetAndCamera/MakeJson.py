import json

file_path = "./sample.json"

Item_Name = "hello"
Amount = "3"
ImagePath = "./sds"
Item_place = "3-3"

data = {}
data['Item_Name'] = Item_Name
data['Amount'] = Amount
data['time'] = "now()"
data['ImagePath'] = ImagePath
data['Item_place'] = Item_place

print(data)

with open(file_path, 'w') as outfile:
    json.dump(data, outfile)