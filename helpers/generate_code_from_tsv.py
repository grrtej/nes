import csv
from datetime import datetime, timezone
from pathlib import Path

# locate tsv
SCRIPT_DIR = Path(__file__).resolve().parent
MASSWERK_TSV = SCRIPT_DIR / "masswerk_6502.tsv"

# address mode: (instruction size in bytes,)
ADDRESS_MODES = {
    "A": (1,),
    "abs": (3,),
    "abs,X": (3,),
    "abs,Y": (3,),
    "#": (2,),
    "impl": (1,),
    "ind": (3,),
    "X,ind": (2,),
    "ind,Y": (2,),
    "rel": (2,),
    "zpg": (2,),
    "zpg,X": (2,),
    "zpg,Y": (2,),
}


def main():
    # grab all data from file and close it right away
    with open(MASSWERK_TSV, newline="") as f:
        data = f.readlines()

    # this will contain 256 items representing various instruction metadata
    instructions = []

    # parse
    tsv = csv.reader(data, delimiter="\t")
    for row in tsv:
        for cell in row:
            if cell == "---":
                instructions.append(("UNK", -1))
            else:
                name, mode = cell.split()
                instructions.append((name, ADDRESS_MODES[mode][0]))

    assert len(instructions) == 256

    # timestamp
    now = datetime.now(timezone.utc).isoformat(timespec="seconds")

    # cpp header code
    print(
        rf"""// generated from https://www.masswerk.at/6502/instruction-tables/ on {now}"""
    )
    print(r"""#pragma once""")
    print()
    print(r"""struct InstructionMetadata {""")
    print(r"""    const char *name;""")
    print(r"""    int size;""")
    print(r"""};""")
    print()
    # i dont understand much about these keywords but i've heard this is the ideal usecase
    # inline constexpr: inline prevents memory duplication, constexpr computes as much as
    # possible during compile time
    print(
        r"""inline constexpr InstructionMetadata INSTRUCTION_METADATA_TABLE[256] = {"""
    )
    for instruction in instructions:
        # initializer list to create struct instance
        print(rf"""    {{"{instruction[0]}", {instruction[1]}}},""")
    print(r"""};""")


if __name__ == "__main__":
    main()
