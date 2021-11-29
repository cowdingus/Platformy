import json

if __name__ == '__main__':
    width = 8
    height = 12
    id = 0

    block_descriptions = [];

    for y in range(height):
        for x in range(width):
            block_descriptions.append({'id': id, 'name': '', 'atlasIndex': 0, 'textureRect': [x * 8, y * 8,8,8]})
            id += 1

    print(json.dumps(block_descriptions))
