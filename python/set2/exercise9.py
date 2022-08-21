def pkcs7_padding(message, fixed_length):
    if len(message) % fixed_length == 0:
        return message
    padding = fixed_length - (len(message) % fixed_length)
    pad_value = hex(padding).lstrip('0x')
    if len(pad_value) == 1:
        pad_value = '0' + pad_value
    pad_value = bytes.fromhex(pad_value)
    padded_string = message + b''.join(pad_value for _ in range(padding))

    return padded_string


def exercise9():
    message = b'YELLOW SUBMARINE'
    fixed_length = 20
    padded_message = pkcs7_padding(message, fixed_length)

    print("------EXERCISE 9------")
    print("Message:", message)
    print("Padded Message:", padded_message)
