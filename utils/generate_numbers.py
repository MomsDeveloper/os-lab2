from pathlib import Path
from random import randint
import sys

if len(sys.argv) != 3:
    print("Usage: python generate_data.py <max_storage_MB> <file_name>")
    sys.exit(1)

filename = Path(sys.argv[2])
# max storage in MB
max_storage_mb = int(sys.argv[1])
# symbol storage in bytes
symbol_storage = 4

num_integers = (max_storage_mb * 1024 * 1024) // symbol_storage

print('Generating {} random integers'.format(num_integers))
with open(filename, 'wb+') as f:
    for _ in range(num_integers):
        random_number = randint(1, 2**16 - 1)
        f.write(random_number.to_bytes(symbol_storage, 'little'))

print(f'File {filename} was created')

# print('Reading the generated file')
# with open(filename, 'rb') as f:
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
