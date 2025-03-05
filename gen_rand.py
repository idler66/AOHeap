import matplotlib.pyplot as plt

# 数据
algorithms = ['Algorithm A', 'Algorithm B', 'Algorithm C']
x = [10, 20, 30, 40, 50]  # 输入规模
y1 = [1, 2, 3, 4, 5]  # 算法 A 的性能
y2 = [2, 3, 2, 5, 4]  # 算法 B 的性能
y3 = [3, 4, 5, 6, 7]  # 算法 C 的性能

# 绘图
plt.plot(x, y1, label='Algorithm A', marker='o')
plt.plot(x, y2, label='Algorithm B', marker='s')
plt.plot(x, y3, label='Algorithm C', marker='^')

# 设置图表
plt.title('Performance Comparison of Algorithms')
plt.xlabel('Input Size')
plt.ylabel('Performance Metric')
plt.legend()
plt.grid()
plt.show()