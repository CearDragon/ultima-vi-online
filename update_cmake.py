import re
with open("CMakeLists.txt", "r") as f:
    data = f.read()
data = data.replace("\"../include\"", "\"${CMAKE_CURRENT_SOURCE_DIR}/src/common/include\"")
data = data.replace("\"../lib\"", "\"${CMAKE_CURRENT_SOURCE_DIR}/src/common/lib\"")
data = data.replace("\"${CMAKE_CURRENT_SOURCE_DIR}/../include\"", "\"${CMAKE_CURRENT_SOURCE_DIR}/src/common/include\"")
data = data.replace("\"${CMAKE_CURRENT_SOURCE_DIR}/../lib\"", "\"${CMAKE_CURRENT_SOURCE_DIR}/src/common/lib\"")
def replace_path(match):
    path = match.group(1)
    if "client" in path or "myddraw" in path or "dmusic" in path or "sound" in path or "frame" in path or "client_bmp" in path or "new_getspr" in path or "old_getspr" in path or "Resource.h" in path or "small.ico" in path:
        return "\"src/client/" + path + "\""
    elif "host" in path:
        return "\"src/server/" + path + "\""
    else:
        return "\"src/common/" + path + "\""
data = re.sub(r"\"../(.*?)\"", replace_path, data)
data = data.replace("src/client/build/", "build/")
data = data.replace("src/common/build/", "build/")
data = data.replace("src/server/build/", "build/")
data = data.replace("src/common/../bin", "bin")
data = data.replace("src/client/../bin", "bin")
# update bin paths specifically
data = re.sub(r"\"../../bin/.*?\"", "\"build/bin/\"", data)
with open("CMakeLists.txt", "w") as f:
    f.write(data)

