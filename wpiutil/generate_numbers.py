import os
import shutil
import sys

MAX_NUM = 20

dirname, _ = os.path.split(os.path.abspath(__file__))
cmake_binary_dir = sys.argv[1]

with open(f"{dirname}/src/generate/GenericNumber.java.in", "r") as templateFile:
    template = templateFile.read()
    rootPath = f"{cmake_binary_dir}/generated/main/java/edu/wpi/first/wpiutil/math/numbers"

    if os.path.exists(rootPath):
        shutil.rmtree(rootPath)
    os.makedirs(rootPath)

    for i in range(MAX_NUM + 1):
        f = open(f"{rootPath}/N{i}.java", "w")
        f.write(template.replace("${num}", str(i)))
        f.close()

with open(f"{dirname}/src/generate/Nat.java.in", "r") as templateFile:
    template = templateFile.read()
    outputPath = f"{cmake_binary_dir}/generated/main/java/edu/wpi/first/wpiutil/math/Nat.java"
    getterFile = open(f"{dirname}/src/generate/NatGetter.java.in", "r")
    getter = getterFile.read()
    getterFile.close()

    if os.path.exists(outputPath):
        os.remove(outputPath)

    for i in range(MAX_NUM + 1):
        template += getter.replace("${num}", str(i))

    template += "}\n"

    f = open(outputPath, "w")
    f.write(template)
    f.close()
