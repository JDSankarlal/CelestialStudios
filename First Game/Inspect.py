# Written by Emilian Cioca.
# Free for use by anyone obtaining this file.
#
# Requires Resharper's Command Line Tools https://www.jetbrains.com/resharper/features/command-line.html
#
# Usage:
#   Arg1 - Path to inspectcode.x86.exe
#   Arg2 - Path to .sln File
#   Arg3 - The name of the project in the solution to inspect
#   Arg4 - Output directory

import sys
import re
import subprocess

line_regex = re.compile(r'TypeId="(\S+)" File="(\S+)" Offset="(?:...+?)" Line="(\d+)" Message="(...+?)"')
black_list = [
    "CppCompilerErrors",
    "CppMoveOperationWithoutNoexceptSpecification",
    "CppParameterMayBeConst",
    "CppLocalVariableMayBeConst",
    "CppDefaultCaseNotHandledInSwitchStatement",
    "CppZeroConstantCanBeReplacedWithNullptr"
]


def main():
    inspector_app = sys.argv[1]
    solution_file = sys.argv[2]
    project_name = sys.argv[3]
    output_dir = sys.argv[4]

    output_file = output_dir + '\\' + project_name

    args = [
        "--toolset=15.0",
        "--swea",
        "--project=\"{0}\"".format(project_name),
        "-o=\"{0}.xml\"".format(output_file),
        solution_file
    ]

    subprocess.call([inspector_app] + args)

    # parse output
    with open(output_file + ".xml", 'r') as data, open(output_file + "_Parsed.txt", "w+") as output:
        matches = re.findall(line_regex, data.read())

        last_file_name = ""
        file_dict = {}
        for (error_id, file_name, line_number, message) in matches:
            if error_id in black_list:
                continue

            if error_id == "CppMissingIncludeGuard" and file_name.endswith(".inl"):
                continue

            if file_name != last_file_name:
                file_dict[file_name] = []
                last_file_name = file_name

            file_dict[file_name].append((error_id[3:], int(line_number), message))

        for file, value_list in file_dict.items():
            output.write("--- " + file + " ---\n")
            value_list.sort()

            last_error_id = ""
            for (error_id, line_number, message) in value_list:
                if error_id != last_error_id:
                    if last_error_id:
                        output.write("\n\n")
                    output.write("\t[ " + error_id + " ]\n\t\t" + message + "\n\t\tLines: ")
                    last_error_id = error_id
                  
                output.write(str(line_number) + ", ")
            output.write("\n\n\n")

    print("Done!\n")


if __name__ == '__main__':
    main()
