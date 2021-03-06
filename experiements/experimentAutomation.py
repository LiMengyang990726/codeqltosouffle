from pathlib import Path
import os

repo_path_abs = "/home/mengyang/FYP/"
gitfacts_path = "/home/mengyang/FYP/ext-gitfacts"
output_path_abs = "/home/mengyang/FYP/experiments_2021_06_03/"
query_file_path_abs = "/home/mengyang/FYP/EvoMe/experiements/"
evome_path = "/home/mengyang/FYP/EvoMe"
cslicer_path = "/home/mengyang/FYP/gitslice/target/cslicer-1.0.0-jar-with-dependencies.jar"
log_file = os.path.join(output_path_abs, "log-2021-06-03.txt")

for path in Path('./').rglob('*.txt'):
    repo_path_rel = str(path.parents[1])
    output_path_rel = os.path.join(str(path.parents[1]), os.path.splitext(path.name)[0] + str(path.parents[0])[-1])
    query_file_path_rel = os.path.join(str(path.parents[0]), path.name)
    repo_path = os.path.join(repo_path_abs, repo_path_rel)
    output_path = os.path.join(output_path_abs, output_path_rel)
    query_file_path = os.path.join(query_file_path_abs, query_file_path_rel)
    command = "python3.7 ../scripts/run.py --repo_path %s \
         --gitfacts_path %s \
         --output_path %s \
         --query_file_path %s \
         --evome_path %s \
         --cslicer_path %s >> %s" % (repo_path, gitfacts_path, output_path, query_file_path, evome_path, cslicer_path, log_file)
    f = open(log_file, "a")
    f.write(command)
    f.write("\n")
    f.close()
    os.system(command)
