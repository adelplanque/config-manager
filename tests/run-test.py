import argparse
import os
import subprocess
import tempfile
import yaml
import difflib
import sys


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("filename", help="yaml test configuration")
    return parser.parse_args()


class Tester():

    def __init__(self, args):
        with open(args.filename, "r") as infile:
            self.data = yaml.safe_load(infile.read())

    def run(self):
        with tempfile.TemporaryDirectory() as workdir:
            for filename, content in self.data["files"].items():
                dirname = os.path.join(workdir, os.path.dirname(filename))
                os.makedirs(dirname, exist_ok=True)
                with open(os.path.join(workdir, filename), "w") as outfile:
                    outfile.write(content)
            self.workdir = workdir
            status = [
                self.run_test(testcase["command"], testcase["expected"])
                for testcase in self.data["tests"]
            ]
        return 0 if all(status) else 1

    def run_test(self, cmd, expected):
        sys.stdout.write("*" * 80 + "\n")
        sys.stdout.write(f"# {cmd.strip()}\n")
        sys.stdout.write("*" * 80 + "\n")
        env = dict(os.environ)
        env["WORKDIR"] = self.workdir
        p = subprocess.run(
            f"set -x; {cmd}",
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            env=env,
        )
        result = p.stdout.decode("utf-8")
        if p.returncode == 0 and result == expected:
            status = True
        else:
            diff = difflib.Differ().compare(
                expected.splitlines(keepends=True), result.splitlines(keepends=True)
            )
            sys.stdout.write("--- expected\n")
            sys.stdout.write("+++ result\n")
            sys.stdout.writelines(diff)
            sys.stdout.write("*" * 80 + "\n")
            sys.stdout.write(p.stderr.decode("utf-8"))
            sys.stdout.write("*" * 80 + "\n")
            status = False
        sys.stdout.write("\n")
        return status


def main():
    return Tester(parse_args()).run()


if __name__ == "__main__":
    sys.exit(main())
