
Installation and usage
======================

In order to run this shell, you need ``Python 3.6``.
No other packages needed.

Example usage::

    python3.6 -m pip install -r requirements.txt
    cd ./cli/src/
    python3.6 main.py

This package is continiously tested on Linux (using Travis CI) and Windows (using AppVeyor).
Code coverage is beign run in Travis. This ensures stable and cross-platform pleasant user experience.

.. automodule:: cli

Shell module
============

.. automodule:: cli.shell
    :members:


Environment module
==================

.. automodule:: cli.environment
    :members:


Commands module
===============

.. automodule:: cli.commands
    :members:

Single commands module
======================
.. automodule:: cli.single_command 
    :members:

Preprocessor module
===================

.. automodule:: cli.preprocessor
    :members:


Lexer module
============

.. automodule:: cli.lexer
    :members:


Parser module
=============

.. automodule:: cli.parser
    :members:


Streams module
==============

.. automodule:: cli.streams
    :members:


Exceptions module
=================

.. automodule:: cli.exceptions
    :members:
