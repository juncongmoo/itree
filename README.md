# iTree

`itree` is a library for interval tree data structures.


```python
>>> import tree
>>> structure = [[1], [[[2, 3]]], [4]]
>>> tree.flatten(structure)
[1, 2, 3, 4]
>>> tree.map_structure(lambda v: v**2, structure)
[[1], [[[4, 9]]], [16]]
```

`tree` is backed by an optimized C++ implementation suitable for use in
demanding applications, such as machine learning models.

## Installation

From PyPI:

```shell
$ pip install dm-tree
```

Directly from github using pip:

```shell
$ pip install git+git://github.com/deepmind/tree.git
```

Build from source:

```shell
$ python setup.py install
```

## Support

If you are having issues, please let us know by filing an issue on our
[issue tracker](https://github.com/deepmind/tree/issues).

## License

The project is licensed under the Apache 2.0 license.
