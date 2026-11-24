"""Tiny S-expression reader / writer for KiCad files (no dependencies)."""


class Sym(str):
    """Unquoted atom (keyword / number) - written back without quotes."""


def parse(text):
    """Parse one S-expression; strings -> str, atoms -> Sym, lists -> list."""
    pos = 0
    n = len(text)

    def skip():
        nonlocal pos
        while pos < n and text[pos] in " \t\r\n":
            pos += 1

    def read():
        nonlocal pos
        skip()
        c = text[pos]
        if c == "(":
            pos += 1
            out = []
            while True:
                skip()
                if text[pos] == ")":
                    pos += 1
                    return out
                out.append(read())
        if c == '"':
            pos += 1
            buf = []
            while text[pos] != '"':
                if text[pos] == "\\":
                    pos += 1
                    buf.append({"n": "\n", "t": "\t"}.get(text[pos], text[pos]))
                else:
                    buf.append(text[pos])
                pos += 1
            pos += 1
            return "".join(buf)
        start = pos
        while pos < n and text[pos] not in " \t\r\n()":
            pos += 1
        return Sym(text[start:pos])

    return read()


def dump(x, indent=0):
    """Write back as KiCad style text."""
    if isinstance(x, list):
        if not x:
            return "()"
        simple = all(not isinstance(e, list) for e in x)
        if simple:
            return "(" + " ".join(dump(e) for e in x) + ")"
        pad = "\t" * (indent + 1)
        parts = [dump(x[0])]
        for e in x[1:]:
            if isinstance(e, list):
                parts.append("\n" + pad + dump(e, indent + 1))
            else:
                parts.append(" " + dump(e))
        return "(" + "".join(parts) + "\n" + "\t" * indent + ")"
    if isinstance(x, Sym):
        return str(x)
    if isinstance(x, bool):
        return "yes" if x else "no"
    if isinstance(x, (int, float)):
        s = f"{x:.4f}".rstrip("0").rstrip(".")
        return s if s not in ("-0", "") else "0"
    s = str(x).replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n")
    return '"' + s + '"'


def find(lst, key):
    """First child list whose head is key."""
    for e in lst:
        if isinstance(e, list) and e and e[0] == key:
            return e
    return None


def find_all(lst, key):
    return [e for e in lst if isinstance(e, list) and e and e[0] == key]
