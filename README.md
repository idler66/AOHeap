# AOHeap
AOHeap is the abbreviation of Adjacent Ordered Heap. 
It introduces the total order relationship between the parent node and the child node in the heap into the child nodes. 
It is currently the best performing heap data structure.

1）在生成标准子树后，根节点的next链条长度大于K，则通过剪出常数个B子树，把其长度降低到K一下。
1）把生成的子树群，重新转化为多个标准子树。
It breaks through the lower limit of the amortized time of Ω(logN) for the classic sorting of a single element.
在上述两种操作下，该堆结构的所有操作的时间复杂度均为O(1)。

## License
This project is licensed under the [LICENSE](LICENSE) file, which uses many elements of the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 license.



