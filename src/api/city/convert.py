import json

with open('city.list.json', encoding='utf-8') as f:
    cities = json.load(f)

with open('city_map.h', 'w', encoding='utf-8') as f:
    f.write('#include <unordered_map>\n')
    f.write('#include <string>\n\n')
    f.write('struct CityInfo {\n')
    f.write('    float lat;\n')
    f.write('    float lon;\n')
    f.write('    long long id;\n')
    f.write('};\n\n')
    f.write('static const std::unordered_map<std::string, CityInfo> city_map = {\n')
    for city in cities:
        name = city['name'].replace('"', '\\"')
        country = city['country']
        city_id = int(city['id'])
        lat = city['coord']['lat']
        lon = city['coord']['lon']
        key = f'{name},{country}'
        f.write(f'    {{"{key}", {{ {lat}f, {lon}f, {city_id} }} }},\n')
    f.write('};\n')