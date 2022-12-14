libformat
==========

.. contents:: Table of Contents


Introduction
-------------

|Github Stars| |Github Forks| |Github Open Issues| |Github Open PRs|

**Format** is a ``printf``-style string formatting library.

.. code:: c

  char *hello_world = format(&hello_world, "Hello, %s!", "World");


Technologies Used
------------------

- Windows 10 x64
- Visual Studio Code
- C Language


Project Status
---------------

This project is currently in **development**.


Version Naming
---------------

This library uses *semantic versioning*:

.. code::

MAJOR.MINOR.PATCH

Where an increment in:

- ``MAJOR`` = Incompatible changes (may require code to be updated).
- ``MINOR`` = Backwards compatible feature changes.
- ``PATCH`` = Backwards compatible bug fixes.


Getting Started
----------------

TODO


Contributing
-------------

Want to contribute? Great!

To fix a bug or enhance an existing module, follow these steps:

- `Fork <https://github.com/nicdgonzalez/libformat/fork>`_ the repository
  and create a new branch:

.. code:: console

  $ git clone "https://github.com/[username]/[repository_name].git"
  $ git checkout -b "improve-feature"

- Make the appropriate changes and stage the modified files:

.. code:: console

  $ git add <changed file(s)>

- Commit changes:

.. code:: console

  $ git commit -m "Improve feature."

- Push to the new branch:

.. code:: console

  $ git push "origin" "improve-feature"

- Create a `Pull Request <https://github.com/nicdgonzalez/libformat/pulls>`_.


Bug/Feature Request
--------------------

If you find a bug (program failed to run and/or gave undesired results)
or you just want to request a feature, kindly open a new issue
`here <https://github.com/nicdgonzalez/libformat/issues>`_.

..
  ****************************************************************************
.. |Github Stars| image:: https://badgen.net/github/stars/nicdgonzalez/libformat
.. |Github Forks| image:: https://badgen.net/github/forks/nicdgonzalez/libformat
.. |Github Open Issues| image:: https://badgen.net/github/open-issues/nicdgonzalez/libformat
  :target: https://github.com/nicdgonzalez/libformat/issues?q=is%3Aissue+is%3Aopen+
.. |Github Open PRs| image:: https://badgen.net/github/open-prs/nicdgonzalez/libformat
  :target: https://github.com/nicdgonzalez/libformat/pulls?q=is%3Apr+is%3Aopen+
