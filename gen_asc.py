import matplotlib.pyplot as plt

# 数据
algorithms = ['Skew', 'Binomial', 'Fibonacci']
x = [4, 5, 6, 7, 8]  # 输入规模
y1 = [9381, 7799.9, 14477, 21472.6, 4839.94]  # 算法 Skew 的性能
y2 = [2954, 2485.9, 2474.46, 2505.9, 462.462]  # 算法 Binomial 的性能
y3 = [1987, 1900.2, 1994.22, 1944.19, 407.694]  # 算法 Fibonacci 的性能

# 绘图
plt.plot(x, y1, label='Skew', marker='o')
plt.plot(x, y2, label='Binomial', marker='s')
plt.plot(x, y3, label='Fibonacci', marker='^')

# 设置图表
plt.title('Performance Comparison of Algorithms')
plt.xlabel('log10(number of nodes)')
plt.ylabel('Performance Metric')
plt.legend()
plt.grid()
plt.show()