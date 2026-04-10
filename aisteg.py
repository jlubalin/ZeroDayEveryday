from dotenv import load_dotenv
from google import genai
import os
import subprocess
import stegjpeg

# The client automatically looks for the GEMINI_API_KEY environment variable
load_dotenv()

client = genai.Client()

# Make the request to the free Gemini model
response = client.models.generate_content(
    # gemini-2.5-pro is also an option
    model="gemini-2.5-flash",
    contents="Write a C program to play blackjack. Just the code, no extra text"
)

# Print the response
ans : str = response.text
ans = ans[4:-3]
#ans.replace("\n", "\\n")
#print(ans)

with open("code.c", "w") as f:
    f.write(ans)

compile_process = subprocess.run(
    ["gcc", "code.c", "-o", "aibin"], 
    capture_output=True, 
    text=True
)

if compile_process.returncode != 0:
    print("Compilation failed")
    print(compile_process.stderr)
else:
    print("Success")
    os.remove("code.c")

with open("aibin", "rb") as f:
    binary = f.read()

binary_str = str(binary)
binary_str = binary_str[2:-1]

stegjpeg.encrypt("pizza.jpg", binary_str, "stegai.png")