#/bin/env python3

import itertools
import os


def get_headers(p):
    p = os.path.abspath(p)
    deps = set()
    for src in itertools.chain.from_iterable(
        (
            os.path.join(root, filename)
            for filename in filenames
            if os.path.splitext(filename)[1] in (".Po", ".Plo")
        )
        for root, dirs, filenames in os.walk(p)
    ):
        with open(src) as infile:
            deps.update(
                itertools.chain.from_iterable(
                    (
                        os.path.normpath(os.path.join(p, item))
                        for item in line.split(":")[1].split()
                        if os.path.splitext(item)[1] in (".h", ".hpp")
                    )
                    for line in infile.read().replace("\\\n", "").splitlines()
                )
            )
    return deps

pwd = os.path.abspath(".")
deps = get_headers(".")
deps.update(get_headers("../src"))
deps = [
    dep
    for dep in (os.path.relpath(x, pwd) for x in sorted(deps))
    if not dep.startswith("../")
]
print("\t" + " \\\n\t".join(deps))
