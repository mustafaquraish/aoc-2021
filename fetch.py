#!/usr/bin/env python3

################################################################################
#                      Fetch AOC input / sample input
# 
# This script will download the input for the current day, and will also attempt
# to scrape the sample input from the AoC problem definition for the day. To use
# this, you will need to get your Session cookie from the AoC website, which
# can be acquired from your browser's dev tools. Place this session value in any
# of the following places (checked in this order):
#
#      - As the environment variable AOC_SESSION
#      - In an arbitrary file which is passed in through the CLI
#      - <cwd>/.session
#      - ~/.aocsession
#      - ~/aoc/session
#      - ~/.config/aoc/session
#
# To change where the output files are stored, you can tweak the two strings
# below, which will be used to generate the paths. [[DAY]] and [[YEAR]] year will
# be replaced with the current day and year, respectively (as entered through the
# CLI).
#
# Run the script as follows:
#       python3 fetch.py [--session <session_file>] <year> <day>
#
# The following non-standard libraries are required: (requests, beautifulsoup4)
#       pip install requests beautifulsoup4
#
################################################################################

FULL_INPUT_FORMAT = "input/[[DAY]].txt"
SAMPLE_INPUT_FORMAT = "input/[[DAY]]s.txt"

################################################################################

from argparse import ArgumentParser
from bs4 import BeautifulSoup
from functools import cache
from pathlib import Path
import os
import requests

@cache
def get_session(session_file=None):
    if key := os.environ.get("AOC_SESSION"):
        return key.strip()

    paths = [
        Path.cwd() / ".session",
        Path.home() / ".aocsession",
        Path.home() / "aoc" / "session",
        Path.home() / ".config" / "aoc" / "session",
    ]
    if session_file is not None:
        paths = [session_file, *paths]

    for path in paths:
        if path.exists():
            with path.open() as fp:
                return fp.read().strip()

    print("No session key found anywhere.")
    os.exit(1)

def get_from_format(args, fmt):
    fmt = fmt.replace("[[DAY]]", str(args.day))
    fmt = fmt.replace("[[YEAR]]", str(args.year))
    path = Path(fmt)
    if path.exists():
        if args.force:
            print(f"{fmt} already exists. Overwriting...")
        else:
            replace = input(f"{fmt} already exists. Overwrite? [y/N] ")
            if replace.lower() != "y":
                return None
    return path

def get_input_file(args):
    if (path := get_from_format(args, FULL_INPUT_FORMAT)) is None:
        return

    session = requests.Session()
    session.cookies.set("session", get_session(args.session_file), domain=".adventofcode.com")

    year, day = int(args.year), int(args.day)
    input_url = f"https://adventofcode.com/{year}/day/{day}/input"
    body = session.get(input_url, headers={
        "User-Agent": "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0"
    })
    if body.status_code != 200:
        print(f"Could not download {input_url}.")
        return

    print(f"Saving {path}...")
    with path.open("w") as fp:
        fp.write(body.content.decode("utf-8").strip())

def get_sample_file(args):
    if (path := get_from_format(args, SAMPLE_INPUT_FORMAT)) is None:
        return

    year, day = int(args.year), int(args.day)

    response = requests.get(f"https://adventofcode.com/{year}/day/{day}")
    parser = BeautifulSoup(response.text, 'html.parser')
    tags = parser.find_all(["p", "pre"])
    
    found_marker = False
    sample_text = None
    for t in tags:
        if t.name == "p":
            if "(your puzzle input)" in t.text:
                found_marker = True
        if found_marker and t.name == "pre":
            sample_text = t.text
            break
    
    if sample_text is None:
        print("Could not find sample input.")
        return

    print(f"Saving {path}...")
    with path.open('w') as fp:
        fp.write(sample_text.strip())


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("year", type=str, help="Year of the Advent of Code")
    parser.add_argument("day", type=str, help="Day of the Advent of Code")
    parser.add_argument("--session-file", "-s", type=str, help="Custom session file", default=None)
    parser.add_argument("--force", "-f", action='store_true', help="Force overwrite existing files")

    args = parser.parse_args()

    get_input_file(args)
    get_sample_file(args)
