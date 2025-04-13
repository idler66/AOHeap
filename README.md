# AOHeap
AOHeap is the abbreviation of Adjacent Ordered Heap. 
It introduces the total order relationship between the parent node and the child node in the heap into the child nodes. 
It breaks through the lower limit of the amortized time of Ω(logN) for the classic sorting of a single element.
It is currently the best performing heap data structure.

## License
This project is licensed under the [LICENSE](LICENSE) file, which uses many elements of the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 license.


临序堆把堆父节点和子节点之间的全序关系引入到子节点之间。

通过偏序链的旋转、树的横向生长和分组合并三个核心机制，临序堆突破了经典的排序单个元素要花费O(logN)的摊销时间下限，其最差时间复杂度为Ω(logN)，是已知支持键值变更功能性能最好的堆数据结构。

快速临序堆用容量为L的数组存储树根节点（避免指针操作），键值变更和删除堆顶节点时生成的树可批量地合并，其各种操作的均摊时间复杂度T与L和堆中元素N的关系式为 T ≈ log(N/T)/logL。
与Ω(logN)而言，这是非常大的性能提升。取L=128，N=2^32，T=2^m,有m + 7*2^m ≈ 32, m < 3, 树的平均高度 T < 8。这是临序堆的一种非常高效的实现形式。

理论和测试表明，临序堆优于fibonacci堆、pairing堆、dary堆、binomial堆和skew堆。

键值变更次数越多、堆中元素愈多，元素负载数据量愈大、入堆和出堆顺序越一致（一致会产生额外的性能提升，不一致性能不会降低），其性能优势愈大。

临序堆实现简单，性能高且对各种情况的输入是稳定的，不会出现突然的性能严重下滑，可完全取代已有支持键值变更的各种堆算法。
