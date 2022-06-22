# Thanks to: https://graphicdesign.stackexchange.com/a/90833

DATA = []

with open('palette.act', 'rb') as actFile:
    for _ in range(256):
        raw = actFile.read(3)
        colour = '0x' + raw.encode('hex')
        DATA.append(colour)

with open('palette.txt', 'w') as f:
    for row in DATA:
        f.write(row + '\n')
