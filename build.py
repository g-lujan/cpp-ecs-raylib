import os

folder_names = {
	"Windows (MinGW)": "build-mingw",
	"Windows (MSVC)": "build-msvc",
}

compiler_options = {
        "Windows (MinGW)" : "-B./" + folder_names["Windows (MinGW)"] + " -G \"MinGW Makefiles\" -DWINDOWS_BUILD=ON",
        "Windows (MSVC)" : "-B./" + folder_names["Windows (MSVC)"] + " -DWINDOWS_BUILD=ON",
        "Mac"  : "", 
        "Linux" : "",
}

def pick_compiler():
    print("Please choose a build type")
    while True:
        options_list = [*compiler_options]
        for idx, element in enumerate(options_list):
            print("{}) {}".format(idx+1, element))
        try:
            choice = int(input("Enter choice: "))
            if 0 < choice <= len(options_list):
                return options_list[choice-1]
            else:
                print("Pick a valid option")
        except:
            print("Pick a valid option")

compiler_choice = pick_compiler()
cmake_command = "cmake -S. " + compiler_options[compiler_choice]
print(cmake_command)
os.system(cmake_command)
os.chdir(folder_names[compiler_choice])
os.system("cmake --build .")

