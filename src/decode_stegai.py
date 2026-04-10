import stegjpeg
import codecs
import subprocess

binaryAsStr = stegjpeg.decrypt("stegai.png")

binary = codecs.decode(binaryAsStr, 'unicode_escape').encode('latin1')

with open("decoded_ai", "wb") as f:
    f.write(binary)

print("Successfully decoded to decoded_ai")

subprocess.run(["./decoded_ai"])