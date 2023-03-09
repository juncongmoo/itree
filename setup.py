"""Setup for pip package."""

import os
import shutil
import subprocess
import sys

import setuptools
from setuptools.command import build_ext

here = os.path.dirname(os.path.abspath(__file__))


def _get_tree_version():
    """Parse the version string from itree/__init__.py."""
    with open(os.path.join(here, "itree", "version.py")) as f:
        try:
            version_line = next(line for line in f if line.startswith("__version__"))
        except StopIteration:
            raise ValueError("__version__ not defined in itree/version.py")
        else:
            ns = {}
            exec(version_line, ns)  # pylint: disable=exec-used
            return ns["__version__"]


def _parse_requirements(path):
    with open(os.path.join(here, path)) as f:
        return [
            line.rstrip() for line in f if not (line.isspace() or line.startswith("#"))
        ]


class CMakeExtension(setuptools.Extension):
    """An extension with no sources.

    We do not want distutils to handle any of the compilation (instead we rely
    on CMake), so we always pass an empty list to the constructor.
    """

    def __init__(self, name, source_dir=""):
        super().__init__(name, sources=[])
        self.source_dir = os.path.abspath(source_dir)


class BuildCMakeExtension(build_ext.build_ext):
    """Our custom build_ext command.

    Uses CMake to build extensions instead of a bare compiler (e.g. gcc, clang).
    """

    def run(self):
        self._check_build_environment()
        for ext in self.extensions:
            self.build_extension(ext)

    def _check_build_environment(self):
        """Check for required build tools: CMake, C++ compiler, and python dev."""
        try:
            subprocess.check_call(["cmake", "--version"])
        except OSError as e:
            ext_names = ", ".join(e.name for e in self.extensions)
            raise RuntimeError(
                f"CMake must be installed to build the following extensions: {ext_names}"
            ) from e
        print("Found CMake")

    def build_extension(self, ext):
        extension_dir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name))
        )
        build_cfg = "Debug" if self.debug else "Release"
        cmake_args = [
            f"-DPython3_EXECUTABLE={sys.executable}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extension_dir}",
            f"-DCMAKE_BUILD_TYPE={build_cfg}",
        ]
        os.makedirs(self.build_temp, exist_ok=True)
        subprocess.check_call(
            ["cmake", ext.source_dir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", ".", f"-j{os.cpu_count()}", "--config", build_cfg],
            cwd=self.build_temp,
        )

        # Force output to <extension_dir>/. Amends CMake multigenerator output paths
        # on Windows and avoids Debug/ and Release/ subdirs, which is CMake default.
        tree_dir = os.path.join(extension_dir, "itree")  # pylint:disable=unreachable
        for cfg in ("Release", "Debug"):
            cfg_dir = os.path.join(extension_dir, cfg)
            if os.path.isdir(cfg_dir):
                for f in os.listdir(cfg_dir):
                    shutil.move(os.path.join(cfg_dir, f), tree_dir)


VERSION = _get_tree_version()
DESCRIPTION = "A Interval Tree Library"

setuptools.setup(
    name="py-itree",
    version=VERSION,
    author="Juncong Moo",
    author_email="<juncongmoo@gmail.com>",
    description=DESCRIPTION,
    long_description=open(os.path.join(here, "README.md"), encoding="utf-8").read(),
    long_description_content_type="text/markdown",
    packages=setuptools.find_packages(),
    install_requires=_parse_requirements("requirements.txt"),
    cmdclass=dict(build_ext=BuildCMakeExtension),
    ext_modules=[CMakeExtension("_itree", source_dir="itree")],
    zip_safe=False,
    include_package_data=True,
    keywords=["tree", "itree", "interval tree"],
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Education",
        "Intended Audience :: Information Technology",
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Topic :: Scientific/Engineering :: Mathematics",
        "Topic :: Software Development :: Libraries",
    ],
    license="Apache 2.0",
)
