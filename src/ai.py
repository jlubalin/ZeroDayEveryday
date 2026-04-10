from dotenv import load_dotenv
from google import genai
import os
import subprocess

# The client automatically looks for the GEMINI_API_KEY environment variable
load_dotenv()

client = genai.Client()

# Make the request to the free Gemini model
response = client.models.generate_content(
    # gemini-2.5-pro is also an option
    model="gemini-3.1-flash-lite-preview",
    contents="Write a C program that prints hello world. Just the code, no extra text"
)

# Print the response
ans : str = response.text
print(ans)
if ans[0] == '`':
        ans = ans[4:-3]
#ans.replace("\n", "\\n")
print(ans)

with open("code.c", "w") as f:
    f.write(ans)

# compile_process = subprocess.run(
#     ["gcc", "code.c", "-o", "aibin"], 
#     capture_output=True, 
#     text=True
# )

# if compile_process.returncode != 0:
#     print("Compilation failed")
#     print(compile_process.stderr)
# else:
#     print("Success")
#     os.remove("code.c")