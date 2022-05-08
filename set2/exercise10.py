from base64 import b64decode

from exercise9 import pkcs7_padding


def implement_cbc_mode(ciphertext: bytes, key: bytes):
    block_length = 16
    if len(ciphertext) % block_length != 0:
        ciphertext = pkcs7_padding(ciphertext, block_length)
    init_vextor = b""



def exercise10():
    with open("files/10.txt") as f:
        lines = f.readlines()

    lines = [lines[i].rstrip() for i in range(len(lines))]

    ciphertext = "".join(lines)
    ciphertext = b64decode(ciphertext)
    key = b"YELLOW SUBMARINE"
