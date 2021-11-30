# Copyright 2019 DeepMind Technologies Limited. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or  implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
"""Configuration file for the Sphinx documentation builder."""

# This file only contains a selection of the most common options. For a full
# list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.

# pylint: disable=g-bad-import-order
# pylint: disable=g-import-not-at-top
import datetime
import inspect
import os
import sys

sys.path.insert(0, os.path.abspath("../"))

import itree

# -- Project information -----------------------------------------------------

project = "iTree"
copyright = f"{datetime.date.today().year}, Juncong Moo, Apache 2.0"  # pylint: disable=redefined-builtin
author = "Juncong Moo"
version = itree.__version__
release = itree.__version__

# -- General configuration ---------------------------------------------------

master_doc = "index"

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.linkcode",
    "sphinx.ext.napoleon",
    "sphinx.ext.doctest",
    ################################################################
    "sphinx.ext.napoleon",
    "sphinx.ext.todo",
    "sphinx_click.ext",
    "sphinx_copybutton",
    "recommonmark",
    "sphinxcontrib.spelling",
    "sphinx_markdown_tables",
    "sphinx.ext.mathjax",
    "sphinxcontrib.images",  # https://sphinxcontrib-images.readthedocs.io/en/latest/
    "rst2pdf.pdfbuilder",
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = "sphinx"

# Remove the prompt when copying examples
copybutton_prompt_text = r">>> |\.\.\.|» |$ "
copybutton_prompt_is_regexp = True


# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# -- Options for autodoc -----------------------------------------------------

autodoc_default_options = {
    "member-order": "bysource",
    "special-members": True,
    "exclude-members": "__repr__, __str__, __weakref__",
}

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

html_theme_options = {
    # 'collapse_navigation': False,
    # 'sticky_navigation': False,
}

from recommonmark.transform import AutoStructify


def setup(app):
    app.add_config_value(
        "recommonmark_config",
        {
            #'url_resolver': lambda url: github_doc_root + url,
            "auto_toc_tree_section": "Contents",
            "enable_math": False,
            "enable_inline_math": False,
            "enable_eval_rst": True,
            "enable_auto_doc_ref": True,
        },
        True,
    )
    app.add_transform(AutoStructify)


source_suffix = [".rst", ".md", ".rmd"]
# -- Options for doctest -----------------------------------------------------

doctest_global_setup = """
import collections
import numpy as np
import itree
"""

# -- Source code links -------------------------------------------------------


def linkcode_resolve(domain, info):
    """Resolve a GitHub URL corresponding to Python object."""
    if domain != "py":
        return None

    try:
        mod = sys.modules[info["module"]]
    except ImportError:
        return None

    obj = mod
    try:
        for attr in info["fullname"].split("."):
            obj = getattr(obj, attr)
    except AttributeError:
        return None
    else:
        obj = inspect.unwrap(obj)

    try:
        filename = inspect.getsourcefile(obj)
    except TypeError:
        return None

    try:
        source, lineno = inspect.getsourcelines(obj)
    except OSError:
        return None

    return "https://github.com/juncongmoo/itree/blob/master/itree/%s#L%d#L%d" % (
        os.path.relpath(filename, start=os.path.dirname(itree.__file__)),
        lineno,
        lineno + len(source) - 1,
    )
