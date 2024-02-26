import project_settings
import os , subprocess , sys
from pathlib import Path

args = project_settings.ProcessArguments(sys.argv)
config = project_settings.GetArgValue(args , "c" , "debug")
project = project_settings.GetArgValue(args , "p" , project_settings.EXE_NAME)
test = project_settings.GetArgValue(args , "t" , "false")

def CheckAndRunTest(test_path):
    if test_path.endswith("_tests.exe") or test_path.endswith("_test.exe"):
        print("Running: {}".format(test_path))
        
        ret = 0
        if project_settings.IsWindows():
            ret = subprocess.call(
                [
                    "cmd.exe" , "/c" , "{}\\run.bat".format(project_settings.TOOLS_DIR) , 
                    "Debug" , Path(test_path).stem ,
                ] , 
                cwd=os.getcwd()
            )
        else:
            ret = subprocess.call(["{}".format(test_path)] , cwd=subdir)
    
        if ret != 0:
            return 1

        return 0

    else:
        return -1

def RunAllTests():
    test_dir = "{}/bin/Debug"
    for subdir , dirs , files in os.walk(test_dir.format(os.getcwd())):
        for file in files:
            if CheckAndRunTest(file) > 0:
                print("Test failed: {}".format(file))
                return 1
    return 0
                
def RunTest(unit_name):
    test_path = "{}/bin/Debug/{}_tests/{}_tests.exe".format(os.getcwd() , unit_name , unit_name)
    if not os.path.exists(test_path):
        print("Test not found: {}".format(test_path))
        return 1
    return CheckAndRunTest(test_path)

if test != "false":
    if test != 0:
        sys.exit(RunTest(test))
    else:
        sys.exit(RunAllTests())

if (config == "debug"):
    config = "Debug"
    
if (config == "release"):
    config = "Release"

exepath = "{}/bin/{}/{}".format(os.getcwd() , config , project_settings.EXE_NAME)
ret = 0

if project_settings.IsWindows():
    print("Running: {}\\run.bat {} {}".format(project_settings.TOOLS_DIR , config , project))
    ret = subprocess.call(
        [
            "cmd.exe" , "/c" , "{}\\run.bat".format(project_settings.TOOLS_DIR) , 
            config , project ,
        ] , 
        cwd=os.getcwd()
    )
else:
    ret = subprocess.call(["{}".format(exepath)] , cwd=exepath)

sys.exit(ret)
