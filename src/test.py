import stegjpeg

stegjpeg.encrypt("pizza.jpg", "This is the hidden message", "steg.png")

secret = stegjpeg.decrypt("steg.png")

print("The hidden message was:", secret)