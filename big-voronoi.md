big-voronoi(1) -- Consider: voronoi diagram, but in 3D and in parallel
======================================================================

## SYNOPSIS

`big-voronoi` [OPTIONS]

## DESCRIPTION

Consider: voronoi diagram, but in 3D and in parallel.

A voronoi diagram is an α-dimensional space, wherein each point takes on the
colour of the control point closest thereto.

This program can generate 3D voronoi diagrams quickly, and changing the distance
function within it is easy.

## OPTIONS

  -s --size <DIAGRAM_SIZE>

    Specify the size of the output diagram.

    DIAGRAM_SIZE is in DIAGRAM SIZE FORMAT.

    Default: 900x900x900.

  -c --colours <COLOURS_FILE|COLOURS_COUNT>

    Specify the amount of control points' colours or the file therecontaining.

    If the value is a positive integer, then COLOURS_COUNT is that integer
    and COLOURS_COUNT colours are generated.
    Note: the first 20 colours are constant (see assets/default_colours file
    in the repo), the ones over 20 will be randomly generated.

    Otherwise, COLOURS_FILE is parsed according to COLOURS FILE FORMAT,
    and COLOURS_COUNT is the amount of successfully parsed colours thereinfrom.

    Default: 20.

  -p --points <POINTS_FILE|POINTS_COUNT>

    Specify the amount of control points or the file therecontaining.

    If the value is a positive integer, then POINTS_COUNT is that integer
    and POINTS_COUNT points are randomly generated within DIAGRAM_SIZE.

    Otherwise, POINTS_FILE is parsed according to POINTS FILE FORMAT,
    and POINTS_COUNT is the amount of successfully parsed points thereinfrom.

    Default: COLOURS_COUNT.

  -j --jobs <JOB_COUNT>

    Specify the amount of threads to use for diagram generation.

    JOB_COUNT is a positive integer.

    Default: autodetected amount of logical CPUs.

  -o --out-directory <OUTDIR>

    Specify the directory to which the diagram will be saved.

    OUTDIR must exist and be a directory.

    Default: current working directory.

  --version

    Print the version of the big-voronoi binary.

  -h --help

    Print short usage information.

## DIAGRAM SIZE FORMAT

A string is a valid diagram size if it matches the following regex:

```regex
([[:digit:]]+)x([[:digit:]]+)x([[:digit:]]+)
```

Or, put more succintly:

```
D+xD+xD+
```

Wherein `D` is a digit character, `+` means "1 or more times" and `x` is a literal `"x"`.

## COLOURS FILE FORMAT

A line-based format, wherein a line contains a colour if it is a valid CSS3 colour.

If a line does not contain a CSS3 colour according, it is ignored.

## POINTS FILE FORMAT

A line-based format, wherein a line contains a point if it matches this regex:

```regex
[^[:digit:]]*([[:digit:]]+)[^[:digit:]]+([[:digit:]]+)[^[:digit:]]+([[:digit:]]+)[^[:digit:]]*
```

Or, put more succintly:

```
N*D+N+D+N+D+N*
```

Wherein `N` is a non-digit character, `D` is a digit character,
        `*` means "0 or more times" and `+` means "1 or more times".

If a line does not contain a point according to that specification, it is ignored.

## AUTHOR

Written by nabijaczleweli &lt;<nabijaczleweli@gmail.com>&gt;

## REPORTING BUGS

&lt;<https://github.com/nabijaczleweli/big-voronoi/issues>&gt;

## SEE ALSO

&lt;<https://github.com/nabijaczleweli/big-voronoi>&gt;
