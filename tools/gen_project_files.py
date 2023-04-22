'''
Script to automatically add all .cpp and .h files under the Open-Rival folder to
the Visual Studio project.

This does not (yet) update the filters file, so any newly-added files will
appear at the top level of the Solution Explorer by default.

This is basically a dirty hack until we get a better solution (like CMake) up
and running.

    USAGE:
      python gen_project_files.py
'''

import os
import xml.etree.ElementTree as ET
from glob import glob

import sys

PROJECT_DIR = '../projects/Open-Rival/'
SRC_DIR = PROJECT_DIR + 'src/'
HDR_DIR = PROJECT_DIR + 'include/'

def find_files(dir_name, glob_pattern):
    files = []
    for dir_result in os.walk(dir_name):
        path = dir_result[0]
        for file in glob(os.path.join(path, glob_pattern)):
            sanitized_filename = ( file
                    .replace(PROJECT_DIR, '')
                    .replace('/', '\\')
            )
            files.append(sanitized_filename)
    return files

def main():
    # Parse project file to find the nodes we need to replace
    ET.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
    project_xml = ET.parse(PROJECT_DIR + 'Open-Rival.vcxproj')
    project_xml_root = project_xml.getroot()
    cpp_files_node = None
    hdr_files_node = None
    for child in project_xml_root:
        if not child.tag.endswith('ItemGroup'):
            continue
        if child.find('{http://schemas.microsoft.com/developer/msbuild/2003}ClCompile') != None:
            cpp_files_node = child
            continue
        if child.find('{http://schemas.microsoft.com/developer/msbuild/2003}ClInclude') != None:
            hdr_files_node = child
            continue

    # Ensure we found the required nodes
    if not cpp_files_node:
        print("Failed to find node containing ClCompile elements")
        return
    if not cpp_files_node:
        print("Failed to find node containing ClInclude elements")
        return

    # Find all files in the project
    # See: https://stackoverflow.com/a/50280771/1624459
    src_files = find_files(SRC_DIR, '*.cpp')
    hdr_files = find_files(HDR_DIR, '*.h')

    # Build XML string for CPP files
    cpp_xml_lines = ['  <ItemGroup>']
    for file in src_files:
        if file == 'src\\pch.cpp':
            # Special case for pch.cpp
            cpp_xml_lines.append('    <ClCompile Include="src\pch.cpp">')
            cpp_xml_lines.append('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'">Create</PrecompiledHeader>')
            cpp_xml_lines.append('      <PrecompiledHeader Condition="\'$(Configuration)|$(Platform)\'==\'Release|Win32\'">Create</PrecompiledHeader>')
            cpp_xml_lines.append('    </ClCompile>')
        else:
            cpp_xml_lines.append('    <ClCompile Include="' + file + '" />')
    cpp_xml_lines.append('  </ItemGroup>')
    cpp_xml_str = '\n'.join(cpp_xml_lines)
    cpp_xml = ET.fromstring(cpp_xml_str)

    # Build XML string for header files
    hdr_xml_lines = ['  <ItemGroup>']
    for file in hdr_files:
        hdr_xml_lines.append('    <ClInclude Include="' + file + '" />')
    hdr_xml_lines.append('    <ClInclude Include="resource.h" />')
    hdr_xml_lines.append('  </ItemGroup>')
    hdr_xml_str = '\n'.join(hdr_xml_lines)
    hdr_xml = ET.fromstring(hdr_xml_str)
    
    # Update the XML
    cpp_files_node.clear()
    cpp_files_node.extend(cpp_xml)
    hdr_files_node.clear()
    hdr_files_node.extend(hdr_xml)
    
    # Write to the XML file
    project_xml.write(PROJECT_DIR + 'Open-Rival.vcxproj', encoding = "utf-8", xml_declaration = True)

if __name__ == '__main__':
    main()
