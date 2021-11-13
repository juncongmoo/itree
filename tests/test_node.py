import unittest
import itree

class TestNode(unittest.TestCase):

    def test_empty_node(self):
        fruit = itree.Node('fruit')

        self.assertEqual(fruit.nid, 0)
        self.assertEqual(fruit.span(), 0)
        self.assertEqual(fruit.name, 'fruit')
        self.assertEqual(fruit.start, 0)
        self.assertEqual(fruit.end, 0)
        self.assertEqual(fruit.extra, {})
        self.assertEqual(fruit.nodes, [])
        self.assertEqual(str(fruit), '[🔵 n=fruit]')
        
        apple = itree.Node('apple',0,1,{"x":1})
        self.assertEqual(str(apple), '[🍁 n=apple,s=0.00,e=1.00,x=1,c=0]')
        
        fruit.append(apple)
        self.assertEqual(len(fruit.nodes), 1)
        fruit.end=100
        self.assertEqual(str(fruit), "[🟢 n=fruit,s=0.00,e=100.00,x=0,c=1]")
        
        
        pear = itree.Node('pear',4,5,{"x":4})
        fruit.add_child(pear)
        self.assertEqual(str(fruit), "[🟢 n=fruit,s=0.00,e=100.00,x=0,c=2]")
    
    def test_node_append(self):
        fruit = itree.Node('fruit')
        apple = itree.Node('apple',0,1,{"x":1})
        fruit.append(apple)
        self.assertEqual(id(fruit.nodes[0]), id(apple))
        
    def test_node_add_child(self):
        fruit = itree.Node('fruit')
        apple = itree.Node('apple',0,1,{"x":1})
        fruit.add_child(apple)
        self.assertNotEqual(id(fruit.nodes[0]), id(apple))

    def test_node_add_child_with_children(self):
        fruit = itree.Node('fruit')
        apple = itree.Node('apple',0,1,{"x":1})
        apple.nodes=[itree.Node("red-apple"), itree.Node("green-apple")]
        fruit.add_child(apple)
        
        self.assertNotEqual(id(fruit.nodes[0]), id(apple))
        self.assertEqual(id(fruit.nodes[0].nodes[0]), id(apple.nodes[0]))
        

if __name__ == '__main__':
    unittest.main()
