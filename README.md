# AOHeap
AOHeap is the abbreviation of Adjacent Ordered Heap. 
It introduces the total order relationship between the parent node and the child node in the heap into the child nodes. 
It breaks through the lower limit of the amortized time of Ω(logN) for the classic sorting of a single element.
It is currently the best performing heap data structure.

## License
This project is licensed under the [LICENSE](LICENSE) file, which uses many elements of the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 license.


临序堆把堆父节点和子节点之间的全序关系引入到子节点之间。通过偏序链的旋转、树的横向生长和分组合并三个核心机制，临序堆突破了经典的排序单个元素要花费O(logN)的摊销时间下限，其最差时间复杂度为Ω(logN)，是已知支持键值变更功能性能最好的堆数据结构。

快速临序堆用容量为L的数组存储树根节点（避免指针操作），键值变更和删除堆顶节点时生成的树可批量地合并，其各种操作的均摊时间复杂度T与L和堆中元素N的关系式为T≈log(N/T)/logL。

理论和测试表明，临序堆优于fibonacci堆、pairing堆、dary堆、binomial堆和skew堆，键值变更次数越多、元素负载数据量愈大、元素入堆和出堆顺序越一致，其性能优势愈大。临序堆实现简单，性能高，可完全取代已有支持键值变更的各种堆算法。
