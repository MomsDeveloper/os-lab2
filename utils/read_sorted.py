filename = "./output/ema-with-cache/file.bin"

symbol_storage = 4

with open(filename, 'rb') as f:
    for i in range(100):
        print(int.from_bytes(f.read(symbol_storage), 'little'))

