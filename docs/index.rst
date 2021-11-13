##################
iTree Documentation
##################

.. thumbnail:: forest.jpg
   :align: center

.. toctree::
   :maxdepth: 2
   :hidden:

   api
   changes
   recipes


Installation
============

Install ``itree`` by running::

  $ pip install py-itree

Quick Start
============

.. note::

  🟢  is normal node, 🔵 is zero interval node, 🍁 is leaf node


  >>> import itree
  >>> itree.Node('fruit', 0, 40)
  [🍁 n=fruit,s=0.00,e=40.00,x=0,c=0]
  >>> itree.Node('fruit')
  [🔵 n=fruit]

----

  >>> a=itree.Node('fruit', 0, 200)
  >>> b=itree.Node('apple', 10, 20)
  >>> print(a)
  [🍁 n=fruit,s=0.00,e=40.00,x=0,c=0]
  >>> a.append(b)
  >>> print(a)
  [🟢 n=fruit,s=0.00,e=200.00,x=0,c=1]
  >>> print(b)
  [🍁 n=apple,s=10.00,e=20.00,x=0,c=0]
  >>> print(a.nodes)
  [[🍁 n=apple,s=10.00,e=20.00,x=0,c=0]]
  >>> 

License
=======

Tree is licensed under the Apache 2.0 License.
