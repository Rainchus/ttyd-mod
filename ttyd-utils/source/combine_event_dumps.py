#! /usr/bin/python3.6

"""Combines all events generated by export_events.py.

This file must be run after the following binaries, using the same --out_path:
- export_events.py (and its dependencies)

Concatenates the text files generated by export_events.py into a single file
containing every unique event (as determined by name x area) once."""
# Jonathan Aldrich 2022-03-17

import codecs
import os
import sys
import subprocess
from pathlib import Path

import jdalibpy.flags as flags

FLAGS = flags.Flags()

# Output directory; should contain these outputs from previous binaries:
# - event scripts from export_events (in "/events" dir underneath).
# The combined events will be exported to "(out path)/events/combined/all.txt".
FLAGS.DefineString("out_path", "")

# Whether to display debug strings.
FLAGS.DefineInt("debug_level", 1)

class CombineEventsError(Exception):
    def __init__(self, message=""):
        self.message = message
        
def _GetEventFilenames(events_path):
    if FLAGS.GetFlag("debug_level"):
        print("Getting filenames of event script dumps...")
    return [f for f in os.listdir(events_path) if f.endswith('.txt')]
    
def _GetEventLookupDict(events_path, event_files):
    if FLAGS.GetFlag("debug_level"):
        print("Looking up event areas / addresses...")
    
    lookup = {}
    for fn in event_files:
        area = fn[:fn.find("_", 1)]
        address = None
        
        # Open file and inspect lines until finding one that ends in an address.
        f = codecs.open(events_path / fn, "r", encoding="utf-8")
        for line in f:
            pos = line.find("] AT ")
            if pos >= 0:
                address = int(line[pos + 5 :][:8], 16)
                break
        f.close()
                
        if address:
            lookup["%s_%08x" % (area, address)] = events_path / fn
    
    return lookup     

def _CombineEventFiles(event_lookup, out_path):
    if FLAGS.GetFlag("debug_level"):
        print("Combining event dumps into single text file...")
        
    outfile = codecs.open(out_path, "w", encoding="utf-8")
    outfile.write("Original disassembly by PistonMiner's ttydasm tool.\n\n")
        
    for k,fn in sorted(event_lookup.items()):
        outfile.write("Script %s%s:\n" % ("@", k))
        
        evt = codecs.open(fn, "r", encoding="utf-8")
        found_start = False
        for line in evt:
            if found_start and not line.strip():
                break
            if line.find("START OF DISASSEMBLY") >= 0:
                found_start = True
            if found_start:
                outfile.write(line)
        evt.close()
        outfile.write("\n")
        
    outfile.close()
    
    if FLAGS.GetFlag("debug_level"):
        print("Done! Output in %s." % out_path)

def main(argc, argv):
    out_path = FLAGS.GetFlag("out_path")
    if not out_path or not os.path.exists(Path(out_path)):
        raise CombineEventsError("--out_path must point to a valid directory.")
    out_path = Path(out_path)
    
    if not os.path.exists(out_path / "events"):
        raise CombineEventsError(
            "You must first run export_events.py using the same --out_path.")
     
    events_path = out_path / "events"
    event_files = _GetEventFilenames(events_path)
    event_lookup = _GetEventLookupDict(events_path, event_files)
    
    if FLAGS.GetFlag("debug_level"):
        print("\nFound scripts:")
        for k,v in sorted(event_lookup.items())[:5]:
            print((k,v))
        print("...")
        for k,v in sorted(event_lookup.items())[-5:]:
            print((k,v))
        print("")    
    
    if not os.path.exists(events_path / "combined"):
        os.makedirs(events_path / "combined")
    _CombineEventFiles(event_lookup, events_path / "combined/all.txt")

if __name__ == "__main__":
    (argc, argv) = FLAGS.ParseFlags(sys.argv[1:])
    main(argc, argv)