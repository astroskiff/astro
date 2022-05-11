# Josh A. Bosley 
# April 12 2022
# This script compiles and executes all files within the directory
# and ensure that they have the expected return code

import re
import glob
import sys
import os
import subprocess
import threading
import time

if len(sys.argv) < 2:
    print("Expected path to astro binary")
    exit(1)

parallel = False
astro = sys.argv[1]

print("\n<< PARSER CHECKS >>\n")
check_directory = os.getcwd()
print("CWD : ", check_directory)

test_directories = [
  check_directory + "/passes",
  check_directory + "/failures",
]

def time_to_ms_str(t):
  return str(round(t * 1000, 4)) + "ms"

def display_result(result_item):
    out = "-" * 10
    out += "\n\n"
    out += "Test File    : " + result_item["name"] + "\n"
    out += "Parser    : "

    if not result_item["parser_result"]["success"]:
      out += "[FAILED] after " + time_to_ms_str(result_item["parser_result"]["time"]) + "\n"
      out += "\n"
      out += "\t---- output ----\n"
      out += result_item["parser_result"]["output"]
      out += "\t----------------\n"
      print(out)
      exit(1)
    else:
      out += "[PASSED] after " + time_to_ms_str(result_item["parser_result"]["time"]) + "\n"

    print(out)

def test_item(id, expected_result, item):
  results = {}
  parser_start = time.time()
  parser_result = subprocess.run([astro, "--parse-only", item], stdout=subprocess.PIPE)
  parser_end = time.time()
  parser_status = True

  decoded = parser_result.stdout.decode("utf-8")

  results["name"] = item

  results["parser_result"] = {
    "time": parser_end - parser_start,
    "success": parser_result.returncode == int(expected_result),
    "output": decoded
  }
  return results

def retrieve_objects_from(directory):
  os.chdir(directory)
  items_in_dir = glob.glob("*.ast")
  results = []
  for item in items_in_dir:
    result = {}
    result["expected_code"] = item.split("_")[0]
    result["path"] = directory + "/" + item
    results.append(result)
  return results

def build_exec_list(dirs):
  exec_list = []
  for dir in dirs:
    print("Scanning directory : ", dir)
    exec_list.append(retrieve_objects_from(dir))
  print("")
  return exec_list

def task(id, jobs):
  results = []
  for item in jobs:
    results.append(test_item(id, item["expected_code"], item["path"]))
  for item in results:
    display_result(item)

def linear_run():
  exec_list = build_exec_list(test_directories)
  os.chdir(check_directory)
  for item in exec_list:
    task(0, item)


print("\n<< LINEAR >>\n")
run_time_start = time.time()
linear_run()
run_time_end = time.time()

print("-" * 10)
print("\nChecks complete after ", round(run_time_end - run_time_start, 4), " seconds\n")

exit(0)